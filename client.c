
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
	const char* name = "tcp:tcp_stream_class";
	/*const struct stream_class a;
	const struct pstream_class* class = &a;
	*/
	struct stream b;
	struct stream* p = &b;
	struct stream **streamp = &p;
	uint8_t dscp = 0;
        size_t n = 28;
	const char *buffer = "Hello,this is client,thanks!";
	stream_open(name,streamp,dscp);
	stream_connect(p);
	stream_send(p,buffer,n);
	stream_close(p);
	return 0;
}
