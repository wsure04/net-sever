target=tcpepoll client

all:$(target)

tcpepoll:tcpepoll.cpp
	g++ tcpepoll.cpp -o tcpepoll
client:client.cpp
	g++ client.cpp -o client

clean:
	rm -rf $(target)