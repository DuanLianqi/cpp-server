server:
	g++ src/util.cpp src/InetAddress.cpp src/Socket.cpp src/Epoll.cpp src/Channel.cpp src/EventLoop.cpp src/Server.cpp  server.cpp -o server && \
	g++ src/util.cpp src/InetAddress.cpp src/Socket.cpp client.cpp -o client
clean:
	rm server && rm client