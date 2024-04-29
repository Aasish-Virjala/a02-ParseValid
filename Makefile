CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = parse

$(TARGET): parse.o ast.o
	$(CXX) $(CXXFLAGS) -o $@ $^

parse.o: parse.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ parse.cpp

ast.o: ast.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ ast.cpp

clean:
	rm -f *.o $(TARGET)
