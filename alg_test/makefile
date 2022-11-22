SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
CRYPTOPP_LIB=../cryptopp/libcryptopp.a
AFFIX_BASE_LIB=../affix-base/affix-base.a

CPPFLAGS+=-g
CPPFLAGS+=-std=c++20
CPPFLAGS+=-pthread
CPPFLAGS+=-I.
CPPFLAGS+=-I"../"
CPPFLAGS+=-I"../affix-base/"
CPPFLAGS+=-I"../asio/asio/include/"

BINARY=main

all: $(BINARY)

$(BINARY): $(OBJECTS) $(AFFIX_BASE_LIB) $(CRYPTOPP_LIB)
	# Build the final binary
	g++ $(CPPFLAGS) -o $@ $^
	
%.o: %.c
	# Build a single object file
	g++ $(CPPFLAGS) -c -o $@ $^
	
$(CRYPTOPP_LIB):
	# Build cryptopp
	make -C ../cryptopp/
	
$(AFFIX_BASE_LIB):
	# Build affix-base
	make -C ../affix-base/
	
clean:
	rm -rf *.o
	rm -rf $(BINARY)

