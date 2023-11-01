#include <extras/err.h>
#include <stdio.h>
#include <sys/mount.h>
#include <unistd.h>

#define BUF_SIZE 1024

static void usage_error() {
  errexit("Usage: mount device mount-point\n"
          "\t-t fs-type (required)");
}

static void copy_string(char *source, char *destination) {
  if (snprintf(destination, BUF_SIZE, "%s", source) < 0) {
    errexit("snprintf");
  }
}

int main(int argc, char *argv[]) {
  int opt;
  char *device, *mount_point;
  char fs_type[BUF_SIZE];
  unsigned long mount_flags = 0;

  while ((opt = getopt(argc, argv, ":t:h")) != -1) {
    switch (opt) {
    case 't':
      copy_string(optarg, fs_type);
      break;
    case 'h':
    case ':':
    case '?':
      usage_error();
    }
  }

  if (optind + 2 != argc) {
    usage_error();
  }

  device = argv[optind++];
  mount_point = argv[optind++];

  mount(device, mount_point, fs_type, mount_flags);

  return 0;
}
