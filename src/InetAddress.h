#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <arpa/inet.h>

class InetAddress {
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();

    void setInetAddr(sockaddr_in addr);
    sockaddr_in getAddr() const;
    socklen_t getAddrLen() const;

private:
    struct sockaddr_in addr;
};

#endif
