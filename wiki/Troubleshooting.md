# 🔧 Troubleshooting - Solución de Problemas

## 🚨 Problemas Comunes y Soluciones

---

## 📦 Problemas de Compilación

### Error: "Cannot open file glew32.lib"

**Síntomas:**
```
LINK : fatal error LNK1104: cannot open file 'glew32.lib'
```

**Soluciones:**

1. **Verificar rutas de biblioteca:**
   - Click derecho en el proyecto → Propiedades
   - Enlazador → General → Directorios de bibliotecas adicionales
   - Agregar la ruta donde está `glew32.lib`

2. **Verificar el nombre correcto:**
   - Si usas GLEW estático: `glew32s.lib`
   - Si usas GLEW dinámico: `glew32.lib`

3. **Descargar GLEW:**
   - Si falta, descargar de: http://glew.sourceforge.net/
   - Extraer en una carpeta y agregar las rutas

---

### Error: "Cannot open source file Camera.h"

**Síntomas:**
```
fatal error C1083: Cannot open include file: 'Camera.h': No such file or directory
```

**Soluciones:**

1. **Verificar ubicación del archivo:**
   - Asegurar que `Camera.h` está en el directorio del proyecto

2. **Configurar directorios de inclusión:**
   - Propiedades → C/C++ → General → Directorios de inclusión adicionales
   - Agregar `$(ProjectDir)` si no está

3. **Verificar mayúsculas:**
   - En algunos sistemas, `camera.h` ≠ `Camera.h`

---

### Error: "Unresolved external symbol"

**Síntomas:**
```
error LNK2019: unresolved external symbol _glewInit referenced in function _main
```

**Soluciones:**

1. **Agregar bibliotecas al enlazador:**
   - Propiedades → Enlazador → Entrada → Dependencias adicionales
   - Agregar: `opengl32.lib;glew32s.lib;glfw3.lib;assimp-vc140-mt.lib`

2. **Definir GLEW_STATIC:**
   - Si usas GLEW estático
   - Propiedades → C/C++ → Preprocesador → Definiciones del preprocesador
   - Agregar: `GLEW_STATIC`

---

### Error: "Incompatible platform toolset"

**Síntomas:**
```
error MSB8020: The build tools for v142 cannot be found
```

**Soluciones:**

1. **Cambiar toolset:**
   - Propiedades → General → Conjunto de herramientas de la plataforma
   - Seleccionar el instalado en tu Visual Studio (v143, v142, v141)

2. **Instalar el toolset faltante:**
   - Abrir Visual Studio Installer
   - Modificar → Componentes individuales
   - Buscar y seleccionar el toolset necesario

---

## 🎮 Problemas de Ejecución

### La ventana se abre pero está en negro

**Causas posibles:**

1. **GPU no soporta OpenGL 3.3:**
   - Verificar con OpenGL Extensions Viewer
   - Actualizar drivers de GPU

2. **Shaders no compilan:**
```cpp
// Agregar al código para debug
GLint success;
GLchar infoLog[512];
glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "SHADER ERROR: " << infoLog << std::endl;
}
```

3. **Texturas no cargan:**
   - Verificar rutas de imágenes
   - Asegurar que las imágenes existen

**Solución:**
```cpp
// Verificar carga de texturas
unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
if (!image) {
    std::cout << "Failed to load texture: " << path << std::endl;
}
```

---

### Error: "Failed to initialize GLFW"

**Síntomas:**
```
Failed to initialize GLFW
```

**Soluciones:**

1. **Verificar glfw3.dll:**
   - Copiar `glfw3.dll` al directorio del ejecutable

2. **Reinstalar drivers de GPU:**
   - Drivers desactualizados pueden causar este error

3. **Ejecutar como administrador:**
   - En algunos sistemas, puede requerir permisos

---

### Error: "Failed to create GLFW window"

**Síntomas:**
```
Failed to create GLFW window
```

**Soluciones:**

1. **GPU no soporta OpenGL 3.3:**
```cpp
// Intentar con versión menor
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
```

2. **Resolución no soportada:**
   - Cambiar WIDTH y HEIGHT a valores más bajos
   - Ej: 1280x720 → 800x600

---

### No se reproduce el audio

**Causas:**

1. **Archivo no encontrado:**
   - Verificar que `musica.mp3` está en el directorio correcto
   - Usar ruta absoluta para debug:
```cpp
ma_sound_init_from_file(&engine, "C:/ruta/completa/musica.mp3", ...);
```

2. **Formato no compatible:**
   - Verificar que el MP3 es válido
   - Probar con un WAV simple

3. **Dispositivo de audio no disponible:**
```cpp
ma_result result = ma_engine_init(NULL, &engine);
if (result != MA_SUCCESS) {
    std::cout << "Audio error code: " << result << std::endl;
}
```

---

### Rendimiento bajo (FPS bajos)

**Síntomas:**
- Movimiento entrecortado
- < 30 FPS

**Soluciones:**

1. **Compilar en Release:**
   - Cambiar de Debug a Release
   - La diferencia puede ser de 10x en rendimiento

2. **Verificar GPU dedicada:**
   - En laptops con Intel + NVIDIA/AMD
   - Panel de control de NVIDIA → Gestionar configuración 3D
   - Seleccionar la aplicación y forzar GPU de alto rendimiento

3. **Reducir resolución:**
```cpp
const GLuint WIDTH = 800, HEIGHT = 600;  // Más bajo
```

4. **Verificar V-Sync:**
```cpp
glfwSwapInterval(1);  // V-Sync ON (limita a 60 FPS pero suave)
glfwSwapInterval(0);  // V-Sync OFF (FPS ilimitado pero puede tener tearing)
```

---

### Modelos no se ven o aparecen rotos

**Causas:**

