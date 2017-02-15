include Makefile.inc
HEADERS := $(wildcard *.h)
SOURCES := $(wildcard *.c)

CFLAGS=-Wall -pedantic -std=gnu99
EXE=np_shell
OBJ=function.o environment.o navigation.o parse.o exec.o

.PHONY: clean

all: CFLAGS += -g
all: $(EXE)

release: CFLAGS += -O3
release: $(EXE)
$(EXE): main.o $(OBJ)
	@echo "linking $@..."
	@$(CC) -o $@ $^

bin/%.o: /%.c
	@echo "compiling $<..."
	@mkdir -p bin/
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@echo "cleaning all your mess..."
	@rm -rf $(EXE) $(OBJ) bin/main.o
	@rm -rf bin/

doc: doc/index.html

doc/index.html: Doxyfile $(HEADERS) $(SOURCES)
	@echo "generating doc with doxygen..."
	@doxygen
