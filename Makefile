all:
	clang++ -I src/include -L src/lib -o hello_world hello_world.cpp -l SDL2main -l SDL2