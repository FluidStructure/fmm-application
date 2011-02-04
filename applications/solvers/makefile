
SUBDIRS := testVector testFMM

all:
	@for i in $(SUBDIRS); do \
	(cd $$i; $(MAKE)); done

clean:
	@for i in $(SUBDIRS); do \
	(cd $$i; $(MAKE) clean); done
