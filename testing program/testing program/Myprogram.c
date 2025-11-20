#include<stdio.h>
#include"mymath.h"
int sum(int op1, int op2);
int main() {
	int num1, num2;
	scanf_s("%d%d", &num1, &num2);
	printf("the max number is %d\n", max(num1, num2));
	printf("the min number is %d\n", min(num1, num2));
	printf("the sum is %d\n", sum(num1, num2));
	return 0;
}

int sum(int op1, int op2) {
	return (op1 + op2);
}