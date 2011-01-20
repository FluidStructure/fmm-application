# Define the symbols we might want to change:
CC	:= g++

OBJECTS	:= *.cxx
HEADS := *.h

INCDIR := ../../lninclude
LIBDIR := ../../lib

libprimitives.a: $(OBJECTS) $(HEADS)
	$(CXX) -c $(OBJECTS)
	ar rcs $(LIBDIR)/libfmm.a *.o
	ln -s $(PWD)/fmm.h $(PWD)/$(INCDIR)/

clean:
	rm -v *.o *.a
