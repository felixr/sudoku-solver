rssolve: rssolve.c
	$(CC) -O9 -march=i686 -Wall -o $@ $<

check: check.c
	$(CC) -O3 -march=i686 -Wall -o $@ $<
