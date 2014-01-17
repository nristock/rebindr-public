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
 * TcpDnsServer.hpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#ifndef TCPDNSSERVER_HPP_
#define TCPDNSSERVER_HPP_

#include "../../ch/boost.hpp"
#include "../../ch/boostnet.hpp"

#include "TcpDnsConnection.hpp"

namespace rebindr {
namespace dns {

class TcpDnsServer {
public:
	TcpDnsServer(io_service& ioService);
private:
	tcp::acceptor localSocketAcceptor;

	void startAccept();
	void handleAccept(TcpDnsConnection::pointer newConnection, const error_code& error);
};

} /* namespace dns */
} /* namespace rebindr */

#endif /* TCPDNSSERVER_HPP_ */
