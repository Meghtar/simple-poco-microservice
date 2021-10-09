CC = g++
STANDARD = c++2a
POCO_LINKS = -lPocoFoundation -lPocoUtil -lPocoNet -lPocoJSON -lglog -lPocoMongoDB
TEST_LINKS = -lgtest -lpthread
SRC_DIR = src
TEST_DIR = test
BIN_DIR = bin
TARGET = server
TEST_TARGET = unit_tests
include $(SRC_DIR)/BaseServer/Makefile.sources
include $(TEST_DIR)/Makefile.sources

.PHONY: server
server:
	@echo Making server
	@echo $(SOURCES)
	g++ -std=$(STANDARD) $(SRC_DIR)/main.cpp $(SOURCES) $(POCO_LINKS) -o $(BIN_DIR)/$(TARGET) -Wall

.PHONY: test
test:
	@echo Making test
	@echo $(TEST_SOURCES)
	g++ -std=$(STANDARD) $(TEST_DIR)/main.cpp -I $(SRC_DIR) $(SOURCES) $(TEST_SOURCES) $(POCO_LINKS) $(TEST_LINKS) -o $(BIN_DIR)/$(TEST_TARGET) -Wall
	./$(BIN_DIR)/$(TEST_TARGET)
