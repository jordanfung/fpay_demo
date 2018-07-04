CXXFLAG = -I../core/base -I../3parts/include -I.
.PHONY: all clean
all:
	g++ -Werror -std=c++11 -o test -O3 -lpthread -lz  $(CXXFLAG)  test.cpp -lpthread -DJSON_NEST
clean:
	rm test
