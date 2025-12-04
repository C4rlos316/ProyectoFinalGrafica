# 🎥 Sistema de Cámara

## 📐 Visión General

El sistema de cámara del Zoológico Virtual permite dos modos de visualización:
1. **Primera Persona** - Vista inmersiva desde los ojos del jugador
2. **Tercera Persona** - Vista desde atrás del modelo del personaje (León "Alex")

---

## 🏗️ Arquitectura de la Cámara

### Clase Camera (Camera.h)

```cpp
class Camera {
public:
    // Atributos de posición y orientación
    glm::vec3 position;       // Posición en el mundo
    glm::vec3 front;          // Vector hacia donde mira
    glm::vec3 up;             // Vector "arriba" de la cámara
    glm::vec3 right;          // Vector "derecha" de la cámara
    glm::vec3 worldUp;        // Arriba del mundo (0,1,0)
    
    // Ángulos de Euler
    GLfloat yaw;              // Rotación horizontal (-90° inicial)
    GLfloat pitch;            // Rotación vertical (0° inicial)
    
    // Opciones de cámara
    GLfloat movementSpeed;    // Velocidad de movimiento (6.0)
    GLfloat mouseSensitivity; // Sensibilidad del mouse (0.25)
    GLfloat zoom;             // Campo de visión (45°)
    
    // Tipo de cámara
    Camera_Type cameraType;   // FIRST_PERSON o THIRD_PERSON
};
```

---

## 🎮 Modos de Cámara

### 1. Primera Persona (FIRST_PERSON)

**Características:**
- Vista directa desde la posición de la cámara
- No se renderiza modelo del jugador
- Mayor inmersión
- Ideal para exploración detallada

**Posición inicial:**
```cpp
glm::vec3(0.0f, 1.0f, 21.0f)
// X: Centro
// Y: Altura de los ojos (1 metro)
// Z: Alejado del centro (21 metros hacia atrás)
```

**Matriz View:**
```cpp
glm::mat4 view = camera.GetViewMatrix();
// Equivalente a:
view = glm::lookAt(camera.position, 
                   camera.position + camera.front, 
                   camera.up);
```

---

### 2. Tercera Persona (THIRD_PERSON)

**Características:**
- Cámara posicionada detrás del jugador
- Muestra el modelo del León "Alex"
- Mejor orientación espacial
- Vista más cinemática

**Configuración:**
```cpp
const GLfloat THIRD_PERSON_DISTANCE = 5.0f;  // Distancia detrás del jugador
```

**Cálculo de posición de cámara:**
```cpp
glm::vec3 GetThirdPersonPosition() {
    // Posición de la cámara = posición del jugador - (frente * distancia) + altura
    glm::vec3 offset = front * THIRD_PERSON_DISTANCE;
    offset.y = 0.0f;  // Mantener en el plano horizontal
    
    glm::vec3 cameraPos = position - offset;
    cameraPos.y = position.y + 2.0f;  // Elevar cámara 2 metros
    
    return cameraPos;
}
```

**Matriz View en 3ra Persona:**
```cpp
glm::mat4 GetViewMatrix() {
    if (cameraType == THIRD_PERSON) {
        glm::vec3 cameraPos = GetThirdPersonPosition();
        return glm::lookAt(cameraPos, position, worldUp);
    } else {
        return glm::lookAt(position, position + front, up);
    }
}
```

---

## ⌨️ Control de Cámara

### Cambio de Modo

**Tecla:** TAB

**Implementación:**
```cpp
// En KeyCallback()
if (key == GLFW_KEY_TAB && action == GLFW_PRESS && !teclaTAB_presionada) {
    teclaTAB_presionada = true;
    
    // Alternar entre modos
    if (camera.cameraType == FIRST_PERSON) {
        camera.cameraType = THIRD_PERSON;
    } else {
        camera.cameraType = FIRST_PERSON;
    }
}

if (key == GLFW_KEY_TAB && action == GLFW_RELEASE) {
    teclaTAB_presionada = false;
}
```

---

### Movimiento (WASD o Flechas)

