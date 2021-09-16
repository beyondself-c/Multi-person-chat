all:pollclient pollserver

pollserver:pollserver.cpp
	g++ -g -o pollserver pollserver.cpp _public.cpp

pollclient:pollclient.cpp
	g++ -g -o pollclient pollclient.cpp

clean:
	rm -f pollclient pollserver
