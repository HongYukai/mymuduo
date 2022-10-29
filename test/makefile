.PHONY:compile
compile:
	g++ -c test.cpp -o timer_test.o
.PHONY:link
link:
	g++ -o timer_test timer_test.o
.PHONY:clean
clean:
	rm -f *.o timer_test
.PHONY:run
run:
	make compile
	make link
	./timer_test