DEFAULT_PORT=20219
DEBUG_PARAM=-DDEF_PROCESS
#-DDEF_PROCESS
all: client server

client:
	gcc ${DEBUG_PARAM} -o client client.c
server:
	gcc ${DEBUG_PARAM} -pthread -o server server.c
clean:
	rm client server
