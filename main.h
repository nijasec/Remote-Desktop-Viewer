
#define _WIN32_WINNT 0x0501

#include <stdio.h>
#include <windows.h>
#include "lodepng.h"
#include <stdint.h>
#include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>




#define DEFAULT_BUFLEN 65535

#define CONNECTED 1
#define CLOSED 2
struct arguments {
    SOCKET server;
	short port;
	int scount;
	long send;
	long rcvd;
    struct context *ctx;
};

struct context {
   
    
    int stopping;
	fd_set ReadSet;
    fd_set WriteSet;
    struct session *session;
};
struct tcp_session {
    int uid;
   
    int version;
  

 
   

    uint64_t sent;
    uint64_t received;

   
    uint8_t client_state;
	
	
   struct segment *forward;
};

struct session {
   
	int id;
    SOCKET client;
	
	union{
		struct tcp_session tcp;
		
	};
    
    struct session *next;
	
};
struct segment {
    uint32_t len;
    uint32_t sent;
   
    uint8_t *data;
    struct segment *next;
};

struct imageinfo{
  uint8_t *data;
   size_t size;
   uint32_t w;
   uint32_t h;
};


struct imageinfo *getBitmapScreenshot();
struct imageinfo *getPNGfromBMP(struct imageinfo *imginfo);

int decodeBMPIMG(struct imageinfo *bmpinfo);
void startserver(struct arguments *args);
int monitor_tcp_session(const struct arguments *args, struct session *s);
int check_tcp_session(const struct arguments *args,struct session *s);
void clear_tcp_data(struct tcp_session *cur);