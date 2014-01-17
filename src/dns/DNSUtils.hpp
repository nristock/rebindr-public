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
 * DNSUtils.hpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#ifndef DNSUTILS_HPP_
#define DNSUTILS_HPP_

#include <iostream>

#include "DNSQuery.hpp"

using std::ostream;

namespace rebindr {
namespace dns {

void writeIntToStream(ostream& stream, int value);
void writeShortToStream(ostream& stream, short value);
void writeByteToStream(ostream& stream, char value);
void writeResponseHeader(ostream& stream, Query* response);
void writeResponseEntryHeader(ostream& stream, short recordType, short dataLength);

} /* namespace dns */
} /* namespace rebindr */

#endif /* DNSUTILS_HPP_ */
