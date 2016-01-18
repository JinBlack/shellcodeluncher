all:
	gcc -m32 tester.c -o tester_32
	gcc -m64 tester.c -o tester_64

clean:
	rm tester_32 tester_64