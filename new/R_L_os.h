#pragma once
#define Max_length 40
#define Max_user 60
int registrate(int x, int z, char ar[1][Max_length], char ar2[][Max_length], char ar3[1][Max_length], char ar4[][Max_length]);//x对应op,ar是注册用户列,ar2是已有用户列,ar3注册密码列，ar4是已有密码列//
int login(int x,int z, char ar[1][Max_length], char ar2[][Max_length], char ar3[1][Max_length], char ar4[][Max_length]);//x对应op,ar是验证用户列,ar2是已有用户列,ar3验证密码列，ar4是已有密码列//