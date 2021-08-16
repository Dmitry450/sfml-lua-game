bin/sfmlgame: build/main.o build/entity.o build/world.o build/animation.o build/stdroutines.o build/lua_api.o
	g++ -Wall build/main.o build/entity.o build/world.o build/animation.o build/stdroutines.o build/lua_api.o -o bin/sfmlgame -llua -lsfml-graphics -lsfml-window -lsfml-system

build/main.o: src/main.cpp src/camera.hpp src/world.hpp src/entitymgr.hpp src/entity.hpp src/resource_holder.hpp src/lua_api.hpp
	g++ -Wall -c src/main.cpp -o build/main.o

build/entity.o: src/entity.cpp src/entity.hpp src/world.hpp src/animation.hpp
	g++ -Wall -c src/entity.cpp -o build/entity.o

build/world.o: src/world.cpp src/world.hpp src/camera.hpp
	g++ -Wall -c src/world.cpp -o build/world.o

build/animation.o: src/animation.cpp src/animation.hpp
	g++ -Wall -c src/animation.cpp -o build/animation.o 

build/stdroutines.o: src/stdroutines.cpp src/stdroutines.hpp src/entity.hpp
	g++ -Wall -c src/stdroutines.cpp -o build/stdroutines.o

build/lua_api.o: src/lua_api.cpp src/lua_api.hpp src/entitymgr.hpp src/entity.hpp src/world.hpp src/resource_holder.hpp
	g++ -Wall -c src/lua_api.cpp -o build/lua_api.o
