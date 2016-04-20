CFLAGS  = -mcpu=power8 -m64 -mvsx -O2
CXXFLAGS = -std=gnu++11 -mcpu=power8 -m64 -lpthread -mvsx -O2

TARGETS = test1 parsum test2

all: $(TARGETS)

test2.cpp: altivecmm.h
altivecmm.h: altivecmm_typetable.h

clean:
	$(RM) *.o
	$(RM) $(TARGETS)

rebuild: clean all
