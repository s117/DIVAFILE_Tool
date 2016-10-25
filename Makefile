##########################################################
## Copyright (C) 2016 S117 <admin@0x10c.pw>             ##
## Bitman Lab.                                          ##
## Licensed under the terms of the GNU GPL, version 2   ##
## http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt ##
##########################################################

PROJECT_DIR=$(shell pwd)
PROJECT_NAME=DIVAFILE_Tool

SRC_DIR=$(PROJECT_DIR)/$(PROJECT_NAME)/source
INCLUDE_DIR=$(PROJECT_DIR)/$(PROJECT_NAME)/include
LIB_DIR=

BUILD_DIR=$(PROJECT_DIR)/build
OBJ_DIR=$(BUILD_DIR)/obj
BIN_DIR=$(BUILD_DIR)/bin
PROGRAM=$(BIN_DIR)/$(PROJECT_NAME)

OUT_DIR+=$(OBJ_DIR)
OUT_DIR+=$(BUILD_DIR)
OUT_DIR+=$(BIN_DIR)

EXTENSION=c
OBJS=$(patsubst $(SRC_DIR)/%.$(EXTENSION), $(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.$(EXTENSION)))

INCLUDE=\
		-I$(INCLUDE_DIR)

CC=gcc
CFLAGS=-Wall -W -g -O2
LDFLAGS=


.PHONY: all clean rebuild

all: directories $(OBJS) 
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS) 

directories: $(OUT_DIR)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(EXTENSION) 
	$(CC) $< -o $@ -c $(CFLAGS) $(INCLUDE) 

clean:
	rm -frv $(BUILD_DIR)

rebuild: clean all
