SOURCES=$(wildcard affix-base/*.cpp)
CPPFLAGS+=-fpermissive
CPPFLAGS+=-I.
CPPFLAGS+=-I"asio/asio/include/"
CPPFLAGS+="cryptopp/libcryptopp.a"

affix-base.a: $(SOURCES) cryptopp/libcryptopp.a
	make -C cryptopp/
	g++ -std=c++20 -c $(CPPFLAGS) $(SOURCES)
	ar rvs affix-base.a *.o

