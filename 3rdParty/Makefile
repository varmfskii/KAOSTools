export LIBDIR

DIRS=FlicLib LoadPNGLib PugiXML zlib

all: $(DIRS)

.PHONY: all $(DIRS) zlib install clean distclean

FlicLib:
	make -C $@

LoadPNGLib:
	make -C $@

PugiXML:
	make -C $@

install: all
	for f in $(DIRS);do make -C $$f $@;done

clean:
	for f in $(DIRS);do make -C $$f $@;done

distclean:
	for f in $(DIRS);do make -C $$f $@;done


