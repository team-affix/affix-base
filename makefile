all:
	make -C affix-base/
	make -C cryptopp/
	make -C alg_test/
	
clean:
	make clean -C affix-base/
	make clean -C cryptopp/
	make clean -C alg_test/

