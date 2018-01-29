IDIR =./include
CC=gcc
CXX=g++
ODIR=./obj/
SRC=./src/
BIN=./bin/
TST=./test/

# test
test : test_main.cpp integer.cpp integer.o aux_func.o 
	$(CXX) -g -o $(BIN)$@ $(TST)test_main.cpp $(ODIR)integer.o $(ODIR)aux_func.o -I $(IDIR)

integer.o:
	$(CXX) -c $(SRC)integer.cpp -I $(IDIR)
	mv integer.o $(ODIR) 

aux_func.o:
	$(CXX) -c $(SRC)aux_func.cpp -I $(IDIR)
	mv aux_func.o $(ODIR) 


test_main.cpp: 

integer.cpp: 