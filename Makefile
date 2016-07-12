#compiler
CC = g++ -std=c++11

TARGET = libblockdsp
OUTPUTFILE = libblockdsp.a

# OUTPUTDIR will be DELETED on `make clean`
OUTPUTDIR = ./bin
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


OBJECTS = ./$(OUTPUTDIR)/*.o

all:
	@mkdir -p $(OUTPUTDIR)
	$(CC) -c $(CFLAGS) $(INCLUDES) $(SRC_FILES)
	@mv *.o $(OUTPUTDIR)

install:
	libtool -o $(TARGET).a -static $(OBJECTS)
	@mv $(TARGET).a /usr/local/lib
	@mkdir -p /usr/local/include/blockdsp
	@cp -f $(PUBLIC_HEADERS) /usr/local/include/blockdsp
	@cp -f blockdsp.h /usr/local/include/blockdsp.h

test:
	$(CC) -o "dsptest.out" dsptest/main.cpp -I/usr/local/include $(LIBS_SEARCH) -lblockdsp $(LIBS)
	@./dsptest.out
	@rm ./dsptest.out
clean:
	@rm -r $(OUTPUTDIR)

