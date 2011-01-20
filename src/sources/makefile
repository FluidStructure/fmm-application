# Define the symbols we might want to change:
OBJECTS	:= 
HEADS := pointElements.h

all:
	$(MAKE) $(LIB_DIR)/libsources.a

$(LIB_DIR)/libsources.a: $(OBJECTS) $(HEADS)
	# $(CXX) -c $(OBJECTS)
	# ar rcs $(LIB_DIR)/libsources.a *.o
	ln -sf $(PWD)/*.h $(INC_DIR)/

clean:
	rm -v *.o
