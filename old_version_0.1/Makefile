app:main.o http_conn.o
	g++ -Wall -g main.o http_conn.o -o app
main.o:
	g++ -Wall -g -c main.cpp -o main.o
http_conn.o:
	g++ -Wall -g -c http_conn.cpp -o http_conn.o

clean:
	rm -r *.o app