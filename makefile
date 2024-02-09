all:
	g++ -Iinclude -Iinclude/GLFW -Iinclude/glad -Iinclude/headers -Llib -o main src/*.cpp src/*.c -lglfw3dll 


clean:
	rm -rf main *.o *.exe