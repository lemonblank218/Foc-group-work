#include "opening.h"
#include<stdio.h>
char open(void)//开机界面
{
    for (int i = 0; i < 10; i++)
    {
        printf("=");
    }
    printf("Login");
    for (int i = 0; i < 10; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("1. Login\n2. Register(If you do not have an account)\n3. Exit\n");
    for (int i = 0; i < 25; i++)
    {
        printf("=");
    }
    printf("\n");
    return 0;
}

