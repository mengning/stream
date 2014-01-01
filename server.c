
#include<stdio.h> 			/* perror */
#include<stdlib.h>			/* exit	*/
#include<sys/types.h>		/* WNOHANG */
#include<sys/wait.h>		/* waitpid */
#include<string.h>			/* memset */
#include<pthread.h>


#include "stream.h"
#include "stream-provider.h"


int main(int argc,char** argv)
{

    struct pstream *p_pstream; 
    struct stream *p_stream;
    size_t n = 50;
    uint8_t dscp = 1;
    char buffer[50];
    int actualrecv;
    const char* name = "ptcp:1234";

    if(pstream_open(name,&p_pstream,dscp))
        printf("pstream open failure!\n");
    else
    {
while(1)
{  
    
	while(pstream_accept(p_pstream, &p_stream))
            ;
        printf("\nget one steam connect success!\n");

        printf("p_pstream name:%s\n", pstream_get_name(p_pstream));
        printf("p_pstream bound_port:%d\n", pstream_get_bound_port(p_pstream));
	printf("p_stream name:%s\n", stream_get_name(p_stream));


        printf("<remote_ip,remote_port> : <%d,%d>\n", stream_get_remote_ip(p_stream),stream_get_remote_port(p_stream));
        printf("<local_ip,local_port> : <%d,%d>\n", stream_get_local_ip(p_stream),stream_get_local_port(p_stream));


	
        actualrecv = stream_recv(p_stream,buffer,n);
 
       if(actualrecv < 0)
           printf("pstream receive failure!\n");
       else if(actualrecv == 0)
           printf("pstream receive 0 bytes!\n");
       else
           printf("pstream actual receive %d bytes,buffer:%s\n\n",actualrecv,buffer);

}


    stream_close(p_stream);
    pstream_close(p_pstream);

    }   
 


    return 0;
}
