/*
author: Wonjae Kim
UID: 117283365
Directory ID: wkim1128
*/
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include <ctype.h>
#include "document.h"
#define MAX_COMMAND_LENGTH 1024

void add_paragraph_after_user_interface(Document *doc, const char *command);
void add_line_after_user_interface(Document *doc, const char *command);
void print_document_user_interface(Document *doc, const char *command);
void quit_command_user_interface(const char *command);
void exit_command_user_interface(const char *command);
void append_line_user_interface(Document *doc, const char *command);
void remove_line_user_interface(Document *doc, const char *command);
void load_file_user_interface(Document *doc, const char *command);
void replace_text_user_interface(Document *doc, const char *command);
void highlight_text_user_interface(Document *doc, const char *command);
void remove_text_user_interface(Document *doc, const char *command);
void save_document_user_interface(Document *doc, const char *command);
void reset_document_user_interface(Document *doc, const char *command);
void trim_leading_spaces(char *str);

void add_paragraph_after_user_interface(Document *doc, const char *command) {
    int par_num = 0;
    char line[MAX_COMMAND_LENGTH + 1];
    char new_line[MAX_COMMAND_LENGTH + 1];

    /* check if command and check if par_num is biger than 0 */
    if (sscanf(command, "%s %d %s", line, &par_num, new_line) != 2 || par_num < 0) {
        printf("Invalid Command\n");
    }

    if (add_paragraph_after(doc, par_num) == FAILURE) {
        printf("add_paragraph_after failed\n");
    }
}

void add_line_after_user_interface(Document *doc, const char *command) {
    int par_num = 0, line_num = 0, check;
    char line[MAX_COMMAND_LENGTH + 1];
    char *line_start;
    char star = '\0';

    /* Scan the command */
    check = sscanf(command, "%s %d %d %c", line, &par_num, &line_num, &star);

    /* check all the cases */
    if (check != 4 || par_num < 1 || line_num < 0 || star != '*') {
        printf("Invalid Command\n");
    }

    /* Find star */
    line_start = strchr(command, '*');
    if (line_start == NULL || line_start == command) {
        printf("Invalid Command\n");
    }

    /* let line_start point after star */
    line_start++;

    /* Call add_line_after */
    if (add_line_after(doc, par_num, line_num, line_start) == FAILURE) {
        printf("add_line_after failed\n");
    }
}

void print_document_user_interface(Document *doc, const char *command) {
    int valid;
    char line[MAX_COMMAND_LENGTH + 1], new_line[MAX_COMMAND_LENGTH + 1];

    /* get the command*/
    valid = sscanf(command, "%s%s", line, new_line);

    /* check the validation */
    if (valid != 1) {
        printf("Invalid Command\n");
    } else {
        print_document(doc);
    }
}

/*  printf("[debug] name: %s, temp: %s\n", line, new_line); */
void quit_command_user_interface(const char *command) {
    int valid;
    char line[MAX_COMMAND_LENGTH + 1], extra_char;

    valid = sscanf(command, "%s %c", line, &extra_char);

    if (valid != 1 || strcmp(line, "quit") != 0) {
        printf("Invalid Command\n");
    }
}

/* basically same as quit_command_user_interface */
void exit_command_user_interface(const char *command) {
    int valid;
    char line[MAX_COMMAND_LENGTH + 1], extra_char;

    valid = sscanf(command, "%s %c", line, &extra_char);

    if (valid != 1 || strcmp(line, "exit") != 0) {
        printf("Invalid Command\n");
    }
}

void append_line_user_interface(Document *doc, const char *command) {
    int par_num;
    char *content, star = '\0';
    char line[MAX_COMMAND_LENGTH + 1];

    /* check all the cases */
    if ((sscanf(command, " %s %d %c", line, &par_num, &star)) != 3 
        || par_num < 1 || star != '*') {
        printf("Invalid Command\n");
    }

    /* find star */
    content = strchr(command, '*');
    if (content == NULL) {
        printf("Invalid Command\n");
    }

    /* let content point after star */
    content++;

    /* call append_line */
    if (append_line(doc, par_num, content) == FAILURE) {
        printf("append_line failed\n");
    }
}

void remove_line_user_interface(Document *doc, const char *command) {
    int par_num, line_num;
    char command_name[MAX_COMMAND_LENGTH];
    int valid;

    /* check the validation */
    valid = sscanf(command, "%s %d %d", command_name, &par_num, &line_num);

    /* Check if valid parameters */
    if (valid != 3 || par_num < 1 || line_num < 1) {
        printf("Invalid Command\n");
    }

    /* call function */
    if (remove_line(doc, par_num, line_num) == FAILURE) {
        printf("remove_line failed\n");
    }
}

void load_file_user_interface(Document *doc, const char *command) {
    char filename[MAX_COMMAND_LENGTH + 1];
    char line[MAX_COMMAND_LENGTH + 1];
    char new_line[MAX_COMMAND_LENGTH + 1];

    /* check amount of input*/
    if (sscanf(command, "%s%s%s", line, filename, new_line) != 2) {
        printf("Invalid Command\n");
    }

    /* check if filename exceed 80*/
    if (strlen(filename) >= 80) {
        printf("Invalid Command\n");
    }

    if (load_file(doc, filename) == FAILURE) {
        printf("load_file failed\n");
    }
}

