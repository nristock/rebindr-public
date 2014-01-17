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
 * TargetRequset.hpp
 *
 *  Created on: Jan 2, 2014
 *      Author: monofraps
 */

#ifndef TARGETREQUSET_HPP_
#define TARGETREQUSET_HPP_

#include <string>
#include <functional> // boost/functinal.hpp breaks compilation somehow

using std::string;
using std::function;

namespace rebindr {
namespace http {
namespace target {

struct TargetRequest {
	int id;
	string authorization;
	string postData;
	string url;
	function<void(string)> callback;
};

}
}
}

#endif /* TARGETREQUSET_HPP_ */
