/*
 * rebindr
 * Copyright (C) 2014 Nicolas Ristock (monofraps@gmail.com)
 *
 *
 * This file is part of rebindr.
 *
 * rebindr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * rebindr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with rebindr.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * TargetRegistry.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: monofraps
 */


#include <cstdlib>
#include <atomic>
#include "../../ch/stdlib.hpp"
#include "../../ch/boost.hpp"
#include "../../ch/boostnet.hpp"
#include "../../ch/boostbind.hpp"

#include "TargetRegistry.hpp"

using std::atomic_int;
using std::system;


namespace rebindr {
namespace http {
namespace target {

shared_ptr<TargetRegistry> TargetRegistry::instance;
atomic_int g_requestId(0);

TargetRegistry::TargetRegistry(io_service& ioService) : currentTargetAddress(0), timer(ioService, boost::posix_time::seconds(10)) {
	using namespace boost::asio; //placeholders::
	timer.async_wait(boost::bind(&TargetRegistry::checkClients, this, placeholders::error));
}

void TargetRegistry::init(io_service& ioService) {
	if (TargetRegistry::instance == NULL) {
		TargetRegistry::instance = shared_ptr<TargetRegistry>(
				new TargetRegistry(ioService));
	}
}

void TargetRegistry::scheduleRequest(shared_ptr<HTTPRequest> httpRequest,
		function<void(string)> callback) {
	shared_ptr<TargetRequest> tRequest(new TargetRequest());

	tRequest->id = g_requestId.fetch_add(1);
	tRequest->url = httpRequest->url;
	tRequest->authorization = httpRequest->authorization;
	tRequest->postData = httpRequest->content;
	tRequest->callback = callback;

	cout << "Scheduling target request " << tRequest->id << endl;

	pendingRequests.push(tRequest);
}

void TargetRegistry::onTargetResponse(shared_ptr<HTTPRequest> request, ulong ipv4Address) {
	if(currentTargetAddress != ipv4Address) return;

	string* rcnt = &(request->content);
	int requestId = atoi(rcnt->substr(rcnt->find("@=") + 2, rcnt->find("@&") - rcnt->find("@=") - 2).c_str());
	cout << "RID: " << requestId << endl;
	typedef vector<shared_ptr<TargetRequest>>::iterator TargetRequestListIterator;

	TargetRequestListIterator pendingDeletion;
	for(TargetRequestListIterator iterator = runningRequests.begin(); iterator != runningRequests.end(); iterator++) {
		if((*iterator)->id == requestId) {
			pendingDeletion = iterator;

			string payload = rcnt->substr(rcnt->find("@&") + 2);

			(*iterator)->callback(payload);
			break;
		}
	}

	runningRequests.erase(pendingDeletion);
}

void TargetRegistry::announceClient(ulong ipv4Address) {
	if(knownClients.count(ipv4Address) == 0) {
		// Add client to list
		knownClients[ipv4Address] = time(NULL);

		// Add firewall rule
		stringstream command;
		command << "iptables -A INPUT -p tcp --dport 80 -s " << boost::asio::ip::address_v4(ipv4Address).to_string() << " -j REJECT";
		//system(command.str().c_str());
		cout << "IPTBLOCK: " << command.str() << endl;
	}
}

void TargetRegistry::refreshClient(ulong ipv4Address) {
	if(knownClients.count(ipv4Address) > 0) {
		knownClients[ipv4Address] = time(NULL);
	}
}

void TargetRegistry::checkClients(const error_code& error) {
	if(error != 0) return;

	cout << "Checking client DB" << endl;

	typedef vector<ARTMIterator> RemovalList;
	typedef RemovalList::iterator RemovalIterator;

	time_t now = time(NULL);
	RemovalList pendingRemovals;

	for(ARTMIterator iterator = knownClients.begin(); iterator != knownClients.end(); iterator++) {
		if(now - iterator->second > 10) {
			pendingRemovals.push_back(iterator);
		}
	}

	for(RemovalIterator iterator = pendingRemovals.begin(); iterator != pendingRemovals.end(); iterator++) {
		// Remove firewall rule
		stringstream command;
		command << "iptables -D INPUT -p tcp --dport 80 -s " << boost::asio::ip::address_v4((*iterator)->first).to_string();
		//system(command.str().c_str());
		cout << "IPTFREE: " << command.str() << endl;

		// Remove client from list
		knownClients.erase(*iterator);
	}

	// Reset the timer
	using namespace boost::asio; // placeholders::
	timer.expires_at(timer.expires_at() + boost::posix_time::seconds(10));
	timer.async_wait(boost::bind(&TargetRegistry::checkClients, this, placeholders::error));
}

void TargetRegistry::shutdown() {
	timer.cancel();
}

void TargetRegistry::setTarget(ulong ipv4Address) {
	pendingRequests.empty();
	runningRequests.clear();

	currentTargetAddress = ipv4Address;

	cout << "Set target to: " << currentTargetAddress << endl;
}

shared_ptr<TargetRequest> TargetRegistry::poll(ulong ipv4Address) {
	cout << "POLL Active IP4: " << currentTargetAddress << " | Polling IP4: " << ipv4Address << endl;

	if((ipv4Address == currentTargetAddress) && (pendingRequests.size() != 0)) {
		shared_ptr<TargetRequest> tRequest = pendingRequests.front();
		pendingRequests.pop();
		runningRequests.push_back(tRequest);

		return tRequest;
	}

	return shared_ptr<TargetRequest>(NULL);
}

vector<ulong> TargetRegistry::getAvailableClients() {
	vector<ulong> ret;
	for(ARTMIterator iterator = knownClients.begin(); iterator != knownClients.end(); iterator++) {
			ret.push_back(iterator->first);
		}

	return ret;

}


} /* namespace target */
} /* namespace http */
} /* namespace rebindr */

