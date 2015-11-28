#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <sstream>

#include "crypto.h"
#include "generate.h"
#include "utility.h"

#define PROGRAM "artic"
#define VERSION "1.0.2"
#define PORT 2503

using namespace std;

/* List of valid short options */
const char* short_options = "rdvVh";

/* List of valid long options */
const struct option long_options[] = {
  { "receive"	, 0, NULL, 'r'},
  { "no-delete" , 0, NULL, 'd'},
  { "version"   , 0, NULL, 'v'},
  { "verbose"   , 0, NULL, 'V'},
  { "help"      , 0, NULL, 'h'},
  { NULL        , 0, NULL,  0 }
};

string cryptSendTemp = "cryptSend.txt";
string cryptRecvTemp = "cryptRecv.txt";

int main(int argc, char** argv) {
  bool isSender    = true;
  bool delRecvTemp = true;
  bool verboseMode = false;
  int e, d, n;

  string filename;
  string ipaddr;

  int pos = 1;
  int next_option;

  do {
    next_option = getopt_long(argc, argv, short_options, long_options, NULL);
    switch (next_option) {
      case 'r': isSender    = false;
                break;
      case 'd': delRecvTemp = false;
                break;
      case 'V': verboseMode = true;
                break;
      case 'v': print_version(stdout, 0);
      case 'h': print_usage(stdout, 0);
      case '?': print_usage(stderr, 1);
      default : continue;
    }
    ++pos;
  } while (next_option != -1);

  if (isSender) {
    if (pos < argc) {
      ipaddr = argv[pos++];
      if (strcmp(ipaddr.c_str(), "super") == 0) print_super();
    }
    else print_usage(stderr, 1);

    if (pos < argc) {
      filename = argv[pos];
      if (!fileExists(filename.c_str())) {
        printf("File %s not found\n", filename.c_str());
        exit(0);
      }
    }
    else print_usage(stderr, 1);
  }

  printf("Welcome to %s vers. %s\n\n", PROGRAM, VERSION);
  printf("Initializing %s...\n", PROGRAM);

  if (isSender) {
    printf("Checking connection to %s ... ", ipaddr.c_str());
    sleep(1);

    struct sockaddr_in address;
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("* socket error\n");
      exit(1);
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = inet_addr(ipaddr.c_str());
    int len = sizeof(address);
    int result = connect(sockfd, (struct sockaddr *)&address, len);

    if (result == -1) {
      printf("failed. Cannot connect.\n");
      exit(1);
    }
    else printf("ok\n");

    printf("--> Receiving public keys... ");
    char key1[BUFSIZ], key2[BUFSIZ];
    int tmp;
    int len1 = recv(sockfd, key1, BUFSIZ, 0);
    if (len1) tmp = send(sockfd, "ok", 2, 0);
    int len2 = recv(sockfd, key2, BUFSIZ, 0);
    if (len1 && len2) {
      key1[len1] = key2[len2] = '\0';
      e = atoi(key1);
      n = atoi(key2);
      printf("ok\n");
    }
    else {
      printf("failed. Cannot receive keys.\n");
      printf("%s will now terminate.\n", PROGRAM);
      exit(1);
    }

    printf("--> Encrypting message... ");
    if (encrypt(filename, cryptSendTemp, e, n)) printf("success\n");
    else {
      printf("failed\n");
      printf("%s will now terminate.\n", PROGRAM);
      exit(1);
    }

    printf("--> Now sending packets... ");
    FILE* fin;
    fin = fopen(cryptSendTemp.c_str(), "r");
    if (!fin) {
      printf("failed. Crypt text cannot be found.\n");
      printf("%s will now terminate.\n", PROGRAM);
      exit(1);
    }
    char buf[BUFSIZ];
    while (fscanf(fin, " %s", buf) != EOF) {
      tmp = send(sockfd, buf, strlen(buf), 0);
      int ans_len = recv(sockfd, buf, BUFSIZ, 0);
      buf[ans_len] = '\0';
      if (strcmp(buf, "ok") != 0) {
        printf("failed. Error while sending packets.\n");
        printf("%s will now terminate.\n", PROGRAM);
        exit(1);
      }
    }
    tmp = send(sockfd, "fin", 3, 0);
    fclose(fin);
    tmp = send(sockfd, filename.c_str(), filename.length(), 0);
    printf("ok!\nPackets sent.\n");

    unlink(cryptSendTemp.c_str());

    close(sockfd);
  }
  else {
    int server_sockfd, client_sockfd;
    socklen_t server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("* socket error\n");
      exit(1);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);
    server_len = sizeof(server_address);
    if (bind(server_sockfd, (struct sockaddr *)&server_address, server_len) < 0) {
      printf("* creating socket error\n");
      exit(1);
    }

    listen(server_sockfd, 5);

    printf("\nWaiting for packets...\n");
    client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
    if (client_sockfd) {
      printf("(*) Connection Received\n");

      printf("--> Generating random keys... ok\n");
      generate_e_d(e, d, n, verboseMode);

      printf("--> Sending public keys to client... ");
      char key1[10], key2[10], buf[BUFSIZ];
      int len1, len2 = 0;
      sprintf(key1, "%d", e);
      sprintf(key2, "%d", n);
      len1 = send(client_sockfd, key1, strlen(key1), 0);
      int tmp = recv(client_sockfd, buf, BUFSIZ, 0);
      if (strcmp(buf, "ok") == 0)
        len2 = send(client_sockfd, key2, strlen(key2), 0);
      if (len1 && len2) printf("ok\n");
      else {
        printf("failed. Cannot send keys.\n");
        printf("%s will now terminate\n", PROGRAM);
        exit(1);
      }

      printf("--> Receiving encrypted packets... ");
      FILE *fout;
      bool isFirst = true;
      fout = fopen(cryptRecvTemp.c_str(), "w");
      tmp = recv(client_sockfd, buf, BUFSIZ, 0);
      buf[tmp] = '\0';
      if (!tmp) printf("failed\n");
      else {
        while (strcmp(buf, "fin") != 0) {
          int send_tmp = send(client_sockfd, "ok", 2, 0);
          if (!isFirst) fprintf(fout, " ");
          isFirst = false;
          fprintf(fout, buf);
          tmp = recv(client_sockfd, buf, BUFSIZ, 0);
          buf[tmp] = '\0';
        }
        fprintf(fout, "\n");
        fclose(fout);
        recv(client_sockfd, buf, BUFSIZ, 0);
        filename = buf;
        printf("ok\n");

        printf("--> Decrypting packets... ");
        string newfilename;
        parse_filename(filename, newfilename);
        newfilename = "received/" + newfilename;
        if (decrypt(cryptRecvTemp, newfilename, d, n)) printf("success\n");
        else {
          printf("failed\n");
          printf("%s will now terminate.\n", PROGRAM);
          exit(1);
        }
      }
      printf("Packets received successfully!\n");

      if (fileExists(cryptRecvTemp) && delRecvTemp) unlink(cryptRecvTemp.c_str());
      close(client_sockfd);
    }
  }
  exit(0);
}
