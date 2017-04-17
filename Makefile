CXX = g++-5
CXXFLAGS = -std=c++14 -Wall -MMD  
OBJECTS = main.o board.o character.o item.o cell.o posn.o 
EXEC = cc3k
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} 

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}

