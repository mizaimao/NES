all:
	clang++ -std=gnu++14 -fcolor-diagnostics -fansi-escape-codes -g /Users/frank/Projects/NES/main.cpp -I src/include -L src/lib -o /Users/frank/Projects/NES/main -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio