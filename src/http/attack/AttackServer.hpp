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
 * AttackServer.h
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#ifndef ATTACKSERVER_HPP_
#define ATTACKSERVER_HPP_

#include "../../ch/boostnet.hpp"

#include "AttackTcpConnection.hpp"

namespace rebindr {
namespace http {
namespace attack {

class HTTPAttackServer {
public:
	HTTPAttackServer(io_service& ioService);
private:
	tcp::acceptor localSocketAcceptor;

	void startAccept();
	void handleAccept(HTTPAttackTcpConnection::pointer newConnection, const error_code& error);
};

} /* namespace attack */
} /* namespace http */
} /* namespace rebindr */

#endif /* ATTACKSERVER_HPP_ */
