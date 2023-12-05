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
        "Rotierende Würfel", 
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

    // Erzeuge und binde das Vertex-Array-Objekt (VAO), Vertex Buffer Object (VBO) und Index Buffer Object (EBO)
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Haupt-Renderloop: Warte auf Ereignisse, lösche den Bildschirm, zeichne den Würfel, tausche Puffer aus und wiederhole
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();  // Verarbeite alle ausstehenden Ereignisse

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //1. Rot, 2. Grün, 3. Blau, 4. Aplhawert (Transparent)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        // Ändere die Position der Kamera (View-Matrix)
        glm::mat4 view = glm::lookAt(glm::vec3(20.0f, 20.0f, 20.0f),   // Kameraposition
                                    glm::vec3(0.0f, 0.0f, 0.0f),   // Anvisierter Punkt
                                    glm::vec3(0.0f, -5.0f, 0.0f));  // Up-Vektor

        // Erzeuge die Projektionsmatrix (Perspektive)
        glm::mat4 projection = glm::perspective(glm::radians(50.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

        // Zeichne mehrere Würfel
        for (int i = 0; i < 10; ++i) {
            // Erzeuge die Modelmatrix (Rotationsmatrix) und animiere sie
            float angle = glm::radians(40.0f * i + static_cast<float>(glfwGetTime()) * 50.0f);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(cos(angle) * 10.0f, sin(angle) * 3.0f, 0.0f));
            model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 1.0f));

            // Setze die Matrizen im Shader
            unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

            unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
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
