PROJECT = parSum
SOURCE = main.cpp
CSTANDARD = c++11
#CSTANDARD = gnu++11

CFLAGS = -msse2 -O3
LIBS = -lpthread

build: $(SOURCES)
	$(CXX) -std=$(CSTANDARD) $(LIBS) $(CFLAGS) $(SOURCE) -o $(PROJECT)

assembler: $(SOURCES)
	$(CXX) -std=$(CSTANDARD) $(LIBS) $(CFLAGS) $(SOURCE) -S

clean:
	rm -f $(PROJECT)
