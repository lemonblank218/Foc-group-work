#ifndef FRIEND_MANAGER_H
#define FRIEND_MANAGER_H
#define MAX_NAME_LEN 50
#define MAX_FRIENDS 100
#define MAX_PENDING 50
typedef struct {
    char name[MAX_NAME_LEN];
    char friends[MAX_FRIENDS][MAX_NAME_LEN];   
    char pending[MAX_PENDING][MAX_NAME_LEN];   
    int friend_count;
    int pending_count;
} User;
extern User *current_user;
void accept_friends_menu(void);          
void show_current_friends_menu(void);    
void set_current_user(const char *username);  
void add_pending_request(const char *requester);  
#endif
