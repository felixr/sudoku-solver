rssolve: rssolve.c
	$(CC) -std=c99 -O9 -Wall -o $@ $<

check: check.c
	$(CC) -O3 -Wall -o $@ $<
