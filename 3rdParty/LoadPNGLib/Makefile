CXXFLAGS+=-Iinclude
SRC=lodepng.cpp lodepng_unittest.cpp lodepng_util.cpp pngdetail.cpp
SRCS=$(addprefix src/,$(SRC))
OBJS=$(SRCS:cpp=o)
TGTS=libloadpng.a

all: $(TGTS)

libloadpng.a: $(OBJS)
	ar rcs $@ $^

.PHONY: all clean distclean install

clean:
	rm -f $(OBJS) *~ *#

distclean: clean
	rm -f $(TGTS)

install:
	cp $(TGTS) $(LIBDIR)

