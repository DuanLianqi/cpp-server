#include <iostream>
#include <cstring>
#include <unistd.h>
#include "src/util.h"
#include "src/InetAddress.h"
#include "src/Socket.h"
#include "src/Buffer.h"

#define MAX_BUFFER    1024

int main() {
    Socket *clientSock = new Socket();
    InetAddress *servAddr = new InetAddress("127.0.0.1", 12345);
    clientSock->connect(servAddr);

    Buffer *sendBuffer = new Buffer();
    Buffer *readBuffer = new Buffer();

    while(true) {
        sendBuffer->getline();
        ssize_t writeBytes = write(clientSock->getFd(), sendBuffer->c_str(), sendBuffer->size());
        if(writeBytes == -1) {
            std::cout << "Server has already disconnected, can't write any more." << std::endl;
            break;
        }

        ssize_t alreadyReadBytes = 0;
        char buf[MAX_BUFFER] = {0};
        while(true) {
            bzero(buf, MAX_BUFFER);

            ssize_t readBytes = read(clientSock->getFd(), buf, MAX_BUFFER);
            if(readBytes > 0) {
                readBuffer->append(buf, readBytes);
                alreadyReadBytes += readBytes;
            } else if(readBytes == 0) {
                std::cout << "Server has already disconnected." << std::endl;
                exit(EXIT_SUCCESS);
            }

            if(readBytes >= sendBuffer->size()) {
                std::cout << "There has a message from server, context is : " << readBuffer->c_str() << std::endl;
                break;
            }
        }
        readBuffer->clear();
    }

    delete clientSock;
    delete servAddr;
    delete sendBuffer;
    delete readBuffer;
    return 0;
}
