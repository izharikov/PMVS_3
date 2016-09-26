DEFAULT_PORT=20226
DEBUG_PARAM=-DDEF_PROCESS
#-DDEF_PROCESS
all:	client server
client:
	gcc ${DEBUG_PARAM} -o client client.c
	echo "./client localhost "$(DEFAULT_PORT) > client.sh
	chmod 766 client.sh
server:
	gcc ${DEBUG_PARAM} -pthread -o server server.c
	echo "./server "$(DEFAULT_PORT) > server.sh
	chmod 766 server.sh
clean:
	rm client server client.sh server.sh
