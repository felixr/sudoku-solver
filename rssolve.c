/* vim: set sw=4:  */

#include <stdio.h>
#include <time.h>
#include <sys/resource.h>

#define RESOLUTION              1000000L

static long t()
{
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return (ru.ru_utime.tv_sec + ru.ru_stime.tv_sec) * RESOLUTION + ru.ru_utime.tv_usec + ru.ru_stime.tv_usec;
}


#define TABLE

#ifndef TABLE
#define BOX(X,R,C) X[R / 27 * 27 + C / 3 * 6 + R % 9 / 3 * 3 + C]
#define ROW(X,R,C) X[R / 9 * 9 + C]
#define COL(X,R,C) X[C * 9 + R % 9]
#define GETCOVER(C,I,J) { for (J = 0; J < 9; J++) { a |= BOX(C,I,J) | COL(C,I,J) | ROW(C,I,J); }}

#else

#define N 9
static const int cellbox_map[N*N] =
    {
        0, 0, 0, 1, 1, 1, 2, 2, 2,
        0, 0, 0, 1, 1, 1, 2, 2, 2,
        0, 0, 0, 1, 1, 1, 2, 2, 2,
        3, 3, 3, 4, 4, 4, 5, 5, 5,
        3, 3, 3, 4, 4, 4, 5, 5, 5,
        3, 3, 3, 4, 4, 4, 5, 5, 5,
        6, 6, 6, 7, 7, 7, 8, 8, 8,
        6, 6, 6, 7, 7, 7, 8, 8, 8,
        6, 6, 6, 7, 7, 7, 8, 8, 8
    };

static const int boxes[N][N] =
    {
        { 0, 1, 2, 9, 10, 11, 18, 19, 20},
        { 3, 4, 5, 12, 13, 14, 21, 22, 23},
        { 6, 7, 8, 15, 16, 17, 24, 25, 26},
        {27, 28, 29, 36, 37, 38, 45, 46, 47},
        {30, 31, 32, 39, 40, 41, 48, 49, 50},
        {33, 34, 35, 42, 43, 44, 51, 52, 53},
        {54, 55, 56, 63, 64, 65, 72, 73, 74},
        {57, 58, 59, 66, 67, 68, 75, 76, 77},
        {60, 61, 62, 69, 70, 71, 78, 79, 80}
    };

#define BOX(X,I,O) X[ boxes[ cellbox_map[I]][O]]
#define FBOX(X,I,O) X[ boxes[I][O]]
#define ROW(X,R,C) X[R / 9 * 9 + C]
#define COL(X,R,C) X[C * 9 + R % 9]

#define GETCOVER(C,I,J) { int box=cellbox_map[i]; \
        for (J = 0; J < 9; J++) {\
            a |= FBOX(C,box,J) | COL(C,I,J) | ROW(C,I,J);\
        }}

#endif


#define TWO(c)     (0x1u << (c))
#define MASK(c)    (((unsigned int)(-1)) / (TWO(TWO(c)) + 1u))
#define COUNT(x,c) ((x) & MASK(c)) + (((x) >> (TWO(c))) & MASK(c))

int bitcount (unsigned int n)
{
    n = COUNT(n, 0) ;
    n = COUNT(n, 1) ;
    n = COUNT(n, 2) ;
    n = COUNT(n, 3) ;
    n = COUNT(n, 4) ;
    /* n = COUNT(n, 5) ;    for 64-bit integers */
    return n ;
}


int solutions, n, q[81];



inline static void print_sudoku(int *s)
{
    int c;
    for (n = 0;n < 81;n++)
    {
	/* 48 = '0', 49 = '1',... */
	for (c = 48; s[n] /= 2; c++);
	putchar(c);
	if (n % 9 == 8)
	    puts("");
    }
    puts("");
}

/*
 * check soduko rules
 */
int check(int *p)
{
    int i, j;
    for (i = 0; i < 81; i++)
    {
        int box_a = 0, box_b = 0, row_a = 0, row_b = 0, col_a = 0, col_b = 0;
        j = 0;
        while (j < 9)
        {
            box_a |= BOX(p, i, j); box_b += BOX(p, i, j);
            col_a |= COL(p, i, j); col_b += COL(p, i, j);
            row_a |= ROW(p, i, j); row_b += ROW(p, i, j);

            j++;
        }

        if ( row_a - row_b || col_a - col_b || box_a - box_b )
        { //printf("%d %d , %d %d , %d %d \n", row_a, row_b, col_a, col_b, box_a , box_b );
            return 1;
        }
    }
    return 0;
}

int solve(int *p)
{
    int i = 81, j = 0, a = 0;
    int x = 1;
    int c[81];

    for (n = 0; n < 81; n++)
        c[n] = p[n];

    /* find forced cells */
    while (x)
    {
        x = 0;
        for (i = 0; i < 81; i++)
        {
            if ( c[i] == 0 )
            {
                a = 0;
                GETCOVER(c, i, j);
                if ( bitcount(a) == 8 )
                {
                    int d;
                    for ( d = 2; d < 1024; d *= 2)
                        if ( ~a & d )
                        {
                            c[i] = d;
                            x++;
                            break;
                        }
                }
            }
        }
    }


    i = -1;
    for (n = 80; n > 0 ; n--)
        if (c[n] == 0 )
        {
            i = n;
            break;
        }

    if ( i >= 0 )
    {
        GETCOVER(c, i, j);
        for (j = 2;j < 1024;j *= 2)
            if (~a & j)
            {
                c[i] = j;
                solve(c);
            }
        return 0;
    }
    else
    {
        solutions++;
        return 1;
    }

    return n == i;
}

int main()
{
    int o;
    long t1, t2;
    float f;
    int c = 1;


    while (1)
    {
        for (n = 0;n < 81;n++)
        {
            while ((o = getchar()) != EOF && o < 33)
                ;
            if ( o == EOF)
                return 0;
            o ^= 48;
            q[n] = (o > 9 || o == 0) ? 0 : 1 << o;
        }

        t1 = t();


        if ( check(q) )
        {
            solutions = -1;
        }
        else
        {
            solutions = 0;
            solve(q);
        }

        switch ( solutions )
        {
            case 0:
                printf("Case %d: Impossible.\n", c);
                break;
            case 1:
                printf("Case %d: Unique.\n", c);
                break;
            case - 1:
                printf("Case %d: Illegal.\n", c);
                break;
            default:
                printf("Case %d: Ambigous.\n", c);
                break;
        }

        t2 = t();
        f = (t2 - t1);
        printf("time: %e millisecs solutions: %d\n", f, solutions) ;


        c++;
    }
    return 0;
}