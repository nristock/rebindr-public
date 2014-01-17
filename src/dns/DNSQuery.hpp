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
 * DNSQuery.hpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#ifndef DNSQUERY_HPP_
#define DNSQUERY_HPP_

#include <list>
using std::list;

namespace rebindr {
namespace dns {

#define REQ_RESP_MASK 0x8000
#define OP_CODE_MASK 0x7800
#define AUTH_ANS_MASK 0x0400
#define TRUNCED_MASK 0x0200
#define REC_DES_MASK 0x0100
#define REC_AVAIL_MASK 0x0080
#define RET_CODE_MASK 0x000F

#define NAME_ERROR_RCODE 0x0003
#define NOT_IMPLEMENTED_RCODE 0x0004

#define CLASS_IN 0x0001

#define RRT_A 0x0001		// A/IPv4 resource record
#define RRT_NS 0x0002		// NS/NameServer resource record
#define RRT_CNAME 0x0005	// CNAME/Alias resource record
#define RRT_SOA 0x0006		// SOA/StartOfAuthority resource record
#define RRT_PTR 0x000C		// PTR/Pointer resource record
#define RRT_MX 0x000F		// MX/Mail resource record
#define RRT_SRV 0x0021		// SRV resource record
#define RRT_ALL 0x00FF		// ALL resource records
#define RRT_AAAA 0x001C		// AAAA/IPv6 resource record

// Represents an almost raw query - byte order in network byte order
struct Query {
	unsigned short transactionID;
	unsigned short flags;
	unsigned short numQuestionRecords;
	unsigned short numAnswerRecords;
	unsigned short numAuthRecords;
	unsigned short numAdditionalRecords;
	char queryEntries[65536];
};

// Represents a query (question) entry
struct QueryQuestion {
	std::string questionName;
	unsigned short questionType;
	unsigned short questionClass;
};

// Represents an analyzed query - mostly exposes flags
struct AnalyzedQuery {
	Query* baseQuery;

	bool isRequest;
	char opCode;
	bool isAuthoritative;
	bool isTruncated;
	bool recursionDesired;
	char returnCode;

	list<QueryQuestion> questions;
};

}
}

#endif /* DNSQUERY_HPP_ */
