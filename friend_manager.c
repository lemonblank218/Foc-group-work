#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "friend_manager.h"

// 全局当前用户指针
User *current_user = NULL;

// 设置当前用户
void set_current_user(const char *username) {
    static User user;
    strcpy(user.name, username);
    user.friend_count = 0;
    user.pending_count = 0;
    current_user = &user;
}

// 添加待处理好友请求
void add_pending_request(const char *requester) {
    if (current_user == NULL || current_user->pending_count >= MAX_PENDING) {
        return;
    }
    
    // 检查是否已经在待处理列表中
    for (int i = 0; i < current_user->pending_count; i++) {
        if (strcmp(current_user->pending[i], requester) == 0) {
            return; // 已经存在
        }
    }
    
    // 检查是否已经是好友
    for (int i = 0; i < current_user->friend_count; i++) {
        if (strcmp(current_user->friends[i], requester) == 0) {
            return; // 已经是好友
        }
    }
    
    // 添加到待处理列表
    strcpy(current_user->pending[current_user->pending_count], requester);
    current_user->pending_count++;
}

// 显示待处理好友请求菜单
void accept_friends_menu(void) {
    if (current_user == NULL) {
        printf("Error: No user logged in.\n");
        return;
    }
    
    if (current_user->pending_count == 0) {
        printf("No pending friend requests for %s.\n", current_user->name);
        return;
    }
    
    // 显示待处理请求列表
    printf("Pending friend requests for %s:\n", current_user->name);
    for (int i = 0; i < current_user->pending_count; i++) {
        printf("%d. %s\n", i + 1, current_user->pending[i]);
    }
    printf("%d. All\n", current_user->pending_count + 1);
    printf("%d. Back\n", current_user->pending_count + 2);
    
    // 获取用户输入
    char input[256];
    printf("Enter indices (space separated), press Enter to finish: ");
    fgets(input, sizeof(input), stdin);
    
    // 解析输入
    char *token = strtok(input, " \n");
    int indices[MAX_PENDING];
    int index_count = 0;
    
    while (token != NULL && index_count < MAX_PENDING) {
        int num = atoi(token);
        if (num >= 1 && num <= current_user->pending_count + 2) {
            indices[index_count++] = num;
        }
        token = strtok(NULL, " \n");
    }
    
    // 处理选择
    int all_selected = 0;
    int back_selected = 0;
    
    for (int i = 0; i < index_count; i++) {
        int choice = indices[i];
        
        if (choice == current_user->pending_count + 1) { // "All"
            all_selected = 1;
            break;
        } else if (choice == current_user->pending_count + 2) { // "Back"
            back_selected = 1;
            break;
        } else if (choice >= 1 && choice <= current_user->pending_count) {
            int idx = choice - 1;
            char *friend_name = current_user->pending[idx];
            
            // 添加到好友列表
            if (current_user->friend_count < MAX_FRIENDS) {
                strcpy(current_user->friends[current_user->friend_count], friend_name);
                current_user->friend_count++;
                printf("Friend requests updated for %s.\n", friend_name);
                
                // 从待处理列表中移除
                for (int j = idx; j < current_user->pending_count - 1; j++) {
                    strcpy(current_user->pending[j], current_user->pending[j + 1]);
                }
                current_user->pending_count--;
                
                // 调整后续索引（因为数组改变了）
                for (int k = i + 1; k < index_count; k++) {
                    if (indices[k] > choice) {
                        indices[k]--;
                    }
                }
            }
        }
    }
    
    // 处理"All"选项
    if (all_selected) {
        for (int i = 0; i < current_user->pending_count; i++) {
            if (current_user->friend_count < MAX_FRIENDS) {
                strcpy(current_user->friends[current_user->friend_count], current_user->pending[i]);
                current_user->friend_count++;
                printf("Friend requests updated for %s.\n", current_user->pending[i]);
            }
        }
        current_user->pending_count = 0;
        printf("Friend requests updated for all.\n");
    }
    
    if (back_selected) {
        printf("Returning to previous menu.\n");
    }
}

// 显示当前好友列表
void show_current_friends_menu(void) {
    if (current_user == NULL) {
        printf("Error: No user logged in.\n");
        return;
    }
    
    if (current_user->friend_count == 0) {
        printf("Your friends list is empty.\n");
        return;
    }
    
    printf("Your friends:\n");
    for (int i = 0; i < current_user->friend_count; i++) {
        printf("%d. %s\n", i + 1, current_user->friends[i]);
    }
}

// 添加好友（模拟其他用户发送请求）
void simulate_friend_request(const char *from_user) {
    if (current_user != NULL) {
        add_pending_request(from_user);
    }
}
