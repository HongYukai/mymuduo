.PHONY:compile
compile:
	g++ -c test.cpp -o timer_test.o
	g++ -c test_timerHeap.cpp -o timerHeap_test.o
.PHONY:link
link:
	g++ -o timer_test timer_test.o
	g++ -o timerHeap_test timerHeap_test.o -lmymuduo -lpthread
.PHONY:clean
clean:
	rm -f *.o timer_test timerHeap_test
.PHONY:run-timer
run-timer:
	make compile
	make link
	./timer_test
.PHONY:run-th
run-th:
	make compile
	make link
	./timerHeap_test

