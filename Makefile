C=gcc
OBJS=logmon.o
SRCS=logmon.c
TARGET=logmon

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS)

$(OBJS) :
	$(CC) -c $(SRCS)

clean:
	rm -f $(OBJS) $(TARGET)
