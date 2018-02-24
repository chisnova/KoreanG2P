#include <stdio.h>
#include "function.h"
#include <string>

int main()
{
	char test1[100]="아름다운소원은";
    char result1[100];
	convert(test1,result1);
	printf("%s\n",result1);

	char test[100]="이루워져야";
    char result[100];
	convert(test,result);
	printf("%s\n",result);
	return 0;
}
