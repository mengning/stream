
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

	int rec;
	int error,erro;
	struct pstream *pstreamp;
	struct stream *streamp;
		
	size_t n = 1024;
	uint8_t dscp = 1;
	
	char *buffer = (char *)malloc(sizeof(char)*n);
	memset(buffer,0,sizeof *buffer);	
	const char* name = "ptcp:1234";
	error = pstream_open(name,&pstreamp,dscp);
	if(error != 0)printf("pstream open failure!\n");
	if(error == 0)
	{	
		do{
			erro = pstream_accept(pstreamp,&streamp);
			/*if(erro != 0 ){
				printf("pstream accept failure!\n");
				return 0;
			}*/
		}while(erro);
		if(erro == 0){
			printf("connect success!\r\n");			
		}	
		rec = stream_recv(streamp,buffer,n);
		if(rec < 1 && rec > n ){
			printf("recv failure!\n");
			stream_close(streamp);
			pstream_close(pstreamp);
			return 0;
		}
		
		printf("recv success!\n");
		printf("%d\n",rec);
		printf("%s\n",buffer);		
	}
		
	stream_close(streamp);
	pstream_close(pstreamp);
	return 0;
}
