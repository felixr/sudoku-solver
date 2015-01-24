rssolve: rssolve.c
	$(CC) -O9 -Wall -o $@ $<

check: check.c
	$(CC) -O3 -Wall -o $@ $<
