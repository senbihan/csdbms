IDIR =./include
CC=gcc
CXX=g++
ODIR=./obj/
SRC=./src/
BIN=./bin/
TST=./test/

test : test_main.cpp integer.cpp
	$(CXX) -g -o $(BIN)$@ $(TST)test_main.cpp $(SRC)integer.cpp -I $(IDIR)

test_main.cpp: 

integer.cpp: 