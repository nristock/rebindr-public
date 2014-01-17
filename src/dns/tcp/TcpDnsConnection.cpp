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
 * HTTPTcpConnection.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#include "../../ch/stdlib.hpp"
#include "../../ch/boost.hpp"
#include "../../ch/boostnet.hpp"
#include "../../ch/boostbind.hpp"

#include "../DNSQuery.h"
#include "../DNSUtils.h"
#include "TcpDnsConnection.h"
#include "../DNSQueryHandler.h"

namespace rebindr {
namespace dns {

TcpDnsConnection::pointer TcpDnsConnection::create(io_service& ioService) {
	return pointer(new TcpDnsConnection(ioService));
}

TcpDnsConnection::TcpDnsConnection(io_service& ioService) : localSocket(ioService) {
}

tcp::socket& TcpDnsConnection::socket() {
	return localSocket;
}

void TcpDnsConnection::start() {
  	handleDnsRequest(error_code(), 0);
}

void TcpDnsConnection::handleDnsRequest(const error_code& error, size_t length) {
	if (!error) {
		if (length == 0) {
			using namespace boost::asio; // placeholders::
			async_read(localSocket,
					buffer(dnsRequest),
					transfer_at_least(12),
					bind(&TcpDnsConnection::handleDnsRequest,
							shared_from_this(),
							placeholders::error,
							placeholders::bytes_transferred));
		} else {
			cout << "Received: " << length << endl;
			Query* query = (Query*) (this->dnsRequest.data() + 2);

			shared_ptr<streambuf> sendBuffer(
					new streambuf());
			ostream sendStream(sendBuffer.get());

			shared_ptr<streambuf> responseBuffer(
								new streambuf());
						ostream responseStream(responseBuffer.get());

			DNSQueryHandler handler(query,
					localSocket.remote_endpoint().address());
			handler.analyzeQuery();
			handler.printQueryInfo();

			// run this once to get the total size of the message
			handler.writeResponse(responseStream);

			writeShortToStream(sendStream, responseBuffer->size());
			handler.writeResponse(sendStream);

			using namespace boost::asio; // placeholders:;
			async_write(localSocket, sendBuffer->data(),
					bind(&TcpDnsConnection::handleSend, shared_from_this(), sendBuffer,
							placeholders::error,
							placeholders::bytes_transferred));
		}
	}
}

void TcpDnsConnection::shutdown() {
	localSocket.close();
}

void TcpDnsConnection::handleSend(shared_ptr<streambuf>,
		const error_code& error, size_t length) {
	cout << "DNS response sent: " << length << endl;

	if(error ) {
		cout << "Error sending DNS response: " << error << endl;
	}
	shutdown();
}


} /* namespace dns */
} /* namespace rebindr */
