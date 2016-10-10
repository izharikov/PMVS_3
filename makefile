DEFAULT_PORT=20219
DEBUG_PARAM=-DDEF_PROCESS
all: client server

client:
	gcc ${DEBUG_PARAM} -o client client.c
server:
ifdef DEBUG_PARAM
	gcc ${DEBUG_PARAM} -o server server.c
else
	gcc ${DEBUG_PARAM} -pthread -o server server.c
endif
clean:
	rm client server
