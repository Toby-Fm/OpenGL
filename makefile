windows:
	g++ -Iinclude -Iinclude/GLFW -Iinclude/glad -Iinclude/headers -Llib -o main src/*.cpp src/*.c -lglfw3dll

linux:
	g++ -Iinclude -Iinclude/GLFW -Iinclude/glad -Iinclude/headers -Llib -o main src/*.cpp src/*.c -lglfw3

clean:
	rm -rf main *.o *.exe