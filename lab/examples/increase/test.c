#include <stdio.h>

int main(void)
{
	int x = 2;
	printf("Hello world\n");

	if(x == 3)
	{
		//many instructions are here, but not executed
		printf("x is 2\n");
		x = 3+4;
		x = 39;
		x = 3.4 + 32 * 32 / 32 + 42;
	}
	return 0;
}
