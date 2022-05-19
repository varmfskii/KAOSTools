CXXFLAGS+=-Iinclude
SRC=AnimationFrame.cpp Layer.cpp Map.cpp NamedProperty.cpp Object.cpp	\
	ObjectGroup.cpp ObjectGroupLayer.cpp PropertyBag.cpp Size.cpp	\
	Stagger.cpp Tile.cpp TilesetCache.cpp Tileset.cpp		\
	TilesetDescriptor.cpp TilesetImage.cpp TilesetLayer.cpp
SRCS=$(addprefix src/,$(SRC))
OBJS=$(SRCS:cpp=o)
TGTS=libtiled.a

all: $(TGTS)

libtiled.a: $(OBJS)
	ar rcs $@ $^

.PHONY: all clean distclean install

clean:
	rm -f $(OBJS) *~ *#

distclean: clean
	rm -f $(TGTS)

install: all
	cp $(TGTS) $(LIBDIR)


