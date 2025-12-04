# ❓ FAQ - Preguntas Frecuentes

## 📚 Preguntas Generales

### ¿Qué es este proyecto?

Es un **Zoológico Virtual Interactivo** desarrollado en C++ y OpenGL 3.3 como proyecto final de la materia de Computación Gráfica en la UNAM. Incluye 5 hábitats, 13 animales animados, sistema de iluminación dinámica y audio ambiental.

---

### ¿En qué plataformas funciona?

**Principalmente Windows 10/11 (64-bit)**

El proyecto está optimizado y probado para Windows. Para Linux o macOS se requieren ajustes en:
- Rutas de archivos (usar `/` en lugar de `\`)
- Recompilación de librerías
- Posibles ajustes en GLFW/GLEW

---

### ¿Necesito conocimientos previos?

**Para usar el programa:** No, solo seguir la [Guía de Instalación](Instalacion.md)

**Para modificar el código:**
- ✅ C++ básico-intermedio
- ✅ Conceptos de OpenGL
- ✅ Álgebra lineal (matrices, vectores)
- ✅ Conocimientos de gráficos 3D

---

### ¿Es código abierto?

Sí, el código está disponible en [GitHub](https://github.com/C4rlos316/ProyectoFinalGrafica) con fines educativos y académicos.

---

## 🔧 Instalación y Configuración

### ¿Qué software necesito instalar?

**Requerido:**
- Visual Studio 2022 (o 2019)
  - Con "Desarrollo para el escritorio con C++"
- Windows 10/11

**Incluido en el repositorio:**
- Todas las librerías (GLEW, GLFW, GLM, Assimp, SOIL2, miniaudio)
- Modelos 3D
- Texturas
- Shaders

Ver [Instalación](Instalacion.md) para detalles.

---

### ¿Funciona en Visual Studio Code?

No está configurado para VSCode. Necesitarías:
- Configurar CMake o Makefile
- Configurar rutas de inclusión
- Configurar linkado de librerías

Es posible pero requiere configuración manual.

---

### ¿Puedo usar MinGW o GCC?

Teóricamente sí, pero:
- El proyecto está configurado para MSVC
- Las librerías incluidas son para Visual Studio
- Necesitarías recompilar las dependencias

**Recomendación:** Usar Visual Studio 2022 para evitar problemas.

---

### ¿Por qué no compila en modo Debug?

Posibles causas:
- Las librerías incluidas son para Release
- Debug tiene verificaciones adicionales que pueden causar errores

**Solución:** Compilar en modo **Release**.

---

## 🎮 Uso del Programa

### ¿Cómo cambio entre primera y tercera persona?

Presiona **TAB** para alternar entre los modos de cámara.

Ver [Controles](Controles.md) para todos los controles.

---

### ¿Por qué algunos animales no se mueven?

Hay dos tipos de animaciones:
- **Automáticas:** Pingüino y Ave del aviario (siempre activas)
- **Activadas por tecla:** Los demás animales requieren presionar su tecla

Ver [Sistema de Animación](Sistema-Animacion.md) para detalles.

---

### ¿Puedo cambiar la música?

Sí:
1. Reemplaza `musica.mp3` en el directorio raíz
2. O modifica la ruta en `Main.cpp` (línea donde se carga el audio)

Formatos soportados: MP3, WAV, FLAC, OGG

---

### ¿Cómo salgo del programa?

Presiona **ESC** o cierra la ventana con la X.

---

### ¿Por qué la cámara atraviesa los objetos?

El proyecto **no tiene sistema de colisiones** implementado. Es una limitación conocida y comportamiento esperado.

**Posible mejora futura:** Implementar bounding boxes para colisiones.

---

## 🎨 Gráficos y Rendimiento

### ¿Qué GPU necesito?

**Mínimo:**
- GPU compatible con OpenGL 3.3
- Intel HD Graphics 4000 o superior
- NVIDIA GeForce GTX 660
- AMD Radeon HD 7850

**Recomendado:**
- NVIDIA GeForce GTX 1050 o superior
- AMD RX 560 o superior

---

### ¿Cómo mejoro el rendimiento?

1. **Compilar en Release** (no Debug)
2. **Asegurar uso de GPU dedicada** (en laptops)
3. **Reducir resolución** en `Main.cpp`
4. **Actualizar drivers de GPU**

Ver [Troubleshooting](Troubleshooting.md#rendimiento-bajo-fps-bajos) para más detalles.

---

### ¿Puedo cambiar la resolución?

Sí, edita `Main.cpp` (línea ~147):
```cpp
const GLuint WIDTH = 1920, HEIGHT = 1080;  // Full HD
const GLuint WIDTH = 2560, HEIGHT = 1440;  // 2K
const GLuint WIDTH = 3840, HEIGHT = 2160;  // 4K
```

**Nota:** Resoluciones más altas requieren GPU más potente.

---

### ¿Por qué todo se ve muy oscuro?

Ajustar la luz ambiental en el código:
```cpp
glUniform3f(ambientLoc, 0.5f, 0.5f, 0.5f);  // Más brillante
```

O activar más luces puntuales.

Ver [Sistema de Iluminación](Sistema-Iluminacion.md).

---

## 🛠️ Modificación del Código

### ¿Cómo agrego más animales?

1. **Agregar modelo 3D** (.obj) a `/Models/`
2. **Cargar en Main.cpp:**
```cpp
Model modelNuevoAnimal((GLchar*)"Models/nuevo/animal.obj");
```
3. **Crear variables de animación**
4. **Implementar lógica de animación**
5. **Renderizar en el bucle principal**

Ver [Sistema de Animación](Sistema-Animacion.md) para técnicas.

---

### ¿Cómo cambio la posición de un animal?

Busca la variable `basePos` del animal en `Main.cpp`:
```cpp
glm::vec3 pinguBasePos = glm::vec3(7.25f, 0.5f, -9.8f);
//                                   X     Y      Z
```

Cambiar X, Y, Z según necesites.

---

### ¿Cómo agrego más luces?

1. **Aumentar tamaño del array** en `Main.cpp`:
```cpp
glm::vec3 pointLightPositions[8];  // Era 7, ahora 8
```

2. **Agregar posición** en el array
3. **Actualizar el bucle** de configuración de luces
4. **Modificar fragment shader** para soportar más luces

---

### ¿Cómo cambio los colores de las luces?

En `Main.cpp`, busca la configuración de luces:
```cpp
glUniform3f(ambientLoc, 0.2f, 0.4f, 0.8f);  // Azul
//                      R     G     B
```

Ver [Sistema de Iluminación](Sistema-Iluminacion.md#colores-por-hábitat).

---

### ¿Puedo agregar sombras?

No están implementadas actualmente. Requiere:
- Shadow mapping
- Pase adicional de renderizado
- Framebuffer para depth map
- Modificar shaders

**Complejidad:** Alta - Proyecto avanzado

---

## 🎯 Características Técnicas

### ¿Qué versión de OpenGL usa?

**OpenGL 3.3 Core Profile**

Core Profile significa:
- Sin funciones deprecated (glBegin/glEnd)
- Shaders obligatorios
- Pipeline moderno

---

### ¿Por qué OpenGL 3.3 y no 4.x?

**Razones:**
- ✅ Mejor compatibilidad con GPUs antiguas
- ✅ Suficiente para el proyecto
- ✅ Ampliamente soportado
- ✅ Balance entre moderno y compatible

OpenGL 4.x tiene features avanzadas (compute shaders, etc.) que no son necesarias para este proyecto.

---

### ¿Qué son los shaders?

Programas que corren en la GPU para:
- **Vertex Shader:** Transformar vértices (posición, normales)
- **Fragment Shader:** Calcular color de cada píxel (iluminación, texturas)

Escritos en **GLSL** (OpenGL Shading Language).

Ver [Shaders y Renderizado](Shaders.md).

---

### ¿Qué es el modelo Phong?

Un modelo de iluminación que calcula la luz en 3 componentes:
1. **Ambient** - Luz base
2. **Diffuse** - Luz dispersa (mate)
3. **Specular** - Brillos/reflejos

Ver [Sistema de Iluminación](Sistema-Iluminacion.md#modelo-de-iluminación-phong).

---

### ¿Qué es una animación jerárquica?

Un sistema donde las partes del cuerpo tienen relaciones padre-hijo:
```
Cuerpo (padre)
  ├─ Cabeza (hijo)
  ├─ Pata delantera izq (hijo)
  └─ ...
```

Los hijos heredan transformaciones del padre.

Ver [Sistema de Animación](Sistema-Animacion.md#arquitectura-del-sistema).

---

## 📦 Recursos y Assets

### ¿Puedo usar mis propios modelos 3D?

Sí, siempre que:
- ✅ Sean formato **.obj** (con .mtl)
- ✅ Tengan normales calculadas
- ✅ Tengan coordenadas UV si usan texturas
- ✅ Escala apropiada

**Software recomendado:** Blender (gratuito)

---

### ¿Qué formatos de imagen soporta?

Para texturas:
- ✅ JPG/JPEG
- ✅ PNG
- ✅ TGA
- ✅ BMP (puede tener problemas)

**Recomendado:** JPG o PNG

---

### ¿Dónde puedo conseguir modelos 3D gratuitos?

Sitios recomendados:
- **Sketchfab** - https://sketchfab.com/
- **Free3D** - https://free3d.com/
- **CGTrader** - https://www.cgtrader.com/free-3d-models
- **TurboSquid** - https://www.turbosquid.com/Search/3D-Models/free

**Importante:** Verificar licencias de uso.

---

## 🎓 Académico

### ¿Puedo usar este código para mi proyecto?

**Con fines educativos:** Sí, pero:
- ✅ Da crédito a los autores originales
- ✅ Entiende el código que usas
- ✅ No lo presentes como 100% tuyo
- ✅ Agrega tus propias contribuciones

---

### ¿Qué calificación obtuvo este proyecto?

Este proyecto fue desarrollado como proyecto final de la materia de Computación Gráfica en la UNAM, Facultad de Ingeniería.

---

### ¿Hay documentación del código?

Sí, esta wiki completa que incluye:
- [Arquitectura](Arquitectura.md)
- [Sistema de Animación](Sistema-Animacion.md)
- [Sistema de Iluminación](Sistema-Iluminacion.md)
- [Sistema de Cámara](Sistema-Camara.md)
- Y más...

---

## 🔮 Futuro del Proyecto

### ¿Habrá actualizaciones?

El proyecto está completo para la entrega académica. Posibles mejoras futuras:
- Sistema de colisiones
- Sombras dinámicas
- Audio posicional 3D
- Más animales y hábitats
- Sistema de partículas (agua, fuego)

---

### ¿Puedo contribuir al proyecto?

Sí, vía pull requests en GitHub. Áreas de mejora:
- Optimizaciones de rendimiento
- Nuevas animaciones
- Documentación
- Corrección de bugs

---

### ¿Está planificada una versión VR?

No actualmente, pero sería un proyecto interesante. Requeriría:
- Integración de OpenVR/OpenXR
- Ajustes de cámara para estéreo
- Controles para VR
- Optimizaciones de rendimiento

---

## 📞 Soporte

### ¿Dónde reporto un bug?

GitHub Issues:
https://github.com/C4rlos316/ProyectoFinalGrafica/issues

Incluye:
- Descripción del problema
- Pasos para reproducir
- Sistema operativo
- GPU y drivers
- Screenshots si aplica

---

### ¿Cómo contacto a los autores?

Vía GitHub:
- Oscar Cruz Soria - [@soeil1](https://github.com/soeil1)
- Ana Isabel Díaz Bautista - [@anaisabelkitty](https://github.com/anaisabelkitty)
- Carlos Mario Hernández Gutiérrez - [@C4rlos316](https://github.com/C4rlos316)

---

### ¿Hay algún video tutorial?

Actualmente no, pero puedes seguir:
1. [Guía de Instalación](Instalacion.md)
2. [Controles](Controles.md)
3. Esta Wiki completa

---

## 🎉 Agradecimientos

### ¿Qué librerías se usaron?

- **GLEW** - Extensiones OpenGL
- **GLFW** - Ventanas y eventos
- **GLM** - Matemáticas
- **Assimp** - Carga de modelos
- **SOIL2** - Texturas
- **stb_image** - Procesamiento de imágenes
- **miniaudio** - Audio

Todas son open source. ¡Gracias a sus desarrolladores!

---

### ¿Hay agradecimientos especiales?

Al profesor y compañeros de la materia de Computación Gráfica de la UNAM que inspiraron y apoyaron este proyecto.

---

**¿Tu pregunta no está aquí? Abre un [issue en GitHub](https://github.com/C4rlos316/ProyectoFinalGrafica/issues)! 💬**
