#include <stdio.h>

int bitcount (unsigned int n)
{
    register unsigned int tmp;
    tmp = n - ((n >> 1) & 033333333333)
          - ((n >> 2) & 011111111111);
    return ((tmp + (tmp >> 3)) & 030707070707) % 63;
}


int n, q[81];

int check(int *p)
{
    int i, j;

    for (i = 0; i < 81; i++)
    {
        int box_a = 0, box_b = 0, row_a = 0, row_b = 0, col_a = 0, col_b = 0;
        j = 0;
        while (j < 9)
        {
            box_a |= p[i / 27 * 27 + j / 3 * 6 + i % 9 / 3 * 3 + j];
            box_b += p[i / 27 * 27 + j / 3 * 6 + i % 9 / 3 * 3 + j]; 

            col_a |= p[j * 9 + i % 9];
            col_b += p[j * 9 + i % 9];

            row_a |= p[i / 9 * 9 + j];
            row_b += p[i / 9 * 9 + j];

            j++;
        }

        if ( row_a - row_b || col_a - col_b || box_a - box_b )
        {
            int o;
            printf("i=%d ( row %d %d , col %d %d , box %d %d )\n", i, row_a, row_b, col_a, col_b, box_a , box_b );
            puts("");
            for (n = 0;n < 81;n++)
            {
                for (o = 48; p[n] /= 2; o++)
                    ;
                putchar(o);
                if ( n%3 == 2)
                    putchar(' ');
                if ( n%9 == 8)
                    putchar('\n');
            }
            return 1;
        }
    }



    return 0;
}

int main()
{
    int o;

    for (n = 0;n < 81;n++)
    {

        while ((o = getchar()) < 33)
            ;
        o ^= 48;
        q[n] = (o > 9 || o == 0) ? 0 : 1 << o;
    }

    if ( check(q) )
    {
        printf(": Illegal\n");
        return 0;
    }

    return 0;
}
