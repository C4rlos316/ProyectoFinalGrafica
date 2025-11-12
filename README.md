# 🦁 Zoológico Virtual Interactivo en OpenGL  
![C++](https://img.shields.io/badge/C%2B%2B-OpenGL%203.3-blue) 
![status](https://img.shields.io/badge/estado-completo-green) 
![version](https://img.shields.io/badge/version-v1.0.0-yellow)
![license](https://img.shields.io/badge/licencia-educativa-lightgrey)

### 🏆 Proyecto Final de **Computación Gráfica – UNAM (FI)**  
**Autores:** Oscar Cruz Soria, Ana Isabel Díaz Bautista, Carlos Mario Hernández Gutiérrez  
📅 **Fecha de entrega:** 12 Noviembre 2025  
🎓 **Versión:** `v1.0.0`

---

## 🌍 Descripción General

El **Zoológico Virtual Interactivo** es un entorno 3D desarrollado en **C++ y OpenGL**, que simula un zoológico con múltiples hábitats, animales animados jerárquicamente, un sistema completo de iluminación dinámica, audio ambiental y cámara dual.

Este proyecto busca combinar arte, animación y programación gráfica moderna para crear una experiencia inmersiva y educativa.

---

## ✨ Características Principales

### 🎮 Interacción
- Cámara **1ra persona** (movimiento libre con WASD + Mouse)  
- Cámara **3ra persona** con modelo de personaje (León "Alex")  
- Alternancia con tecla **TAB**  
- Movimiento independiente del FPS (`deltaTime`)  

### 🌅 Renderizado 3D
- Motor gráfico basado en **OpenGL 3.3 Core Profile**
- **80+ modelos 3D (.obj)** organizados por hábitat  
- Sistema de **materiales Phong** y texturas múltiples  
- **Skybox 360°** con seis texturas HDRI  
- Soporte de transparencias (aviario de vidrio)

### 💡 Iluminación
- 1 luz direccional (sol global)
- 7 luces puntuales (por hábitat)
- 1 spotlight (linterna)  
- Luz central animada con efecto senoidal  
- Atenuación realista y color por zona

### 🐾 Animaciones Jerárquicas (13 especies)
| Hábitat | Especies y acciones |
|----------|--------------------|
| **Acuario** | Pingüino (auto), Tortuga marina (T), Nutria (N) |
| **Selva** | Capibara (B), Mono (M), Guacamaya (O) |
| **Sabana** | Elefante (V), Jirafa (J), Cebra (L) |
| **Desierto** | Camello (C), Cóndor (Z), Tortuga terrestre (X) |
| **Aviario** | Ave central (automática) |


### 🎵 Sistema de Audio
- Implementación con **miniaudio**
- Reproducción en loop (`musica.mp3`)
- Activación automática en el arranque

---

## 🏗️ Estructura del Proyecto
```
ProyectoFinalGrafica/
├── Main.cpp                    # Archivo principal (totalmente documentado)
├── Camera.h                    # Cámara 1ra/3ra persona
├── Shader.h                    # Compilación de shaders
├── Model.h                     # Carga de modelos .obj
├── Texture.h                   # Configuración de texturas
├── stb_image.h                 # Librería de imágenes
├── miniaudio.h                 # Sistema de audio embebido
│
├── Shader/
│   ├── lighting.vs             # Vertex shader (iluminación Phong)
│   ├── lighting.frag           # Fragment shader
│   ├── lamp.vs / lamp.frag     # Shaders para luces puntuales
│   └── skybox.vs / skybox.frag # Shaders del cielo 360°
│
├── Models/                     # 80+ modelos 3D (.obj)
│   ├── acuario/
│   ├── selva/
│   ├── sabana/
│   ├── desierto/
│   └── aviario/
│
├── images/                     # 15+ texturas y skybox
│   ├── right.jpg
│   ├── left.jpg
│   ├── top.jpg
│   ├── bottom.jpg
│   ├── front.jpg
│   └── back.jpg
│
├── musica.mp3                  # Audio ambiental
└── ProyectoFinalGrafica.sln    # Solución de Visual Studio
```

---

## ⚙️ Requisitos

### 🧩 Software y Hardware
- **Visual Studio 2022** (o equivalente)
- **Windows 10 / 11**
- GPU compatible con **OpenGL 3.3**

### 📦 Librerías Utilizadas
- GLEW 2.1.0  
- GLFW 3.3.8  
- GLM 0.9.9  
- SOIL2 / stb_image  
- miniaudio (integrado)

---

## 🔧 Compilación y Ejecución

1. **Clonar el repositorio:**
```bash
   git clone https://github.com/C4rlos316/ProyectoFinalGrafica.git
   cd ProyectoFinalGrafica
```

2. **Abrir el proyecto en Visual Studio:**
```
   ProyectoFinalGrafica.sln
```

3. **Configurar:**
   - Configuration: `Release` o `Debug`
   - Platform: `x64`

4. **Compilar y ejecutar:**
   - `Ctrl + Shift + B` → Compilar
   - `Ctrl + F5` → Ejecutar sin depurar

---

## 🎮 Controles

| Acción | Tecla |
|--------|-------|
| Movimiento | W / A / S / D o Flechas |
| Cambiar vista (1ra / 3ra persona) | TAB |
| Rotar cámara | Mouse |
| Luz central animada | ESPACIO |
| Salir del programa | ESC |

### 🐾 Animaciones

| Hábitat | Teclas |
|---------|--------|
| **Acuario** | N (Nutria), T (Tortuga) |
| **Selva** | B (Capibara), M (Mono), O (Guacamaya) |
| **Sabana** | V (Elefante), J (Jirafa), L (Cebra) |
| **Desierto** | C (Camello), Z (Cóndor), X (Tortuga) |
| **Aviario** | Automático |
| **Pingüino** | Automático |

---

## 👥 Autores

| Nombre | Rol |
|--------|-----|
| Oscar Cruz Soria[@soeil1](https://github.com/soeil1)   | Animaciones(Acuario,Sabana,Aviario), texturas, escenarios |
| Ana Isabel Díaz Bautista[@anaisabelkitty](https://github.com/anaisabelkitty)   | Animaciones (Sabana, Selva), texturas, escenarios |
| Carlos Mario Hernández Gutiérrez [@C4rlos316](https://github.com/C4rlos316)   | Animaciones (Sabana, Desierto), testing, texturas, escenarios |

---

## 📄 Licencia

**Proyecto desarrollado con fines educativos y académicos**  
para la asignatura de **Computación Gráfica e Interaccion Humano Computadora – UNAM (Facultad de Ingeniería)**.

📂 Repositorio: [ProyectoFinalGrafica](https://github.com/C4rlos316/ProyectoFinalGrafica)
