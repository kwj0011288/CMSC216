/*
author: Wonjae Kim
UID: 117283365
Directory ID: wkim1128
*/

#include "document.h"
#include <stdio.h>
#include <string.h>

int init_document(Document *doc, const char *name) {
    int name_len;

    if (doc == NULL || name == NULL) {

        return FAILURE;
    }
    /* set doc name */
    strcpy(doc->name, name);

    name_len = strlen(name);

    if (name_len > MAX_STR_SIZE) {

        return FAILURE;
    }

    return SUCCESS;
}

int reset_document(Document *doc) {

    if (doc == NULL) {

        return FAILURE;

    }
    /* set length to 0 */
    doc->number_of_paragraphs = 0;

    return SUCCESS;
}

int print_document(Document *doc) {
    int i, j;

    if (doc == NULL) {

        return FAILURE;

    }

    /*print document name*/
    printf("Document name: \"%s\"\n", doc->name);

    /*print numer of paragraphs*/
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

    /*print paragraphs by lines*/
    for (i = 0; i < doc->number_of_paragraphs; i++) {

        if (doc->paragraphs[i].number_of_lines > 0) {

            for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {

                printf("%s\n", doc->paragraphs[i].lines[j]);
            }

            if (i < doc->number_of_paragraphs - 1) {

                printf("\n");
            }
        }
    }
    return SUCCESS;
}


int add_paragraph_after(Document *doc, int paragraph_number) {
    int len = 0, i;

    char *shift;

    if (doc == NULL || paragraph_number == MAX_PARAGRAPHS ) {

        return FAILURE;

    }

    len = doc->number_of_paragraphs;

    if (len > MAX_PARAGRAPHS || paragraph_number > len) {

        return FAILURE;

    }
    
    /* loop through to move down the paragraph */
    for (i = doc->number_of_paragraphs; i > paragraph_number; i--) {

        doc->paragraphs[i] = doc->paragraphs[i - 1];
    }
    /* set line of the new paragraph to nothing */
    doc->paragraphs[paragraph_number].number_of_lines = 0;

    /* increment the number of paragraph */
    doc->number_of_paragraphs++;

    return SUCCESS;
}

int add_line_after(Document *doc, int paragraph_number, int line_number,
                   const char *new_line) {
    int i, par_num;

    if (doc == NULL || new_line == NULL) {

        return FAILURE;

    }

    par_num = doc->number_of_paragraphs;
    

    if (par_num < paragraph_number || par_num == MAX_PARAGRAPHS || doc->paragraphs[paragraph_number - 1].number_of_lines == MAX_PARAGRAPH_LINES) {

        return FAILURE;
    }

    /* starts it from the end, and find the right spot to add new line */
    for (i = doc->paragraphs[paragraph_number - 1].number_of_lines;
         i > line_number; i--) {

        strcpy(doc->paragraphs[paragraph_number - 1].lines[i],
               doc->paragraphs[paragraph_number - 1].lines[i - 1]);

    }
    /* Copy the new line to the position next to the specified line number */
    strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number],
           new_line);
    /* increase the number of lines */
    doc->paragraphs[paragraph_number - 1].number_of_lines++;

    return SUCCESS;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number,
                               int *number_of_lines) {
    int par_num, num_line;

    if (doc == NULL || number_of_lines == NULL) {

        return FAILURE;
    }

    par_num = doc->number_of_paragraphs;

    if (par_num < paragraph_number) {

        return FAILURE;

    }

    /* get paragraph lines */
    num_line = doc->paragraphs[paragraph_number - 1].number_of_lines;

    /* plug it in */
    *number_of_lines = num_line;

    return SUCCESS;
}

