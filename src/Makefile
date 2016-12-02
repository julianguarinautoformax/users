AR=ar
CC=gcc
CP=cp
CXX=g++
CFLAGS=-c -std=c++14 -I. -Wunused-variable -Wno-unknown-pragmas -fpermissive
LFLAGS=crv
LDFLAGS=-L../lib -L../list/lib -L../queue/lib -ltcpserver -llist -lqueue 
SOURCES=Users.cpp  DirUtils.cpp 
OBJECTS=$(SOURCES:.cpp=.o) 
OUTPUT=libUsers.a 
MKDIR=mkdir



.phony:all
all:$(OUTPUT)


$(OUTPUT):$(OBJECTS) 
	$(AR) $(LFLAGS) $(OUTPUT) $(OBJECTS) 	



%.o:%.cpp
	$(CXX) $(CFLAGS) $<

clean: 
	rm -rf *.o $(OUTPUT)

