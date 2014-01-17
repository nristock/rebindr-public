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
 * boostnet.hpp
 *
 * Convenience header - includes basic boost networking related stuff
 *
 *  Created on: Jan 4, 2014
 *      Author: monofraps
 */

#ifndef BOOSTNET_HPP_
#define BOOSTNET_HPP_

#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>

#include <boost/system/error_code.hpp>

using boost::asio::streambuf;
using boost::asio::io_service;
using boost::asio::deadline_timer;

using boost::asio::ip::udp;
using boost::asio::ip::tcp;
using boost::asio::ip::address;

using boost::system::error_code;


#endif /* BOOSTNET_HPP_ */
