/*
author: Wonjae Kim
UID: 117283365
Directory ID: wkim1128
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "document.h"

void test_remove_text();

int main() {

    test_remove_text();
    return 0;
}

void test_remove_text() {
    Document doc;
    /* test name */
    const char *doc_name = "test3";
    int data_lines = 1;

    /* initialize the data*/
    char data[20][MAX_STR_SIZE + 1] = {
        "I am hungry"};

    char target[6] = "hungry";

    char replacement[10] = "starving";

    init_document(&doc, doc_name);
    
    load_document(&doc, data, data_lines);

    remove_text(&doc, target);

    print_document(&doc);
}
