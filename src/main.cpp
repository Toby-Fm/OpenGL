#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
)";

// Dreieckskoordinaten (in Normalized Device Coordinates)
float vertices[] = {
    -0.5f, -0.5f, 0.0f,  // Unten links
    0.5f, -0.5f, 0.0f,  // Unten rechts
    0.0f,  0.5f, 0.0f   // Oben
};

int main() {
    // Initialisiere GLFW
    glfwInit();

    // Setze GLFW-Fenstereinstellungen für OpenGL-Version und Profil
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Erstelle ein GLFW-Fenster mit bestimmten Abmessungen und Titel
    GLFWwindow* window = glfwCreateWindow(
        1920, 
        1080, 
        "Was zum flying fuck alter", 
        NULL, 
        NULL
    );

    // Überprüfe, ob das Fenster erfolgreich erstellt wurde
    if (window == NULL) {
        std::cout << "Fehler beim Erstellen des Fensters" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Setze das aktuelle Kontext-Fenster auf das erstellte GLFW-Fenster
    glfwMakeContextCurrent(window);

    // Initialisiere GLAD, um OpenGL-Funktionen zu laden
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Kompiliere und lade Shader
    unsigned int vertexShader, fragmentShader, shaderProgram;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Setze den Rotationswinkel
    float angle = 0.0f;

    // Erzeuge und binde das Vertex-Array-Objekt (VAO) und Vertex Buffer Object (VBO)
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Haupt-Renderloop: Warte auf Ereignisse, lösche den Bildschirm, zeichne das Dreieck, tausche Puffer aus und wiederhole
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();  // Verarbeite alle ausstehenden Ereignisse

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Erzeuge die Rotationsmatrix
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

        // Setze die Modelmatrix im Shader
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);

        // Erhöhe den Rotationswinkel für die nächste Iteration
        angle += 0.5f;
    }

    // Aufräumen und GLFW beenden
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glfwTerminate();
    return 0;
}