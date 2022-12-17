#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


//defining the buffer size.
#define BUFF 1048576

/**
 * Head class prints the first n lines or c bytes of a file based by user input.
 * If no file or argument is given standard input will be used.
 */
int main(int argc, char* args[]) {
    // defining variables

      int lines = 0;
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
              option = true;
              // checking the line count
              if (lines < 1) {
                  setbuf(stdout, NULL);
                  printf("ERROR: Line count must be a number greater than 0\n");
                  return EXIT_FAILURE;
              } // if
              // checks if the argument is -c
          } else if (strcmp(args[x], "-c") == 0) {
              // sets the byte count
              bytes = atoi(args[x + 1]);
              option = true;
              by = bytes;
              if (bytes < 1) {
                  setbuf(stdout, NULL);
                  printf("ERROR: Byte count must be a number greater than 0\n");
                  return EXIT_FAILURE;
              } // if
              // switches the value to look for bytes.
              b = false;
          } // if
      } // if
      // if there is an option it will skip 2 indexes or else it will set the lines to 10.
      if (option == true) {
          x += 2;
      } else {
          lines = 10;
      } // if else
      // if theres no argument or file.
      if (x == argc) {
          argc++;
          arg = true;
      } // if
      // checks for multiple files.
      if (argc - x > 1) {
          m = true;
      } // if
      // loops through the files.
      for (int h = x; h < argc; h++) {
          if (arg != true) {
              f = args[h];
          } // if
          if ((*f == '-') || (arg == true)) {
              if ((*f == '-') && (m == true)) {
                  write(STDOUT_FILENO, "==> ", 4);
                  write(STDOUT_FILENO, "standard input", 14);
                  write(STDOUT_FILENO, " <==\n", 5);
              } // if
              if (b == true) {
                  int p;
                  int linesCnt = 0;
              out:
                  while((p = read(STDIN_FILENO, buf, BUFF) && (linesCnt < lines) > 0)) {
                      if (p == -1) {
                          perror(f);
                          continue;
                      } // if
                      // writes lines
                      if (linesCnt < lines) {
                          int wr = write(STDOUT_FILENO, buf, p);
                          if (wr == -1) {
                              perror(f);
                              continue;
                          } // if´
                          // increases the line count
                          linesCnt++;
                      } else {
                          // exits
                          goto out;
                      } // if
                  } // while
                  // checks if the byte option is selected
              } else {
                  int r = read(STDIN_FILENO, buf, BUFF);
                  int writingTo = bytes;
                  //reads the file
                  while((r > 0) && (writingTo > 0)) {
                      if (r == -1) {
                          perror(f);
                          continue;
                      } // if
                      // checking if the byte count is larger than the amount
                      if (r > writingTo) {
                          r = writingTo;
                      } // if
                      // writes lines
                      int wr1 = write(STDOUT_FILENO, buf, r);
                      // error checking
                      if (wr1 == -1) {
                          perror(f);
                          continue;
                      } // if
                      // checks how many more should be written.
                      writingTo -= r;
                  } // while
              } // if
              int temp = argc - 1;
              if (h != temp) {
                  // prints new lines
                  write(STDOUT_FILENO, "\n", 1);
              } // if
          } else {
              // opens the file.
              int fileDir = open(f, O_RDONLY);
              if (fileDir == -1) {
                  perror(f);
                  write(1, "\n", 1);
                  continue;
              } // if
              // reads the file
              int r2 = read(fileDir, buf, BUFF);
              if (r2 == -1) {
                  perror(f);
                  continue;
              } // if
              // calculates size of file
              off_t sz = lseek(fileDir, 0, SEEK_END);
              if (sz == -1) {
                  perror(f);
                  continue;

              } // if
              // checks to see if bytes exceed file size.
              if (bytes > sz) {
                  bytes = sz;
              } // if
              // counts amount of lines.
              if (b == true) {
                  for (int d = 0, s = 0; (s < lines) && (d < sz); d++) {
                      if (buf[d] == '\n') {
                          s++;
                      } //
                      bytes++;
                  } // for
              } // if
              // print file name if multiple
              if (m == true) {
                  setbuf(stdout, NULL);
                  printf("==> %s <==\n", f);
              } // if
              // writes byes
              int wr3 = write(STDOUT_FILENO, buf, bytes);
              if (wr3 == -1) {
                  perror(f);
                  continue;
              } // if
              int temp1 = argc -1;
              if (h != temp1) {
                  write(STDOUT_FILENO, "\n", 1);
              } // if

              // closes the file
              int clos = close(fileDir);
              if (clos == -1) {
                  perror(f);
                  continue;
              } // if
              // resets the byte count for the next file.
              bytes = by;
          } // if else
      } // for
} // main
