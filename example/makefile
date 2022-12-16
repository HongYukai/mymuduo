.PHONY: echo
echo:
	rm -f testServer
	g++ -o testServer testServer.cpp -lmymuduo -lpthread
	./testServer

.PHOMY: http
http:
	rm -f testHttpServer
	g++ -o testHttpServer testHttpServer.cpp -lmymuduo -lpthread
	sh make.sh $(n) $(p)

