CXX = g++
OBJ = *.o
PRG = BPlusTree
DIR = .
DEBUG =
FO = -o
FC = -c

CXX_SOURCE += $(DIR)/b_plus_tree.cpp
CXX_SOURCE += $(DIR)/main.cpp

$(PRG):$(OBJ)
	$(CXX) $(DEBUG) $(FO) $(PRG) $(OBJ)

$(OBJ):$(CXX_SOURCE)
	$(CXX) $(DEBUG) $(FC) $(CXX_SOURCE) 

clean:
	rm -f $(OBJ) $(PRG)
