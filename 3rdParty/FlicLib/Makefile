CXXFLAGS+=-Iinclude
SRC=decoder.cpp encoder.cpp stdio.cpp StreamIO.cpp
SRCS=$(addprefix src/,$(SRC))
OBJS=$(SRCS:cpp=o)
TGTS=libflic.a

all: $(TGTS)

libflic.a: $(OBJS)
	ar rcs $@ $^

.PHONY: all clean distclean install

clean:
	rm -f $(OBJS) *~ *#

distclean: clean
	rm -f $(TGTS)

install:
	cp $(TGTS) $(LIBDIR)

