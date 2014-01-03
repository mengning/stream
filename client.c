/********************************************************************/
/* Copyright (C) SSE-USTC, 2013-2014                                */
/*                                                                  */
/*  FILE NAME             :  client.c                               */
/*  PRINCIPAL AUTHOR      :  Mengning                               */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2014/01/02                             */
/*  DESCRIPTION           :  test file for stream                   */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Lin zenan,2014/01/02
 *
 */
#include<stdio.h> 			
#include<stdlib.h>			
#include<sys/types.h>		
#include<sys/wait.h>		
#include<string.h>			
#include<pthread.h>
#include<unistd.h>

#include "stream.h"

int main()
{
    struct stream* p_stream;
    const char* stream_name = "tcp:127.0.0.1:1234";
    char pnum[4];
    uint8_t dscp = 1;
    int actualsend = 0;
    int index = 0;
  
    while(1)
    {
        if(stream_open(stream_name,&p_stream,dscp))
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
                actualsend = stream_send(p_stream,pnum,sizeof(pnum));
                index++;

                if(actualsend < 0)
                {
                    printf("stream send failure!\n");
                }
                else if(actualsend == 0)
                {
                    printf("stream send 0 bytes!\n");
                }
                else
                {
                    printf("stream actual send %d bytes,Data:%s\n",actualsend,pnum);
                }
            }
        }

        stream_close(p_stream);
        sleep(1);
    }

    return 0;
}
