#include <cstring>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>

#define PROGRAM "artic"
#define VERSION "1.0.2"

using namespace std;

int hexToDec(char* hex) {
  int a = 1;
  int res = 0;
  for (int i=strlen(hex)-1; i>=0; i--) {
    int k = (hex[i] <= '9') ? (hex[i] - '0') : (hex[i]-'A'+10);
    res += k * a;
    a *= 16;
  }
  return res;
}

int fileExists(string f) {
  struct stat fileStat;
  if (stat(f.c_str(), &fileStat) == -1) return 0;
  else return 1;
}

void parse_filename(string f1, string& f2) {
  f2 = f1.substr(f1.find_last_of("/")+1);
}

void cp(string from, string to) {
  char block[1024];
  int in, out;
  int nread;
  in = open(from.c_str(), O_RDONLY);
  out = open(to.c_str(), O_WRONLY | O_CREAT, S_IRUSR|S_IWUSR);
  while((nread = read(in, block, sizeof(block))) > 0) write(out, block, nread);
}

void print_usage(FILE* stream, int exit_code) {
  fprintf(stream, "Usage: \n  %s [ options ] [[ ip address ] [ inputfile ]]\n", PROGRAM);
  fprintf(stream, "\n%s is a simple program which is used to send\n", PROGRAM);
  fprintf(stream, "an encrypted file between computer on a network.\n\n");
  fprintf(stream, "Options:\n");
  fprintf(stream, "  -h  --help                  Display this usage information.\n");
  fprintf(stream, "  -r  --receive               Receive sent file.\n");
  fprintf(stream, "  -d  --no-delete             Do not delete encrypted file.\n");
  fprintf(stream, "  -V  --verbose               Verbose operation.\n");
  fprintf(stream, "  -v  --version               Output version information and exit.\n");
  fprintf(stream, "\nThis %s has a super power.\n\n", PROGRAM);
  exit(exit_code);
}

void print_version(FILE* stream, int exit_code) {
  fprintf(stream, "%s-%s\n", PROGRAM, VERSION);
  exit(exit_code);
}

void print_super() {
  char list[] =
  {
    0x20, 0x20, 0x2E, 0x2D, 0x22, 0x22, 0x2D, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x28, 0x20, 0x29, 0x2D, 0x22, 0x60, 0x60, 0x60, 0x22, 0x2D, 0x28, 0x20, 0x29, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2E, 0x2D, 0x22, 0x22, 0x2D, 0x2E, 0xA,
    0x20, 0x2F, 0x20, 0x4F, 0x20, 0x4F, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x4F, 0x20, 0x4F, 0x20, 0x5C,
    0xA,  0x20, 0x7C, 0x4F, 0x20, 0x2E, 0x2D, 0x2E, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x30, 0x20, 0x5F, 0x20, 0x30, 0x20, 0x20, 0x20, 0x5C,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x2E, 0x2D, 0x2E, 0x20, 0x4F,
    0x7C, 0xA,  0x20, 0x5C, 0x20, 0x28, 0x20, 0x20, 0x20, 0x29, 0x20, 0x20, 0x27, 0x2E, 0x20, 0x20,
    0x20, 0x20, 0x5F, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2E, 0x27, 0x20, 0x20, 0x28, 0x20, 0x20, 0x20, 0x29,
    0x20, 0x2F, 0xA,  0x20, 0x20, 0x27, 0x2E, 0x60, 0x2D, 0x27, 0x20, 0x20, 0x20, 0x20, 0x20, 0x27,
    0x2D, 0x2E, 0x2F, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x7C, 0x60, 0x5C, 0x2E, 0x2D, 0x27, 0x20, 0x20, 0x20, 0x20, 0x20, 0x27, 0x2D, 0x27,
    0x2E, 0x27, 0xA,  0x20, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x7C, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20,
    0x20, 0x20, 0x2F, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F,
    0xA,  0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C,
    0x20, 0x20, 0x20, 0x27, 0x2E, 0x20, 0x20, 0x27, 0x2E, 0x5F, 0x2E, 0x27, 0x20, 0x20, 0x2E, 0x27,
    0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0xA,  0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x27, 0x2E, 0x20,
    0x20, 0x20, 0x60, 0x27, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x27, 0x60, 0x20, 0x20, 0x20, 0x2E, 0x27,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0xA,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x5C, 0x20, 0x20, 0x20, 0x2E, 0x27, 0x20, 0x20, 0x20, 0x20, 0x27, 0x2D, 0x2E, 0x5F, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2E, 0x2D, 0x27, 0x5C, 0x20, 0x20, 0x20, 0x27, 0x2E,
    0x20, 0x20, 0x20, 0x2F, 0xA,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x2F, 0x60,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0x27, 0x27, 0x27, 0x27, 0x27,
    0x27, 0x29, 0x20, 0x20, 0x20, 0x20, 0x29, 0x20, 0x20, 0x20, 0x20, 0x60, 0x5C, 0x7C, 0xA,  0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x20, 0x20, 0x20, 0x20, 0x28,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2C, 0x5C, 0xA,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x3B, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x27, 0x2D, 0x2E, 0x2E, 0x2D, 0x27,
    0x2F, 0x20, 0x3B, 0xA,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x27, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x7C, 0xA,  0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0x27,
    0x2D, 0x2D, 0x2D, 0x27, 0x60, 0x20, 0x20, 0x20, 0x7C, 0xA,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x3B, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x3B, 0xA,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0xA,  0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x2E, 0x27, 0xA,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x27, 0x2D, 0x2E, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x2E, 0x2D, 0x27, 0xA,  0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x2F, 0x60, 0x22,
    0x20, 0x20, 0x27, 0x20, 0x20, 0x2D, 0x20, 0x2D, 0x20, 0x2D, 0x20, 0x20, 0x27, 0x20, 0x22, 0x60,
    0x60, 0x20, 0x5C, 0x5F, 0x5F, 0xA,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x2F, 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x5E,
    0x5C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0x5C, 0xA,  0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x28, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x2E, 0x27, 0x20, 0x20, 0x20, 0x27, 0x2E, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x29, 0x2F, 0xA,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x27, 0x2E, 0x28, 0x5F, 0x5F, 0x28, 0x5F, 0x5F, 0x2E, 0x2D, 0x27, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x27, 0x2E, 0x5F, 0x5F, 0x29, 0x5F, 0x5F, 0x29, 0x2E, 0x27, 0xA,
    0xA
  };
  for (int i=0; i<strlen(list); i++) printf("%c", list[i]);
  printf("\nBrr... It's cold, isn't it?\n");
  exit(0);
}

