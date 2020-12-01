#include "server.hpp"

int main() {
	server local("local", "80");
	local.start();
}
