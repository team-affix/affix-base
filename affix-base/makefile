SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

BINARY=affix-base.a

CPPFLAGS+=-std=c++20 -c -fpermissive -I. -I"../" -I"../asio/asio/include/"

all: $(BINARY)

$(BINARY): $(OBJECTS)
	# Builds the final library (.a file)
	ar rvs $(BINARY) $(OBJECTS)
	
%.o: %.c
	# Build a single .o file
	g++ $(CPPFLAGS) -o $@ $^
	
clean:
	rm -rf $(OBJECTS)
	rm -rf $(BINARY)

