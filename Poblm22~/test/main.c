#include <stdio.h>

void main() {
	float a, b, c;
	a = 52.0;
	b = 256.0;
	c = a / b;
	printf("%f\n", c);
	c *= b;
	printf("%f\n", c);
}