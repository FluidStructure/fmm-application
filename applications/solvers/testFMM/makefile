# Define the symbols we might want to change:
all:
	make $(APP_BIN_DIR)/testFMM

$(APP_BIN_DIR)/testFMM: testFMM.cxx $(INC_DIR)/* $(LIB_DIR)/*
	$(CXX) testFMM.cxx -I$(INC_DIR) -L$(LIB_DIR) -lprimitives -lmesh -lfmm -o $(APP_BIN_DIR)/testFMM

clean:
	rm -v $(APP_BIN_DIR)/testFMM
