#include "server.hpp"

int main() {
	server local("local", "8080");
	local.start();
}
