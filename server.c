
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
	const char* name = "ptcp:ptcp_pstream_class";
	/*struct pstream_class a;
	struct pstream_class* class = &a;
	*/
	struct pstream b ;
	struct pstream* p = &b;
	struct pstream **pstreamp = &p;

	struct stream c;
	struct stream* q = &c;
	struct stream **streamp = &q;	
	size_t n = 28;
	uint8_t dscp = 0;
	pstream_open(name,pstreamp,dscp);
	char *buffer = (char *)malloc(sizeof(char)*(n+1));
	pstream_accept(p,streamp);
	stream_recv(*streamp,buffer,n);
	stream_close(*streamp);
	pstream_close(*pstreamp);
	return 0;
}
