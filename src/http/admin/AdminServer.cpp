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
#include "../../ch/boostnet.hpp"
#include "../../ch/boostbind.hpp"

#include "AdminTcpConnection.hpp"
#include "AdminServer.hpp"

namespace rebindr {
namespace http {
namespace admin {

HTTPAdminServer::HTTPAdminServer(boost::asio::io_service& ioService) : localSocketAcceptor(ioService, tcp::endpoint(tcp::v4(), 82)) {
	startAccept();
}

void HTTPAdminServer::startAccept() {
	AdminTcpConnection::pointer newConnection = AdminTcpConnection::create(localSocketAcceptor.get_io_service());

	using namespace boost::asio; //placeholders::
	localSocketAcceptor.async_accept(newConnection->socket(),
			bind(&HTTPAdminServer::handleAccept, this, newConnection, placeholders::error));
}

void HTTPAdminServer::handleAccept(AdminTcpConnection::pointer newConnection, const error_code& error) {
	if(!error) {
		newConnection->start();
	}

	startAccept();
}

} /* namespace admin */
} /* namespace http */
} /* namespace rebindr */
