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
  * rebindr.cpp
  *
  *  Created on: Nov 16, 2013
  *      Author: monofraps
  */

#include "dns/udp/UdpDnsServer.h"
#include "dns/tcp/TcpDnsServer.h"
#include "http/attack/AttackServer.h"
#include "http/callback/CallbackServer.h"
#include "http/admin/AdminServer.h"
#include "http/target/TargetRegistry.h"

#define toChar(value) (char*)&value;

int main() {

	try {
		boost::asio::io_service ioService;

		rebindr::dns::UdpDnsServer dnsServer(ioService);
		rebindr::dns::TcpDnsServer tcpDnsServer(ioService);

		rebindr::http::target::TargetRegistry::init(ioService);

		rebindr::http::attack::HTTPAttackServer httpAttackServer(ioService);
		rebindr::http::callback::HTTPCallbackServer httpCallbackServer(ioService);
		rebindr::http::admin::HTTPAdminServer httpAdminServer(ioService);
		ioService.run();
	} catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
