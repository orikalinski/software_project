#include <stdio.h>
#include "MathIsFun.h"

int main(int argc, const char* argv[])
{
	int operation;
	printf("Welcome to Math Is Fun - beta version\n"
	       "Supported operations are:\n"
	       "1 - Power Calculation\n"
	       "2 - Prime Check\n"
	       "3 - Palindrome Check\n"
	       "Please enter operation number (1/2/3): \n");
	scanf("%d", &operation);
	if (operation == 1)
	{
		int x, n, d;
		printf("Please enter three space separated numbers: \n");
		scanf("%d %d %d", &x, &n, &d);
		printf("res = %d\n", funPow(x, n, d));
	}
	else if (operation == 2 || operation == 3)
	{
		int x, res;
		printf("Please enter an integer: \n");
		scanf("%d", &x);
		res = operation == 2 ? funPrimeCheck(x) : funPalindromeCheck(x);
		printf("res = %s\n", res == 1 ? "true" : "false");
	}
	else
	{
		printf("You entered invalid number\n");
	}

}
