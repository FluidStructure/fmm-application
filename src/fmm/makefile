# Define the symbols we might want to change:
OBJECTS	:= *.cxx
HEADS := *.h

all:
	$(MAKE) $(LIB_DIR)/libfmm.a

$(LIB_DIR)/libfmm.a: $(OBJECTS) $(HEADS)
	$(CXX) -c $(OBJECTS) -I$(INC_DIR)
	ar rcs $(LIB_DIR)/libfmm.a *.o
	ar rcs $(LIB_DIR)/libvoam.a *.o
	ln -sf $(PWD)/*.h $(INC_DIR)/

clean:
	rm -v *.o
