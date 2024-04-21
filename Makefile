CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = parse

$(TARGET): parse.o
	$(CXX) $(CXXFLAGS) -o $@ $^

parse.o: parse.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ parse.cpp


clean:
	rm -f *.o $(TARGET)
