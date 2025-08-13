target=tcpepoll client

all:$(target)

tcpepoll:tcpepoll.cpp InetAddress.cpp
	g++ InetAddress.cpp tcpepoll.cpp -o tcpepoll
client:client.cpp
	g++ client.cpp -o client

clean:
	rm -rf $(target)