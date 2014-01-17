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
 * TcpDnsConnection.hpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#ifndef TCPDNSCONNECTION_HPP_
#define TCPDNSCONNECTION_HPP_

#include "../../ch/boost.hpp"
#include "../../ch/boostnet.hpp"

namespace rebindr {
namespace dns {

class TcpDnsConnection: public boost::enable_shared_from_this<TcpDnsConnection> {
public:
	typedef shared_ptr<TcpDnsConnection> pointer;

	static pointer create(io_service& ioService);

	tcp::socket& socket();

	void start();
	void handleSend(shared_ptr<streambuf>,
			const error_code& error, size_t length);
	void shutdown();
private:
	tcp::socket localSocket;

	array<char, 8192> dnsRequest;
	TcpDnsConnection(io_service& ioService);
	void handleDnsRequest(const error_code& error, size_t length);
};

} /* namespace dns */
} /* namespace rebindr */

#endif /* TCPDNSCONNECTION_HPP_ */
