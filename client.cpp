#include <iostream>
#include <cstring>
#include <unistd.h>
#include "util.h"
#include "InetAddress.h"
#include "Socket.h"

#define MAX_BUFFER    1024

int main() {
    Socket *clientSock = new Socket();
    InetAddress *servAddr = new InetAddress("127.0.0.1", 12345);
    clientSock->connect(servAddr);

    while(true) {
        char buf[MAX_BUFFER] = {0};
        std::cin >> buf;
        ssize_t writeBytes = write(clientSock->getFd(), buf, sizeof(buf));
        if(writeBytes == -1) {
            std::cout << "Server has already disconnected, can't write any more." << std::endl;
            break;
        }

        bzero(buf, sizeof(buf));
        ssize_t readBytes = read(clientSock->getFd(), buf, sizeof(buf));
        if(readBytes > 0) {
            std::cout << "There has a message from server, context is : " << buf << std::endl;
        } else if(readBytes == 0) {
            std::cout << "Server has already disconnected." << std::endl;
            break;
        } else if(readBytes == -1) {
            errif(true, "socket read error!");
            break;
        }
    }

    delete clientSock;
    delete servAddr;
    return 0;
}
