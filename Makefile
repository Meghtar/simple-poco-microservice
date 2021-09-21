CC = g++
STANDARD = c++2a
POCO_LINKS = -lPocoFoundation -lPocoUtil -lPocoNet -lPocoJSON
SRC_DIR = src
BIN_DIR = bin
TARGET = server
include $(SRC_DIR)/BaseServer/Makefile.sources
#FILE := $(SRC_DIR)/BaseServer/Makefile.sources
# SOURCES = $(file < $(FILE))
#`cat $(SRC_DIR)/BaseServer/Makefile.sources`
#BaseServer/BaseServer.cpp

.PHONY: server
server:
	@echo Making server
	@echo $(SOURCES)
	g++ -std=$(STANDARD) $(SRC_DIR)/main.cpp $(SOURCES) $(POCO_LINKS) -o $(BIN_DIR)/$(TARGET)