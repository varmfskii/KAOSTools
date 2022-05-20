LIBDIR=${PWD}/lib
DIRS=3rdParty KAOSCommon TiledLib

export LIBDIR

all: $(DIRS)

.PHONY: all $(DIRS) install clean distclean

3rdParty:
	make -C $@

KAOSCommon:
	make -C $@

install: all
	for f in $(DIRS);do make -C $$f $@;done

clean:
	for f in $(DIRS);do make -C $$f $@;done

distclean:
	for f in $(DIRS);do make -C $$f $@;done
	rm $(LIBDIR)/*.a
