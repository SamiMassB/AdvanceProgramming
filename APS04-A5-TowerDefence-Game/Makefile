CXX = g++ -g
OUT = TD
all: $(OUT)

$(OUT): bin/main.o bin/system.o bin/Tower.o bin/GameClock.o bin/Ballon.o bin/Menu.o
	$(CXX) $^ -o $@ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
bin/main.o: src/main.cpp src/config.hpp src/system.hpp
	$(CXX) -c $< -o $@
bin/system.o: src/system.cpp src/system.hpp src/config.hpp
	$(CXX) -c $< -o $@
bin/Tower.o: src/Tower.cpp src/Tower.hpp src/config.hpp src/config.hpp
	$(CXX) -c $< -o $@
bin/GameClock.o: src/GameClock.cpp src/GameClock.hpp src/config.hpp
	$(CXX) -c $< -o $@
bin/Ballon.o: src/Ballon.cpp src/Ballon.hpp src/config.hpp
	$(CXX) -c $< -o $@
bin/Menu.o: src/Menu.cpp src/Menu.hpp src/config.hpp
	$(CXX) -c $< -o $@
clean:
	rm -f bin/*.o $(OUT)
