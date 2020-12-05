#ifndef WEBSERV_HTON_HPP
#define WEBSERV_HTON_HPP
#include <arpa/inet.h>

uint32_t ft_htonl(uint32_t hostlong) {
	long ui = 0;

	ui |= (hostlong & 0xFF000000) >> 24;
	ui |= (hostlong & 0x00FF0000) >> 8;
	ui |= (hostlong & 0x0000FF00) << 8;
	ui |= (hostlong & 0x000000FF) << 24;
	return (ui);
}

uint16_t ft_htons(uint16_t hostshort) {
	long ui = 0;

	ui |= (hostshort & 0xFF) << 8;
	ui |= (hostshort & 0xFF00) >> 8;
	return (ui);
}

#endif //WEBSERV_HTON_HPP
