#all:
#	g++ -Wall -Werror=nounused-variable -Wextra -Werror -pedantic -ggdb -std=c++11 ./src/* -o ./bin/mila2tm

BINARY=./bin/mila2tm
OBJECTS=debug.o \
	strom.o \
	lexan.o \
	parser.o \
	code.o \
	tabsym.o \
	vstup.o

CC=g++
LD=g++
MKDIR_P=mkdir -p
MAKE=make

CLIBS=
CFLAGS=-O2 -Wall -Wextra -Wformat-security -Wshadow -Wlogical-op -Wfloat-equal \
-Werror -pedantic -ggdb -std=c++11

SOURCE_DIR=./src
SRC=./src
BUILD=./bin

all: $(OBJECTS) $(BINARY)

$(BINARY): $(OBJECTS) ./main.o
	$(LD) $(CFLAGS) $(addprefix $(BUILD)/, main.o) \
	$(addprefix $(BUILD)/, $(OBJECTS)) -o $(BINARY) $(CLIBS)

main.o: $(SOURCE_DIR)/main.cpp $(SOURCE_DIR)/lexan.h $(SOURCE_DIR)/parser.h \
$(SOURCE_DIR)/strom.h $(SOURCE_DIR)/code.h
	$(CC) $(CFLAGS) -c -o $(BUILD)/$@ $< $(CLIBS)

debug.o: $(SOURCE_DIR)/debug.cpp $(SOURCE_DIR)/debug.h
	$(CC) $(CFLAGS) -c -o $(BUILD)/$@ $< $(CLIBS)

lexan.o: $(SOURCE_DIR)/lexan.cpp $(SOURCE_DIR)/lexan.h
	$(CC) $(CFLAGS) -c -o $(BUILD)/$@ $< $(CLIBS)

parser.o: $(SOURCE_DIR)/parser.cpp $(SOURCE_DIR)/parser.h
	$(CC) $(CFLAGS) -c -o $(BUILD)/$@ $< $(CLIBS)

strom.o: $(SOURCE_DIR)/strom.cpp $(SOURCE_DIR)/strom.h $(SOURCE_DIR)/tabsym.h
	$(CC) $(CFLAGS) -c -o $(BUILD)/$@ $< $(CLIBS)

tabsym.o: $(SOURCE_DIR)/tabsym.cpp $(SOURCE_DIR)/tabsym.h
	$(CC) $(CFLAGS) -c -o $(BUILD)/$@ $< $(CLIBS)

vstup.o: $(SOURCE_DIR)/vstup.cpp $(SOURCE_DIR)/vstup.h
	$(CC) $(CFLAGS) -c -o $(BUILD)/$@ $< $(CLIBS)

code.o: $(SOURCE_DIR)/code.cpp $(SOURCE_DIR)/code.h
	$(CC) $(CFLAGS) -c -o $(BUILD)/$@ $< $(CLIBS)

