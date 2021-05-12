#gcc   fork1.c  -L ../lib -lapue -I ../common -o fork1

CC=g++

CFLG=-g -c -I$(INC) -std=c++11 -mcx16


LIBPATH = ../lib
#LIB =  -lpthread \
#	   -lboost_system \
#	   -lboost_thread 
#

INC = ../common

SRC := $(wildcard *.cpp)
#OBJ = $(patsubst %.c,%.o,$(wildcard *.c)) 
OBJ := $(patsubst %.cpp,%.o,$(SRC)) 
APIDS := $(patsubst %.cpp,%,$(wildcard *.cpp))


.PHONY:clean
#.SUFFIXES:.cpp .o


#循环内必须用变量必须用{}，不能用(), 是因为借用的shell for循环
#在makefile中的shell变量要用2个$号表示变量名称
all:$(OBJ) 
	@for APID in $(APIDS) ;\
	do \
		${CC}  $${APID}.o -L${LIBPATH}  ${LIB}  -o $${APID} ; \
		echo -e "\033[32mbuild $$APID done\033[0m"; \
	done
	@rm -rf *.o
	 
%.o:%.cpp
	${CC} ${CFLG}  ${SRC}

clean:
	@rm -rf *.o  $(APIDS)

##########################################################################
objects := $(wildcard *.cpp)
#objects := $(wildcard *.h)
show:
	@echo $(objects)
	@echo "****************************************************************************************"
	@echo $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	@echo $(patsubst %.cpp,%,$(wildcard *.cpp))
	@echo "****************************************************************************************"

