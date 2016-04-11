CC = gcc
TARGET = tsschecker_tool
LD_FLAGS = -L/usr/local/lib -L/opt/local/lib -lplist -lpartialzip-1.0 -lz -lcurl -lcrippy-1.0

all : $(TARGET)

tsschecker_tool : tsschecker/main.o tsschecker/download.o tsschecker/jsmn.o tsschecker/tss.o tsschecker/tsschecker.o
		$(CC) tsschecker/main.o tsschecker/download.o tsschecker/jsmn.o tsschecker/tss.o tsschecker/tsschecker.o $(LD_FLAGS) -o $(TARGET)
		@echo "Successfully built $(TARGET)"

main.o : tsschecker/main.c
		$(CC) -c tsschecker/main.c -o tsschecker/main.o

download.o : tsschecker/download.c 
		$(CC) -c tsschecker/download.c -o tsschecker/download.o

jsmn.o : tsschecker/jsmn.c
		$(CC) -c tsschecker/jsmn.c -o tsschecker/jsmn.o

tss.o : tsschecker/tss.c
		$(CC) -c tsschecker/tss.c -o tsschecker/tss.o

tsschecker.o :
		$(CC) -c tsschecker/tsschecker.c -o tsschecker/tsschecker.o

install :
		cp $(TARGET) /usr/local/bin/
		@echo "Installed $(TARGET)"
clean :
		rm -rf tsschecker/*.o $(TARGET)	