void replace_text_user_interface(Document *doc, const char *command) {
    char target[MAX_STR_SIZE + 1], replace[MAX_STR_SIZE + 1];
    char line[MAX_COMMAND_LENGTH];
    int i, j, k, command_len;

    /* Initialize target and replace as empty strings */
    target[0] = replace[0] = '\0';

    /* Extract the command name */
    sscanf(command, "%s", line);

    /* Find the start of the target */
    command_len = strlen(command);
    for (i = strlen(line); i < command_len; i++) {
        if (command[i] == '\"') {
            /* Copy characters */
            for (j = i + 1, k = 0; j < command_len 
                 && command[j] != '\"'; j++, k++) {
                target[k] = command[j];
            }
            target[k] = '\0';
            break;
        }
    }

    /* Find the start of the replace */
    for (i = j + 1; i < command_len; i++) {
        if (command[i] == '\"') {
            /* Copy characters */
            for (j = i + 1, k = 0; j < command_len 
                 && command[j] != '\"'; j++, k++) {
                replace[k] = command[j];
            }
            replace[k] = '\0';
            break;
        }
    }

    /* replace it  */
    if (target[0] != '\0' && replace[0] != '\0') {
        if (replace_text(doc, target, replace) == FAILURE) {
            printf("replace_text failed\n");
        }
    } else {
        printf("Invalid Command\n");
    }
}

void highlight_text_user_interface(Document *doc, const char *command) {
    char temp[MAX_STR_SIZE + 1];
    const char *first, *end;
    char quoteChar = '\"';

    first = strchr(command, quoteChar);
    if (first != NULL) {
        end = strchr(first + 1, quoteChar);
        if (end != NULL) {
            /* substring between the double quotes */
            strncpy(temp, first + 1, end - first - 1);
            temp[end - first - 1] = '\0';

            /* call function*/
            highlight_text(doc, temp);
        } else {
            printf("Invalid Command\n");
        }
    } else {
        printf("Invalid Command\n");
    }
}

void remove_text_user_interface(Document *doc, const char *command) {
    char target[MAX_COMMAND_LENGTH + 1];
    char *start, *end;
    /* find the start and the end of qutation */
    if ((start = strchr(command, '\"')) && (end = strchr(start + 1, '\"'))) {

        /* get rid of qutation*/
        strncpy(target, start + 1, end - start - 1);
        target[end - start - 1] = '\0';

        remove_text(doc, target);

    } else {

        printf("Invalid Command\n");
        
    }
}

void save_document_user_interface(Document *doc, const char *command) {
    char filename[MAX_COMMAND_LENGTH + 1];
    char line[MAX_COMMAND_LENGTH + 1];
    char new_line[MAX_COMMAND_LENGTH + 1];
    int check;

    /*scan the command*/
    check = sscanf(command, "%s %s %s", line, filename, new_line);

    /*if filename is scanned, continue*/
    if (check == 2) {
        if (save_document(doc, filename) == FAILURE) {
            printf("save_document failed\n");
        }
    } else {
        printf("Invalid Command\n");
    }
}

void reset_document_user_interface(Document *doc, const char *command) {

    char line[MAX_STR_SIZE + 1], new_line[MAX_STR_SIZE + 1];
    /* check if input is correct */
    if (sscanf(command, "%s%s", line, new_line) != 1) {
        printf("Invalid Command\n");
    } else {
        reset_document(doc);
    }
}

/* get rid of the leading space for the edge case */
void trim_leading_spaces(char *str) {
    int i = 0, j = 0;
    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }

    if (i > 0) {
        while (str[i]) {
            str[j++] = str[i++];
        }
        str[j] = '\0';
    }
}

/* call proper function*/
void process_command(Document *doc, const char *line) {
    if (strncmp(line, "add_paragraph_after", 19) == 0) {
        add_paragraph_after_user_interface(doc, line);
    } else if (strncmp(line, "add_line_after", 14) == 0) {
        add_line_after_user_interface(doc, line);
    } else if (strncmp(line, "print_document", 14) == 0) {
        print_document_user_interface(doc, line);
    } else if (strncmp(line, "append_line", 11) == 0) {
        append_line_user_interface(doc, line);
    } else if (strncmp(line, "remove_line", 11) == 0) {
        remove_line_user_interface(doc, line);
    } else if (strncmp(line, "load_file", 9) == 0) {
        load_file_user_interface(doc, line);
    } else if (strncmp(line, "replace_text", 12) == 0) {
        replace_text_user_interface(doc, line);
    } else if (strncmp(line, "highlight_text", 14) == 0) {
        highlight_text_user_interface(doc, line);
    } else if (strncmp(line, "remove_text", 11) == 0) {
        remove_text_user_interface(doc, line);
    } else if (strncmp(line, "save_document", 13) == 0) {
        save_document_user_interface(doc, line);
    } else if (strncmp(line, "reset_document", 14) == 0) {
        reset_document_user_interface(doc, line);
    } else if (strncmp(line, "quit", 4) == 0) {
        quit_command_user_interface(line);
    } else if (strncmp(line, "exit", 4) == 0) {
        exit_command_user_interface(line);
    }
}

int main(int argc, char *argv[]) {
    Document doc;
    char line[MAX_COMMAND_LENGTH + 1];
    FILE *input = stdin;

    init_document(&doc, "main_document");

    /* if only one argument */
    if (argc == 1) {
        printf("> ");
        input = stdin;
        /* if two arguments */
    } else if (argc == 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "%s cannot be opened.\n", argv[1]);
            exit(EX_OSERR);
        }
    } else {
        fprintf(stderr, "Usage: user_interface\n");
        fprintf(stderr, "Usage: user_interface <filename>\n");
        exit(EX_USAGE);
    }

    while (fgets(line, MAX_COMMAND_LENGTH, input) != NULL) {
        line[strcspn(line, "\n")] = 0;
        trim_leading_spaces(line);

        process_command(&doc, line);

        if (input == stdin && (strcmp(line, "quit") != 0
            && strcmp(line, "exit") != 0)) {
            printf("> ");
        }
    }

    if (argc == 2) {
        fclose(input);
    }
    return 0;
}
