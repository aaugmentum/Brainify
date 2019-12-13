gcc client.c -o client.o 
gcc -pthread server.c -o server.o -lmysqlclient
./server.o