1. **Archivos .obj no encontrados:**
```cpp
Model model((GLchar*)"Models/animal/modelo.obj");
// Verificar que el path es correcto
```

2. **Texturas faltantes:**
   - Los modelos .obj referencian texturas
   - Asegurar que están en el mismo directorio

3. **Escala incorrecta:**
   - Algunos modelos son muy grandes o pequeños
```cpp
model = glm::scale(model, glm::vec3(0.1f));  // Escalar a 10%
```

---

## 🖼️ Problemas de Texturas

### Textura aparece blanca o negra

**Soluciones:**

1. **Verificar carga:**
```cpp
GLuint texture = SOIL_load_OGL_texture(
    "images/texture.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS
);

if (texture == 0) {
    std::cout << "SOIL error: " << SOIL_last_result() << std::endl;
}
```

2. **Verificar formato:**
   - Usar JPG o PNG
   - Evitar BMP (problemas de compatibilidad)

3. **Verificar ruta:**
   - Usar rutas relativas al ejecutable
   - O rutas absolutas para debug

---

### Textura aparece distorsionada

**Soluciones:**

1. **Invertir Y:**
```cpp
SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 
                      SOIL_FLAG_INVERT_Y);  // ← Agregar esta flag
```

2. **Verificar coordenadas UV:**
   - El modelo .obj debe tener coordenadas de textura válidas

---

## 💡 Problemas de Iluminación

### Objetos demasiado oscuros

**Soluciones:**

1. **Aumentar luz ambiental:**
```cpp
glUniform3f(ambientLoc, 0.5f, 0.5f, 0.5f);  // Más luz ambiental
```

2. **Acercar luces puntuales:**
   - Reducir atenuación
```cpp
float constant = 1.0f;
float linear = 0.045f;     // Reducir
float quadratic = 0.0075f;  // Reducir
```

---

### Objetos demasiado brillantes (blancos)

**Soluciones:**

1. **Reducir componente especular:**
```cpp
glUniform3f(specularLoc, 0.3f, 0.3f, 0.3f);  // Menos brillo
```

2. **Reducir shininess:**
```cpp
glUniform1f(shininessLoc, 8.0f);  // Menos brillante (era 32.0)
```

---

## 🎥 Problemas de Cámara

### Cámara se mueve demasiado rápido/lento

**Soluciones:**

1. **Ajustar velocidad en Camera.h:**
```cpp
const GLfloat SPEED = 3.0f;   // Más lento
const GLfloat SPEED = 10.0f;  // Más rápido
```

---

### Mouse muy sensible o poco sensible

**Soluciones:**

1. **Ajustar sensibilidad:**
```cpp
const GLfloat SENSITIVTY = 0.1f;   // Menos sensible
const GLfloat SENSITIVTY = 0.5f;   // Más sensible
```

---

### Cámara atraviesa objetos

**Explicación:**
- No hay sistema de colisiones implementado
- Es el comportamiento esperado actual

**Posible solución futura:**
- Implementar bounding boxes
- Detección de colisiones con objetos

---

## 🎬 Problemas de Animaciones

### Animación no se activa

**Verificar:**

1. **Tecla correcta:**
   - Revisar [Controles](Controles.md) para la tecla correcta

2. **Sistema de toggle:**
   - La animación se activa/desactiva con la misma tecla
   - Presionar nuevamente para ver el efecto

---

### Animación se ve entrecortada

**Soluciones:**

1. **Mejorar FPS:**
   - Ver sección "Rendimiento bajo"

2. **Verificar deltaTime:**
   - El movimiento debe usar `deltaTime` para suavidad

---

## 💻 Problemas Específicos de Sistema

### Windows Defender bloquea el ejecutable

**Solución:**
- Agregar excepción en Windows Defender
- O firmar el ejecutable (para distribución)

---

### DLL faltantes al ejecutar

**Síntomas:**
```
The code execution cannot proceed because assimp-vc140-mt.dll was not found
```

**Soluciones:**

1. **Copiar DLLs al directorio del .exe:**
   - `assimp-vc140-mt.dll`
   - `glew32.dll` (si es dinámico)

2. **Agregar al PATH:**
   - O copiar a `C:\Windows\System32`

---

## 🔍 Herramientas de Diagnóstico

### Verificar Versión de OpenGL

```cpp
std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
```

### Verificar FPS

```cpp
// En el bucle principal
static double lastTime = glfwGetTime();
static int nbFrames = 0;

double currentTime = glfwGetTime();
nbFrames++;

if (currentTime - lastTime >= 1.0) {
    std::cout << "FPS: " << nbFrames << std::endl;
    nbFrames = 0;
    lastTime = currentTime;
}
```

---

## 📞 Obtener Ayuda

Si ninguna de estas soluciones funciona:

1. **Revisar [FAQ](FAQ.md)** - Preguntas frecuentes
2. **Abrir un issue en GitHub:**
   - https://github.com/C4rlos316/ProyectoFinalGrafica/issues
   - Incluir:
     - Sistema operativo
     - Versión de Visual Studio
     - GPU y drivers
     - Mensaje de error completo
     - Pasos para reproducir

3. **Verificar requisitos:**
   - Ver [Instalación](Instalacion.md) para requisitos mínimos

---

## ✅ Checklist de Diagnóstico

Antes de reportar un problema, verificar:

- [ ] Compilado en modo Release (no Debug)
- [ ] Todos los archivos (.obj, texturas, shaders) en su lugar
- [ ] DLLs copiadas al directorio del ejecutable
- [ ] Drivers de GPU actualizados
- [ ] GPU soporta OpenGL 3.3+
- [ ] Visual Studio 2019 o 2022
- [ ] Configuración x64 (no x86)

---

**¡La mayoría de los problemas tienen solución! 🔧✅**
