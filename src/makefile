target=echo-server client
SOURCES=InetAddress.cpp Socket.cpp Epoll.cpp Channel.cpp EventLoop.cpp TcpServer.cpp Acceptor.cpp Connection.cpp Buffer.cpp EchoServer.cpp ThreadPool.cpp
all:$(target)

echo-server:echo-server.cpp $(SOURCES)
	g++ $(SOURCES) echo-server.cpp -o echo-server -lpthread
client:client.cpp
	g++ client.cpp -o client

clean:
	rm -rf $(target)