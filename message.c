#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "message.h"

// 消息结构：时间戳、发送者、已读标记、正文
typedef struct {
    char ts[20];
    char sender[MAX_NAME];
    int read;
    char text[MAX_MSG];
} Message;

// 去掉行末换行
static void trim_newline(char *s) {
    int n = (int)strcspn(s, "\r\n");
    s[n] = '\0';
}

// 读取一行输入
static void read_line(char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, (int)size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    trim_newline(buf);
}

// 从好友列表文件读取好友
int load_friends(char *path, char friends[MAX_FRIENDS][MAX_NAME], int *count) {
    FILE *fp = fopen(path, "r");
    char line[MAX_LINE];
    *count = 0;
    if (!fp) {
        return 0;
    }
    while (fgets(line, sizeof(line), fp) != NULL && *count < MAX_FRIENDS) {
        trim_newline(line);
        if (line[0] == '\0') {
            continue;
        }
        strncpy(friends[*count], line, MAX_NAME - 1);
        friends[*count][MAX_NAME - 1] = '\0';
        (*count)++;
    }
    fclose(fp);
    return 1;
}

static void append_two_digits(char *buf, int *pos, int value) {
    buf[(*pos)++] = (char)('0' + (value / 10) % 10);
    buf[(*pos)++] = (char)('0' + (value % 10));
}

static void append_four_digits(char *buf, int *pos, int value) {
    buf[(*pos)++] = (char)('0' + (value / 1000) % 10);
    buf[(*pos)++] = (char)('0' + (value / 100) % 10);
    buf[(*pos)++] = (char)('0' + (value / 10) % 10);
    buf[(*pos)++] = (char)('0' + (value % 10));
}

// 生成当前时间字符串：YYYY-MM-DD HH:MM:SS
static void current_timestamp(char *buf, int size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int year = t->tm_year + 1900;
    int pos = 0;

    if (size < 20) {
        if (size > 0) {
            buf[0] = '\0';
        }
        return;
    }

    append_four_digits(buf, &pos, year);
    buf[pos++] = '-';
    append_two_digits(buf, &pos, t->tm_mon + 1);
    buf[pos++] = '-';
    append_two_digits(buf, &pos, t->tm_mday);
    buf[pos++] = ' ';
    append_two_digits(buf, &pos, t->tm_hour);
    buf[pos++] = ':';
    append_two_digits(buf, &pos, t->tm_min);
    buf[pos++] = ':';
    append_two_digits(buf, &pos, t->tm_sec);
    buf[pos] = '\0';
}

// 解析消息文件一行：ts|sender|read|text
static int parse_message_line(char *line, Message *msg) {
    char *p1 = strchr(line, '|');
    char *p2 = p1 ? strchr(p1 + 1, '|') : NULL;
    char *p3 = p2 ? strchr(p2 + 1, '|') : NULL;
    if (!p1 || !p2 || !p3) {
        return 0;
    }

    int ts_len = (int)(p1 - line);
    int sender_len = (int)(p2 - p1 - 1);
    int read_len = (int)(p3 - p2 - 1);

    if (ts_len >= sizeof(msg->ts) || sender_len >= sizeof(msg->sender) || read_len >= 8) {
        return 0;
    }

    strncpy(msg->ts, line, ts_len);
    msg->ts[ts_len] = '\0';

    strncpy(msg->sender, p1 + 1, sender_len);
    msg->sender[sender_len] = '\0';

    char read_buf[8];
    strncpy(read_buf, p2 + 1, read_len);
    read_buf[read_len] = '\0';
    msg->read = atoi(read_buf);

    strncpy(msg->text, p3 + 1, sizeof(msg->text) - 1);
    msg->text[sizeof(msg->text) - 1] = '\0';

    trim_newline(msg->text);
    return 1;
}

