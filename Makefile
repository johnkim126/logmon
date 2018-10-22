CC=gcc
OBJS=logmon.o
SRCS=logmon.c
TARGET=logmon

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -g -o $@ $(OBJS)

$(OBJS) :
	$(CC) -g -c $(SRCS)

clean:
	rm -f $(OBJS) $(TARGET)
