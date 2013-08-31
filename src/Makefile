CXX=g++

FLAGS=-Wall -Wextra -std=c++11 -I.
LD_FLAGS=-lboost_program_options -lboost_system -lboost_filesystem

EXEC=HeaderDependencyAnalyzer

BIN_DIR=../bin
OBJ_DIR=../obj

all : $(BIN_DIR)/$(EXEC)

clean:
	rm $(OBJ_DIR)/*.o
	rm $(BIN_DIR)/$(EXEC)

$(OBJ_DIR)/main.o: main.cpp stringhelper.hpp graphconstructor.hpp dependencygraph.hpp
	$(CXX) -c $(FLAGS) main.cpp -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/stringhelper.o: stringhelper.cpp stringhelper.hpp
	$(CXX) -c $(FLAGS) stringhelper.cpp -o $(OBJ_DIR)/stringhelper.o

$(OBJ_DIR)/graphconstructor.o: graphconstructor.cpp graphconstructor.hpp stringhelper.hpp dependencygraph.hpp
	$(CXX) -c $(FLAGS) graphconstructor.cpp -o $(OBJ_DIR)/graphconstructor.o

$(OBJ_DIR)/dependencygraph.o: dependencygraph.cpp dependencygraph.hpp
	$(CXX) -c $(FLAGS) dependencygraph.cpp -o $(OBJ_DIR)/dependencygraph.o

$(OBJ_DIR)/ianalyzer.o: analyzer/ianalyzer.cpp analyzer/ianalyzer.hpp
	$(CXX) -c $(FLAGS) analyzer/ianalyzer.cpp -o $(OBJ_DIR)/ianalyzer.o

$(OBJ_DIR)/analyzerfactory.o: analyzer/analyzerfactory.cpp analyzer/analyzerfactory.hpp
	$(CXX) -c $(FLAGS) analyzer/analyzerfactory.cpp -o $(OBJ_DIR)/analyzerfactory.o

$(OBJ_DIR)/redondantheaderanalyzer.o: analyzer/redondantheaderanalyzer.cpp analyzer/redondantheaderanalyzer.hpp analyzer/ianalyzer.hpp analyzer/crtp_analyzer.hpp
	$(CXX) -c $(FLAGS) analyzer/redondantheaderanalyzer.cpp -o $(OBJ_DIR)/redondantheaderanalyzer.o

$(BIN_DIR)/$(EXEC) : $(OBJ_DIR)/main.o $(OBJ_DIR)/stringhelper.o $(OBJ_DIR)/graphconstructor.o $(OBJ_DIR)/dependencygraph.o $(OBJ_DIR)/ianalyzer.o $(OBJ_DIR)/analyzerfactory.o $(OBJ_DIR)/redondantheaderanalyzer.o
	$(CXX) $(LD_FLAGS) -o $(BIN_DIR)/$(EXEC) $(OBJ_DIR)/main.o $(OBJ_DIR)/stringhelper.o $(OBJ_DIR)/graphconstructor.o $(OBJ_DIR)/dependencygraph.o $(OBJ_DIR)/ianalyzer.o $(OBJ_DIR)/analyzerfactory.o $(OBJ_DIR)/redondantheaderanalyzer.o
