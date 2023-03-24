.PHONY: all test clean

#PATH=$PATH:yournewpath
#PATH=~/xiao/sqlite3/install

 
all:
#	gcc -shared -fPIC -o libcli.so myDns.c

client:
	@gcc -shared -fPIC -o libcli.so myDns.c client.c getTime.c get_temp.c mySocket.c myTimer.c
	@gcc ./client.c -o cli.out -I ./ -lcli -L ./
	./cli.out -i 127.0.0.1 -p 6666

server:
	@gcc -shared -fPIC -o libser.so myDns.c client.c getTime.c get_temp.c mySocket.c myTimer.c
	@gcc ./server.c -o ser.out -I ./ -lser -L ./
#	./ser.out -i0.0.0.0 -p 6666
	./ser.out -p 6666

sql:
#	gcc sql.c -I -lsqlite3 -L ${PATH}/lib -o sql
#	./sql

uninstall:
	@rm -f *.out *.so

