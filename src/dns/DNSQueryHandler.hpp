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
 * DNSQueryHandler.hpp
 *
 *  Created on: Dec 1, 2013
 *      Author: monofraps
 */

#ifndef DNSQUERYHANDLER_HPP_
#define DNSQUERYHANDLER_HPP_

#include "../ch/stdlib.hpp"
#include "../ch/boostnet.hpp"
#include "DNSQuery.hpp"

namespace rebindr {
namespace dns {

class DNSQueryHandler {
public:
	DNSQueryHandler(Query* query, address remoteAddress);

	void analyzeQuery();
	void writeResponse(ostream& outStream);
	void printQueryInfo();

private:
	void writeResponseRRTA(ostream& outStream);
	void writeResponseRRTSOA(ostream& outStream);
	void writeResponseRRTNS(ostream& outStream);
	void repeatQuery(ostream& outStream);
	void fail(ostream& outStream, short error, Query* response);

	Query* query;
	address remoteAddress;
	AnalyzedQuery analyzedQuery;
};

} /* namespace dns */
} /* namespace rebindr */

#endif /* DNSQUERYHANDLER_HPP_ */
