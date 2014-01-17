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
 * UdpDnsServer.hpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#ifndef UDPDNSSERVER_HPP_
#define UDPDNSSERVER_HPP_

#include "../../ch/boost.hpp"
#include "../../ch/boostnet.hpp"

namespace rebindr {
namespace dns {

class UdpDnsServer {
public:
	UdpDnsServer(io_service& ioService);
private:
	udp::socket localSocket;
	udp::endpoint remoteEndpoint;
	array<char, 65536> recvBuffer;

	void startReceive();
	void handleReceive(const error_code& error, size_t len);
	void handleSend(shared_ptr<streambuf> buffer, const error_code&, size_t len);
};

} /* namespace dns */
} /* namespace rebindr */

#endif /* UDPDNSSERVER_HPP_ */
