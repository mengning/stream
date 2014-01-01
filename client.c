#include<stdio.h> 			/* perror */
#include<stdlib.h>			/* exit	*/
#include<sys/types.h>		/* WNOHANG */
#include<sys/wait.h>		/* waitpid */
#include<string.h>			/* memset */
#include<pthread.h>

#include "poll-loop.h"
#include "stream.h"
#include "stream-provider.h"


int main(int argc,char** argv)
{
    struct stream* p_stream;
    uint8_t dscp = 1;
    /*const char buffer[25] = "Hello,this is client:";*/
    const char* name = "tcp:127.0.0.1:1234";
    int actualsend=0;
    char pnum[4];
    char* ptemp;
    int index=0;
  

while(1)
{
    if(stream_open(name,&p_stream,dscp))
    {
        printf("stream open failure!\n");
    }
    else
    {
        printf("stream open sucessed!\n");

        if(stream_connect(p_stream))
        {
            printf("stream connect failure!\n");
        }
        else
        {
            printf("stream connect successed!\n");
         
            sprintf(pnum,"%d",index);
            actualsend = stream_send(p_stream,pnum,30);
            index++;

            if(actualsend < 0)
                printf("stream send failure!\n");
            else if(actualsend == 0)
                printf("stream send 0 bytes!\n");
            else
                printf("stream actual send %d bytes,from:%s\n",actualsend,pnum);
        }

    }

    stream_close(p_stream);
sleep(1);
}

    return 0;
}
