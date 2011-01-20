
SUBDIRS := solvers

all:
	@for i in $(SUBDIRS); do \
	(cd $$i; $(MAKE) all); done
