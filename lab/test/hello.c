#include <stdio.h>

int func(int, int);
int func2(char, char);

int i = 3;
char c = 'a';

int main(void)
{
	int j, k;
	char a, b;
	a = 'a';
	b = 'b';
	j = 100;

	k = func(i, j);
	if(a == b)
		printf("%d\n", k);
	else
		printf("%d\n", func2(a,b));
	return 0;
}

int func(int a, int b)
{
	return a+b;
}

int func2(char a, char b)
{
	int i,j;
	i = 9;
	j = 10;
	func(i,j);
	return a == b;
}
