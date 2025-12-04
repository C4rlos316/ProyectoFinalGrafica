# 🎬 Sistema de Animación

## 🎯 Visión General

El sistema de animación del Zoológico Virtual implementa **animaciones jerárquicas** para 13 especies de animales, utilizando transformaciones matriciales y interpolación temporal.

---

## 🏗️ Arquitectura del Sistema

### Principios de Animación Jerárquica

```
MODELO PADRE (Cuerpo)
    ├── HIJO 1 (Cabeza)
    ├── HIJO 2 (Extremidad delantera izquierda)
    ├── HIJO 3 (Extremidad delantera derecha)
    ├── HIJO 4 (Extremidad trasera izquierda)
    └── HIJO 5 (Extremidad trasera derecha)
```

**Concepto:** Cada parte del animal hereda las transformaciones de su padre, creando movimientos realistas y coordinados.

---

## 🔧 Técnica de Transformación

### Pipeline de Transformaciones

Para cada parte del cuerpo:

```cpp
1. Model = glm::mat4(1.0f)                     // Matriz identidad
2. Model = glm::translate(Model, posAnimal)    // Posición del animal
3. Model = glm::rotate(Model, rotY, eje_Y)     // Rotación del cuerpo
4. Model = glm::translate(Model, posParte)     // Posición de la parte
5. Model = glm::rotate(Model, rotParte, eje)   // Rotación de la parte
6. Model = glm::translate(Model, -pivote)      // Volver del pivote
7. Model = glm::scale(Model, escala)           // Escalar
```

### Técnica del Pivote

**Problema:** Rotar alrededor del centro del objeto causa movimientos no naturales.

**Solución:** Trasladar al pivote → Rotar → Volver
```cpp
// Rotar una pata alrededor de su articulación
Model = glm::translate(Model, pivote);         // Ir al pivote (cadera)
Model = glm::rotate(Model, angulo, eje);       // Rotar
Model = glm::translate(Model, -pivote);        // Volver
```

---

## 🐾 Animaciones por Animal

### 🐧 Pingüino (Automática)

**Tipo:** Caminata circular continua

**Variables de Estado:**
```cpp
float rotAletaIzqPingu, rotAletaDerPingu;     // Aletas
float rotPataIzqPingu, rotPataDerPingu;       // Patas
float rotCuerpoPingu;                          // Balanceo del cuerpo
glm::vec3 pinguPos;                            // Posición actual
float pinguRotY;                               // Rotación del cuerpo
```

**Lógica de Animación:**
```cpp
// Ciclo de caminata (2 fases)
float t = glfwGetTime() * 2.0f;  // Velocidad del ciclo

// Fase 1: Pata izquierda adelante
rotPataIzqPingu = sin(t) * 30.0f;
rotPataDerPingu = -sin(t) * 30.0f;

// Balanceo de cuerpo
rotCuerpoPingu = sin(t * 2.0f) * 5.0f;

// Movimiento circular
float radius = 0.5f;
pinguPos.x = baseX + cos(t * 0.3f) * radius;
pinguPos.z = baseZ + sin(t * 0.3f) * radius;
pinguRotY = t * 0.3f * 57.3f;  // Convertir a grados
```

**Partes Animadas:**
- Patas (balanceo alternado)
- Aletas (ligero movimiento)
- Cuerpo (balanceo lateral)
- Rotación general (giro en círculo)

---

### 🐢 Tortuga Marina (Tecla T)

**Tipo:** Natación en trayectoria predefinida

**Variables de Estado:**
```cpp
float rotTortugaAcuarioHead;                   // Cabeza
float rotTortugaAcuarioFL, rotTortugaAcuarioFR; // Aletas delanteras
float rotTortugaAcuarioBL, rotTortugaAcuarioBR; // Aletas traseras
float rotTortugaAcuarioCuerpo;                 // Cuerpo
glm::vec3 tortugaAcuarioPos;                   // Posición
```

**Máquina de Estados:**
```cpp
Estado 0: Reposo (0-2 segundos)
Estado 1: Nadando adelante (2-6 segundos)
    - Aletas se mueven rítmicamente
    - Avanza en línea recta
Estado 2: Giro (6-8 segundos)
    - Rotación gradual
Estado 3: Regreso (8-12 segundos)
    - Vuelta a posición inicial
```

