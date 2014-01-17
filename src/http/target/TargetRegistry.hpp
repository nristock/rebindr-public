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
 * TargetRegistry.h
 *
 *  Created on: Jan 2, 2014
 *      Author: monofraps
 */

#ifndef TARGETREGISTRY_H_
#define TARGETREGISTRY_H_

#include "../../ch/stdlib.hpp"
#include "../../ch/boost.hpp"
#include "../../ch/boostnet.hpp" // deadline timer, part of ASIO

#include "TargetRequset.hpp"
#include "../HTTPHeader.hpp"

namespace rebindr {
namespace http {
namespace target {

class TargetRegistry {
private:
	typedef map<ulong, time_t> AddressRefreshTimeMap;
	typedef AddressRefreshTimeMap::iterator ARTMIterator;

	TargetRegistry(io_service& ioService);
	ulong currentTargetAddress;
	deadline_timer timer;

	queue<shared_ptr<TargetRequest>> pendingRequests;
	vector<shared_ptr<TargetRequest>> runningRequests;

	AddressRefreshTimeMap knownClients;

public:
	static shared_ptr<TargetRegistry> instance;
	static void init(io_service& ioService);

	void shutdown();

	void scheduleRequest(shared_ptr<HTTPRequest> httpRequest, function<void(string)> callback);
	void onTargetResponse(shared_ptr<HTTPRequest> request, ulong ipv4Address);

	shared_ptr<TargetRequest> poll(ulong ipv4Address);

	vector<ulong> getAvailableClients();

	void announceClient(ulong ipv4Address);
	void refreshClient(ulong ipv4Address);

	void checkClients(const error_code& error);

	void setTarget(ulong ipv4Address);
};

} /* namespace target */
} /* namespace http */
} /* namespace rebindr */

#endif /* TARGETREGISTRY_H_ */
