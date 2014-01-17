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
 * DNSUtils.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

// hto*/nto* functions
#include <arpa/inet.h>
#include <iostream>

#include "DNSQuery.hpp"
#include "DNSUtils.hpp"

using std::ostream;

namespace rebindr {
namespace dns {

void writeIntToStream(ostream& stream, int value) {
	int temp = htonl(value);
	stream.write((char*) &temp, sizeof(int));
}

void writeShortToStream(ostream& stream, short value) {
	short temp = htons(value);
	stream.write((char*) &temp, sizeof(short));
}

void writeByteToStream(ostream& stream, char value) {
	stream.write(&value, sizeof(char));
}

void writeResponseHeader(ostream& stream, Query* response) {
	writeShortToStream(stream, response->transactionID);
	writeShortToStream(stream, response->flags);
	writeShortToStream(stream, response->numQuestionRecords);
	writeShortToStream(stream, response->numAnswerRecords);
	writeShortToStream(stream, response->numAuthRecords);
	writeShortToStream(stream, response->numAdditionalRecords);
}

void writeResponseEntryHeader(ostream& stream, short recordType,
		short dataLength) {
	// Resource Record Name
	writeByteToStream(stream, 19);
	stream << "lernleistung-rebind";
	writeByteToStream(stream, 2);
	stream << "de";
	writeByteToStream(stream, 0);

	// Resource Record Type
	writeShortToStream(stream, recordType);

	// Resource Record Class (INET)
	writeShortToStream(stream, 1);

	// TTL as 32bit int
	writeIntToStream(stream, 600);

	// Size of following answer payload
	writeShortToStream(stream, dataLength);
}

} /* namespace dns */
} /* namespace rebindr */