**Código de Ejemplo:**
```cpp
if (animarTortugaAcuario) {
    float elapsed = glfwGetTime() - startTimeTortugaAcuario;
    
    if (elapsed < 2.0f) {
        // Reposo
    } else if (elapsed < 6.0f) {
        // Nadar
        float swimCycle = sin((elapsed - 2.0f) * 3.0f);
        rotTortugaAcuarioFL = swimCycle * 45.0f;
        rotTortugaAcuarioFR = -swimCycle * 45.0f;
        tortugaAcuarioPos.z -= deltaTime * 0.5f;
    }
    // ... más estados
}
```

---

### 🦦 Nutria (Tecla N)

**Tipo:** Secuencia de salto de roca a agua

**Fases de Animación:**

1. **Preparación** (0-1s)
   - Agacharse en la roca
   - Rotación del cuerpo hacia adelante

2. **Salto** (1-2s)
   - Parábola balística
   - Extensión del cuerpo
   - Cola hacia arriba

3. **Zambullida** (2-3s)
   - Entrada al agua
   - Rotación del cuerpo
   - Splash implícito

4. **Natación** (3-6s)
   - Movimiento de patas sincronizado
   - Movimiento ondulatorio de cola
   - Avance bajo el agua

**Código de Parábola:**
```cpp
// Trayectoria parabólica del salto
float t = (elapsed - 1.0f) / 1.0f;  // 0 a 1
float height = -4.0f * (t - 0.5f) * (t - 0.5f) + 1.0f;
nutriaPos.y = nutriaPos1_Roca.y + height;
nutriaPos.x = lerp(startX, endX, t);
```

---

### 🐘 Elefante (Tecla V)

**Tipo:** Caminata pesada con balanceo de trompa

**Características:**
- Movimiento lento y majestuoso
- Trompa se balancea de lado a lado
- Orejas con ligero movimiento
- Patas con ciclo de paso pesado

**Partes Animadas:**
```cpp
float rotElefanteTrompa;      // Balanceo de trompa
float rotElefanteOrejaIzq;    // Oreja izquierda
float rotElefanteOrejaDer;    // Oreja derecha
float rotElefantePataFL;      // Pata delantera izquierda
float rotElefantePataFR;      // Pata delantera derecha
float rotElefantePataRL;      // Pata trasera izquierda
float rotElefantePataRR;      // Pata trasera derecha
```

**Ciclo de Caminata:**
```cpp
float walkCycle = sin(t * 1.5f);  // Ciclo lento

// Patrón alternado de patas
rotElefantePataFL = walkCycle * 20.0f;
rotElefantePataRR = walkCycle * 20.0f;
rotElefantePataFR = -walkCycle * 20.0f;
rotElefantePataRL = -walkCycle * 20.0f;

// Trompa con movimiento ondulatorio
rotElefanteTrompa = sin(t * 2.0f) * 15.0f;
```

---

### 🦒 Jirafa (Tecla J)

**Tipo:** Caminata alta con oscilación de cuello

**Características Únicas:**
- Cuello largo con movimiento pendular
- Pasos largos y elegantes
- Rotación sutil de cabeza

**Animación del Cuello:**
```cpp
// El cuello oscila como un péndulo
float neckAngle = sin(t * 1.0f) * 10.0f;
Model = glm::rotate(Model, glm::radians(neckAngle), glm::vec3(1.0f, 0.0f, 0.0f));
```

---

### 🦓 Cebra (Tecla L)

**Tipo:** Trote/galope

**Características:**
- Movimiento más rápido que otros animales
- Patrón de galope (patas delanteras juntas, traseras juntas)
- Cola en movimiento

**Patrón de Galope:**
```cpp
float gallopCycle = sin(t * 4.0f);  // Ciclo rápido

// Patas delanteras se mueven juntas
rotCebraPataFL = gallopCycle * 40.0f;
rotCebraPataFR = gallopCycle * 40.0f;

// Patas traseras se mueven juntas (desfasadas)
rotCebraPataRL = -gallopCycle * 40.0f;
rotCebraPataRR = -gallopCycle * 40.0f;
```

---

### 🐫 Camello (Tecla C)

**Tipo:** Caminata desértica con balanceo de jorobas

**Características:**
- Patrón de caminata "paso de camello" (patas del mismo lado juntas)
- Balanceo de cuello
- Movimiento sutil de jorobas

**Patrón Único:**
```cpp
// Patas del mismo lado se mueven juntas
rotCamelloPataL = sin(t * 1.5f) * 25.0f;  // Izquierdas
rotCamelloPataR = -sin(t * 1.5f) * 25.0f; // Derechas
```

