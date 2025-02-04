# I am compling usig g++
#  Making sure c++17
#  Inlcuding the files that my makefile needs to access
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -I/usr/include/gtest
LDFLAGS = -lgtest -lgtest_main -pthread
# Directories
SRCDIRECT = src
TESTDIRECT = testsrc
OBJDIRECT = obj
BINDIRECT = bin
# CReating file teststrutils and linking it with the cpp and cpptest file
TARGET = teststrutils
SRCS = $(SRCDIRETC)/StringUtils.cpp $(TESTDIRECT)/StringUtilsTest.cpp
OBJS = $(OBJDIRECT)/StringUtils.o $(OBJDIRECT)/StringUtilsTest.o
EXEC = $(BINDIRECT)/$(TARGET)


all: directories $(EXEC)
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
obj/%.o: testsrc/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


run: all
	./$(EXEC)

clean:
	rm -rf $(OBJDIRECT) $(BINDIRECT)

# Create directories
directories:
	mkdir -p $(OBJDIRECT) $(BINDIRECT)
