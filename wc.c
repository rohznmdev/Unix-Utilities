#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// defining the buffer size.
#define BUFF 1048576

/**
 * This class will return the lines/words/byte count of files specified by the input.
 *
 */
int main(int argc, char* args[]) {
    // declaring variables.
    int o;
    int byteTotal = 0;
    int lineTotal = 0;
    int wordTotal = 0;
    int dash = 0;
    bool empty = false;
    bool option = false;
    bool bytes = false;
    bool words = false;
    bool lines = false;
    char buf[BUFF];
    char* f;
    // goes through the options
    while ((o = getopt(argc, args, "wlc")) != -1) {
        switch(o) {
            //words
        case 'w':
            words = true;
            option = true;
            break;
            // lines
        case 'l':
            lines = true;
            option = true;
            break;
            // bytes
        case 'c':
            bytes = true;
            option = true;
            break;
            // default case if wrong argument.
        default:
            setbuf(stdout, NULL);
            printf("ERROR: Invalid Argument:\n");
            return EXIT_FAILURE;
        } // switch
    } // while

    // checking if no option is selected.
    if (option != true) {
        words = true;
        lines = true;
        bytes = true;
    } // if

    // checking if there is no file or options
    if (argc == 1) {
        empty = true;
        argc++;
    }



    // loops through the files.
    for (int i = 1; i < argc; i++) {
        int l = 0;
        int b = 0;
        int w = 0;
        int x;

        // checks to see if it is not empty and then sets the file.
        if(empty != true) {
            f = args[i];
        } // if
        // checks to see whether to check standard input.
        if ((*f == '-') || (empty == true)) {
            if (!(strcmp(f, "-") && !empty == 0)) {
                dash++;
                if (dash + 1 == argc) {
                    argc++;
                    empty = true;
                } // if
                continue;
            } // if
            // use a certain mode.
            x = creat("-", 0644);
            int z = read(STDIN_FILENO, buf, BUFF);
            // this will read the standard input
            while (z > 0) {
                // writes standard input to a file.
                write(x, buf, z/*strlen(buf2)*/);
            } // while
            f = "-";
        } // if
        // opens the file
        x = open(f, O_RDONLY);
        if (x == -1) {
            perror(f);
            continue;
        } // if
        // reads the file
        if (read(x, buf, BUFF) == -1) {
            perror(f);
            continue;
        } // if
        // calculates length of the file in terms of bytes
        off_t sz = lseek(x, 0, SEEK_END);
        if (sz == -1) {
            perror(f);
            continue;
        } // if
        // sets the byte total to the size of the file in bytes.
        b = sz;
        byteTotal += b;
        char last = '\n';
        for (int h = 0; h < sz; h++) {
            // checks for words
            if (buf[h] == ' ') {
                w++;
            } // if
            //checks for lines and words
            if (buf[h] == '\n') {
                l++;
                if (buf[h] != last) {
                    w++;
                } // if
            } // if
            // error checking in count
            last = buf[i];
        } // for
        // adds the words and lines to the total amount.
        lineTotal += l;
        wordTotal += w;

        if (lines == true) {
            printf(" %d", l);
        }
        if (words == true) {
            printf(" %d", w);
        }
        if (bytes == true) {
            printf(" %d", b);
        }
        if (empty != true) {
            printf(" %s\n", f);
        } else {
            printf("\n");
        } // else if
        // closes the file
        int closeF = close(x);
        if (closeF == -1) {
            perror(f);
            continue;
        } // if
    } // for
    //prints the total counts
    int tot = argc - dash - 1;
    if (tot > 1) {
        if (lines == true) {
            printf("%d", lineTotal);
        }
        if (words == true) {
            printf(" %d", wordTotal);
        }
        if (bytes == true) {
            printf(" %d", byteTotal);
        } // else if
        printf(" total \n");
    } // if
    // removes the temp file
    remove("-");
    return EXIT_SUCCESS;
} // main
