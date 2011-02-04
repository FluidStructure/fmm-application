# Define the symbols we might want to change:

all:
	make $(APP_BIN_DIR)/testVector

$(APP_BIN_DIR)/testVector: testVector.cxx $(LIB_DIR)/* $(LIB_DIR)/* 
	$(CXX) testVector.cxx -I$(INC_DIR) -L$(LIB_DIR) -lvoam -o $(APP_BIN_DIR)/testVector

clean:
	rm -v $(APP_BIN_DIR)/testVector
