#compiler
CC = g++ -std=c++11 -ggdb

TARGET = libblockdsp
OUTPUTFILE = libblockdsp.a

# OBJOUTDIR will be DELETED on `make clean`
OBJOUTDIR = ./bin
LIBOUTDIR = ./lib
INCLUDEOUTDIR = ./include

CFLAGS = -Wall -pedantic
INCLUDES = -I "./Audio Manager/" \
		-I "./BlockDSP/" \
		-I "./Code Generation" \
		-I "./Logger/" \
		-I "/usr/local/include"

PUBLIC_HEADERS = "./Audio Manager/autil_audioprocessingunit.hpp" \
				"./Audio Manager/autil_dspkernel.hpp" \
				"./Audio Manager/autil_file.hpp" \
				"./Audio Manager/autil_manager.hpp" \
				"./Audio Manager/autil_thread_manager.hpp" \
				"./BlockDSP/bdsp_apu.hpp" \
				"./BlockDSP/bdsp_node.hpp" \
				"./BlockDSP/bdsp_number.hpp" \
				"./BlockDSP/bdsp_parameter.hpp" \
				"./BlockDSP/bdsp_system.hpp" \
				"./BlockDSP/dsphelpers.hpp" \
				"./BlockDSP/dsputil_delaybuffer.hpp" \
				"./Code Generation/bdsp_apuloader.hpp" \
				"./Code Generation/bdsp_codebuilder.hpp" \
				"./Code Generation/bdsp_compiler.hpp" \
				"./Logger/bdsp_logger.hpp"

LIBS_SEARCH = -L/usr/local/lib

LIBS = -lportaudio \
		-lsndfile \

SRC_FILES = ./Audio\ Manager/*.cpp \
		./BlockDSP/*.cpp \
		./Code\ Generation/*.cpp \
		./Logger/*.cpp


OBJECTS = ./$(OBJOUTDIR)/*.o

all: $(OBJECTS)
	@mkdir -p $(LIBOUTDIR)
	@mkdir -p $(INCLUDEOUTDIR)
	libtool -o $(TARGET).a -static $(OBJECTS)
	@mv *.a $(LIBOUTDIR)
	@cp -f $(PUBLIC_HEADERS) $(INCLUDEOUTDIR)/

$(OBJOUTDIR)/*.o:
	@mkdir -p $(OBJOUTDIR)
	$(CC) -c $(CFLAGS) $(LIBS_SEARCH) $(LIBS) $(INCLUDES) $(SRC_FILES)
	@mv *.o $(OBJOUTDIR)


install: all
	@mkdir -p /usr/local/include/blockdsp
	@cp -f $(PUBLIC_HEADERS) /usr/local/include/blockdsp
	@cp -f blockdsp.h /usr/local/include/blockdsp.h
	@cp -f $(LIBOUTDIR)/*.a /usr/local/lib/

dsptest.o: all
	$(CC) -o "dsptest.out" dsptest/main.cpp -I/usr/local/include $(LIBS_SEARCH) -lblockdsp $(LIBS)

dsptest : dsptest.o
	@./dsptest.out
	@rm -rf ./dsptest.out*
	@rm -rf ./test/resources/dsptest.dylib ./test/resources/dsptest_output

test: all
	cd test/gunit/ && make
	cd test/gunit/ && ./test_all

docs:
	cd docs && doxygen Doxyfile
	open docs/doxy/html/index.html

clean:
	@rm -rf $(OBJOUTDIR)
	@rm -rf $(LIBOUTDIR)/*
	@rm -rf $(INCLUDEOUTDIR)/*
	@cd test/gunit && make clean

.PHONY: clean
.PHONY: install
.PHONY: all
.PHONY: dsptest
.PHONY: test
.PHONY: docs

