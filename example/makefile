.PHONY: build
build:
	g++ -o testServer testServer.cpp -lmymuduo -lpthread
.PHONY: clean
clean:
	rm -f testServer

.PHONY: run
run:
	make clean
	make build
	./testServer