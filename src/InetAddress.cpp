#include <cstring>
#include "InetAddress.h"

InetAddress::InetAddress() {
    memset(&addr, 0, sizeof(addr));
}

InetAddress::InetAddress(const char *ip, uint16_t port) {
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
}

InetAddress::~InetAddress() {

}

void InetAddress::setInetAddr(sockaddr_in _addr) {
    addr = _addr;
}

sockaddr_in InetAddress::getAddr() const {
    return addr;
}

socklen_t InetAddress::getAddrLen() const {
    return sizeof(addr);
}
