target=tcpepoll client
include=InetAddress.cpp Socket.cpp Epoll.cpp Channel.cpp EventLoop.cpp TcpServer.cpp
all:$(target)

tcpepoll:tcpepoll.cpp $(include)
	g++ $(include) tcpepoll.cpp -o tcpepoll
client:client.cpp
	g++ client.cpp -o client

clean:
	rm -rf $(target)