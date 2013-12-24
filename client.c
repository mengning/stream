
#include<stdio.h> 			/* perror */
#include<stdlib.h>			/* exit	*/
#include<sys/types.h>		/* WNOHANG */
#include<sys/wait.h>		/* waitpid */
#include<string.h>			/* memset */
#include<pthread.h>

#include "poll-loop.h"
#include "stream.h"
#include "stream-provider.h"

int main() 
{	
	int snd;
	int error;
	int erro;
	struct stream *streamp;
	uint8_t dscp = 1;
        /*size_t n = 28;*/
	const char *buffer = "Hello,this is client,thanks!";
	size_t n = sizeof(*buffer);
	const char* name = "tcp:127.0.0.1:1234";	
	error = stream_open(name,&streamp,dscp);
	if(error != 0)printf("stream open failure!\n");
	if(error == 0)
	{

		erro = stream_connect(streamp);
		if(erro != 0 ){
			printf("stream connect failure!\n");
			stream_close(streamp);
			return 0;
		}
		printf("connect success!\n");
		snd = stream_send(streamp,buffer,n);
		if(snd < 1 && snd > n){
			printf("snd failure!\n");
			stream_close(streamp);
		}
		
		printf("snd success!\n");			
		printf("%d\n",snd);
		printf("%s\n",buffer);
	}
	
	stream_close(streamp);
	return 0;
}
