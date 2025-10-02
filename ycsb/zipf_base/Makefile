CC=g++
CFLAGS=-std=c++11 -g -Wall -pthread -I./
LDFLAGS= -lpthread -ltbb
ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif
LIBPATH=$(PREFIX)/lib
BINPATH=$(PREFIX)/bin

zipf_gen: src/zipf_gen.cpp
	mkdir -p bin
	$(CC) $(CFLAGS) $(LDFLAGS) src/zipf_gen.cpp -o bin/zipf_gen

install: zipf_gen
	cp bin/zipf_gen $(BINPATH)/zipf_gen
	chmod +x $(BINPATH)/zipf_gen

clean:
	rm -rf bin/zipf_gen

uninstall: zipf_gen
	rm -rf $(BINPATH)/zipf_gen
