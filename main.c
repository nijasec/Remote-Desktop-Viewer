
#include "main.h"


int decodeBMPIMG(struct imageinfo *bmpinfo)
{
  //  struct imageinfo *decodedbmp;
  
    static const unsigned MINHEADER = 54; //minimum BMP header size
   // decodedbmp=(struct imageinfo*)malloc(sizeof(struct imageinfo));
    

  if(bmpinfo->size < MINHEADER) return 1;
  if(bmpinfo->data[0] != 'B' || bmpinfo->data[1] != 'M') return 2; //It's not a BMP file if it doesn't start with marker 'BM'

  uint32_t pixeloffset = bmpinfo->data[10] + 256 * bmpinfo->data[11]; //where the pixel data starts
  //read width and height from BMP header
bmpinfo->w= bmpinfo->data[18] + bmpinfo->data[19] * 256;
  bmpinfo->h = bmpinfo->data[22] + bmpinfo->data[23] * 256;
  //read number of channels from BMP header
  if(bmpinfo->data[28] != 24 && bmpinfo->data[28] != 32) return 4; //only 24-bit and 32-bit BMPs are supported.
  uint32_t numChannels = bmpinfo->data[28] / 8;

  //The amount of scanline bytes is width of image times channels, with extra bytes added if needed
  //to make it a multiple of 4 bytes.
 uint32_t scanlineBytes = bmpinfo->w * numChannels;
  if(scanlineBytes % 4 != 0) scanlineBytes = (scanlineBytes / 4) * 4 + 4;

uint32_t dataSize = scanlineBytes *bmpinfo->h;
  if(bmpinfo->size < dataSize + pixeloffset) return -1; //BMP file too small to contain all pixels

//resizing
bmpinfo->size=bmpinfo->w * bmpinfo->h  * 4;
uint8_t *temp=(uint8_t*)malloc(bmpinfo->size);
  /*
  There are 3 differences between BMP and the raw image buffer for LodePNG:
  -it's upside down
  -it's in BGR instead of RGB format (or BRGA instead of RGBA)
  -each scanline has padding bytes to make it a multiple of 4 if needed
  The 2D for loop below does all these 3 conversions at once.
  */
  for(unsigned y = 0; y < bmpinfo->h; y++)
  for(unsigned x = 0; x < bmpinfo->w; x++) {
    //pixel start byte position in the BMP
    unsigned bmpos = pixeloffset + (bmpinfo->h - y - 1) * scanlineBytes + numChannels * x;
    //pixel start byte position in the new raw image
    unsigned newpos = 4 * y * bmpinfo->w + 4 * x;
    if(numChannels == 3) {
       // printf("color\n");
     temp[newpos + 0] = bmpinfo->data[bmpos + 2]; //R
     temp[newpos + 1] = bmpinfo->data[bmpos + 1]; //G
     temp[newpos + 2] = bmpinfo->data[bmpos + 0]; //B
     temp[newpos + 3] = 255;            //A
    } else {
     temp[newpos + 0] = bmpinfo->data[bmpos + 2]; //R
     temp[newpos + 1] = bmpinfo->data[bmpos + 1]; //G
      temp[newpos + 2] = bmpinfo->data[bmpos + 0]; //B
      temp[newpos + 3] = bmpinfo->data[bmpos + 3]; //A
      
    }
  }
  bmpinfo->data=(uint8_t*)realloc(bmpinfo->data,bmpinfo->size);
  memcpy(bmpinfo->data,temp,bmpinfo->size);
  free(temp);
  
  return 0;
    
}

struct imageinfo *getBitmapScreenshot()
{
  
    struct imageinfo *bmpinfo=(struct imageinfo*)malloc(sizeof(struct imageinfo));
    uint16_t BitsPerPixel =24;
    uint32_t Width= GetSystemMetrics(SM_CXSCREEN);
    uint32_t Height =  GetSystemMetrics(SM_CYSCREEN);
    