---

### 🦅 Cóndor (Tecla Z)

**Tipo:** Vuelo planeado con aleteo

**Fases:**
1. **Aleteo** - Movimiento rápido de alas
2. **Planeo** - Alas extendidas
3. **Círculo** - Movimiento circular en el aire

**Animación de Alas:**
```cpp
float flapCycle = sin(t * 5.0f);

if (flapCycle > 0.7f) {
    // Aleteo rápido
    rotCondorAlaIzq = flapCycle * 60.0f;
    rotCondorAlaDer = -flapCycle * 60.0f;
} else {
    // Planeo
    rotCondorAlaIzq = 45.0f;
    rotCondorAlaDer = -45.0f;
}
```

---

### 🦫 Capibara (Tecla B)

**Tipo:** Caminata tranquila

**Características:**
- Movimiento lento y relajado
- Patas cortas con pasos pequeños
- Balanceo mínimo

---

### 🐵 Mono (Tecla M)

**Tipo:** Columpio en rama

**Animación:**
```cpp
// Movimiento pendular
float swingAngle = sin(t * 2.0f) * 45.0f;

// Brazos extendidos
rotMonoBrazoIzq = 90.0f + swingAngle;
rotMonoBrazoDer = 90.0f + swingAngle;

// Cuerpo sigue el péndulo
rotMonoCuerpo = swingAngle * 0.5f;
```

---

### 🦜 Guacamaya (Tecla O)

**Tipo:** Vuelo circular con aleteo

**Trayectoria:**
```cpp
float radius = 2.0f;
float speed = 2.0f;

guacamayaPos.x = centerX + cos(t * speed) * radius;
guacamayaPos.z = centerZ + sin(t * speed) * radius;
guacamayaPos.y = centerY + sin(t * 4.0f) * 0.3f;  // Ondulación vertical
```

---

### 🐢 Tortuga Terrestre (Tecla X)

**Tipo:** Caminata muy lenta

**Características:**
- Velocidad extremadamente reducida
- Movimiento de patas apenas perceptible
- Cabeza con movimiento sutil

---

### 🕊️ Ave del Aviario (Automática)

**Tipo:** Vuelo enjaulado en círculos

**Restricciones:**
- Confinada al área del aviario (radio limitado)
- Altura constante
- Velocidad moderada

```cpp
float aviaryCycle = glfwGetTime() * 1.5f;
float radiusMax = 1.5f;

avePos.x = cos(aviaryCycle) * radiusMax;
avePos.z = sin(aviaryCycle) * radiusMax;
avePos.y = 2.5f + sin(aviaryCycle * 2.0f) * 0.2f;
```

---

## 🔄 Sistema de Control de Animaciones

### Estructura de Variables por Animal

Cada animal tiene:
```cpp
// Estados de rotación de partes
float rotParte1, rotParte2, ...;

// Posición y orientación
glm::vec3 animalPos;
float animalRotY;
float animalScale;

// Control de animación
bool animarAnimal;              // ¿Está animando?
bool tecla_presionada;          // ¿Tecla presionada?
float startTimeAnimal;          // Tiempo de inicio
```

### Sistema de Teclas (Toggle)

```cpp
// En KeyCallback()
if (key == GLFW_KEY_V && action == GLFW_PRESS && !teclaV_presionada) {
    teclaV_presionada = true;
    animarElefante = !animarElefante;  // Toggle
    if (animarElefante) {
        startTimeElefante = glfwGetTime();  // Reiniciar contador
    }
}
if (key == GLFW_KEY_V && action == GLFW_RELEASE) {
    teclaV_presionada = false;
}
```

---

## ⏱️ Sistema de Timing

### DeltaTime para Movimiento Suave

```cpp
// En el bucle principal
float currentFrame = glfwGetTime();
deltaTime = currentFrame - lastFrame;
lastFrame = currentFrame;

// Uso en movimiento
animalPos.x += velocidad * deltaTime;  // Movimiento independiente de FPS
```

### Ciclos con glfwGetTime()

```cpp
// Animación cíclica continua
float t = glfwGetTime();
float cycle = sin(t * frecuencia) * amplitud;
```

### Animaciones con Duración

```cpp
float elapsed = glfwGetTime() - startTime;

if (elapsed < duracionFase1) {
    // Fase 1
} else if (elapsed < duracionFase2) {
    // Fase 2
} else {
    // Detener o reiniciar
    animar = false;
}
```

---

## 🎨 Interpolación y Suavizado

