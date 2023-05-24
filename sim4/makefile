test: test_01_getInstruction.c sim4_test_commonCode.h sim4.o
	gcc test_01_getInstruction.c sim4.o -o test
sim4.o: sim4.c sim4.h
	gcc -c sim4.c
clean:
	rm -f sim4.o
	rm -f test
