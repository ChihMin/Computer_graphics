all:
	g++ -Wall -o test test.c -framework OpenGL -framework GLUT

clean:
	rm test
