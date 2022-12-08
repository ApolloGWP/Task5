all: compile_child compile_parent run

compile_child: child.c
	gcc child.c -o child.out

compile_parent: parent.c
	gcc parent.c -o parent.out

run: parent.c
	./parent.out
