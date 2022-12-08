<<<<<<< HEAD
.PHONY: echo
run:
	rm -f testServer
	g++ -o testServer testServer.cpp -lmymuduo -lpthread
	./testServer

.PHOMY: http
http:
	rm -f testHttpServer
	g++ -o testHttpServer testHttpServer.cpp -lmymuduo -lpthread
	./testHttpServer
=======
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
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
