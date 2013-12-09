#
# Makefile for stream
#
include ../make.h


OBJS =	stream.o

all:	$(OBJS) 
	$(AR) $(AR_FLAGS) $(STREAM_LIB) $(OBJS) 


.c.o:
	$(CC) $(CC_FLAGS)  $(COMMON_INCLUDE_DIRS) $<

clean:

	$(RM) -f  $(OBJS) $(STREAM_LIB) *.bak


