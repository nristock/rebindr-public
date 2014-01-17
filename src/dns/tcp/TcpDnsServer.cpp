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
 * HTTPCallbackServer.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#include "../../ch/stdlib.hpp"
#include "../../ch/boost.hpp"
#include "../../ch/boostnet.hpp"
#include "../../ch/boostbind.hpp"

#include "TcpDnsServer.hpp"

namespace rebindr {
namespace dns {

TcpDnsServer::TcpDnsServer(io_service& ioService) : localSocketAcceptor(ioService, tcp::endpoint(tcp::v4(), 53)) {
	startAccept();
}

void TcpDnsServer::startAccept() {
	TcpDnsConnection::pointer newConnection = TcpDnsConnection::create(localSocketAcceptor.get_io_service());

	using namespace boost::asio; // placeholders::
	localSocketAcceptor.async_accept(newConnection->socket(),
			bind(&TcpDnsServer::handleAccept, this, newConnection, placeholders::error));
}

void TcpDnsServer::handleAccept(TcpDnsConnection::pointer newConnection, const error_code& error) {
	if(!error) {
		newConnection->start();
	}

	startAccept();
}

} /* namespace dns */
} /* namespace rebindr */
