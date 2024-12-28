tracker:
	g++ tracker.cpp -o tracker.app -lncurses

todo:
	g++ todo.cpp -o todo.app -lncurses

all: tracker todo