// 读入所有消息
static int load_messages(char *path, Message **out_msgs, int *out_count) {
    FILE *fp = fopen(path, "r");
    char line[MAX_LINE];
    Message *msgs = NULL;
    int count = 0;

    if (!fp) {
        *out_msgs = NULL;
        *out_count = 0;
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        Message msg;
        if (!parse_message_line(line, &msg)) {
            continue;
        }
        Message *tmp = (Message *)realloc(msgs, sizeof(Message) * (count + 1));
        if (!tmp) {
            free(msgs);
            fclose(fp);
            *out_msgs = NULL;
            *out_count = 0;
            return 0;
        }
        msgs = tmp;
        msgs[count++] = msg;
    }

    fclose(fp);
    *out_msgs = msgs;
    *out_count = count;
    return 1;
}

// 覆盖写回消息
static int save_messages(char *path, Message *msgs, int count) {
    FILE *fp = fopen(path, "w");
    if (!fp) {
        return 0;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s|%s|%d|%s\n", msgs[i].ts, msgs[i].sender, msgs[i].read, msgs[i].text);
    }
    fclose(fp);
    return 1;
}

static int date_to_int(int y, int m, int d) {
    return y * 10000 + m * 100 + d;
}

// 解析日期输入：dd/mm/yyyy
static int parse_date_input(char *s, int *out_date) {
    int d = 0, m = 0, y = 0;
    if (sscanf(s, "%d/%d/%d", &d, &m, &y) != 3) {
        return 0;
    }
    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 1) {
        return 0;
    }
    *out_date = date_to_int(y, m, d);
    return 1;
}

static int message_date_to_int(char *ts) {
    int y = 0, m = 0, d = 0;
    if (sscanf(ts, "%d-%d-%d", &y, &m, &d) != 3) {
        return 0;
    }
    return date_to_int(y, m, d);
}

// 显示好友选择列表
static void show_friends(char friends[MAX_FRIENDS][MAX_NAME], int count) {
    printf("Your friends:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, friends[i]);
    }
    printf("%d. All\n", count + 1);
    printf("%d. Back\n", count + 2);
}

// 发送消息
static void send_message(char *current_user, char friends[MAX_FRIENDS][MAX_NAME], int friend_count) {
    char message[MAX_MSG];
    char input[MAX_LINE];

    if (friend_count == 0) {
        printf("You have no friends.\n");
        return;
    }

    read_line("Enter message (max 255 chars), press Enter to finish: ", message, sizeof(message));

    show_friends(friends, friend_count);
    read_line("Enter friend numbers (separated by space), press Enter to finish: ", input, sizeof(input));

    int selected[MAX_FRIENDS];
    int selected_count = 0;
    int all_selected = 0;
    int back_selected = 0;

    char *token = strtok(input, " ");
    while (token != NULL && selected_count < MAX_FRIENDS) {
        int num = atoi(token);
        if (num == friend_count + 2) {
            back_selected = 1;
            break;
        }
        if (num == friend_count + 1) {
            all_selected = 1;
            break;
        }
        if (num >= 1 && num <= friend_count) {
            selected[selected_count++] = num - 1;
        }
        token = strtok(NULL, " ");
    }

    if (back_selected) {
        return;
    }

    char ts[20];
    current_timestamp(ts, sizeof(ts));

    if (all_selected) {
        for (int i = 0; i < friend_count; i++) {
            char filename[MAX_NAME + 8];
            strcpy(filename, friends[i]);
            strcat(filename, ".txt");
            FILE *fp = fopen(filename, "a");
            if (fp) {
                fprintf(fp, "%s|%s|0|%s\n", ts, current_user, message);
                fclose(fp);
            }
        }
        printf("Message sent to All\n");
        return;
    }

    for (int i = 0; i < selected_count; i++) {
        char *friend_name = friends[selected[i]];
        char filename[MAX_NAME + 8];
        strcpy(filename, friend_name);
        strcat(filename, ".txt");
        FILE *fp = fopen(filename, "a");
        if (fp) {
            fprintf(fp, "%s|%s|0|%s\n", ts, current_user, message);
            fclose(fp);
            printf("Message sent to %s\n", friend_name);
        }
    }
}

