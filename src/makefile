
SUBDIRS := primitives mesh

all:
	@for i in $(SUBDIRS); do \
	(cd $$i; $(MAKE) all); done
