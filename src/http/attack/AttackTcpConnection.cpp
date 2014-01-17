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
 * AttackTcpConnection.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#include <fstream>
#include <boost/filesystem.hpp>

#include "../../ch/stdlib.hpp"
#include "../../ch/boost.hpp"
#include "../../ch/boostnet.hpp"
#include "../../ch/boostbind.hpp"

#include "../HTTPHeader.hpp"
#include "AttackTcpConnection.hpp"
#include "../HTTPUtils.hpp"
#include "../target/TargetRegistry.hpp"

using std::istreambuf_iterator;
using std::ifstream;

using boost::filesystem::file_size;

namespace rebindr {
namespace http {
namespace attack {

HTTPAttackTcpConnection::pointer HTTPAttackTcpConnection::create(io_service& ioService) {
	return pointer(new HTTPAttackTcpConnection(ioService));
}

HTTPAttackTcpConnection::HTTPAttackTcpConnection(io_service& ioService) : localSocket(ioService) {
	httpHeader.reserve(8192);
}

tcp::socket& HTTPAttackTcpConnection::socket() {
	return localSocket;
}

void HTTPAttackTcpConnection::start() {
	httpHeader.clear();

	handleHttpRequest(error_code(), 0);
}

void HTTPAttackTcpConnection::handleHttpRequest(const error_code& error, size_t length) {
	if(!error) {
		if(length != 0) {
			httpHeader.append(string(httpHeaderBuffer.data(), length));
		}

		if(httpHeader.find("\r\n\r\n") == string::npos) {
			using namespace boost::asio; // placeholders::
			async_read(localSocket, buffer(httpHeaderBuffer), transfer_at_least(1),
					bind(&HTTPAttackTcpConnection::handleHttpRequest, shared_from_this(),
							placeholders::error,
							placeholders::bytes_transferred));
		} else {
			parsedHttpHeader = parseHttpHeader(httpHeader);
			// Register client
			target::TargetRegistry::instance->announceClient(localSocket.remote_endpoint().address().to_v4().to_ulong());

			int fileSize = file_size(ATTACK_INDEX_HTML);

			// HTTP/1.0 - we don't support persistent connections
			using namespace boost::asio; //placeholders::
			async_write(localSocket, buffer(string("HTTP/1.0 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: ") + to_string(fileSize) + string("\r\n\r\n")),
					bind(&HTTPAttackTcpConnection::handleHeaderWrite, shared_from_this(),
							placeholders::error));
		}
	}
}

void HTTPAttackTcpConnection::shutdown() {
	localSocket.close();
}

void HTTPAttackTcpConnection::handleHeaderWrite(const error_code& error) {
	ifstream index(ATTACK_INDEX_HTML);
	string fileContent((istreambuf_iterator<char>(index)), istreambuf_iterator<char>());

	using namespace boost::asio; // placeholders:;
	async_write(localSocket, buffer(fileContent),
						bind(&HTTPAttackTcpConnection::handleWrite, shared_from_this(),
								placeholders::error, placeholders::bytes_transferred));
}

void HTTPAttackTcpConnection::handleWrite(const error_code& error, size_t transferredBytes) {
			shutdown();
}


} /* namespace attack */
} /* namespace http */
} /* namespace rebindr */
