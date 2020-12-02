#include "server.hpp"

int main() {
	server local("local", "8081");
	local.start();
}
