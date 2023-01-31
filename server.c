#include <stdio.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>

#define TRUE (1)
#define FALSE (!TRUE)

void serve_one(int sock) {
  while (TRUE) {
    uint32_t mes;
    if (read(sock, &mes, sizeof(mes)) <= 0)
      break;
    if (mes == 0)
      break;
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    uint32_t sec = t.tv_sec;
    uint32_t nsec = t.tv_nsec;
    write(sock, &sec, sizeof(sec));
    write(sock, &nsec, sizeof(nsec));
  }
}

void serve(int port) {
  struct sockaddr_in s_sa;
  s_sa.sin_family = AF_INET;
  s_sa.sin_addr.s_addr = htonl(INADDR_ANY);
  s_sa.sin_port = htons(port);

  int ssock = socket(AF_INET, SOCK_STREAM,0);
  if (bind(ssock, (struct sockaddr *)&s_sa, sizeof(s_sa)) != 0) {
    perror("bind");
    exit(1);
  }

  if (listen(ssock, 10) != 0) {
    perror("listen");
    exit(1);
  }
  
  while (TRUE) {
    int sock;
    struct sockaddr_in sa;
    unsigned int len = sizeof(sa);
    if ((sock = accept(ssock, (struct sockaddr *)&sa, &len)) < 0) {
      perror("accept()");
      exit(1);
    }
    serve_one(sock);
  }
}

void usage(){
  fprintf(stderr, "USAGE: server port");
  exit(2);
}

int main(int argc, char ** argv) {
  if (argc < 2)
    usage();
  int port = atoi(argv[1]);
  serve(port);
}


  
