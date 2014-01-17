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
 * boost.hpp
 *
 * Convenience header - includes basic boost related stuff
 *
 *  Created on: Jan 4, 2014
 *      Author: monofraps
 */

#ifndef BOOST_HPP_
#define BOOST_HPP_

#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>

using boost::array;
using boost::is_any_of;
using boost::shared_ptr;

using boost::algorithm::split_regex;

#endif /* BOOST_HPP_ */
