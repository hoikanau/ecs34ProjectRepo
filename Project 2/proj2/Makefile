# I am compling usig g++
#  Making sure c++17
#  Inlcuding the files that my makefile needs to access
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -I/usr/include/gtest 
LDFLAGS = -lgtest -lgtest_main -pthread -lexpat
# Directories
SRCDIRECT = src
TESTDIRECT = testsrc
OBJDIRECT = obj
BINDIRECT = bin


SRCS = $(SRCDIRECT)/StringUtils.cpp $(SRCDIRECT)/StringDataSink.cpp $(SRCDIRECT)/StringDataSource.cpp $(TESTDIRECT)/StringUtilsTest.cpp $(TESTDIRECT)/StringDataSinkTest.cpp $(TESTDIRECT)/StringDataSourceTest.cpp
OBJS = $(OBJDIRECT)/StringUtils.o $(OBJDIRECT)/StringUtilsTest.o $(OBJDIRECT)/StringDataSink.o $(OBJDIRECT)/StringDataSource.o $(OBJDIRECT)/StringDataSinkTest.o $(OBJDIRECT)/StringDataSourceTest.o
EXEC = $(BINDIRECT)/teststrutils $(BINDIRECT)/teststrdatasource $(BINDIRECT)/teststrdatasink $(BINDIRECT)/testdsv $(BINDIRECT)/testxml

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
