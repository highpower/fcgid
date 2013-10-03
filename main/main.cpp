#include "acsetup.hpp"

#include <cstdlib>
#include <iostream>
#include <exception>

#include "fastcgi-daemon/server.hpp"
#include "fastcgi-daemon/endpoint.hpp"

int
main(int argc, char *argv[]) {

	try {
		using namespace fcgid;
		
		server s;
		s.listen(endpoint::create_unix_socket("/tmp/fcgid-test.sock", 0));
		s.start(thread_count_type(10));
		return EXIT_SUCCESS;
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
