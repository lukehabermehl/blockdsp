CXX = g++ -std=c++11 -ggdb
CXXFLAGS = -g -L../../lib -L/usr/local/lib -L../lib -lgtest -lgtest_main -lpthread -lblockdsp -lportaudio -lsndfile
INCS = -I ../../include -I../include -I/usr/local/include
OBJS = test_audiomanager_api.o \
		test_thread_manager.o \
		test_dsp_blocks.o \
		test_bdsp_system.o \
		test_code_builder.o \
		test_apu_node.o \
		test_parameter.o 

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCS) -o test_all test_all.cpp $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCS)

clean:
	@rm -f *.o
	@rm -f test_all
	@rm -rf *.dSYM