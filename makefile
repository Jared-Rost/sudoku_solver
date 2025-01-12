# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -O2

v1: v1.cpp
	$(CXX) $(CXXFLAGS) v1.cpp -o v1

v2: v2.cpp
	$(CXX) $(CXXFLAGS) v2.cpp -o v2

v3: v3.cpp
	$(CXX) $(CXXFLAGS) v3.cpp -o v3

v4: v4.cpp
	$(CXX) $(CXXFLAGS) v4.cpp -o v4

v5: v5.cpp
	$(CXX) $(CXXFLAGS) v5.cpp -o v5

v6: v6.cpp
	$(CXX) $(CXXFLAGS) v6.cpp -o v6

v7: v7.cpp
	$(CXX) $(CXXFLAGS) v7.cpp -o v7

v8: v8.cpp
	$(CXX) $(CXXFLAGS) v8.cpp -o v8

# Clean rule to remove generated files
clean:
	rm -f v1 v2 v3 v4 v5 v6 v7 v8 *.o