**Direcciones:**
```cpp
enum Camera_Movement {
    FORWARD,   // W o ↑
    BACKWARD,  // S o ↓
    LEFT,      // A o ←
    RIGHT      // D o →
};
```

**Implementación:**
```cpp
void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
    GLfloat velocity = movementSpeed * deltaTime;
    
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}
```

**Movimiento independiente del framerate:**
```cpp
// En DoMovement()
GLfloat cameraSpeed = 5.0f * deltaTime;

if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    camera.ProcessKeyboard(FORWARD, deltaTime);
if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    camera.ProcessKeyboard(BACKWARD, deltaTime);
if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    camera.ProcessKeyboard(LEFT, deltaTime);
if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    camera.ProcessKeyboard(RIGHT, deltaTime);
```

---

### Rotación (Mouse)

**Variables:**
```cpp
GLfloat lastX = WIDTH / 2.0;   // Última posición X del mouse
GLfloat lastY = HEIGHT / 2.0;  // Última posición Y del mouse
bool firstMouse = true;         // Primera vez que entra el mouse
```

**Callback del Mouse:**
```cpp
void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    // Calcular offset del movimiento
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Invertido (Y va de abajo a arriba)
    
    lastX = xPos;
    lastY = yPos;
    
    // Procesar movimiento
    camera.ProcessMouseMovement(xOffset, yOffset);
}
```

**Procesamiento del Movimiento:**
```cpp
void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, 
                          GLboolean constrainPitch = true) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;
    
    // Actualizar ángulos de Euler
    yaw += xOffset;
    pitch += yOffset;
    
    // Limitar pitch para evitar gimbal lock
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    
    // Actualizar vectores de la cámara
    updateCameraVectors();
}
```

---

## 🔄 Sistema de Vectores

### Actualización de Vectores

```cpp
void updateCameraVectors() {
    // Calcular nuevo vector Front
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
    
    // Recalcular vectores Right y Up
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
```

**Vectores resultantes:**
- **front:** Hacia donde mira la cámara
- **right:** Perpendicular a front (para movimiento lateral)
- **up:** Perpendicular a front y right (arriba local de la cámara)

---

## 📊 Configuración de Proyección

### Matriz de Proyección (Perspectiva)

```cpp
glm::mat4 projection = glm::perspective(
    glm::radians(camera.zoom),           // FOV (Field of View) - 45°
    (GLfloat)SCREEN_WIDTH / SCREEN_HEIGHT, // Aspect ratio
    0.1f,                                 // Near plane
    100.0f                                // Far plane
);
```

**Parámetros:**
- **FOV:** 45° (campo de visión estándar)
- **Aspect Ratio:** 16:9 (1280/720 = 1.778)
- **Near Plane:** 0.1 unidades (objetos más cerca no se renderizan)
- **Far Plane:** 100 unidades (objetos más lejos no se renderizan)

### Enviar al Shader

```cpp
GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
```

---

## 🎯 Renderizado del Modelo del Jugador

En modo tercera persona, se renderiza el León "Alex":

```cpp
if (camera.cameraType == THIRD_PERSON) {
    glm::mat4 model = glm::mat4(1.0f);
    
    // Posicionar en la posición de la cámara lógica
    model = glm::translate(model, camera.position);
    
    // Rotar hacia donde mira la cámara
    float angleY = atan2(camera.front.x, camera.front.z);
    model = glm::rotate(model, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Ajustar posición y escala
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    
    // Renderizar
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    modelAlex.Draw(lightingShader);
}
```

---

## 🔧 Parámetros Configurables

### Velocidad de Movimiento

```cpp
const GLfloat SPEED = 6.0f;  // Unidades por segundo
```

**Modificar:**
- **Más rápido:** `8.0f` - `10.0f`
- **Más lento:** `3.0f` - `4.0f`

### Sensibilidad del Mouse

```cpp
const GLfloat SENSITIVTY = 0.25f;
```

**Modificar:**
- **Más sensible:** `0.4f` - `0.5f`
- **Menos sensible:** `0.1f` - `0.15f`

### Distancia en Tercera Persona

```cpp
const GLfloat THIRD_PERSON_DISTANCE = 5.0f;
```

