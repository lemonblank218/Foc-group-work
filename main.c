/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Console mode (command line) program.                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "friend_manager.h"
void manage_friends_menu(void) {
    int choice;
    
    while (1) {
        printf("\n==================== Manage Friends ====================\n");
        printf("1. Add friends\n");
        printf("2. Accept friends\n");
        printf("3. Delete friends\n");
        printf("4. Show current friends\n");
        printf("5. Back\n");
        printf("========================================================\n");
        printf("Choose an option (1-5): ");
        
        scanf("%d", &choice);
        getchar(); // 清除换行符
        
        switch (choice) {
            case 1:
                printf("Add friends function (not implemented in this module)\n");
                break;
            case 2:
                accept_friends_menu();
                break;
            case 3:
                printf("Delete friends function (not implemented in this module)\n");
                break;
            case 4:
                show_current_friends_menu();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// 主服务菜单
void main_service_menu(void) {
    int choice;
    
    while (1) {
        printf("\n==================== Main Service ====================\n");
        printf("1. Manage friends\n");
        printf("2. Manage messages\n");
        printf("3. Back\n");
        printf("======================================================\n");
        printf("Choose an option (1-3): ");
        
        scanf("%d", &choice);
        getchar(); // 清除换行符
        
        switch (choice) {
            case 1:
                manage_friends_menu();
                break;
            case 2:
                printf("Manage messages function (not implemented)\n");
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}



int main(int argc, char *argv[])
{
      int main_choice;
    
    while (1) {
        printf("\n==================== Login ====================\n");
        printf("1. Login\n");
        printf("2. Register (If you do not have an account)\n");
        printf("3. Exit\n");
        printf("===============================================\n");
        printf("Choose an option (1-3): ");
        
        scanf("%d", &main_choice);
        getchar(); // 清除换行符
        
        switch (main_choice) {
            case 1: {
                // 模拟不同用户的登录和好友请求
                char username[MAX_NAME_LEN];
                printf("Please input your account name: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;
                
                set_current_user(username);
                printf("Login successful. Welcome %s!\n", username);
                
                main_service_menu();
                break;
            }
            case 2:
                printf("Registration function (not implemented in this module)\n");
                break;
            case 3:
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}


