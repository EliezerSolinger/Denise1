/** HTML/XML PARSER LIBRARY*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



#define MULTILINE(...) #__VA_ARGS__

const char *html_sample=MULTILINE(
    <!DOCTYPE html>
    <html>
        <head>
            <title>Hello World</title>
        </head>
        <body>
            <h1>Hello World</h1>
            <p>This is a paragraph.</p>
        </body>
    </html>
);

bool verify_html_closed_tags(const char *html_code) {
    size_t index=0;
    bool waiting_for_tag_open=true;
    // check if all tags are closed properly
    while (html_code[index]!='\0') {
        if(waiting_for_tag_open) {
            if(html_code[index]=='<') waiting_for_tag_open=false;
            if(html_code[index]=='>') return false;
        } else {
            if(html_code[index]=='>') waiting_for_tag_open=true;
            if(html_code[index]=='<') return false;
        }
        index++;
    }   
    return waiting_for_tag_open;
}
typedef struct _HtmlToken {
    size_t start;
    size_t end;
} HtmlToken;

bool validate_html_tags(const char *html_code) {
    size_t index=0;
    bool waiting_for_tag_open=true;
    HtmlToken token={0,0};
    // check if all tags are closed properly
    while (html_code[index]!='\0') {
        if(waiting_for_tag_open) {
            if(html_code[index]=='<') {
                waiting_for_tag_open=false;
                token.start=index;
            }
            if(html_code[index]=='>') return false;
        } else {
            if(html_code[index]=='>') {
                waiting_for_tag_open=true;
                token.end=index;
                fwrite(&html_code[token.start], 1, token.start-token.end+1, stdout);
                /// export token to a list here
                printf("\n");
            }
            if(html_code[index]=='<') return false;
        }
        index++;
    }   
    return waiting_for_tag_open;
}


void main() {
    printf(" is_valid : %i",validate_html_tags(html_sample));
}    
