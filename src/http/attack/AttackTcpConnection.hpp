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
 * AttackTcpConnection.h
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#ifndef ATTACKTCPCONNECTION_HPP_
#define ATTACKTCPCONNECTION_HPP_

#define ATTACK_INDEX_HTML "www/attack.index.html"

#include "../../ch/stdlib.hpp"
#include "../../ch/boost.hpp"
#include "../../ch/boostnet.hpp"

#include "../HTTPHeader.hpp"

namespace rebindr {
namespace http {
namespace attack {

class HTTPAttackTcpConnection: public boost::enable_shared_from_this<
		HTTPAttackTcpConnection> {
public:
	typedef shared_ptr<HTTPAttackTcpConnection> pointer;

	static pointer create(io_service& ioService);

	tcp::socket& socket();

	void start();
	void handleWrite(const error_code& error, size_t transferredBytes);
	void handleHeaderWrite(const error_code& error);
	void shutdown();

private:
	tcp::socket localSocket;

	array<char, 8192> httpHeaderBuffer;

	string httpHeader;
	shared_ptr<rebindr::http::HTTPRequest> parsedHttpHeader;

	HTTPAttackTcpConnection(io_service& ioService);
	void handleHttpRequest(const error_code& error, size_t length);
};

} /* namespace attack */
} /* namespace http */
} /* namespace rebindr */

#endif /* ATTACKTCPCONNECTION_HPP_ */