**Modificar:**
- **Más cerca:** `3.0f`
- **Más lejos:** `7.0f` - `10.0f`

### Campo de Visión (FOV)

```cpp
const GLfloat ZOOM = 45.0f;  // Grados
```

**Modificar:**
- **Zoom in (más cerca):** `30.0f`
- **Zoom out (más lejos):** `60.0f` - `90.0f`

> ⚠️ FOV muy alto (>90°) puede causar distorsión tipo "ojo de pez"

---

## 🎮 Casos de Uso

### Exploración Detallada
**Modo:** Primera Persona  
**Razón:** Vista directa, mayor inmersión

### Vista Cinemática
**Modo:** Tercera Persona  
**Razón:** Ver el personaje, mejor orientación espacial

### Navegación en Espacios Cerrados
**Modo:** Primera Persona  
**Razón:** Mejor percepción de profundidad

### Captura de Screenshots
**Modo:** Tercera Persona  
**Razón:** Composición más atractiva con el personaje

---

## 🐛 Limitaciones y Consideraciones

### Colisiones
❌ **No implementado:** El sistema actual no tiene detección de colisiones.
- La cámara puede atravesar paredes y objetos
- Posible mejora futura: implementar bounding boxes

### Gravedad
❌ **No implementado:** No hay sistema de física.
- La cámara puede "volar"
- La altura Y se mantiene constante (a menos que se modifique)

### Suavizado de Movimiento
✅ **Implementado:** Uso de `deltaTime`
- Movimiento suave independiente del framerate

### Límites del Mundo
❌ **No implementado:** No hay límites de mundo.
- El usuario puede alejarse infinitamente
- Posible mejora: implementar boundaries

---

## 📐 Matemáticas de la Cámara

### Conversión Yaw/Pitch a Vectores

**Ángulos de Euler:**
- **Yaw (ψ):** Rotación alrededor del eje Y (horizontal)
- **Pitch (θ):** Rotación alrededor del eje X (vertical)

**Fórmulas:**
```
front.x = cos(yaw) * cos(pitch)
front.y = sin(pitch)
front.z = sin(yaw) * cos(pitch)
```

### Producto Cruz para Right y Up

```
right = normalize(cross(front, worldUp))
up = normalize(cross(right, front))
```

**Propiedades:**
- Los tres vectores son perpendiculares entre sí
- Forman un sistema de coordenadas ortonormal

---

## 🎓 Conceptos Técnicos

### LookAt Matrix
La función `glm::lookAt()` crea una matriz view que:
1. Traslada el mundo a la posición de la cámara
2. Rota el mundo para que la cámara "mire" al objetivo

### Gimbal Lock
Limitamos el pitch a ±89° para evitar gimbal lock:
```cpp
if (pitch > 89.0f) pitch = 89.0f;
if (pitch < -89.0f) pitch = -89.0f;
```

### Delta Time
Multiplicar el movimiento por `deltaTime` asegura:
- Movimiento consistente en diferentes FPS
- 60 FPS o 120 FPS dan la misma velocidad real

---

## 🔗 Referencias

- [Controles](Controles.md) - Lista completa de controles de cámara
- [Arquitectura](Arquitectura.md) - Integración en el sistema general
- [Código Estructura](Codigo-Estructura.md) - Implementación detallada

---

## 📝 Código Completo de Ejemplo

```cpp
// Inicialización
Camera camera(glm::vec3(0.0f, 1.0f, 21.0f));

// En el bucle de renderizado
GLfloat currentFrame = glfwGetTime();
deltaTime = currentFrame - lastFrame;
lastFrame = currentFrame;

// Procesar movimiento
DoMovement();

// Obtener matrices
glm::mat4 view = camera.GetViewMatrix();
glm::mat4 projection = glm::perspective(
    glm::radians(camera.zoom),
    (float)SCREEN_WIDTH / SCREEN_HEIGHT,
    0.1f, 100.0f
);

// Enviar al shader
glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

// Renderizar modelo del jugador si está en 3ra persona
if (camera.cameraType == THIRD_PERSON) {
    // ... renderizar León Alex
}
```

---

**¡La cámara es la ventana al mundo 3D! 🎥👁️**
