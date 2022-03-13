TARGET:=wavtone
SRCS:=wavtone.c
OBJS:=$(SRCS:.c=.o)

CFLAGS:= -O0
LIBS:= -lm

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $@ $^ $(LIBS)

%.o: %.c %.h
	gcc $(CFLAGS) -c $<

%.o: %.c
	gcc $(CFLAGS) -c $<

clean:
	rm -rf $(TARGET) $(OBJS)