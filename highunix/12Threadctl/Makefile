#gcc   fork1.c  -L ../lib -lapue -I ../common -o fork1

CC=gcc

CFLG=-g -c -I$(INC) -std=c11

LIBPATH = ../lib
LIB = -lapue  \
      -lpthread \
      -lbsd

INC = ../common

#SRC = fork1.c  \
#	  fork2.c  \
#	  vfork1.c \
#      wait1.c  \
#	  tellwait1.c \
#	  tellwait2.c \
#	  echoall.c   \
#	  exec1.c 

SRC = $(wildcard *.c)

##OBJ = fork1.o  \
#	  fork2.o  \
#	  vfork1.o \
#	  wait1.o  \
#	  tellwait1.o \
#	  tellwait2.o \
#	  echoall.o   \
#	  exec1.o
#


SRC := $(wildcard *.c)
#OBJ = $(patsubst %.c,%.o,$(wildcard *.c)) 
OBJ := $(patsubst %.c,%.o,$(SRC)) 
APIDS := $(patsubst %.c,%,$(wildcard *.c))


.PHONY:clean
#.SUFFIXES:.cpp .o


#循环内必须用变量必须用{}，不能用(), 是因为借用的shell for循环
all:$(OBJ) 
	@for APID in $(APIDS) ;\
	do \
		${CC}  $${APID}.o -L${LIBPATH}  ${LIB}  -o $${APID} ; \
		echo -e "\033[32mbuild $$APID done\033[0m"; \
	done
	@rm -rf *.o
	 
%.o:%.c
	${CC} ${CFLG}  ${SRC}

clean:
	@rm -rf *.o  $(APIDS)

##########################################################################
objects := $(wildcard *.c)
#objects := $(wildcard *.h)
show:
	@echo $(objects)
	@echo "****************************************************************************************"
	@echo $(patsubst %.c,%.o,$(wildcard *.c))
	@echo $(patsubst %.c,%,$(wildcard *.c))
	@echo "****************************************************************************************"

