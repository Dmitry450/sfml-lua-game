bin/sfmlgame: build/main.o build/entity.o build/resource_holder.o build/world.o build/animation.o build/stdroutines.o
	g++ build/main.o build/entity.o build/resource_holder.o build/world.o build/animation.o build/stdroutines.o -o bin/sfmlgame -lsfml-graphics -lsfml-window -lsfml-system

build/main.o: src/main.cpp src/camera.hpp src/world.hpp src/entitymgr.hpp src/entity.hpp
	g++ -c src/main.cpp -o build/main.o

build/entity.o: src/entity.cpp src/entity.hpp src/world.hpp src/animation.hpp
	g++ -c src/entity.cpp -o build/entity.o

build/resource_holder.o: src/resource_holder.cpp src/resource_holder.hpp
	g++ -c src/resource_holder.cpp -o build/resource_holder.o

build/world.o: src/world.cpp src/world.hpp src/camera.hpp
	g++ -c src/world.cpp -o build/world.o

build/animation.o: src/animation.cpp src/animation.hpp
	g++ -c src/animation.cpp -o build/animation.o 

build/stdroutines.o: src/stdroutines.cpp src/stdroutines.hpp src/entity.hpp
	g++ -c src/stdroutines.cpp -o build/stdroutines.o
