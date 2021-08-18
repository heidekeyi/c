//
// Created by 12131 on 2021/7/15.
//

#include "gui_ZJF.h"

static void gui_menu();

static void next_line();

static void gui_operation(int *operator);

static void get_word(char *buffer, int size);


void gui_menu() {
    printf("**********config system************\n");
    printf("r. read\n");
    printf("w. write\n");
    printf("q. exit\n");
    printf("please input action:");
}

void next_line() {
    int ch;
    while ('\n' != (ch = getchar())) {
        printf("***********%d***********\n", ch);
    }
}

void gui_operation(int *operator) {
    int ch;
    int val = 0;
    int first = 1;
    while ((ch = getchar())) {
//        printf("*******%d*********\n", ch);
        switch (ch) {
            case 'r':
                val = 'r';
                break;
            case 'w':
                val = 'w';
                break;
            case 'q':
                val = 'q';
                break;
            default:
                if (!isspace(ch)) {
                    first = 0;
                }
                break;
        }
        if (val) {
            *operator = val;
            next_line();
            break;
        }
        if (ch != '\n' && !first) {
            next_line();
            first = 1;
            gui_menu();
        }
    }
}

void get_word(char *buffer, int size) {
    int first = 1;
    int index = 0;
    int ch;
    fflush(stdin);
    while (index < size - 1) {
        ch = getchar();
        if (ch == '\n') {
            if (index) {
                break;
            } else {
                next_line();
                continue;
            };
        }
        if (isspace(ch) && first) {
            continue;
        }
        buffer[index++] = (char) ch;
        first = 0;
    }
    buffer[index] = '\0';
}

void gui_test_ZJF() {
    logging_init_config(0, "w", "./app.log");
    int isEnd = 0;
    int operator;
    void *config = NULL;
    char key[128];
    char value[128];
    char *ptr;
    int rv = config_init_ZJF(&config, "./app.log");
    if (rv) {
        logging_ZJF("e", "init config error: %d", rv);
        goto end;
    }
    while (!isEnd) {
        gui_menu();
        gui_operation(&operator);
        fflush(stdout);
        fflush(stdin);
        switch (operator) {
            case 'r':
                printf("read key: ");
                get_word(key, sizeof(key));
                rv = config_read_ZJF(config, key, &ptr);
                if (rv) {
                    log_ZJF("e", "read config error: %d", rv);
                    goto end;
                }
                if (ptr) {
                    printf("value: %s\n", ptr);
                } else {
                    puts("*key value not exist*\n");
                }
                break;
            case 'w':
                printf("write key: ");
                get_word(key, sizeof(key));
                printf("write value: ");
                get_word(value, sizeof(value));
                rv = config_write_ZJF(config, key, value);
                if (rv) {
                    log_ZJF("e", "read config error: %d", rv);
                    goto end;
                }
                break;
            case 'q':
                isEnd = 1;
                break;
            default:
                break;
        }
    }
    end:
    if (config && config_free_ZJF(&config)) {
        log_ZJF("e", "free config error");
    }
}