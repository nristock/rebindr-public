AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = $(ACLOCAL_FLAGS) -I m4 -I shave


bin_PROGRAMS = rebindr
rebindr_SOURCES =   src/rebindr.cpp \

                    src/http/HTTPUtils.cpp \
                    src/http/target/TargetRegistry.cpp \
                    src/http/admin/AdminServer.cpp \
                    src/http/admin/AdminTcpConnection.cpp \
                    src/http/attack/AttackServer.cpp \
                    src/http/attack/AttackTcpConnection.cpp \
                    src/http/callback/CallbackServer.cpp \
                    src/http/callback/CallbackTcpConnection.cpp \

                    src/dns/DNSQueryHandler.cpp \
                    src/dns/DNSUtils.cpp \
                    src/dns/udp/UdpDnsServer.cpp \
                    src/dns/tcp/TcpDnsConnection.cpp \
                    src/dns/tcp/TcpDnsServer.cpp

rebindr_CPPFLAGS = $(BOOST_CPPFLAGS)
rebindr_LDFLAGS = $(BOOST_SYSTEM_LDFLAGS) $(BOOST_THREAD_LDFLAGS) $(BOOST_FILESYSTEM_LDFLAGS) $(BOOST_REGEX_LDFLAGS) -ldl
rebindr_LDADD = $(BOOST_SYSTEM_LIBS) $(BOOST_THREAD_LIBS) $(BOOST_FILESYSTEM_LIBS) $(BOOST_REGEX_LIBS) -ldl
