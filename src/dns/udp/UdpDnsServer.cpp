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
 * UdpDnsServer.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#include "../../ch/stdlib.hpp"
#include "../../ch/boost.hpp"
#include "../../ch/boostnet.hpp"
#include "../../ch/boostbind.hpp"

#include "UdpDnsServer.hpp"
#include "../DNSQuery.hpp"
#include "../DNSUtils.hpp"
#include "../DNSQueryHandler.hpp"

namespace rebindr {
namespace dns {

UdpDnsServer::UdpDnsServer(io_service& ioService) :
		localSocket(ioService, udp::endpoint(udp::v4(), 53)) {
	startReceive();
}

void UdpDnsServer::startReceive() {
	using namespace boost::asio; //for placeholders::
	localSocket.async_receive_from(buffer(recvBuffer),
			remoteEndpoint,
			bind(&UdpDnsServer::handleReceive, this,
					placeholders::error,
					placeholders::bytes_transferred));
}

void UdpDnsServer::handleReceive(const error_code& error,
		size_t length) {

	using namespace boost::asio; // for placeholders:: and error::
	if (!error || error == error::message_size) {
		cout << "Received: " << length << endl;

		Query* query = (Query*) (this->recvBuffer.data());

		shared_ptr<streambuf> sendBuffer(
				new streambuf());
		ostream sendStream(sendBuffer.get());

		DNSQueryHandler handler(query, remoteEndpoint.address());
		handler.analyzeQuery();
		handler.printQueryInfo();
		handler.writeResponse(sendStream);

		localSocket.async_send_to(sendBuffer->data(), remoteEndpoint,
				bind(&UdpDnsServer::handleSend, this, sendBuffer,
						placeholders::error,
						placeholders::bytes_transferred));

		//delete response;

		startReceive();
	}
}

void UdpDnsServer::handleSend(shared_ptr<streambuf>,
		const error_code& error, size_t length) {
	cout << "DNS response sent: " << length << endl;

	if(error ) {
		cout << "Error sending DNS response: " << error << endl;
	}

}

} /* namespace dns */
} /* namespace rebindr */
