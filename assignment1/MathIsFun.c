#include <stdio.h>
#include "MathIsFun.h"

/*
 * Calculates the largest integer less or equal than the square root of x.
 * funSqrt(10) = 3
 * funSqrt(16) = 4
 * @param x - An integer for which the function applies
 * @return the value of |_sqrt(x)_|, if x is negative then the function
 * returns -1.
 */
int funSqrt(int x); // ==> Declaration is always in the beginning of the file.

int funPow(int x, int n, int d) {
	int to_return;
	if (n < 0)
		return 0;
	else if (n == 0)
		return 1;
	else if (n % 2 == 0)
	{
		int returned_value = funPow(x, n/2, d);
		to_return = returned_value * returned_value % d;
	}
	else if (n % 2 == 1)
	{
		int returned_value = funPow(x, (n-1)/2, d);
		to_return = x * returned_value * returned_value % d;
	}
	return d > 0 ? (to_return > 0 ? to_return : to_return + d) : to_return;
}

int funSqrt(int x) {
	if (x < 0)
		return -1;
	int i;
	for (i = 0; i * i <= x; i++)
		continue;
	return i - 1;
}

bool funPrimeCheck(int x) {
	if (x <= 0)
		return false;
	int i;
	for (i = 2; i <= funSqrt(x); i++)
		 if (x % i == 0)
			return false;
	return true;
}

int getNumberOfDigits(int x) {
	int i = 0;
	while ( x != 0)
	{
		i ++;
		x = x / 10;
	}
	return i;
}

bool funPalindromeCheck(int x) {
	if (x < 0)
		return false;
	else if (x == 0)
		return true;
	int nDigits = getNumberOfDigits(x);
	int i;
	char firstDigit;
	char lastDigit;
	char xStr[nDigits];
	sprintf(xStr, "%d", x);
	for (i = 0; i < nDigits / 2; i++)
	{
		firstDigit = xStr[nDigits - (i + 1)];
		lastDigit = xStr[i];
		if (firstDigit != lastDigit)
			return false;
	}
	return true;
}
