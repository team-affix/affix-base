CPPFLAGS+=-fpermissive
CPPFLAGS+=affix-base/*.cpp
CPPFLAGS+=-I.
CPPFLAGS+=-I"asio/asio/include/"
CPPFLAGS+="cryptopp/libcryptopp.a"

ifneq ($(OS),Windows_NT)
obj.o: affix-base/*.cpp
	make -C cryptopp/
	g++ -std=c++20 -c $(CPPFLAGS)
	ar rvs affix-base.a *.o
else
obj.o: affix-base/*.cpp
	make -C cryptopp/
	g++ -std=c++20 -c $(CPPFLAGS)
	ar rvs affix-base.a *.o
endif

