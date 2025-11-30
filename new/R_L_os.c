#include "R_L_os.h"
#include<stdio.h>
#include<string.h>
#define Max_length 40
#define Max_user 60
void clear_buffer();

void read_string(char ar[][Max_length])
{
    fgets(ar, Max_length, stdin);
}

int registrate(int x,int z, char ar[1][Max_length], char ar2[][Max_length], char ar3[1][Max_length], char ar4[][Max_length])//x对应op,z是用户数量,ar是注册用户列,ar2是已有用户列,ar3注册密码列，ar4是已有密码列//
{
    if (x == 2)
    {
        int ck = 1;
        printf("\n");
        for (int i = 0; i < 10; i++) printf("=");
        printf("  Registration  ");
        for (int i = 0; i < 10; i++) printf("=");
        printf("\n");
        //用户注册
        printf("Seting account(under16 words):");
        read_string(ar);
        for (int i = 0; i < z; i++)
        {
            if (strcmp(ar2[i], ar) == 0)
            {
                printf("already registrate,turning to login");
                login(1,z, ar, ar2, ar3, ar4);
                break;
            }
        }
        strncpy(ar2[z], ar, strlen(ar));
        printf("\n");
        printf("Seting password(under 16 words):");
        read_string(ar3);
        strncpy(ar4[z], ar3, strlen(ar3));
        ar4[z][strlen(ar3 + 1)] = '\0';
        z++;
        printf("registrate successful,turning to login");
        login(1,z, ar, ar2, ar3, ar4);
       
        return 0;
    }
}

int login(int x,int z, char ar[1][Max_length], char ar2[][Max_length], char ar3[1][Max_length], char ar4[][Max_length])//x对应op,ar是验证用户列,ar2是已有用户列,ar3验证密码列，ar4是已有密码列//
{
    if (x == 1)//验证操作
    {
        int check = 0;
        // 正确清空字符串
        memset(ar, 0, Max_length);
        memset(ar3, 0, Max_length);
        printf("\n");
        for (int i = 0; i < 10; i++) printf("=");
        printf("  User Login  ");
        for (int i = 0; i < 10; i++) printf("=");
        printf("\n");
        //登录
        printf("Account: ");
        read_string(ar);
        printf("Passwords: ");
        read_string(ar3);
        //开始账户查重
        for (int i = 0; i < z; i++)
        {
            if (strcmp(ar2[i], ar) == 0 && strcmp(ar4[i], ar3) == 0)
            {
                check++;
                printf("welcome");
                return 1;
            }

        }
        printf("Wrong password or account, please try again!");
        return login(1, z, ar, ar2, ar3, ar4);
        //可加功能:重置密码,注册用户
    }
    else
    {
        return 0;
    }
    return 0;

}

void clear_buffer() {
    while (getchar() != '\n');  // 循环读取缓冲区字符，直到清空换行符
}







