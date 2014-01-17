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
 * HTTPUtils.hpp
 *
 *  Created on: Nov 16, 2013
 *      Author: monofraps
 */

#ifndef HTTPUTILS_HPP_
#define HTTPUTILS_HPP_

#include "../ch/stdlib.hpp"
#include "../ch/boost.hpp"

#include "HTTPHeader.hpp"

namespace rebindr {
namespace http {

shared_ptr<HTTPRequest> parseHttpHeader(string& input);


} /* namespace http */
} /* namespace rebindr */

#endif /* HTTPUTILS_HPP_ */
