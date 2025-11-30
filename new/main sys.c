#include<stdio.h>
#include"opening.h"
#include"R_L_os.h"
#define Max_length 40
#define Max_user 60



int main(void)
{
	int op1=0,user_num=0,cont2=0;
	char id_new[Max_length]="", id_store[Max_user][Max_length]= {{0}}, id_check[Max_length]="", paswod_new[Max_length]="", paswod_store[Max_user][Max_length]= {{0}}, paswod_check[Max_length]="";
	open();
	scanf("%d", &op1);
	clear_buffer();
	registrate(op1, user_num, id_new, id_store, paswod_new, paswod_store);
	login(op1, user_num, id_new, id_store, paswod_new, paswod_store);

}