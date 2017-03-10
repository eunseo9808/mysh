SRCS	= $(wildcard src/*.c)
OBJECTS	= $(SRCS:.c=.o)
COMMAND = $(wildcard mycmd/my*)
CFLAGS	= -g -W -Wall -Iinclude
PROJECT	= mysh 

remake:clean all

all: $(PROJECT) MYCMD

$(PROJECT):$(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ 

MYCMD:
	@for dir in $(COMMAND); do \
	make -C $$dir || exit $?; \
	done

clean:
	$(RM) src/*.o $(PROJECT)

