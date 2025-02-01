# I am compling usig g++
#  Making sure c++17
#  Inlcuding the files that my makefile needs to access
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -I/usr/include/gtest
LDFLAGS = -lgtest -lgtest_main -pthread

# Directories
SRCDIR = src
TESTDIR = testsrc
OBJDIR = obj
BINDIR = bin

# Files
TARGET = teststrutils
SRCS = $(SRCDIR)/StringUtils.cpp $(TESTDIR)/StringUtilsTest.cpp
OBJS = $(OBJDIR)/StringUtils.o $(OBJDIR)/StringUtilsTest.o
EXEC = $(BINDIR)/$(TARGET)

# Rules
all: directories $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Generic pattern for compiling .cpp files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(TESTDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(EXEC)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Create directories
directories:
	mkdir -p $(OBJDIR) $(BINDIR)
