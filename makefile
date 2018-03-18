IDIR =./include
CC=gcc
CXX=g++-7 -std=c++17 -g -Wall
ODIR=./obj/
SRC=./src/
BIN=./bin/
TST=./test/

# test
test : test_main.cpp user_inteface.o integer.o reader.o writer.o operations.o 
	$(CXX) -o $(BIN)$@ $(TST)test_main.cpp $(ODIR)user_interface.o $(ODIR)reader.o $(ODIR)writer.o $(ODIR)operations.o -I $(IDIR)

integer.o:
	$(CXX) -c $(SRC)integer.cpp -I $(IDIR)
	mv integer.o $(ODIR) 

user_inteface.o:
	$(CXX) -c $(SRC)user_interface.cpp -I $(IDIR)
	mv user_interface.o $(ODIR)

operations.o:
	$(CXX) -c $(SRC)operations.cpp -I $(IDIR)
	mv operations.o $(ODIR)

reader.o:
	$(CXX) -c $(SRC)reader.cpp -I $(IDIR)
	mv reader.o $(ODIR)

writer.o:
	$(CXX) -c $(SRC)writer.cpp -I $(IDIR)
	mv writer.o $(ODIR)


test_main.cpp: 

integer.cpp: 

reader.cpp:

writer.cpp: