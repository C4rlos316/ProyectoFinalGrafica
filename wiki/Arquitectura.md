# 🏗️ Arquitectura del Proyecto

## 📐 Diseño General

El Zoológico Virtual está construido siguiendo una arquitectura modular que separa las responsabilidades en componentes especializados.

---

## 🧩 Componentes Principales

### 1. **Motor de Renderizado (OpenGL 3.3)**
- Gestión del contexto gráfico mediante GLFW
- Pipeline de renderizado moderno con shaders programables
- Soporte para múltiples objetos y transformaciones jerárquicas

### 2. **Sistema de Gestión de Escena**
- Organización espacial en 5 hábitats
- Sistema de coordenadas mundial
- Gestión de transformaciones (Model-View-Projection)

### 3. **Sistema de Recursos**
- Cargador de modelos 3D (formato .obj via Assimp)
- Gestor de texturas (SOIL2/stb_image)
- Compilador de shaders (Shader class)

### 4. **Sistema de Interacción**
- Procesamiento de entrada (teclado y mouse)
- Cámara con dos modos de operación
- Sistema de callbacks GLFW

### 5. **Sistemas Auxiliares**
- Audio (miniaudio)
- Iluminación dinámica
- Animación temporal

---

## 🔄 Flujo de Ejecución

```
┌─────────────────────────────────────────────────────┐
│              INICIALIZACIÓN                          │
├─────────────────────────────────────────────────────┤
│ 1. Inicializar GLFW y crear ventana                 │
│ 2. Inicializar GLEW (extensiones OpenGL)            │
│ 3. Configurar OpenGL (depth test, blending)         │
│ 4. Cargar shaders                                    │
│ 5. Cargar modelos 3D                                 │
│ 6. Cargar texturas                                   │
│ 7. Configurar skybox                                 │
│ 8. Inicializar sistema de audio                     │
│ 9. Configurar callbacks (input)                     │
└─────────────────────────────────────────────────────┘
                        ↓
┌─────────────────────────────────────────────────────┐
│           BUCLE PRINCIPAL (Game Loop)                │
├─────────────────────────────────────────────────────┤
│ MIENTRAS (ventana abierta):                         │
│   1. Calcular deltaTime                             │
│   2. Procesar entrada (DoMovement)                  │
│   3. Actualizar estado de animaciones               │
│   4. Actualizar posición de cámara                  │
│   5. RENDERIZAR:                                     │
│      ├─ Limpiar buffers                             │
│      ├─ Activar shader de iluminación               │
│      ├─ Configurar matrices (view, projection)      │
│      ├─ Configurar luces                            │
│      ├─ Dibujar escenario (pisos, estructuras)      │
│      ├─ Dibujar modelos decorativos                 │
│      ├─ Dibujar animales (con transformaciones)     │
│      ├─ Dibujar modelo del jugador (3ra persona)    │
│      ├─ Dibujar luces (cubos visuales)              │
│      └─ Dibujar skybox                              │
│   6. Intercambiar buffers (swap)                    │
│   7. Procesar eventos (poll)                        │
└─────────────────────────────────────────────────────┘
                        ↓
┌─────────────────────────────────────────────────────┐
│              FINALIZACIÓN                            │
├─────────────────────────────────────────────────────┤
│ 1. Liberar recursos de audio                        │
│ 2. Limpiar VAOs/VBOs                                │
│ 3. Destruir ventana GLFW                            │
│ 4. Terminar GLFW                                     │
└─────────────────────────────────────────────────────┘
```

---

## 🗂️ Organización del Código

### **Main.cpp** (Archivo principal - ~2500 líneas)

```cpp
// SECCIÓN 1: Includes y Declaraciones (líneas 1-120)
├─ Librerías externas (GLEW, GLFW, GLM, etc.)
├─ Headers propios (Shader, Camera, Model, Texture)
├─ Prototipos de funciones
└─ Definición de miniaudio

// SECCIÓN 2: Variables Globales (líneas 121-800)
├─ Configuración de ventana
├─ Estado de cámara
├─ Posiciones de luces
├─ Variables de animación por cada animal (13 especies)
└─ Estados de teclas y timing

// SECCIÓN 3: Función main() (líneas 801-2000)
├─ Inicialización de sistemas
├─ Carga de recursos
├─ Bucle principal de renderizado
└─ Limpieza y finalización

// SECCIÓN 4: Funciones Auxiliares (líneas 2001-2300)
├─ ConfigurarVAO()
├─ ConfigurarTexturaRepetible()
└─ DibujarPiso()

// SECCIÓN 5: Callbacks (líneas 2301-2500)
├─ KeyCallback() - Eventos de teclado
├─ MouseCallback() - Movimiento del mouse
└─ DoMovement() - Procesamiento de movimiento
```

### **Headers Modulares**

