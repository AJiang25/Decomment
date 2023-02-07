/*-------------------------------------------------------------------*/
/* decomment.c                                                       */
/* Author: Arnold Jiang                                              */
/*-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* defines constants of all states in the textual DFA */
enum State {
    NORMAL, IN_STRING, ESCAPE_IN_STRING, IN_CHAR, ESCAPE_IN_CHAR, 
    MAYBE_IN_COMMENT, IN_COMMENT, MAYBE_EXITING_COMMENT
};

/* Handles the NORMAL state of the dfa. c is the current dfa character.
Writes c to stdout as specified by the dfa and then returns the next 
state. */
enum State Normal(int c) {
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

/* Handles the IN_STRING state of the dfa. c is the current 
dfa character. Writes c to stdout as specified by the dfa and 
then returns the next state. */
enum State String(int c) {
    if (c == '\"') {
        putchar(c);
        return NORMAL;
    }
    else if (c == '\\') {
        putchar(c);
        return ESCAPE_IN_STRING;
    }
    else {
        putchar(c);
        return IN_STRING;
    }
}

/* Handles the ESCAPE_IN_STRING state of the dfa. c is the current 
dfa character. Writes c to stdout and then returns the In_STRING 
state as specificed by the dfa. */
enum State escapeString(int c) {
    putchar(c);
    return IN_STRING;
}

/* Handles the IN_CHAR state of the dfa. c is the current 
dfa character. Writes c to stdout and then returns the next 
state as specificed by the dfa. */
enum State Char(int c) {
    if (c == '\'') {
        putchar(c);
        return NORMAL;
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

/* Handles the ESCAPE_IN_CHAR state of the dfa. c is the current 
dfa character. Writes c to stdout and then returns IN_CHAR as 
specificed by the dfa. */
enum State escapeChar(int c) {
    putchar(c);
    return IN_CHAR;
}

/* Handles the MAYBE_IN_COMMENT state of the dfa. c is the current 
dfa character. Writes c to stdout as specified by the dfa and 
then returns the next state. */
enum State maybeInComment(int c) {
    if (c == '\"') {
        putchar('/');
        putchar(c);
        return IN_STRING;
    }
    else if (c == '\'') {
        putchar('/');
        putchar(c);
        return IN_CHAR;
    }
    else if (c == '/') {
        putchar(c);
        return MAYBE_IN_COMMENT;
    }
    else if (c == '*') {
        putchar(' ');
        return IN_COMMENT;
    }
    else {
        putchar('/');
        putchar(c);
        return NORMAL;
    }
}

/* Handles the IN_COMMENT state of the dfa. c is the current 
dfa character. Writes c to stdout as specified by the dfa and 
then returns the next state. */
enum State inComment(int c) {
    if (c == '*') 
        return MAYBE_EXITING_COMMENT;
    else {
        if (c == '\n') {
            putchar(c);
        }
        return IN_COMMENT;
    }
}

/* Handles the MAYBE_EXITING_COMMENT state of the dfa. c is 
the current dfa character. Writes c to stdout as specified 
by the dfa and then returns the next state. */
enum State maybeExitingComment(int c) {
    if (c == '/') {
        return NORMAL;
    }
    else if (c == '*') {
        return MAYBE_EXITING_COMMENT;
    }
    else {
        if (c == '\n') {
            putchar(c);
        }
        return IN_COMMENT;
    }
}

/* Reads text from stdin. For every character read, the program
continuously moves through states until EOF, as outlined by the dfa.
For every noncomment, the result is written to stdout. Returns
EXIT_FAILURE if in unterminated comment, else returns EXIT_SUCCESS. */
int main(void) {
    /* Uses the dfa approach. The initial state of the program */
    enum State state = NORMAL;
    /* character variable that maintains current DFA char*/
    int c;
    /* counts the total number of newlines */
    int counter = 0;
    /* tracks the line number of the last time a comment began */
    int last = 0;

    while ((c = getchar()) != EOF) {
        if (state == NORMAL) {
            state = Normal(c);
            last = 0;
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
            state = maybeInComment(c);
        }
        else if (state == IN_COMMENT) {
            state = inComment(c);   
        }
        else if (state == MAYBE_EXITING_COMMENT) {
            state = maybeExitingComment(c);
        }
        if (c == '\n') {
            counter++;
            last++;
        }
    }
    counter = (counter - last)+ 1;
    /* addresses corner case of a false comment start */
    if (state == MAYBE_IN_COMMENT) {
        putchar('/');
    }
    /* addresses unterminated comment cases */
    if (state == IN_COMMENT || state == MAYBE_EXITING_COMMENT) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", counter);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}