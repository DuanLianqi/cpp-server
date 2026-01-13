#include <cstring>
#include "InetAddress.h"

InetAddress::InetAddress() : addrLen(sizeof(addr)) {
    bzero(&addr, addrLen);
}

InetAddress::InetAddress(const char *ip, uint16_t port) : addrLen(sizeof(addr)) {
    bzero(&addr, addrLen);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
}

InetAddress::~InetAddress() {

}
