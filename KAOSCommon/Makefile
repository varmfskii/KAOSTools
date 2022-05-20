CXXFLAGS+=-I../include
SRC=Color.cpp ColorImage.cpp EventConsole.cpp Image.cpp			\
	ImageUtils.cpp Logging.cpp NativeProperty.cpp PackedImage.cpp	\
	PackedImageRow.cpp Palette.cpp Property.cpp Utilities.cpp	\
	xml.cpp shlwapi.cpp
SRCS=$(addprefix src/,$(SRC))
OBJS=$(SRCS:cpp=o)
TGTS=libkaos.a

all: $(TGTS)

libkaos.a: $(OBJS)
	ar rcs $@ $^

.PHONY: all clean distclean install

clean:
	rm -f $(OBJS) *~ *#

distclean: clean
	rm -f $(TGTS)

install: all
	cp $(TGTS) $(LIBDIR)

