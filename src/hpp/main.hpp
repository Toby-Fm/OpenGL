#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model; // Modelmatrix für Transformation
    uniform mat4 view;  // Viewmatrix für Kameraposition
    uniform mat4 projection; // Projektionsmatrix für Perspektive
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)";

// Würfelkoordinaten (jeweils 3 für x, y, z)
float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f
};

// Reihenfolge der Indices für die Würfelkanten
unsigned int indices[] = {
    0, 1, 1, 2, 2, 3, 3, 0, // Vorderseite
    4, 5, 5, 6, 6, 7, 7, 4, // Rückseite
    0, 4, 1, 5, 2, 6, 3, 7  // Verbindungen Vorderseite <-> Rückseite
};