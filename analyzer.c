#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define LETTER        0
#define DIGIT         1
#define UNKNOWN       99
#define EOF_CLASS     -1

#define INT_LIT       10
#define IDENT         11
#define ASSIGN_OP     20
#define ADD_OP        21
#define SUB_OP        22
#define MULT_OP       23
#define DIV_OP        24
#define LEFT_PAREN    25
#define RIGHT_PAREN   26
#define SEMICOLON     27
#define KW_IF         30
#define KW_INT        31
#define KW_WHILE      32
#define KW_RETURN     33
#define LEFT_BRACE    40
#define RIGHT_BRACE   41
#define GREATER_THAN  42
#define END_OF_FILE   -1

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int nextToken;
FILE *in_fp;

void addChar() {
    if (lexLen < 99) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    } else {
        printf("Error - lexeme too long\n");
    }
}

void getChar() {
    nextChar = getc(in_fp);
    if (nextChar != EOF) {
        if (isalpha(nextChar) || nextChar == '_') charClass = LETTER;
        else if (isdigit(nextChar)) charClass = DIGIT;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF_CLASS;
    }
}

void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

int lookup(char ch) {
    switch (ch) {
        case '(': addChar(); return LEFT_PAREN;
        case ')': addChar(); return RIGHT_PAREN;
        case '+': addChar(); return ADD_OP;
        case '-': addChar(); return SUB_OP;
        case '*': addChar(); return MULT_OP;
        case '/': addChar(); return DIV_OP;
        case '=': addChar(); return ASSIGN_OP;
        case ';': addChar(); return SEMICOLON;
        case '{': addChar(); return LEFT_BRACE;
        case '}': addChar(); return RIGHT_BRACE;
        case '>': addChar(); return GREATER_THAN;
        default : addChar(); return UNKNOWN;
    }
}

int checkKeyword(const char* str) {
    if (!strcmp(str, "if")) return KW_IF;
    if (!strcmp(str, "int")) return KW_INT;
    if (!strcmp(str, "while")) return KW_WHILE;
    if (!strcmp(str, "return")) return KW_RETURN;
    return IDENT;
}

int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        case LETTER:
            addChar(); getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar(); getChar();
            }
            nextToken = checkKeyword(lexeme);
            break;

        case DIGIT:
            addChar(); getChar();
            while (charClass == DIGIT) {
                addChar(); getChar();
            }
            nextToken = INT_LIT;
            break;

        case UNKNOWN:
            nextToken = lookup(nextChar);
            getChar();
            break;

        case EOF_CLASS:
            nextToken = END_OF_FILE;
            strcpy(lexeme, "EOF");
            break;
    }

    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}

int main() {
    in_fp = stdin;
    getChar();

    while (nextToken != END_OF_FILE) {
        lex();
    }

    return 0;
}
