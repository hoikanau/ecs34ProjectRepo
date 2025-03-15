# I am compling usig g++
#  Making sure c++17
#  Inlcuding the files that my makefile needs to access
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./include -I/usr/include/gtest -I./src
LDFLAGS = -lgtest -lgtest_main -pthread -lexpat
# Directories
SRCDIRECT = src
TESTDIRECT = testsrc
OBJDIRECT = obj
BINDIRECT = bin


SRCS = $(SRCDIRECT)/StringUtils.cpp \
	$(SRCDIRECT)/StringDataSink.cpp \
	$(SRCDIRECT)/StringDataSource.cpp \
	$(TESTDIRECT)/StringUtilsTest.cpp \
	$(TESTDIRECT)/StringDataSinkTest.cpp \
	$(TESTDIRECT)/StringDataSourceTest.cpp \
	$(SRCDIRECT)/CDSVReader.cpp \
	$(SRCDIRECT)/CDSVWriter.cpp \
	$(SRCDIRECT)/XMLReader.cpp \
	$(SRCDIRECT)/XMLWriter.cpp \
	$(TESTDIRECT)/XMLTest.cpp \
	$(TESTDIRECT)/DSVTest.cpp \
	$(SRCDIRECT)/OpenStreetMap.cpp \
	$(TESTDIRECT)/OpenStreetMapTest.cpp \
	$(SRCDIRECT)/CSVBusSystem.cpp \
	$(TESTDIRECT)/CSVBusSystemTest.cpp \
	$(SRCDIRECT)/DijkstraPathRouter.cpp \
	$(SRCDIRECT)/DijkstraPathRouterTest.cpp \
	$(SRCDIRECT)/BusSystemIndexer.cpp \
	$(TESTDIRECT)/CSVBusSystemIndexerTest.cpp \
	$(SRCDIRECT)/DijkstraTransportationPlanner.cpp
OBJS = $(OBJDIRECT)/StringUtils.o \
	$(OBJDIRECT)/StringUtilsTest.o \
	$(OBJDIRECT)/StringDataSink.o \
	$(OBJDIRECT)/StringDataSource.o \
	$(OBJDIRECT)/StringDataSinkTest.o \
	$(OBJDIRECT)/StringDataSourceTest.o \
	$(OBJDIRECT)/CDSVReader.o \
	$(OBJDIRECT)/CDSVWriter.o \
	$(OBJDIRECT)/XMLReader.o \
	$(OBJDIRECT)/XMLWriter.o \
	$(OBJDIRECT)/XMLTest.o \
	$(OBJDIRECT)/DSVTest.o \
	$(OBJDIRECT)/OpenStreetMap.o \
	$(OBJDIRECT)/OpenStreetMapTest.o \
	$(OBJDIRECT)/CSVBusSystem.o \
	$(OBJDIRECT)/CSVBusSystemTest.o \
	$(OBJDIRECT)/DijkstraPathRouter.o \
	$(OBJDIRECT)/DijkstraPathRouterTest.o \
	$(OBJDIRECT)/BusSystemIndexer.o \
	$(OBJDIRECT)/CSVBusSystemIndexerTest.o \
	$(OBJDIRECT)/DijkstraTransportationPlanner.o

EXEC = $(BINDIRECT)/teststrutils \
	$(BINDIRECT)/teststrdatasource \
	$(BINDIRECT)/teststrdatasink \
	$(BINDIRECT)/testdsv \
	$(BINDIRECT)/testxml \
	$(BINDIRECT)/testosm \
	$(BINDIRECT)/testcsvbs \
	$(BINDIRECT)/testdpr \
	$(BINDIRECT)/testcsvbsi \
	$(BINDIRECT)/testtpcl \
	$(BINDIRECT)/testtp 

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