int get_number_lines(Document *doc, int *number_of_lines) {
    int lines = 0, i;

    if (doc == NULL) {

        return FAILURE;

    }

    for(i = 0; i < doc->number_of_paragraphs; i++) {
         lines += doc->paragraphs[i].number_of_lines;
        
    }
    /* get number of lines */
  

    /* plug it in */
    *number_of_lines = lines;

    return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line) {
    int line_num;

    if (doc == NULL || new_line == NULL) {

        return FAILURE;

    }

    /* get the line numbers */
    line_num = doc->paragraphs[paragraph_number - 1].number_of_lines;

    /* use add line after function to append line */
    return add_line_after(doc, paragraph_number, line_num, new_line);
}

int remove_line(Document *doc, int paragraph_number, int line_number) {
    int i;

    Paragraph *paragraph;

    if (doc == NULL) {

        return FAILURE;

    }

    if (paragraph_number > doc->number_of_paragraphs || paragraph_number <= 0) {

        return FAILURE;

    }

    /* intialize this to prevent the repetitive code */
    paragraph = &doc->paragraphs[paragraph_number - 1];

    if (line_number >= paragraph->number_of_lines || line_number < 0) {

        return FAILURE;

    }

    /* find the spot where the lines will be removed */
    for (i = line_number - 1; i < paragraph->number_of_lines - 1; i++) {

        strcpy(paragraph->lines[i], paragraph->lines[i + 1]);

    }
    /* remove the line  */
    paragraph->lines[paragraph->number_of_lines - 1][0] = '\0';

    /* decrease the number of lines */
    paragraph->number_of_lines--;

    return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1],
                  int data_lines) {
    int i, len, index = 0;

    if (doc == NULL || data == NULL || data_lines == 0) {

        return FAILURE;

    }

    for (i = 0; i < data_lines; i++) {
        len = strlen(data[i]);

        /* if data has nothing... */
        if (len == 0) {
            /* increment the index */
            index++;

            /* increment the number of paragraph */
            doc->number_of_paragraphs++;

            /* set number of lines to 0 */
            doc->paragraphs[index].number_of_lines = 0;

        } else {

            /* copy the string from data to doc */
            strcpy(doc->paragraphs[index].lines[doc->paragraphs[index].number_of_lines],
                   data[i]);

            /* increment the number of lines */
            doc->paragraphs[index].number_of_lines++;
        }
    }

    /* increment the number of paragraphs */
    doc->number_of_paragraphs++;

    return SUCCESS;
}

int replace_text(Document *doc, const char *target, const char *replacement) {
    int i, j;

    char temp[MAX_STR_SIZE];

    char *target_position, *result;

    if (doc == NULL || target == NULL || replacement == NULL) {

        return FAILURE;

    }

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {

            /* set temp to null first */
            temp[0] = '\0';

            /* get the lines to replace */
            result = doc->paragraphs[i].lines[j];

            while ((target_position = strstr(result, target)) != NULL) {

                /* get string before the target */
                strncat(temp, result, target_position - result);

                /* add the replacement */
                strcat(temp, replacement);

                /* move pointer to the after target string */
                result = target_position + strlen(target);
            }
            /* add after target string to temp */
            strcat(temp, result);

            /* copy string to actual doc */
            strcpy(doc->paragraphs[i].lines[j], temp);
        }
    }
    return SUCCESS;
}

int highlight_text(Document *doc, const char *target) {
    char replace[MAX_STR_SIZE + 1] = "";

    if (doc == NULL || target == NULL) {

        return FAILURE;

    }

    /* make a new string for the replacement */
    /* put highlight start first */
    strcat(replace, HIGHLIGHT_START_STR);

    /* add target */
    strcat(replace, target);

    /* put highlight end */
    strcat(replace, HIGHLIGHT_END_STR);

    /* replace new string to the target */
    replace_text(doc, target, replace);

    return SUCCESS;
}

int remove_text(Document *doc, const char *target) {

    if (doc == NULL || target == NULL) {

        return FAILURE;
    }

    /* call the replace text function to replace target to empty string */
    replace_text(doc, target, "");

    return SUCCESS;
}