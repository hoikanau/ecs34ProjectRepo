# Compiler Settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -I/usr/include/gtest
LDFLAGS = -lgtest -lgtest_main -pthread

# Directories to create on when make 
SRCDIR = src
TESTSRCDIR = testsrc
OBJDIR = obj
BINDIR = bin

# Files for linking
TARGET = teststrutils
SRCS = $(SRCDIR)/StringUtils.cpp $(TESTSRCDIR)/StringUtilsTest.cpp
OBJS = $(OBJDIR)/StringUtils.o $(OBJDIR)/StringUtilsTest.o
EXEC = $(BINDIR)/$(TARGET)

all: directories $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(TESTSRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(EXEC)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Create directories
directories:
	mkdir -p $(OBJDIR) $(BINDIR)
