
SUBDIRS := primitives mesh fmm

all:
	@for i in $(SUBDIRS); do \
	(cd $$i; $(MAKE) all); done

clean:
	@for i in $(SUBDIRS); do \
	(cd $$i; $(MAKE) clean); done
