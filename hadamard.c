#include <stdio.h>

int main()
{
    int a[12][12] = {
	{1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1},
	{1, 0, 1, 0, 1, 1,   1, 0, 0, 0, 1, 0},
	{1, 0, 0, 1, 0, 1,   1, 1, 0, 0, 0, 1},
	{1, 1, 0, 0, 1, 0,   1, 1, 1, 0, 0, 0},
	{1, 0, 1, 0, 0, 1,   0, 1, 1, 1, 0, 0},
	{1, 0, 0, 1, 0, 0,   1, 0, 1, 1, 1, 0},

	{1, 0, 0, 0, 1, 0,   0, 1, 0, 1, 1, 1},
	{1, 1, 0, 0, 0, 1,   0, 0, 1, 0, 1, 1},
	{1, 1, 1, 0, 0, 0,   1, 0, 0, 1, 0, 1},
	{1, 1, 1, 1, 0, 0,   0, 1, 0, 0, 1, 0},
	{1, 0, 1, 1, 1, 0,   0, 0, 1, 0, 0, 1},
	{1, 1, 0, 1, 1, 1,   0, 0, 0, 1, 0, 0},
    };

    for(int i=1; i<12; i++)
    {
	if(a[i][1] == 1)
	{
	    for(int j=2; j<12; j++)
		printf("%d ", a[i][j]);
	    printf("\n");
	}
    }

    return 0;
}
