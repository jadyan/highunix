#gcc   fork1.c  -L ../lib -lapue -I ../common -o fork1

CC=g++

CFLG=-g -c -I$(INC) -std=c++11

LIBPATH = ../lib
#LIB = -lapue

INC = ../common

SRC := $(wildcard *.cpp)
#OBJ = $(patsubst %.c,%.o,$(wildcard *.c)) 
OBJ := $(patsubst %.cpp,%.o,$(SRC)) 
APIDS := $(patsubst %.cpp,%,$(wildcard *.cpp))
APP := Item31


.PHONY:clean
#.SUFFIXES:.cpp .o


#循环内必须用变量必须用{}，不能用(), 是因为借用的shell for循环
#在makefile中的shell变量要用2个$号表示变量名称
all:$(OBJ) 
	$(CC) $(OBJ) -o  $(APP)
	@rm -rf *.o
	 
%.o:%.cpp
	${CC} ${CFLG}  ${SRC}

clean:
	@rm -rf *.o  $(APP)

##########################################################################
objects := $(wildcard *.cpp)
#objects := $(wildcard *.h)
show:
	@echo $(objects)
	@echo "****************************************************************************************"
	@echo $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	@echo $(patsubst %.cpp,%,$(wildcard *.cpp))
	@echo "****************************************************************************************"

