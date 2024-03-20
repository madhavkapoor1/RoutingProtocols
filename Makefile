all: lsr dvr

lsr: src/linkstate.cpp src/common.cpp src/common.hpp
	g++ -g -o lsr src/linkstate.cpp src/common.cpp -Iinclude

dvr: src/distancevector.cpp src/common.cpp src/common.hpp
	g++ -g -o dvr src/distancevector.cpp src/common.cpp -Iinclude

clean:
	rm -f lsr dvr
