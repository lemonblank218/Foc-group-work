#ifndef MESSAGE_H
#define MESSAGE_H

#define MAX_NAME 64
#define MAX_MSG 256
#define MAX_LINE 512
#define MAX_FRIENDS 128

int load_friends(char *path, char friends[MAX_FRIENDS][MAX_NAME], int *count);
void manage_messages_menu(char *current_user,
                          char friends[MAX_FRIENDS][MAX_NAME],
                          int friend_count);

#endif
