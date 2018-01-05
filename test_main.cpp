// NewProj.cpp : Defines the entry point for the console application.

//#include "stdafx.h" //this project was tested on MS-VC++ where this line is mandatory
#include "fluidpt.h"

int main(int argc, char* argv[])
{
	fixedpt<16> fx1; fx1 = (5.3f);
	fixedpt<10> fx2; fx2 = (5.3f);//fx1;
	fx2 -= 3;

	printf("%d\n", (int)fx1);
	printf("%d\n", (int)fx1);
	printf("%f\n", (float)fx1);

	printf("%f\n", (float)fx2);
	printf("0x%x\n", fx2.asFixed<16>());

	fx1 -= fx2;
	fx1 += 1;

	float test[] = { .35f, 1.3f, 1.9f, 2.3f, 5.4f, 36.5f };
	for (int i = 0; i < 6; i++) {
		fixedpt<24> fx4 = test[i];
		printf("%f\t", (float)fx4);
		fixedpt<28> fx5;
		fx5.inverse(fx4);
		printf("%f\t", (float)fx5);
		fixedpt<20> fx6;
		fx6.multiply(fx4,fx5);
		printf("%f\n", (float)fx6);
	}

	floatpt fx4, fx5, fx6;
	for (int i = 0; i < 6; i++) {
		fx4 = test[i];
		printf("%f\t", (float)fx4);
		fx5.inverse(fx4);
		printf("%f\t", (float)fx5);
		fx6.multiply(fx4, fx5);
		printf("%f\n", (float)fx6);
	}
	return 0;
}

