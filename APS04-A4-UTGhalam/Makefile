CMP = g++ -std=c++20
EXE = UTGhalam


all: $(EXE)

$(EXE): src/main.o src/Test.o src/Question.o src/Template.o src/System.o    
	$(CMP) $^ -o $@

src/main.o: src/main.cpp
	$(CMP) -c $< -o $@

src/Question.o: src/Question.cpp src/Question.h
	$(CMP) -c $< -o $@

src/Test.o: src/Test.cpp src/Test.h
	$(CMP) -c $< -o $@

src/System.o: src/System.cpp src/System.h
	$(CMP) -c $< -o $@

src/Template.o: src/Template.cpp src/Template.h
	$(CMP) -c $< -o $@
