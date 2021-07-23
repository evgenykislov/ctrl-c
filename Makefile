SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,out/%.o,$(SRC))
OUT = ctrl-c.so

FLAGS = -fPIC -I include -shared 

init : 
	mkdir -p out/

.PHONY : build
build : init $(OUT)

$(OUT) : $(OBJ)
	$(CXX) $^ -o $@ -shared $(FLAGS)

out/%.o : src/%.cpp
	$(CXX) -c $< -o $@ $(FLAGS)
