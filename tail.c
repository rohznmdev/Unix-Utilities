#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// defining the buffer size.
#define BUFF 1048576
/**
 * THis will print the last n lines or c bytes based on user input. If '-' or no argument is given
 * standard input is assumed.
 */
int main(int argc, char* args[]) {
    // declaring variables.
    int lines = 0;
    int l = 0;
    int bytes = 0;
    int by = 0;
    int x = 1;

    bool arg = false;
    bool b = true;
    char buf[BUFF];
    bool m = false;
    bool option = false;
    char* f;
    // checks to see if there is an argument
    if (argc > 2) {
        // checks if the argument is -n
        if (strcmp(args[x], "-n") == 0) {
            // if it is -n it will set the line count
            lines = atoi(args[x + 1]);
            l = lines;
            option = true;
            // cehcking the line count
            if (lines < 1) {
                setbuf(stdout, NULL);
                printf("ERROR: Line count must be a number greater than 0\n");
                return EXIT_FAILURE;
            } // if
            // checks if the argument is -c
        } else if (strcmp(args[x], "-c") == 0) {
            // if it is -c it will set the byte count
            bytes = atoi(args[x + 1]);
            by = bytes;
            option = true;
            // checking the byte count
            if (bytes < 1) {
                setbuf(stdout, NULL);
                printf("ERROR: Bytes count must be a number greater than 0\n");
                return EXIT_FAILURE;
            } // if
            // switches the value to look for bytes.
            b = false;
        } // else if
    } // if
    // if there is an option, it will skip 2 indexes or else it will set the lines to 10.
    if (option == true) {
        x += 2;
    } else {
        lines = 10;
        l = 10;
    } // if else
    // if theres no argument or file.
    if (argc == x) {
        argc++;
        arg = true;
    } // if
    // looking for multiple files
    int temp = argc - x;
    if (temp > 1) {
        m = true;
    } // if
    // loops through the files.
    for (int h = x; h < argc; h++) {
        int js;
        if (arg != true) {
            // sets the file to the current argument.
            f = args[h];
        }// if
        // checking to see whether to use standard input.
        if((*f == '-') || (arg == true)) {
            if ((*f == '-') && (m == true)) {
                write(STDOUT_FILENO, "==> ", 4);
                write(STDOUT_FILENO, "standard input", 14);
                write(STDOUT_FILENO, " <==\n", 5);
            } // if
            // creating a temp file to save the input to.
            js = creat("-", 0644);
            if (js == -1) {
                perror("-");
                continue;
            } // if
            // reads the standard input
            int r = read(STDIN_FILENO, buf, BUFF);
            // writes standard input to the file.
            while (r > 0) {
                int e = write(js, buf, r);
                // checking for errors.
                if (e == -1) {
                    perror("-");
                    continue;
                } // if
            } // while
            f = "-";
        } // if
        // opens the newly created file.

        js = open(f, O_RDONLY);
        if (js == -1) {
            perror(f);
            continue;
        } // if
        // reads file
        int r1 = read(js, buf, BUFF);
        if (r1 == -1) {
            perror(f);
            continue;
        } // if
        // counting the number of bytes in the file
        off_t sz = lseek(js, 0, SEEK_END);
        // error checking
        if (sz == -1) {
            perror(f);
            continue;
        } // if
        // checking to see if byte size exceeds file size.
        if (bytes > sz) {
            bytes = sz;
        } // if
        // this will check whether or not to look for bytes.
        if (b == true) {
            // creating byte counting variables
            int tot = 0;
            int totBytes = 0;
            // coutning lines
            for (int i = 0; i < sz; i++) {
                if (buf[i] == '\n') {
                    tot++;
                } // if
            } // for
            // checking if lines does not exceed total lines.
            if (lines > tot) {
                lines = tot;
            } // if
            // counts all bytes till last n lines.
            for (int d = 0, s = 0; (s < tot - lines) && (d < sz); d++) {
                if (buf[d] == '\n') {
                    s++;
                } // if
                totBytes++;
            } // for
            bytes = sz - totBytes;
        } // if
        off_t seek = lseek(js, (-1 * bytes), SEEK_END);
        if (seek == -1) {
            perror(f);
            continue;
        } // if
        // reads the bytes
        int r2 = read(js, buf, BUFF);
        if (r2 == -1) {
            perror(f);
            continue;
        } // if
        // prints multiple files
        if ((*f != '-') && (m == true)) {
            setbuf(stdout, NULL);
            printf("==> %s <==\n", f);
        } // if
        // writes to standard output
        int wr = write(STDOUT_FILENO, buf, bytes);
        if (wr == -1) {
            perror(f);
            continue;
        } // if
        int temporary = argc - 1;
        // writes to standard output
        if (h != temporary) {
            write(STDOUT_FILENO, "\n", 1);
        } // if
        // closes the temp file
        int clos = close(js);
        if (clos == -1) {
            perror(f);
            continue;
        } // if
        // reset the count for the next file if needed
        bytes = by;
        lines = l;
        // removes temp file.
        remove("-");
    } // for
} // main
