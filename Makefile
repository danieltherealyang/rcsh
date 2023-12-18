BUILD_DIR=build
SOURCE_DIR=source
INCLUDE=$(SOURCE_DIR)/include
CPP_FILES=$(wildcard $(SOURCE_DIR)/*.cpp)
lexer:
	re2c $(SOURCE_DIR)/lexer.re -o $(SOURCE_DIR)/lexer.cpp --header $(INCLUDE)/lexer.h
default: lexer
	g++ -I$(INCLUDE) $(CPP_FILES) -o $(BUILD_DIR)/test
run: default
	./$(BUILD_DIR)/test