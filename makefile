DEFAULT_PORT=20222
all:	client server
client:
	gcc -o client client.c
	echo "./client localhost "$(DEFAULT_PORT) > client.sh
	chmod 766 client.sh
server:
	gcc -o server server.c
	echo "./server "$(DEFAULT_PORT) > server.sh
	chmod 766 server.sh
clean:
	rm client server client.sh server.sh