     // Create Header
    BITMAPFILEHEADER Header;
    memset(&Header, 0, sizeof(Header));
    Header.bfType = 0x4D42;
    Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Create Info
    BITMAPINFO Info;
    memset(&Info, 0, sizeof(Info));
    Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    Info.bmiHeader.biWidth = Width;
    Info.bmiHeader.biHeight = Height;
    Info.bmiHeader.biPlanes = 1;
    Info.bmiHeader.biBitCount = BitsPerPixel;
    Info.bmiHeader.biCompression = BI_RGB;
    Info.bmiHeader.biSizeImage = Width * Height * (BitsPerPixel > 24 ? 4 : 3);
    uint8_t * Pixels=NULL;
    HDC MemDC = CreateCompatibleDC(0);//Context);
    HBITMAP Section = CreateDIBSection(MemDC, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
 
  CURSORINFO cursor = { sizeof(cursor) };
	GetCursorInfo(&cursor);
     int x,y;
	if (cursor.flags == CURSOR_SHOWING) 
	{
		RECT rcWnd;
		RECT rc;
		HWND hwnd = GetDesktopWindow();
		GetWindowRect(hwnd,&rc);

		POINT point;
		GetCursorPos(&point);

		GetWindowRect(hwnd, &rcWnd);
        
		//_ICONINFOEXW
        ICONINFO info = { sizeof(info) };
        GetIconInfo(cursor.hCursor,&info);
		 x = point.x;
		 y = point.y;
		

	}
    
    
   
    DeleteObject(SelectObject(MemDC, Section));
    BitBlt(MemDC, 0, 0, Width, Height, GetDC(0), 0, 0, SRCCOPY);
    DrawIcon(MemDC,x,y,cursor.hCursor);
    
    
    DeleteDC(MemDC);
   
      // Concatenate everything
    bmpinfo->data = (uint8_t*)malloc(sizeof(Header) + sizeof(Info.bmiHeader) + (((BitsPerPixel * Width + 31) & ~31) / 8) * Height);

    memcpy(bmpinfo->data , (unsigned char*)&Header, sizeof(Header));
    memcpy(bmpinfo->data  + sizeof(Header), ( unsigned char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
    memcpy(bmpinfo->data  + sizeof(Header) + sizeof(Info.bmiHeader), Pixels, (((BitsPerPixel * Width + 31) & ~31) / 8) * Height);
     size_t size=sizeof(Header) + sizeof(Info.bmiHeader) + (((BitsPerPixel * Width + 31) & ~31) / 8) * Height;
     //memcpy(bmpinfo->data,bmp,size);
     //free(bmp);
     bmpinfo->size=size;
     DeleteObject(Section);
    return bmpinfo;
    
    
}

struct imageinfo *getPNGfromBMP(struct imageinfo *imginfo)
{
    int pnginfo;
   
    pnginfo=decodeBMPIMG(imginfo);
   
    if(pnginfo!=0)
    {
        printf("BMP decoding error  \n");
        return NULL;
    }
    
     struct imageinfo *pngimage=(struct imageinfo*)malloc(sizeof(struct imageinfo));
     pngimage->w=imginfo->w;
     pngimage->h=imginfo->h;
   
    lodepng_encode_memory(&pngimage->data,&pngimage->size,imginfo->data,imginfo->w,imginfo->h,LCT_RGBA, 8);
    
   
    
    
    return pngimage;
    
}


int CreateSocketInformation(struct arguments *args, SOCKET ClientSocket)
{
    struct session *s = malloc(sizeof(struct session));
    s->client=ClientSocket;
    s->id=args->scount++;
    s->next=NULL;
    s->tcp.client_state=CONNECTED;
    s->tcp.received=0;
    s->tcp.sent=0;
    s->tcp.forward=NULL;
    // Search session add new connection at end
	/*struct session *cur = args->ctx->session;
	struct session *p=NULL;
	/while (cur != NULL){
		p=cur;
		cur = cur->next;
		
	}*/
    s->next = args->ctx->session;
	args->ctx->session = s;
    
	 
    
}
void queue_client_data(const struct arguments *args, struct tcp_session *cur, const uint8_t *data,uint32_t datalen)
{
	struct segment *p = NULL;
	struct segment *s = cur->forward;

	while (s != NULL)
	{

		p = s;
		s = s->next;
	}

	
		struct segment *n = (struct segment *) malloc(sizeof(struct segment));

		n->len = datalen;
		n->sent = 0;
		n->data = (uint8_t*) malloc(datalen);
		memcpy(n->data, data, datalen);
       
		n->next = s;
		if (p == NULL)
			cur->forward = n;
		else
			p->next = n;
	
}

int monitor_tcp_session(const struct arguments *args, struct session *s) {

    
  if(s->tcp.client_state == CONNECTED){
           
        
        if(s->tcp.forward !=NULL)
            FD_SET(s->client,&args->ctx->WriteSet);
        else{
            printf("\nset to read");
             FD_SET(s->client,&args->ctx->ReadSet);
             
        }
        }
        else{
        return 0;
        }

  	return 1;
}
int check_tcp_session(const struct arguments *args,struct session *s)
{
    if(s->tcp.client_state == CLOSED)
    {
        printf("\nclosed");
        closesocket(s->client);
        return 1;
    }
    return 0;
       
}
void clear_tcp_data(struct tcp_session *cur)
{
	struct segment *s = cur->forward;
	while (s != NULL)
	{
		struct segment *p = s;
		s = s->next;
		free(p->data);
		free(p);
	}
}


int main(int argc, char **argv)
{
	printf("Screen Capture Application\n");
    printf("*************************\n");
       char q;
  
  //socket
   WSADATA wsaData;
    int iResult;
      int Ret;
int total;

   
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    int port=5000;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, "5000", &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
   
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);
    struct arguments *args = (struct arguments *) malloc(sizeof(struct arguments));
	struct context *ctx = (struct context *) malloc(sizeof(struct context));
	memset(ctx, 0, sizeof(struct context));
	args->ctx = ctx;
	args->server = ListenSocket;
	args->port = port;
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }


 u_long NonBlock = 1;

   if (ioctlsocket(ListenSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)

   {

      printf("ioctlsocket() failed with error %d\n", WSAGetLastError());

      return 1;

   }
   
      struct timeval timeout;
      timeout.tv_sec=5;
      int i;
      
      printf("\n Server is running on port:%d",port);
while(TRUE)

   {
         // Prepare the Read and Write socket sets for network I/O notification

      FD_ZERO(&args->ctx->ReadSet);

      FD_ZERO(&args->ctx->WriteSet);

 
     // Always look for connection attempts

      FD_SET(args->server, &args->ctx->ReadSet);
      struct session *s = args->ctx->session;

		while (s != NULL)
		{

		//printf("\ncheckin monitor");
			monitor_tcp_session(args,s);
        
			
			s = s->next;
            
		}
struct session *sl = NULL;
s = args->ctx->session;
			while (s != NULL)
			{
				int del = 0;

								
					del = check_tcp_session(args, s);

				if (del)
				{

					if (sl == NULL)
						args->ctx->session = s->next;
					else
						sl->next = s->next;

					struct session *c = s;
					s = s->next;
					
						
					
					clear_tcp_data(&c->tcp);
					printf("\nclosed session id:%d ",c->id);
					
					free(c);
					
					
				}
				else
				{
					sl = s;
					s = s->next;
				}
			}

     // printf("\nfdcount:%d",args->ctx->ReadSet.fd_count);
       if ((total = select(0, &args->ctx->ReadSet, &args->ctx->WriteSet, NULL, &timeout)) == SOCKET_ERROR)

      {

         printf("select() returned with error %d\n", WSAGetLastError());

         return 1;

      }

         
         // Check for arriving connections on the listening socket.

      if (FD_ISSET(args->server, &args->ctx->ReadSet))

      {
          total--;
          SOCKET newsocket;
          if ((newsocket = accept(args->server, NULL, NULL)) != INVALID_SOCKET)

         {

            // Set the accepted socket to non-blocking mode so the server will

            // not get caught in a blocked condition on WSASends

            NonBlock = 1;

            if (ioctlsocket(newsocket, FIONBIO, &NonBlock) == SOCKET_ERROR)

            {

               printf("ioctlsocket(FIONBIO) failed with error %d\n", WSAGetLastError());

              // return 1;

            }

            else
               printf("ioctlsocket(FIONBIO) is OK!\n");

 

            if (CreateSocketInformation(args,newsocket) == 0)

            {

                 printf("CreateSocketInformation(AcceptSocket) failed!\n");

                 //return 1;

            }

            else

                printf("CreateSocketInformation() is OK!\n");

 

         }
         else
         {

            if (WSAGetLastError() != WSAEWOULDBLOCK)

            {

               printf("accept() failed with error %d\n", WSAGetLastError());

               return 1;

            }

            else

               printf("accept() is fine!\n");

         }
      }
         struct session *ses=args->ctx->session;
         for (i = 0; total>0 && ses!=NULL; i++)
            {
            //    printf("\nenterinf=g loop");
               
                    if(FD_ISSET(ses->client,&args->ctx->ReadSet))
                    {
                        total--;
                          iResult = recv(ses->client, recvbuf, recvbuflen, 0);
                          
        if (iResult > 0) {
           // printf("Bytes received: %d\n", iResult);
            ses->tcp.received+=iResult;
            
          unsigned  char okmsg[260];
            
          //  printf("\nrcvd:%s",recvbuf);
 struct imageinfo *img= getBitmapScreenshot();
     // printf("\nwidth:%d -- height:%d",img->w,img->h);
  if(img!=NULL){
      printf("\nimg->size:%d",img->size);
       struct imageinfo *pngimg=getPNGfromBMP(img);
       
        printf("\nnew png image sent size:%d",pngimg->size);
       // sprintf(okmsg,"HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: HSmrc0sMlYUkAGmm5OPpG2HaGWk=\r\nSec-WebSocket-Protocol: chat\r\n");
    sprintf(okmsg,"HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: %d\r\n\r\n",pngimg->size);
   queue_client_data(args,&ses->tcp,okmsg,strlen(okmsg));
    queue_client_data(args,&ses->tcp,pngimg->data,pngimg->size);
    free(pngimg->data);
    free(pngimg);
      free(img->data);
      free(img);
      
                    }
        }
        else if(iResult==0)
        {
          //  printf("read closed");
            ses->tcp.client_state=CLOSED; 
        }
      else  {
         /// printf("\nlast error:%d",WSAGetLastError());
           if (WSAGetLastError() != WSAEWOULDBLOCK)
                                            
                        ses->tcp.client_state=CLOSED; 
                    }
                    }  
                    if(FD_ISSET(ses->client,&args->ctx->WriteSet))
                    {
                        // Forward data
total--;
											while (ses->tcp.forward != NULL)
											{
											 									//sending

												ssize_t sent = send(ses->client, ses->tcp.forward->data + ses->tcp.forward->sent, ses->tcp.forward->len - ses->tcp.forward->sent,0);
            //                                    printf("sent:%d",sent);
												if (sent < 0)
												{
													if (WSAGetLastError() != WSAEWOULDBLOCK)
													{
													 	ses->tcp.client_state = CLOSED;
                                                   		//	printf("\nsend closed");						
														break;
													}
													else
													{
														 printf("\nretry seniding");
														break;
													}
												}
                                                else if(sent==0)
                                                    ses->tcp.client_state = CLOSED;
												else
												{
												 										//c->tcp.sent += sent;
													ses->tcp.forward->sent += sent;
													ses->tcp.sent+=sent;
													if (ses->tcp.forward->sent == ses->tcp.forward->len)
													{
														struct segment *p = ses->tcp.forward;
														ses->tcp.forward = ses->tcp.forward->next;
														free(p->data);
														free(p);
													}
													else
													{
														printf("\npartial sent");
													}
												}
											}
                                            if(ses->tcp.forward==NULL)//transfer completed
                                            ses->tcp.client_state=CLOSED;
                                                
                        
                    }
                 ses=ses->next;   
                       
                
                
            }
          
      } 
         
   

   
    
	return 0;
    }
    
