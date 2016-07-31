TARGET = tsschecker_tool
CFLAGS += -Wall -std=c11
LDFLAGS += -lplist -lpartialzip-1.0 -lz -lcurl -lcrippy-1.0 -lxml2 -lm

all : $(TARGET)

tsschecker_tool : tsschecker/main.o tsschecker/download.o tsschecker/jsmn.o tsschecker/tss.o tsschecker/tsschecker.o
		$(CC) $(CFLAGS) tsschecker/main.o tsschecker/download.o tsschecker/jsmn.o tsschecker/tss.o tsschecker/tsschecker.o $(LDFLAGS) -o $(TARGET)
		@echo "Successfully built $(TARGET)"

main.o : tsschecker/main.c
		$(CC) $(CFLAGS) -c tsschecker/main.c -o tsschecker/main.o

download.o : tsschecker/download.c 
		$(CC) $(CFLAGS) -c tsschecker/download.c -o tsschecker/download.o

jsmn.o : tsschecker/jsmn.c
		$(CC) $(CFLAGS) -c tsschecker/jsmn.c -o tsschecker/jsmn.o

tss.o : tsschecker/tss.c
		$(CC) $(CFLAGS) -c tsschecker/tss.c -o tsschecker/tss.o

tsschecker.o :
		$(CC) $(CFLAGS) -c tsschecker/tsschecker.c -o tsschecker/tsschecker.o

install :
		cp $(TARGET) /usr/local/bin/
		@echo "Installed $(TARGET)"
clean :
		rm -rf tsschecker/*.o $(TARGET)	
