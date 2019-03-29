all: blockchain.cc
	c++ blockchain.cc -std=c++11 `pkg-config --cflags --libs openssl`

.PHONY: clean
clean:
	rm -rf a.out*

