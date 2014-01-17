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
 * HTTPUtils.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#include "../ch/stdlib.hpp"
#include "../ch/boost.hpp"

#include "HTTPHeader.hpp"
#include "HTTPUtils.hpp"

namespace rebindr {
namespace http {

shared_ptr<HTTPRequest> parseHttpHeader(string& input) {
	shared_ptr<HTTPRequest> httpHeader(new HTTPRequest());
	httpHeader->content = "";
	httpHeader->authorization = "";

	StringVector headerLines;
		split_regex(headerLines, input, boost::regex("\r\n"));

	string requestLine = headerLines[0];
	httpHeader->method = requestLine.substr(0, requestLine.find(" "));

	int urlBegin = requestLine.find(" ") + 1;
	int urlEnd = requestLine.find_last_of(" ");
	httpHeader->url = requestLine.substr(urlBegin, urlEnd - urlBegin);

	for (StringVectorIterator line = headerLines.begin(); line != headerLines.end(); line++) {
		if (line->empty()) {
			continue;
		}

		if (line->find("Host: ") != string::npos) {
			httpHeader->host = line->substr(line->find(" "), line->length());
		} else if(line->find("Content-Length: ") != string::npos) {
			httpHeader->contentLength = atoi(line->substr(line->find(" "), line->length() - line->find(" ")).c_str());
		} else if(line->find("Authorization: ") != string::npos) {
			httpHeader->authorization = line->substr(line->find_first_of(" ") + 1);
		}

		cout << *line << endl;
	}
	cout << endl;

	return httpHeader;
}

} /* namespace http */
} /* namespace rebindr */