| Archivo | Propósito | Responsabilidades |
|---------|-----------|-------------------|
| **Camera.h** | Sistema de cámara | - Definir modos de cámara (1ra/3ra persona)<br>- Procesar movimiento y rotación<br>- Calcular matrices view |
| **Shader.h** | Gestión de shaders | - Compilar vertex/fragment shaders<br>- Vincular programa<br>- Gestionar uniforms |
| **Model.h** | Carga de modelos 3D | - Importar archivos .obj con Assimp<br>- Procesar meshes y materiales<br>- Renderizar modelos |
| **Texture.h** | Gestión de texturas | - Cargar cubemaps (skybox)<br>- Configurar parámetros de textura |
| **Mesh.h** | Representación de mallas | - Almacenar vértices, normales, UVs<br>- Gestionar VAO/VBO/EBO<br>- Renderizar geometría |

---

## 🌍 Organización Espacial

El zoológico está dividido en cuadrantes:

```
        -X (Izquierda)          |          +X (Derecha)
                                |
    ┌─────────────────────┬─────┴─────┬─────────────────────┐
    │                     │           │                     │
    │    DESIERTO         │  AVIARIO  │     ACUARIO        │
    │  (-X, +Z)           │  (0, 0)   │   (+X, -Z)         │
+Z  │  • Camello          │  • Ave    │   • Pingüino       │
    │  • Cóndor           │   central │   • Tortuga        │
    │  • Tortuga terrestre│           │   • Nutria         │
    │                     │           │                     │
────┼─────────────────────┼───────────┼─────────────────────┼────
    │                     │  CENTRO   │                     │
    │    SABANA           │  ENTRADA  │     SELVA          │
    │  (-X, -Z)           │           │   (+X, +Z)         │
-Z  │  • Elefante         │           │   • Capibara       │
    │  • Jirafa           │           │   • Mono           │
    │  • Cebra            │           │   • Guacamaya      │
    │                     │           │                     │
    └─────────────────────┴───────────┴─────────────────────┘
```

**Coordenadas de referencia:**
- Centro: `(0.0, 0.0, 0.0)`
- Radio de cada hábitat: ~7-8 unidades
- Altura de cámara inicial: `1.0` unidades
- Posición inicial de cámara: `(0.0, 1.0, 21.0)`

---

## 🔌 Sistema de Plugins/Librerías

### Librerías de Terceros

| Librería | Versión | Propósito | Integración |
|----------|---------|-----------|-------------|
| **GLEW** | 2.1.0 | Extensiones OpenGL | Vinculada estáticamente |
| **GLFW** | 3.3.8 | Ventanas y eventos | Vinculada dinámicamente |
| **GLM** | 0.9.9 | Matemáticas 3D | Header-only |
| **Assimp** | 3.x | Importación de modelos | DLL (assimp-vc140-mt.dll) |
| **SOIL2** | 1.x | Carga de texturas | Integrada en proyecto |
| **stb_image** | 2.x | Procesamiento de imágenes | Header-only |
| **miniaudio** | 0.11.x | Sistema de audio | Header-only |

### Configuración de Linkado

**Incluir:**
- `opengl32.lib`
- `glew32s.lib` (o glew32.lib si es dinámico)
- `glfw3.lib` (o glfw3dll.lib)
- `assimp-vc140-mt.lib`
- `SOIL2.lib`

---

## 🎯 Patrones de Diseño Utilizados

### 1. **Singleton Implícito**
- La cámara (`camera`) es una única instancia global
- Sistema de audio (un solo `ma_engine`)

### 2. **Factory Pattern (Implícito)**
- Funciones `ConfigurarVAO()` crean objetos de geometría
- Carga de modelos mediante `Model()` constructor

### 3. **State Pattern**
- Estados de animación por animal (animando/detenido)
- Estados de cámara (primera/tercera persona)
- Estados de teclas en array `keys[1024]`

### 4. **Observer Pattern (Callbacks)**
- `KeyCallback()` observa eventos de teclado
- `MouseCallback()` observa movimiento del mouse

---

## 🔄 Pipeline de Renderizado

```
VERTEX DATA (VAO/VBO)
      ↓
VERTEX SHADER (transformaciones MVP)
      ↓
PRIMITIVE ASSEMBLY (triángulos)
      ↓
RASTERIZATION (fragmentos)
      ↓
FRAGMENT SHADER (iluminación Phong, texturas)
      ↓
DEPTH TEST & BLENDING
      ↓
FRAMEBUFFER (pantalla)
```

**Shaders utilizados:**
1. `lighting.vs/frag` - Objetos con iluminación Phong
2. `lamp.vs/frag` - Cubos de luz (sin iluminación)
3. `skybox.vs/frag` - Skybox (sin profundidad)

---

## 📊 Métricas del Proyecto

- **Líneas de código:** ~2,500 (Main.cpp) + ~1,000 (Headers)
- **Modelos 3D:** 80+
- **Texturas:** 15+
- **Shaders:** 6 archivos (3 pares vs/frag)
- **Animales animados:** 13
- **Luces:** 9 (1 direccional + 7 puntuales + 1 spotlight)
- **Hábitats:** 5

---

## 🔗 Próximos Pasos

- Ver [Estructura de Archivos](Estructura-Archivos.md) para detalles de cada archivo
- Ver [Sistema de Cámara](Sistema-Camara.md) para entender el control de vista
- Ver [Sistema de Animación](Sistema-Animacion.md) para las técnicas de animación
