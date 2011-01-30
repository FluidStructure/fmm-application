# Define the symbols we might want to change:
export CXX := g++ -g

export APP_DIR := $(PWD)/applications
export APP_BIN_DIR := $(APP_DIR)/bin

export SRC_DIR := $(PWD)/src

export INC_DIR := $(PWD)/lninclude
export LIB_DIR := $(PWD)/lib


all:
	cd $(SRC_DIR) && ($(MAKE) all)
	cd $(APP_DIR) && ($(MAKE) all)
