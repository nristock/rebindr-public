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
 * AdminTcpConnection.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#include "../../ch/stdlib.hpp"
#include "../../ch/boost.hpp"
#include "../../ch/boostnet.hpp"
#include "../../ch/boostbind.hpp"

#include <fstream>
#include <functional>
#include <boost/filesystem.hpp>

#include "../HTTPHeader.hpp"
#include "AdminTcpConnection.hpp"
#include "../HTTPUtils.hpp"
#include "../target/TargetRegistry.hpp"

using std::ifstream;
using std::istreambuf_iterator;

using boost::algorithm::split_regex;

namespace rebindr {
namespace http {
namespace admin {

AdminTcpConnection::pointer AdminTcpConnection::create(io_service& ioService) {
	return pointer(new AdminTcpConnection(ioService));
}

AdminTcpConnection::AdminTcpConnection(boost::asio::io_service& ioService) :
		localSocket(ioService) {
	httpMessage.reserve(8192);
}

tcp::socket& AdminTcpConnection::socket() {
	return localSocket;
}

void AdminTcpConnection::start() {
	httpMessage.clear();

	handleHttpRequest(error_code(), 0);
}

void AdminTcpConnection::handleTargetResponse(string response) {
	using namespace boost::asio; //placeholders::
	async_write(localSocket,
			buffer(
					response),
			bind(&AdminTcpConnection::handleWrite, shared_from_this(),
					placeholders::error));
}

void AdminTcpConnection::handleHttpRequest(const error_code& error,
		size_t length) {
	if (!error) {
		if (length != 0) {
			httpMessage.append(string(receiveBuffer.data(), length));
		}

		if (httpMessage.find("\r\n\r\n") == string::npos) {
			using namespace boost::asio; //placeholders::
			async_read(localSocket, buffer(receiveBuffer),
					transfer_at_least(1),
					bind(&AdminTcpConnection::handleHttpRequest,
							shared_from_this(), placeholders::error,
							placeholders::bytes_transferred));
		} else {
			if (parsedHttpRequest == NULL) {
				parsedHttpRequest = parseHttpHeader(httpMessage);
			}
			int bytesMissing = parsedHttpRequest->contentLength
					- (httpMessage.length() - httpMessage.find("\r\n\r\n")) - 4;

			if (bytesMissing > 0) {
				using namespace boost::asio; //placeholders::
				async_read(localSocket, buffer(receiveBuffer),
						transfer_at_least(bytesMissing),
						bind(&AdminTcpConnection::handleHttpRequest,
								shared_from_this(), placeholders::error,
								placeholders::bytes_transferred));
				return;
			} else if (parsedHttpRequest->method == "POST") {
				parsedHttpRequest->content = httpMessage.substr(httpMessage.find("\r\n\r\n"));
			}

			if (parsedHttpRequest->url.compare("/rebind/list") == 0) {
				int fileSize = boost::filesystem::file_size(
						"www/admin.list.html");

				using namespace boost::asio; //placeholders::
				async_write(localSocket,
						buffer(
								string(
										"HTTP/1.0 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: ")
										+ to_string(fileSize)
										+ string("\r\n\r\n")),
						bind(&AdminTcpConnection::handleAdminListIndex,
								shared_from_this(), placeholders::error));
			} else if (parsedHttpRequest->url.compare("/rebind/listjs")
					== 0) {
				vector<ulong> targets = target::TargetRegistry::instance->getAvailableClients();
				string response;
				for(vector<ulong>::iterator iterator = targets.begin(); iterator != targets.end(); iterator++) {
					response += boost::asio::ip::address_v4(*iterator).to_string() + "\r\n";
				}

				using namespace boost::asio; //placeholders::
				async_write(localSocket,
						buffer(
								string(
										"HTTP/1.0 200 OK\r\nContent-Type: text/json; charset=UTF-8\r\nContent-Length: ")
										+ to_string(response.length())
										+ string("\r\n\r\n")
										+ response),
						bind(&AdminTcpConnection::handleWrite,
								shared_from_this(), placeholders::error));
			} else if (parsedHttpRequest->url.find("/rebind/set")
					!= string::npos) {
				string target = parsedHttpRequest->url.substr(
						parsedHttpRequest->url.find("?") + 1);
				cout << "Setting target to: " << target << endl;
				target::TargetRegistry::instance->setTarget(
						localSocket.remote_endpoint().address().to_v4().to_ulong());

				int fileSize = boost::filesystem::file_size(
						"www/admin.list.html");

				using namespace boost::asio; //placeholders::
				async_write(localSocket,
						buffer(
								string(
										"HTTP/1.0 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: ")
										+ to_string(fileSize)
										+ string("\r\n\r\n")),
						bind(&AdminTcpConnection::handleAdminListIndex,
								shared_from_this(), placeholders::error));
			} else if (parsedHttpRequest->url.find("/favicon")
					!= string::npos) {
				return;
			} else {
				target::TargetRegistry::instance->scheduleRequest(parsedHttpRequest,
						std::bind(&AdminTcpConnection::handleTargetResponse,
								shared_from_this(), std::placeholders::_1));
			}

		}
	}
}

void AdminTcpConnection::shutdown() {
	localSocket.close();
}

void AdminTcpConnection::handleAdminListIndex(
		const boost::system::error_code& error) {
	ifstream index("www/admin.list.html");
	string fileContent((istreambuf_iterator<char>(index)),
			istreambuf_iterator<char>());

	using namespace boost::asio; //placeholders::
	async_write(localSocket, buffer(fileContent),
			bind(&AdminTcpConnection::handleWrite, shared_from_this(),
					placeholders::error));
}

void AdminTcpConnection::handleWrite(const error_code& error) {
	shutdown();
}

} /* namespace callback */
} /* namespace http */
} /* namespace rebindr */
