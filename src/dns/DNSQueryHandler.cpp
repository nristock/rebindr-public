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
 * DNSQueryHandler.cpp
 *
 *  Created on: Dec 1, 2013
 *      Author: monofraps
 */

#include "../ch/stdlib.hpp"
#include "../ch/boost.hpp"
#include "../ch/boostnet.hpp"

#include "DNSQuery.hpp"
#include "DNSUtils.hpp"
#include "DNSQueryHandler.hpp"

namespace rebindr {
namespace dns {

DNSQueryHandler::DNSQueryHandler(Query* query, address remoteAddress) {
	this->remoteAddress = remoteAddress;
	this->query = query;
}

void DNSQueryHandler::analyzeQuery() {
	analyzedQuery.baseQuery = query;

	analyzedQuery.isRequest = !(query->flags & REQ_RESP_MASK);
	analyzedQuery.opCode = ntohs(query->flags & OP_CODE_MASK);
	analyzedQuery.isAuthoritative = query->flags & AUTH_ANS_MASK;
	analyzedQuery.isTruncated = query->flags & TRUNCED_MASK;
	analyzedQuery.recursionDesired = query->flags & REC_DES_MASK;
	analyzedQuery.returnCode = query->flags & RET_CODE_MASK;


	char* nextChar = query->queryEntries;
	for(unsigned short i = 0; i < ntohs(query->numQuestionRecords); i++) {
		QueryQuestion question;
		question.questionName = "";

		while (*nextChar != 0x00) {
			short numCharacters = *(nextChar++);
			for (int j = 0; j < numCharacters; j++, nextChar++) {
				question.questionName.append(1, *nextChar);
			}
			question.questionName.append(".");
		}

		question.questionType = *((short*) ++nextChar);

		nextChar++;
		question.questionClass = *((short*) ++nextChar);
		nextChar++;

		analyzedQuery.questions.push_back(question);
	}
}

void DNSQueryHandler::writeResponse(std::ostream& sendStream) {
	// only answers first question
	list<QueryQuestion>::iterator iter = analyzedQuery.questions.begin();

	Query* response = new Query();
	response->transactionID = ntohs(query->transactionID);
	response->flags = REQ_RESP_MASK | AUTH_ANS_MASK;

	response->numAdditionalRecords = 0;
	response->numQuestionRecords = 0;
	response->numAuthRecords = 0;

	short questionType = ntohs(iter->questionType);
	if(iter->questionName != "lernleistung-rebind.de.") {
		fail(sendStream, NAME_ERROR_RCODE, response);
		return;
	}

	if(questionType == RRT_A) {
		cout << "Received A request - answering with 2 A records." << endl;
		response->numAnswerRecords = 2;
		response->numQuestionRecords = 1;
		writeResponseHeader(sendStream, response);
		repeatQuery(sendStream);
		writeResponseRRTA(sendStream);
	} else if(questionType == RRT_NS) {
		cout << "Received NS request - answering with 2 NS records." << endl;
		response->numAnswerRecords = 2;
		writeResponseHeader(sendStream, response);
		writeResponseRRTNS(sendStream);
	} else if(questionType == RRT_SOA) {
		cout << "Received SOA request - answering with SOA record." << endl;
		response->numAnswerRecords = 1;
		writeResponseHeader(sendStream, response);
		writeResponseRRTSOA(sendStream);
	} else if(questionType == 255) {
		cout << "Received ALL request - answering with all records." << endl;
		response->numAnswerRecords = 5;
		writeResponseHeader(sendStream, response);
		writeResponseRRTSOA(sendStream);
		writeResponseRRTNS(sendStream);
		writeResponseRRTA(sendStream);
	} else {
		cout << "Received unknown request. Answering with OK, 0 response entries." << endl;
		response->numQuestionRecords = 0;
		response->numAnswerRecords = 0;
		writeResponseHeader(sendStream, response);
	}

}

void DNSQueryHandler::printQueryInfo() {
	cout << "DNS Query (id=" << query->transactionID << ")" << endl;
	cout << "Flags:" << endl;
	cout << "\tIsRequest: " << analyzedQuery.isRequest << endl;
	cout << "\tOpCode: " << (short)analyzedQuery.opCode << endl;
	cout << "\tIsAuthoritative: " << analyzedQuery.isAuthoritative << endl;
	cout << "\tIsTruncated: " << analyzedQuery.isTruncated << endl;
	cout << "\tRecursionDesired: " << analyzedQuery.recursionDesired << endl;
	cout << "\tReturnCode: " << (short)analyzedQuery.returnCode << endl;
	cout << "Question Resource Record Count: " << ntohs(analyzedQuery.baseQuery->numQuestionRecords) << endl;
	cout << "Answer Resource Record Count: " << ntohs(analyzedQuery.baseQuery->numAnswerRecords) << endl;
	cout << "Authority Resource Record: " << ntohs(analyzedQuery.baseQuery->numAuthRecords) << endl;
	cout << "Additional Resource Record Count: " << ntohs(analyzedQuery.baseQuery->numAdditionalRecords) << endl;

	cout << "Question Body" << endl;

	for(std::list<QueryQuestion>::iterator iter = analyzedQuery.questions.begin(); iter != analyzedQuery.questions.end(); iter++) {
		cout << "\tQuestionName: " << iter->questionName << endl;
		cout << "\t\tQustionType: ";

		short questionType = ntohs(iter->questionType);
		if(questionType == RRT_A) cout << "A ";
		if(questionType == RRT_CNAME) cout << "CNAME ";
		if(questionType == RRT_SOA) cout << "SOA ";
		if(questionType == RRT_MX) cout << "MX ";
		if(questionType == RRT_NS) cout << "NS ";
		if(questionType == RRT_PTR) cout << "PTR";
		if(questionType == RRT_SRV) cout << "SRV ";
		if(questionType == 255) cout << "ALL ";
		cout << "(" << questionType << ")" << endl;

		cout << "\t\tQustionClass: " << ntohs(iter->questionClass) << endl;
	}

	cout << endl;
}

void DNSQueryHandler::repeatQuery(std::ostream& sendStream) {
	writeByteToStream(sendStream, 19);
	sendStream << "lernleistung-rebind";
	writeByteToStream(sendStream, 2);
	sendStream << "de";
	writeByteToStream(sendStream, 0);

	writeShortToStream(sendStream, 1);
	writeShortToStream(sendStream, 1);
}

void DNSQueryHandler::fail(std::ostream& sendStream, short error, Query* response) {
	response->flags |= error;

	response->numQuestionRecords = 0;
	response->numAnswerRecords = 0;
	writeResponseHeader(sendStream, response);
}

void DNSQueryHandler::writeResponseRRTA(std::ostream& sendStream) {
	writeResponseEntryHeader(sendStream, RRT_A, 4);

	// IP Address
	writeByteToStream(sendStream, 172);
	writeByteToStream(sendStream, 5);
	writeByteToStream(sendStream, 5);
	writeByteToStream(sendStream, 100);

	writeResponseEntryHeader(sendStream, RRT_A, 4);

	// Secondary IP is client's IP (rebind magic here!)
	writeIntToStream(sendStream, (int)remoteAddress.to_v4().to_ulong());

}

void DNSQueryHandler::writeResponseRRTSOA(std::ostream& sendStream) {
	writeResponseEntryHeader(sendStream, RRT_SOA, 71);

	// Master Name
	writeByteToStream(sendStream, 3);
	sendStream << "ns1";
	writeByteToStream(sendStream, 19);
	sendStream << "lernleistung-rebind";
	writeByteToStream(sendStream, 2);
	sendStream << "de";
	writeByteToStream(sendStream, 0);

	// Responsible Name (EMail)
	writeByteToStream(sendStream, 7);
	sendStream << "nicolas";
	writeByteToStream(sendStream, 9);
	sendStream << "monofraps";
	writeByteToStream(sendStream, 3);
	sendStream << "net";
	writeByteToStream(sendStream, 0);

	// Serial/Version Number
	writeIntToStream(sendStream, 1);

	// Refresh Interval
	writeIntToStream(sendStream, 60);

	// Retry Interval
	writeIntToStream(sendStream, 60);

	// Expire
	writeIntToStream(sendStream, 60);

	// Negative Caching TTL
	writeIntToStream(sendStream, 60);
}

void DNSQueryHandler::writeResponseRRTNS(std::ostream& sendStream) {
	writeResponseEntryHeader(sendStream , RRT_NS, 28);
	writeByteToStream(sendStream, 3);
	sendStream << "ns1";
	writeByteToStream(sendStream, 19);
	sendStream << "lernleistung-rebind";
	writeByteToStream(sendStream, 2);
	sendStream << "de";
	writeByteToStream(sendStream, 0);

	writeResponseEntryHeader(sendStream, RRT_NS, 28);
	writeByteToStream(sendStream, 3);
	sendStream << "ns2";
	writeByteToStream(sendStream, 19);
	sendStream << "lernleistung-rebind";
	writeByteToStream(sendStream, 2);
	sendStream << "de";
	writeByteToStream(sendStream, 0);
}

} /* namespace dns */
} /* namespace rebindr */
