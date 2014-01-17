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
 * stdlib.hpp
 *
 * Convenience header - includes basic std lib stuff
 *
 *  Created on: Jan 4, 2014
 *      Author: monofraps
 */

#ifndef STDLIB_HPP_
#define STDLIB_HPP_

#include <ctime>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <queue>
#include <map>

#include <iostream>

using std::string;
using std::stringstream;

using std::list;
using std::vector;
using std::map;
using std::queue;

using std::ostream;

using std::cout;
using std::endl;

using std::size_t;
using std::time_t;

using std::to_string;


typedef vector<string> StringVector;
typedef StringVector::iterator StringVectorIterator;

#endif /* STDLIB_HPP_ */
