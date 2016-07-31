CXX = g++ -std=c++11 -ggdb
CXXFLAGS = -g -L/usr/local/lib -L../lib -lgtest -lgtest_main -lpthread -lblockdsp -lportaudio -lsndfile
INCS = -I../include -I../../ -I/usr/local/include
OBJS = test_audiomanager_api.o \
		test_thread_manager.o \
		test_audiofile.cpp

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCS) -o test_all test_all.cpp $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCS)

clean:
	rm *.o
	rm test_all
	rm -r *.dSYM