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
 * CallbackTcpConnection.cpp
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
#include "CallbackTcpConnection.hpp"
#include "../HTTPUtils.hpp"
#include "../target/TargetRegistry.hpp"

using std::ifstream;
using std::istreambuf_iterator;

using boost::algorithm::split_regex;

using namespace boost::asio;

namespace rebindr {
namespace http {
namespace callback {

HTTPCallbackTcpConnection::pointer HTTPCallbackTcpConnection::create(
		io_service& ioService) {
	return pointer(new HTTPCallbackTcpConnection(ioService));
}

HTTPCallbackTcpConnection::HTTPCallbackTcpConnection(
		boost::asio::io_service& ioService) :
		localSocket(ioService) {
	httpMessage.reserve(8192);
}

HTTPCallbackTcpConnection::~HTTPCallbackTcpConnection() {
}

tcp::socket& HTTPCallbackTcpConnection::socket() {
	return localSocket;
}

void HTTPCallbackTcpConnection::start() {
	httpMessage.clear();

	handleHttpRequest(error_code(), 0);
}

void HTTPCallbackTcpConnection::handleHttpRequest(const error_code& error,
		size_t length) {
	if (!error) {
		target::TargetRegistry::instance->refreshClient(localSocket.remote_endpoint().address().to_v4().to_ulong());

		if (length != 0) {
			httpMessage.append(string(receiveBuffer.data(), length));
		}

		if (httpMessage.find("\r\n\r\n") == string::npos) {
			async_read(localSocket, buffer(receiveBuffer), transfer_at_least(1),
					bind(&HTTPCallbackTcpConnection::handleHttpRequest,
							shared_from_this(), placeholders::error,
							placeholders::bytes_transferred));
		} else {
			if (parsedHttpRequest == NULL) {
				parsedHttpRequest = parseHttpHeader(httpMessage);
			}
			int bytesMissing = parsedHttpRequest->contentLength
					- (httpMessage.length() - httpMessage.find("\r\n\r\n")) - 4;

			if (bytesMissing > 0) {
				async_read(localSocket, buffer(receiveBuffer),
						transfer_at_least(bytesMissing),
						bind(&HTTPCallbackTcpConnection::handleHttpRequest,
								shared_from_this(), placeholders::error,
								placeholders::bytes_transferred));
				return;
			} else if (parsedHttpRequest->method == "POST") {
					parsedHttpRequest->content = httpMessage.substr(httpMessage.find("\r\n\r\n"));
			}

			if (parsedHttpRequest->url.find("/poll") != string::npos) {
				stringstream answer;

				auto pendingRequest = target::TargetRegistry::instance->poll(localSocket.remote_endpoint().address().to_v4().to_ulong());
				if(pendingRequest != NULL) {
					answer << "request(" << pendingRequest->id << ", '" << pendingRequest->url << "'," << ((pendingRequest->postData.empty()) ? ("null") : ("'" + pendingRequest->postData + "'")) << ", " << ((pendingRequest->authorization.empty()) ? ("null") : ("'" + pendingRequest->authorization + "'")) << ");";
				}

				async_write(localSocket,
						buffer(
								string(
										"HTTP/1.0 200 OK\r\nContent-Type: text/javascript; charset=UTF-8\r\nContent-Length: ")
										+ to_string(answer.str().length())
										+ string("\r\n\r\n") + answer.str()),
						bind(&HTTPCallbackTcpConnection::handleWrite,
								shared_from_this(), placeholders::error));
			} else if (parsedHttpRequest->url.compare("/post") == 0) {
				int fileSize = boost::filesystem::file_size(
						CROSS_SITE_CALLBACK_HTML);

				async_write(localSocket,
						buffer(
								string(
										"HTTP/1.0 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: ")
										+ to_string(fileSize)
										+ string("\r\n\r\n")),
						bind(&HTTPCallbackTcpConnection::handlePostRequest,
								shared_from_this(), placeholders::error));
			} else if (parsedHttpRequest->url.compare("/put") == 0) {
				target::TargetRegistry::instance->onTargetResponse(
						parsedHttpRequest, localSocket.remote_endpoint().address().to_v4().to_ulong());
				async_write(localSocket,
						buffer(
								string(
										"HTTP/1.0 200 OK\r\nContent-Type: text/plain; charset=UTF-8\r\nContent-Length: 0")
										+ string("\r\n\r\n")),
						bind(&HTTPCallbackTcpConnection::handleWrite,
								shared_from_this(), placeholders::error));
			}

		}
	} else {
		cout << "Receive error: " << error << endl;
	}
}

void HTTPCallbackTcpConnection::shutdown() {
	localSocket.close();
}

void HTTPCallbackTcpConnection::handlePostRequest(
		const boost::system::error_code& error) {
	ifstream index(CROSS_SITE_CALLBACK_HTML);
	string fileContent((istreambuf_iterator<char>(index)),
			istreambuf_iterator<char>());

	async_write(localSocket, buffer(fileContent),
			bind(&HTTPCallbackTcpConnection::handleWrite, shared_from_this(),
					placeholders::error));
}

void HTTPCallbackTcpConnection::handleWrite(const error_code& error) {
		shutdown();
}

} /* namespace callback */
} /* namespace http */
} /* namespace rebindr */
