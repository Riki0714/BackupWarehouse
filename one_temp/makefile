.PHONY: all test clean

PRONAME_ONE=client

all:   
#@echo "hello!"
	@gcc -shared -fPIC -o libonePro.so base/*.c
	@gcc ./base/client.c -I ./base/ -lonePro -L ./ -o test
	./test -i 127.0.0.1 -p 8888

client:
	gcc ${PRONAME_ONE}.c -o client_o
	./client_o -i 127.0.0.1 -p 8888

