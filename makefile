IDIR =./include
CC=gcc
CXX=g++-7 -std=c++17
ODIR=./obj/
SRC=./src/
BIN=./bin/
TST=./test/

# test
test : test_main.cpp aux_func.o reader.o writer.o operations.o
	$(CXX) -g -Wall -o $(BIN)$@ $(TST)test_main.cpp $(ODIR)aux_func.o $(ODIR)reader.o $(ODIR)writer.o $(ODIR)operations.o -I $(IDIR)

integer.o:
	$(CXX) -c $(SRC)integer.cpp -I $(IDIR)
	mv integer.o $(ODIR) 

aux_func.o:
	$(CXX) -c $(SRC)aux_func.cpp -I $(IDIR)
	mv aux_func.o $(ODIR) 

reader.o:
	$(CXX) -c $(SRC)reader.cpp -I $(IDIR)
	mv reader.o $(ODIR)

writer.o:
	$(CXX) -c $(SRC)writer.cpp -I $(IDIR)
	mv writer.o $(ODIR)

operations.o:
	$(CXX) -c $(SRC)operations.cpp -I $(IDIR)
	mv operations.o $(ODIR)


test_main.cpp: 

integer.cpp: 

reader.cpp:

writer.cpp: