#include <extras/err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <unistd.h>

#define BUF_SIZE 1024

static void usage_error() {
  errexit("Usage: tail [ -n num ] file\n"
          "\t-n num\t\tnumber of last lines to print (default: 10)");
}

static long get_file_length(FILE *file) {
  fseek(file, 0, SEEK_END);
  return ftell(file);
}

int main(int argc, char *argv[]) {
  int opt;
  int num = 10;

  while ((opt = getopt(argc, argv, ":n:h")) != -1) {
    switch (opt) {
    case 'n':
      num = atoi(optarg);
      break;
    case 'h':
    case ':':
    case '?':
      usage_error();
    }
  }

  FILE *file;

  if ((file = fopen(argv[optind], "r")) == NULL) {
    errexit("could not open file");
  }

  long file_length = get_file_length(file);
  long to_read = MIN(BUF_SIZE, file_length);
  long current_position = file_length - to_read;

  unsigned int lines = 1; /* Last line counts immediately */
  long first_line_position = 0;

  char buffer[BUF_SIZE];
  size_t last_read = 0;
  size_t total_read = 0;

  while (total_read != file_length && lines != num) {
    if (fseek(file, current_position, SEEK_SET) == -1) {
      errexit("could not seek file");
    }

    last_read = fread(buffer, 1, to_read, file);

    if (ferror(file) != 0) {
      errexit("could not read file");
    }

    total_read += last_read;

    for (size_t i = 0; i < last_read; i++) {
      size_t index = last_read - i - 1;

      if (buffer[index] == '\n') {
        if (++lines == num) {
          first_line_position = current_position + index;
          break;
        }
      }
    }

    to_read = MIN(BUF_SIZE, current_position);
    current_position -= to_read;
  }

  printf("lines: %d\n", lines);

  return 0;
}