// 读取消息（全部或未读）
static void read_messages(char *current_user) {
    char filename[MAX_NAME + 8];
    strcpy(filename, current_user);
    strcat(filename, ".txt");

    Message *msgs = NULL;
    int count = 0;

    load_messages(filename, &msgs, &count);

    if (count == 0) {
        printf("No messages found.\n");
        free(msgs);
        return;
    }

    printf("1. Read all messages\n");
    printf("2. Read unread messages only\n");
    printf("3. Back\n");

    char input[MAX_LINE];
    read_line("Choose an option: ", input, sizeof(input));
    int choice = atoi(input);

    if (choice == 3) {
        free(msgs);
        return;
    }

    int shown = 0;
    int changed = 0;

    for (int i = 0; i < count; i++) {
        if (choice == 2 && msgs[i].read) {
            continue;
        }
        printf("[%s] From: %s\n", msgs[i].ts, msgs[i].sender);
        printf("%s\n", msgs[i].text);
        shown++;
        if (!msgs[i].read) {
            msgs[i].read = 1;
            changed = 1;
        }
    }

    if (shown == 0) {
        printf("Found no messages.\n");
    } else if (changed) {
        save_messages(filename, msgs, count);
    }

    free(msgs);
}

// 删除指定时间段的消息
static void delete_messages(char *current_user) {
    char start_buf[32];
    char end_buf[32];
    char account[MAX_NAME];

    read_line("Enter start date (dd/mm/yyyy): ", start_buf, sizeof(start_buf));
    read_line("Enter end date (dd/mm/yyyy): ", end_buf, sizeof(end_buf));
    read_line("Enter account name (or 'all'): ", account, sizeof(account));

    int start_date = 0, end_date = 0;
    if (!parse_date_input(start_buf, &start_date) || !parse_date_input(end_buf, &end_date)) {
        printf("Removed 0 message(s) from %s in period %s - %s.\n", account, start_buf, end_buf);
        return;
    }

    char filename[MAX_NAME + 8];
    strcpy(filename, current_user);
    strcat(filename, ".txt");

    Message *msgs = NULL;
    int count = 0;
    load_messages(filename, &msgs, &count);

    if (count == 0) {
        printf("Removed 0 message(s) from %s in period %s - %s.\n", account, start_buf, end_buf);
        free(msgs);
        return;
    }

    Message *kept = (Message *)malloc(sizeof(Message) * count);
    int kept_count = 0;
    int removed = 0;

    for (int i = 0; i < count; i++) {
        int msg_date = message_date_to_int(msgs[i].ts);
        int in_range = (msg_date >= start_date && msg_date <= end_date);
        int sender_match = (strcmp(account, "all") == 0) || (strcmp(account, msgs[i].sender) == 0);

        if (in_range && sender_match) {
            removed++;
            continue;
        }
        kept[kept_count++] = msgs[i];
    }

    save_messages(filename, kept, kept_count);

    printf("Removed %d message(s) from %s in period %s - %s.\n", removed, account, start_buf, end_buf);

    free(msgs);
    free(kept);
}

// 管理消息菜单
void manage_messages_menu(char *current_user, char friends[MAX_FRIENDS][MAX_NAME], int friend_count) {
    while (1) {
        printf("==================== Manage Messages ====================\n");
        printf("1. Send a message\n");
        printf("2. Read messages\n");
        printf("3. Delete messages\n");
        printf("4. Back\n");
        printf("=========================================================\n");
        printf("Choose an option (1-4): ");

        char input[MAX_LINE];
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        int choice = atoi(input);

        if (choice == 1) {
            send_message(current_user, friends, friend_count);
        } else if (choice == 2) {
            read_messages(current_user);
        } else if (choice == 3) {
            delete_messages(current_user);
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

int main(void) {
    char current_user[MAX_NAME];
    char friends[MAX_FRIENDS][MAX_NAME];
    int friend_count = 0;

    read_line("Enter current user name: ", current_user, sizeof(current_user));

    if (!load_friends("friends.txt", friends, &friend_count)) {
        printf("Warning: friends.txt not found, no friends loaded.\n");
    }

    manage_messages_menu(current_user, friends, friend_count);

    return 0;
}
