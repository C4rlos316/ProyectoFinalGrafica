# 💻 Estructura del Código

## 📋 Visión General

El código principal está en `Main.cpp` (~2500 líneas) con headers modulares para funcionalidad específica.

---

## 📂 Organización del Código

### Main.cpp - Secciones Principales

```cpp
// ============================================
// SECCIÓN 1: Headers e Includes (1-82)
// ============================================
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// ... más includes

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"


// ============================================
// SECCIÓN 2: Prototipos de Funciones (83-120)
// ============================================
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void ConfigurarVAO(GLuint& VAO, GLuint& VBO, float* vertices, size_t size);
void ConfigurarTexturaRepetible(GLuint textureID);
void DibujarPiso(GLuint textureID, glm::vec3 posicion, glm::vec3 escala, 
                 GLuint VAO_Cubo, GLint modelLoc);


// ============================================
// SECCIÓN 3: Variables Globales (121-800)
// ============================================

// --- Configuración de Ventana ---
const GLuint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// --- Sistema de Cámara ---
Camera camera(glm::vec3(0.0f, 1.0f, 21.0f));
bool teclaTAB_presionada = false;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// --- Sistema de Iluminación ---
glm::vec3 pointLightPositions[] = {
    glm::vec3(2.0f, 0.2f, 2.0f),    // Luz 0: Centro
    glm::vec3(4.1f, 5.5f, 12.5f),   // Luz 1: Entrada
    // ... 5 luces más
};

// --- Control de Tiempo ---
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// --- Variables de Animación ---
// Por cada animal (13 especies):
// - Rotaciones de partes del cuerpo
// - Posición actual
// - Flags de control
// - Timing

// Ejemplo: Elefante
float rotElefanteTrompa = 0.0f;
float rotElefantePataFL = 0.0f;
float rotElefantePataFR = 0.0f;
// ... más variables
glm::vec3 elefantePos = glm::vec3(-7.25f, 0.0f, -7.25f);
float elefanteRotY = 0.0f;
float elefanteScale = 1.0f;
bool animarElefante = false;
bool teclaV_presionada = false;
float startTimeElefante = 0.0f;


// ============================================
// SECCIÓN 4: Función main() (801-2000)
// ============================================
int main() {
    // --- 4.1 Inicialización GLFW ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // --- 4.2 Crear Ventana ---
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Zoologico Virtual", 
                                          nullptr, nullptr);
    glfwMakeContextCurrent(window);
    
    // --- 4.3 Configurar Callbacks ---
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // --- 4.4 Inicializar GLEW ---
    glewExperimental = GL_TRUE;
    glewInit();
    
    // --- 4.5 Configurar Viewport ---
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // --- 4.6 Configurar OpenGL ---
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // --- 4.7 Compilar Shaders ---
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader skyboxShader("Shader/skybox.vs", "Shader/skybox.frag");
    
    // --- 4.8 Cargar Modelos 3D ---
    // Animales
    Model modelPinguino((GLchar*)"Models/pinguino/pinguino.obj");
    Model modelElefante((GLchar*)"Models/elefante/elefante.obj");
    // ... 80+ modelos más
    
    // --- 4.9 Cargar Texturas ---
    GLuint skyboxTexture = loadCubemap(faces);
    GLuint texturaAgua = loadTexture("images/agua.jpg");
    GLuint texturaArena = loadTexture("images/arena.jpg");
    // ... más texturas
    
    // --- 4.10 Configurar Geometría ---
    GLuint VAO_Cubo, VBO_Cubo;
    ConfigurarVAO(VAO_Cubo, VBO_Cubo, vertices_cubo, sizeof(vertices_cubo));
    
    GLuint VAO_Skybox, VBO_Skybox;
    ConfigurarVAO(VAO_Skybox, VBO_Skybox, skyboxVertices, sizeof(skyboxVertices));
    
    // --- 4.11 Inicializar Audio ---
    ma_engine engine;
    ma_sound sound;
    ma_engine_init(NULL, &engine);
    ma_sound_init_from_file(&engine, "musica.mp3", 
                            MA_SOUND_FLAG_STREAM, NULL, NULL, &sound);
    ma_sound_set_looping(&sound, MA_TRUE);
    ma_sound_start(&sound);
    
    
    // ========================================
    // 4.12 BUCLE PRINCIPAL DE RENDERIZADO
    // ========================================
    while (!glfwWindowShouldClose(window)) {
        // --- Timing ---
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // --- Procesar Input ---
        glfwPollEvents();
        DoMovement();
        
        // --- Actualizar Animaciones ---
        // Pingüino (automático)
        float tPingu = glfwGetTime() * 2.0f;
        rotPataIzqPingu = sin(tPingu) * 30.0f;
        rotPataDerPingu = -sin(tPingu) * 30.0f;
        pinguPos.x = pinguBasePos.x + cos(tPingu * 0.3f) * 0.5f;
        pinguPos.z = pinguBasePos.z + sin(tPingu * 0.3f) * 0.5f;
        pinguRotY = tPingu * 0.3f * 57.3f;
        
        // Elefante (activado por tecla)
        if (animarElefante) {
            float tElef = glfwGetTime() - startTimeElefante;
            float walkCycle = sin(tElef * 1.5f);
            rotElefantePataFL = walkCycle * 20.0f;
            rotElefantePataFR = -walkCycle * 20.0f;
            rotElefanteTrompa = sin(tElef * 2.0f) * 15.0f;
            // ... más animaciones
        }
        
        // ... animaciones de 11 animales más
        
        
        // --- Limpiar Buffers ---
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);  // Cielo azul
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        // ========================================
        // RENDERIZADO CON ILUMINACIÓN
        // ========================================
        lightingShader.Use();
        
        // --- Configurar Matrices ---
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.zoom),
            (float)SCREEN_WIDTH / SCREEN_HEIGHT,
            0.1f, 100.0f
        );
        
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        // --- Configurar Luces ---
        // Luz direccional
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"),
                    -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),
                    0.3f, 0.3f, 0.3f);
        // ... más propiedades
        
        // Luces puntuales (7)
        for (int i = 0; i < 7; i++) {
            std::string number = std::to_string(i);
            glUniform3f(glGetUniformLocation(lightingShader.Program,
                        ("pointLights[" + number + "].position").c_str()),
                        pointLightPositions[i].x,
                        pointLightPositions[i].y,
                        pointLightPositions[i].z);
            // ... más propiedades
        }
        
        // Spotlight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
                    camera.position.x, camera.position.y, camera.position.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
                    camera.front.x, camera.front.y, camera.front.z);
        // ... más propiedades
        
        // --- Configurar Material ---
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
        
        
        // ========================================
        // RENDERIZADO DE PISOS
        // ========================================
        DibujarPiso(texturaAgua, glm::vec3(7.25f, -0.5f, -7.25f), 
                    glm::vec3(5.0f, 0.1f, 5.0f), VAO_Cubo, modelLoc);  // Acuario
        DibujarPiso(texturaArena, glm::vec3(-7.25f, -0.5f, 7.25f),
                    glm::vec3(5.0f, 0.1f, 5.0f), VAO_Cubo, modelLoc);  // Desierto
        // ... más pisos
        
        
        // ========================================
        // RENDERIZADO DE ESTRUCTURAS
        // ========================================
        glm::mat4 model;
        
        // Aviario
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        modelAviario.Draw(lightingShader);
        
        // ... más estructuras
        
        
        // ========================================
        // RENDERIZADO DE ANIMALES
        // ========================================
        
        // --- Pingüino ---
        model = glm::mat4(1.0f);
        model = glm::translate(model, pinguPos);
        model = glm::rotate(model, glm::radians(pinguRotY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(pinguScale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        modelPinguinoCuerpo.Draw(lightingShader);
        
        // Patas del pingüino (jerárquicas)
        glm::mat4 modelPata = model;
        modelPata = glm::translate(modelPata, glm::vec3(-0.2f, 0.0f, 0.0f));
        modelPata = glm::rotate(modelPata, glm::radians(rotPataIzqPingu), 
                                glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPata));
        modelPinguinoPata.Draw(lightingShader);
        
        // ... 12 animales más con sus partes
        
        
        // ========================================
        // RENDERIZADO DEL PERSONAJE (3ra Persona)
        // ========================================
        if (camera.cameraType == THIRD_PERSON) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, camera.position);
            float angleY = atan2(camera.front.x, camera.front.z);
            model = glm::rotate(model, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            modelAlex.Draw(lightingShader);
        }
        
        
        // ========================================
        // RENDERIZADO DE LUCES (Cubos visuales)
        // ========================================
        lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"),
                          1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"),
                          1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(VAO_Cubo);
        for (int i = 0; i < 7; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"),
                              1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        
        
        // ========================================
        // RENDERIZADO DE SKYBOX
        // ========================================
        glDepthFunc(GL_LEQUAL);
        skyboxShader.Use();
        
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));  // Sin traslación
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"),
                          1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"),
                          1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(VAO_Skybox);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        
        
        // --- Swap Buffers ---
        glfwSwapBuffers(window);
    }
    
    
    // ========================================
    // 4.13 LIMPIEZA Y FINALIZACIÓN
    // ========================================
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
    
    glDeleteVertexArrays(1, &VAO_Cubo);
    glDeleteBuffers(1, &VBO_Cubo);
    glDeleteVertexArrays(1, &VAO_Skybox);
    glDeleteBuffers(1, &VBO_Skybox);
    
    glfwTerminate();
    return 0;
}


// ============================================
// SECCIÓN 5: Funciones Auxiliares (2001-2300)
// ============================================

void ConfigurarVAO(GLuint& VAO, GLuint& VBO, float* vertices, size_t size) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 
                         (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    // Texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                         (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}

void ConfigurarTexturaRepetible(GLuint textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void DibujarPiso(GLuint textureID, glm::vec3 posicion, glm::vec3 escala,
                 GLuint VAO_Cubo, GLint modelLoc) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, posicion);
    model = glm::scale(model, escala);
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    glBindVertexArray(VAO_Cubo);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


// ============================================
// SECCIÓN 6: Callbacks (2301-2500)
// ============================================

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
    
    // Cambio de cámara
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS && !teclaTAB_presionada) {
        teclaTAB_presionada = true;
        camera.cameraType = (camera.cameraType == FIRST_PERSON) 
                            ? THIRD_PERSON : FIRST_PERSON;
    }
    if (key == GLFW_KEY_TAB && action == GLFW_RELEASE) {
        teclaTAB_presionada = false;
    }
    
    // Activar animaciones (toggle)
    if (key == GLFW_KEY_V && action == GLFW_PRESS && !teclaV_presionada) {
        teclaV_presionada = true;
        animarElefante = !animarElefante;
        if (animarElefante) startTimeElefante = glfwGetTime();
    }
    if (key == GLFW_KEY_V && action == GLFW_RELEASE) {
        teclaV_presionada = false;
    }
    
    // ... 12 controles de animación más
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement(xOffset, yOffset);
}

void DoMovement() {
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
```

---

## 📊 Resumen de Estructura

| Sección | Líneas | Contenido |
|---------|--------|-----------|
| Headers e Includes | 1-82 | Librerías y definiciones |
| Prototipos | 83-120 | Declaraciones de funciones |
| Variables Globales | 121-800 | Estado del programa |
| main() | 801-2000 | Bucle principal |
| Funciones Auxiliares | 2001-2300 | Helpers de renderizado |
| Callbacks | 2301-2500 | Eventos de entrada |

---

## 🔗 Referencias

- [Arquitectura](Arquitectura.md) - Diseño general del sistema
- [Sistema de Animación](Sistema-Animacion.md) - Detalles de animaciones
- [Sistema de Iluminación](Sistema-Iluminacion.md) - Configuración de luces
- [Shaders](Shaders.md) - Código de shaders

---

**¡El código bien organizado es más fácil de entender y mantener! 💻✨**
