#include <stdio.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define TRUE (1)
#define FALSE (!TRUE)

void serve_one(int sock) {
  while (TRUE) {
    uint32_t mes;
    if (read(sock, &mes, sizeof(mes)) <= 0)
      break;
    if (mes == 0)
      break;
  }
}

double to_double(struct timespec * ts) {
  double v = ((double)ts->tv_nsec) / 1000000000.0;
  v += ts->tv_sec;
  return v;
}


void client(char * hostname, int port, int interval) {
  int sock;
  struct sockaddr_in ca;
  ca.sin_family = AF_INET;
  ca.sin_port = htons(port);
  
  struct hostent *he = gethostbyname(hostname);
  if (he == NULL) {
    perror("gethostbyname");
    exit(1);
  }
  ca.sin_addr.s_addr = *((int*)he->h_addr);
  
  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket()");
    exit(1);
  }
  if (connect(sock, (struct sockaddr *) &ca, sizeof(ca)) != 0) {
    perror("connect()");
    exit(1);
  }

  while (TRUE) {
    uint32_t val0, val1;

    struct timespec b;
    clock_gettime(CLOCK_REALTIME, &b);
    double b_d = to_double(&b);
    
    printf("test\n");
    // send 4bytes
    val0 = 1;
    write(sock, &val0, sizeof(val0));

    // read 8bytes
    struct timespec st;
    read(sock, &val0, sizeof(val0));
    read(sock, &val1, sizeof(val1));
    st.tv_sec = val0;
    st.tv_nsec = val1;
    double st_d = to_double(&st);
    
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    uint32_t sec = t.tv_sec;
    uint32_t nsec = t.tv_nsec;
    double t_d = to_double(&t);

    double t1 = (st_d - b_d);
    double t2 = (t_d - st_d);
    printf("%f %f estimated_diff: %f\n", t1, t2, (t2-t1) /2.0);
    
    if (interval == 0) {
      val0 = 0;
      write(sock, &val0, sizeof(val0));
      break;
    } else {
      sleep(interval);
    }
  }
  close(sock);
}

void usage(){
  fprintf(stderr, "USAGE: client host port [interval]");
  exit(2);
}

int main(int argc, char ** argv) {
  int interval = 0;
  if (argc < 3)
    usage();
  if (argc > 3)
    interval = atoi(argv[3]);
  char * host = argv[1];
  int port = atoi(argv[2]);
  client(host, port, interval);
}
