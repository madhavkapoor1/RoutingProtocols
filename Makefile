all: lsr dvr

lsr: src/linkstate.cpp
	g++ -o lsr src/linkstate.cpp -Iinclude

dvr: src/distancevector.cpp
	g++ -o dvr src/distancevector.cpp -Iinclude

clean:
	rm -f lsr dvr
