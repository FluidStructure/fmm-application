
SUBDIRS := primitives mesh fmm

all:
	@for i in $(SUBDIRS); do \
	(cd $$i; $(MAKE) all); done