### Interpolación Lineal (Lerp)

```cpp
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// Uso
float posActual = lerp(posInicio, posFin, progreso);
```

### Suavizado con Seno

```cpp
// Movimiento suave de inicio y fin
float smoothT = (1.0f - cos(t * PI)) / 2.0f;
```

### Ease In/Out

```cpp
// Aceleración al inicio, desaceleración al final
float easeInOut(float t) {
    return t < 0.5f 
        ? 2.0f * t * t 
        : -1.0f + (4.0f - 2.0f * t) * t;
}
```

---

## 🛠️ Implementación en Código

### Ejemplo Completo: Animación del Elefante

```cpp
// En Main.cpp (dentro del bucle de renderizado)

if (animarElefante) {
    float t = glfwGetTime() - startTimeElefante;
    
    // Ciclo de caminata
    float walkCycle = sin(t * 1.5f);
    
    // Patas delanteras
    rotElefantePataFL = walkCycle * 20.0f;
    rotElefantePataFR = -walkCycle * 20.0f;
    
    // Patas traseras (desfasadas)
    rotElefantePataRL = -walkCycle * 20.0f;
    rotElefantePataRR = walkCycle * 20.0f;
    
    // Trompa
    rotElefanteTrompa = sin(t * 2.0f) * 15.0f;
    
    // Movimiento
    elefantePos.x += cos(elefanteRotY * DEG_TO_RAD) * velocidad * deltaTime;
    elefantePos.z += sin(elefanteRotY * DEG_TO_RAD) * velocidad * deltaTime;
}

// Renderizado
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, elefantePos);
model = glm::rotate(model, glm::radians(elefanteRotY), glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(elefanteScale));

// Renderizar cuerpo
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
modelElefanteCuerpo.Draw(lightingShader);

// Renderizar trompa (hijo del cuerpo)
glm::mat4 modelTrompa = model;
modelTrompa = glm::translate(modelTrompa, glm::vec3(0.0f, 1.0f, 0.5f)); // Pivote
modelTrompa = glm::rotate(modelTrompa, glm::radians(rotElefanteTrompa), 
                          glm::vec3(1.0f, 0.0f, 0.0f));
modelTrompa = glm::translate(modelTrompa, glm::vec3(0.0f, -1.0f, -0.5f));

glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTrompa));
modelElefanteTrompa.Draw(lightingShader);
```

---

## 📊 Resumen de Animaciones

| Animal | Tipo | Partes Animadas | Complejidad |
|--------|------|-----------------|-------------|
| Pingüino | Caminata circular | 4 (patas, aletas) | Media |
| Tortuga Marina | Natación | 5 (aletas, cabeza) | Alta |
| Nutria | Salto + nado | 6 (patas, cola, cuerpo) | Alta |
| Elefante | Caminata | 7 (patas, trompa, orejas) | Alta |
| Jirafa | Caminata | 5 (patas, cuello) | Media |
| Cebra | Galope | 5 (patas, cola) | Media |
| Camello | Caminata | 5 (patas, cuello) | Media |
| Cóndor | Vuelo | 2 (alas) | Media |
| Capibara | Caminata | 4 (patas) | Baja |
| Mono | Columpio | 3 (brazos, cuerpo) | Media |
| Guacamaya | Vuelo circular | 2 (alas) | Media |
| Tortuga Terrestre | Caminata lenta | 5 (patas, cabeza) | Baja |
| Ave Aviario | Vuelo enjaulado | 2 (alas) | Baja |

---

## 🎓 Conceptos Técnicos

### Transformaciones Matriciales
- **Traslación:** Mover en el espacio 3D
- **Rotación:** Girar alrededor de un eje
- **Escalado:** Cambiar tamaño
- **Composición:** Multiplicación de matrices (orden importa!)

### Jerarquía de Transformaciones
```
Mundo → Hábitat → Animal → Cuerpo → Parte
```

Cada nivel hereda las transformaciones de su padre.

### Sincronización Temporal
- Uso de `glfwGetTime()` para tiempo absoluto
- `deltaTime` para independencia de framerate
- Funciones trigonométricas para ciclos suaves

---

## 🔗 Referencias

- [Controles](Controles.md) - Lista de todas las teclas de animación
- [Código Estructura](Codigo-Estructura.md) - Dónde está el código de cada animación
- [Variables Globales](Variables-Globales.md) - Todas las variables de animación

---

**¡Las animaciones jerárquicas dan vida al zoológico! 🎭🦁**
