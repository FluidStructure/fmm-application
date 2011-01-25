# Define the symbols we might want to change:
OBJECTS	:= *.cxx
HEADS := *.h

all:
	$(MAKE) $(LIB_DIR)/libfmm.a

$(LIB_DIR)/libfmm.a: $(OBJECTS) $(HEADS)
	$(CXX) -c $(OBJECTS) -I$(INC_DIR) -L$(LIB_DIR) -lprimitives
	ar rcs $(LIB_DIR)/libfmm.a *.o
	ln -sf $(PWD)/*.h $(INC_DIR)/

clean:
	rm -v *.o
