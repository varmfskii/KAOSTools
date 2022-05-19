CXXFLAGS+=-Iinclude
SRC=pugixml.cpp
SRCS=$(addprefix src/,$(SRC))
OBJS=$(SRCS:cpp=o)
TGTS=libpugixml.a

all: $(TGTS)

libpugixml.a: $(OBJS)
	ar rcs $@ $^

.PHONY: all clean distclean install

clean:
	rm -f $(OBJS) *~ *#

distclean: clean
	rm -f $(TGTS)

install:
	cp $(TGTS) $(LIBDIR)
