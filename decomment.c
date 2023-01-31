#include <stdio.h>
#include <ctype.h>

/*Enum of all possible states in the DFA*/
enum STATE {
    NORMAL, IN_STRING, ESCAPE_IN_STRING, IN_CHAR, ESCAPE_IN_CHAR, 
    MAYBE_IN_COMMENT, IN_COMMENT, MAYBE_EXITING_COMMENT
};
/*The initial state of the program*/
enum STATE state = NORMAL;

/*Declares all the functions in this program*/
enum STATE Normal(int c);
enum STATE String(int c);
enum STATE escapeString(int c);
enum STATE Char(int c);
enum STATE escapeChar(int c);
enum STATE maybeComment(int c);
enum STATE Comment(int c);
enum STATE maybeExitingComment(int c);

/*Reads a character and moves between states*/
int main(void) {
    int c;
    while ((char) (c = getchar()) != EOF) {
        if (state == NORMAL) {
            state = Normal(c);
        }
        else if (state == IN_STRING) {
            state = String(c);
        }
        else if (state == ESCAPE_IN_STRING) {
            state = escapeString(c);
        }
        else if (state == IN_CHAR) {
            state = Char(c);
        }
        else if (state == ESCAPE_IN_CHAR) {
            state = escapeChar(c);
        }
        else if (state == MAYBE_IN_COMMENT) {
            state = maybeComment(c);
        }
        else if (state == IN_COMMENT) {
            state = Comment(c);
        }
        else if (state == MAYBE_EXITING_COMMENT) {
            state = maybeExitingComment(c);
        }
    }
    return 0;
}

enum STATE Normal(int c) {
    if ((char) c == '\"') {
        putchar(c);
        return IN_STRING;
    }
    else if (c == '\'') {
        putchar(c);
        return IN_CHAR;
    }
    else if (c == '/') {
        return MAYBE_IN_COMMENT;
    }
    else {
        putchar(c);
        return NORMAL;
    }
}

enum STATE String (int c) {
    if (c == '\"') {
        putchar(c);
        return NORMAL;
    }
    else if (c == '\\') {
        putchar(c);
        return IN_CHAR;
    }
    else {
        putchar(c);
        return IN_STRING;
    }
}

enum STATE escapeString(int c) {
    putchar(c);
    return IN_STRING;
}

enum STATE Char(int c) {
    if (c == '\'') {
        putchar(c);
        return IN_CHAR;
    }
    else if (c == '\\') {
        putchar(c);
        return ESCAPE_IN_CHAR;
    }
    else {
        putchar(c);
        return IN_CHAR;
    }
}

enum STATE escapeChar(int c) {
    putchar(c);
    state = IN_CHAR;
}

enum STATE maybeInComment(int c) {
    if (c == '\"') {
        putchar('\\');
        putchar(c);
        return IN_STRING;
    }
    else if (c == '\'') {
        putchar('\\');
        putchar(c);
        return IN_CHAR;
    }
    else if (c == '/') {
        putchar('\\');
        return MAYBE_IN_COMMENT;
    }
    else if (c == '*') {
        return IN_COMMENT;
    }
    else {
        putchar('\\');
        putchar(c);
        return NORMAL;
    }
}

enum STATE inComment(int c) {
    if (c == '*') 
        return MAYBE_EXITING_COMMENT;
    else {
        if (c == '\n') {
            putchar('\n');
        }
        return IN_COMMENT;
    }
}

enum STATE maybeExitingCommentState (int c) {
    if (c == '/') {
        putchar(' ');
        return NORMAL;
    }
    else if (c == '*') {
        return MAYBE_EXITING_COMMENT;
    }
    else {
        if (c == '\n') {
            putchar('\n');
        }
        return IN_COMMENT;
    }
}