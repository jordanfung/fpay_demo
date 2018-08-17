include ../Makefile.inc

CXXFLAGS += -I../server -I. -std=c++11
.PHONY: all clean
all:
	$(CXX) -o test $(COMM_LIB) $(LDFLAGS) $(CXXFLAGS) -DJSON_NEST test.cpp 
	$(CXX) -o initSuperWallet initSuperWallet.cpp $(COMM_LIB) $(LDFLAGS) $(CXXFLAGS) -DJSON_NEST -lhiredis -lglog -lgflags -ltinyxml
clean:
	rm test
	rm initSuperWallet
