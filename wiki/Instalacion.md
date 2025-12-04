# 🚀 Guía de Instalación

## 📋 Requisitos del Sistema

### Hardware Mínimo
- **Procesador:** Intel Core i3 / AMD equivalente
- **RAM:** 4 GB
- **GPU:** Compatible con OpenGL 3.3 o superior
  - NVIDIA GeForce GTX 660 o superior
  - AMD Radeon HD 7850 o superior
  - Intel HD Graphics 4000 o superior
- **Almacenamiento:** 500 MB de espacio libre

### Hardware Recomendado
- **Procesador:** Intel Core i5 / AMD Ryzen 5 o superior
- **RAM:** 8 GB o más
- **GPU:** NVIDIA GeForce GTX 1050 / AMD RX 560 o superior
- **Almacenamiento:** 1 GB de espacio libre

### Sistema Operativo
- **Windows 10** (64-bit) o superior
- **Windows 11** (64-bit)

> ⚠️ **Nota:** El proyecto está optimizado para Windows. Para ejecutar en Linux o macOS, se requieren ajustes en las rutas y posiblemente recompilación de las librerías.

---

## 📦 Dependencias y Librerías

El proyecto utiliza las siguientes librerías:

| Librería | Versión | Propósito | Incluida |
|----------|---------|-----------|----------|
| **GLEW** | 2.1.0 | Extensiones OpenGL | ✅ Sí |
| **GLFW** | 3.3.8 | Gestión de ventanas | ✅ Sí |
| **GLM** | 0.9.9.8 | Matemáticas para gráficos | ✅ Sí |
| **Assimp** | 3.x | Importación de modelos 3D | ✅ Sí (DLL incluida) |
| **SOIL2** | 1.x | Carga de texturas | ✅ Sí |
| **stb_image** | 2.x | Procesamiento de imágenes | ✅ Sí |
| **miniaudio** | 0.11.x | Sistema de audio | ✅ Sí |

> ✅ Todas las dependencias ya están incluidas en el repositorio, no es necesario descargarlas por separado.

---

## 🔧 Software Requerido

### Visual Studio 2022 (Recomendado)
1. Descargar desde: https://visualstudio.microsoft.com/es/downloads/
2. Durante la instalación, seleccionar:
   - ✅ **Desarrollo para el escritorio con C++**
   - ✅ **Herramientas de compilación de C++ de MSVC**
   - ✅ **Windows 10 SDK** (o superior)

### Alternativas
- **Visual Studio 2019** (también compatible)
- **Visual Studio Community** (versión gratuita)

> ⚠️ **Importante:** El proyecto está configurado para Visual Studio. Si usas otro IDE (CLion, Code::Blocks), necesitarás configurar manualmente las rutas de inclusión y linkado.

---

## 📥 Instalación Paso a Paso

### Inicio Rápido

```bash
# 1. Clonar el repositorio
git clone https://github.com/C4rlos316/ProyectoFinalGrafica.git

# 2. Entrar al directorio
cd ProyectoFinalGrafica

# 3. Abrir el proyecto en Visual Studio
# Doble clic en ProyectoFinalGrafica.sln
```

---

### Instalación Detallada

#### **Paso 1: Clonar o Descargar el Repositorio**

**Opción A - Con Git (Recomendado):**
```bash
git clone https://github.com/C4rlos316/ProyectoFinalGrafica.git
cd ProyectoFinalGrafica
```

**Opción B - Descarga Directa:**
1. Ve a: https://github.com/C4rlos316/ProyectoFinalGrafica
2. Click en "Code" → "Download ZIP"
3. Extrae el archivo en tu directorio deseado

---

#### **Paso 2: Verificar la Estructura de Archivos**

Asegúrate de que tienes esta estructura:

```
ProyectoFinalGrafica/
├── ProyectoFinalGrafica.sln       ← Archivo de solución
├── ProyectoFinalGrafica/
│   ├── Main.cpp                    ← Código principal
│   ├── Camera.h
│   ├── Shader.h
│   ├── Model.h
│   ├── Texture.h
│   ├── stb_image.h
│   ├── miniaudio.h
│   ├── Shader/                     ← Shaders GLSL
│   │   ├── lighting.vs
│   │   ├── lighting.frag
│   │   ├── lamp.vs
│   │   ├── lamp.frag
│   │   ├── skybox.vs
│   │   └── skybox.frag
│   ├── Models/                     ← Modelos 3D (.obj)
│   ├── images/                     ← Texturas
│   ├── SOIL2/                      ← Librería de texturas
│   ├── glew32.dll
│   └── assimp-vc140-mt.dll
├── assimp-vc140-mt.dll
├── musica.mp3                      ← Audio ambiental
└── README.md
```

---

#### **Paso 3: Abrir el Proyecto en Visual Studio**

1. **Abre Visual Studio 2022**
2. Click en **"Abrir un proyecto o solución"**
3. Navega a la carpeta del proyecto
4. Selecciona **`ProyectoFinalGrafica.sln`**
5. Click en **"Abrir"**

---

#### **Paso 4: Configurar el Proyecto**

Una vez abierto el proyecto:

1. **Seleccionar Configuración:**
   - En la barra superior, selecciona:
     - **Configuración:** `Release` (o `Debug` para depuración)
     - **Plataforma:** `x64`

2. **Verificar Rutas de Inclusión** (normalmente ya están configuradas):
   - Click derecho en el proyecto → **Propiedades**
   - **C/C++** → **General** → **Directorios de inclusión adicionales**
   - Debe incluir rutas a GLEW, GLFW, GLM, Assimp, SOIL2

3. **Verificar Bibliotecas de Enlace:**
   - **Enlazador** → **Entrada** → **Dependencias adicionales**
   - Debe incluir: `opengl32.lib`, `glew32s.lib`, `glfw3.lib`, `assimp-vc140-mt.lib`

> 💡 **Tip:** Si usas la solución `.sln` incluida, estas configuraciones ya están establecidas.

---

#### **Paso 5: Compilar el Proyecto**

1. **Compilar:**
   - Presiona **`Ctrl + Shift + B`**
   - O click en **"Compilar" → "Compilar solución"**

2. **Verificar la compilación:**
   - La ventana de salida debe mostrar: `Compilación: 1 correctos, 0 incorrectos`
   - Si hay errores, ver [Solución de Problemas](#-solución-de-problemas)

---

#### **Paso 6: Ejecutar el Proyecto**

1. **Ejecutar sin depuración:**
   - Presiona **`Ctrl + F5`**
   - O click en **"Depurar" → "Iniciar sin depurar"**

2. **Ejecutar con depuración:**
   - Presiona **`F5`**
   - O click en **"Depurar" → "Iniciar depuración"**

3. **Resultado esperado:**
   - Se abre una ventana de 1280x720 píxeles
   - Se muestra el zoológico con skybox
   - Se reproduce música de fondo
   - Puedes moverte con WASD y mouse

---

## 🔧 Configuración Avanzada

### Cambiar la Resolución

Edita `Main.cpp` (línea ~147):
```cpp
const GLuint WIDTH = 1920, HEIGHT = 1080;  // Full HD
// O cualquier otra resolución deseada
```

### Ajustar el Audio

Para cambiar el volumen, edita `Main.cpp` (después de inicializar el audio):
```cpp
ma_engine_set_volume(&engine, 0.5f);  // 50% volumen (0.0 a 1.0)
```

Para usar otra música, reemplaza `musica.mp3` en el directorio raíz.

### Modificar la Velocidad de Cámara

Edita `Camera.h` (línea ~32):
```cpp
const GLfloat SPEED = 10.0f;  // Más rápido
const GLfloat SPEED = 3.0f;   // Más lento
```

---

## ⚠️ Solución de Problemas

### Error: "No se puede abrir el archivo glew32.lib"

**Solución:**
1. Verifica que GLEW esté correctamente referenciado
2. En propiedades del proyecto:
   - **Enlazador** → **General** → **Directorios de bibliotecas adicionales**
   - Agrega la ruta donde está `glew32.lib`

### Error: "No se encuentra assimp-vc140-mt.dll"

**Solución:**
1. Copia `assimp-vc140-mt.dll` al directorio de salida
2. O asegúrate de que está en el mismo directorio que el `.exe`

### La ventana se abre pero está en negro

**Solución:**
1. Verifica que tu GPU soporta OpenGL 3.3:
   ```bash
   # Descarga y ejecuta OpenGL Extensions Viewer
   https://www.realtech-vr.com/home/glview
   ```
2. Actualiza los drivers de tu tarjeta gráfica

### No se reproduce el audio

**Solución:**
1. Verifica que `musica.mp3` está en el directorio raíz del proyecto
2. Comprueba que tu dispositivo de audio está funcionando
3. El formato MP3 debe ser compatible (estándar MP3)

### Error de compilación: "cannot open source file"

**Solución:**
1. Verifica que todos los archivos `.h` estén en su lugar
2. Comprueba las rutas en **Propiedades** → **C/C++** → **General**

### Rendimiento bajo (FPS bajos)

**Solución:**
1. Compila en modo **Release** (no Debug)
2. Asegúrate de que estás usando la GPU dedicada (no integrada)
3. Reduce la resolución de ventana

---

## 🧪 Verificación de la Instalación

Para verificar que todo funciona correctamente:

✅ **Checklist de Verificación:**
- [ ] El proyecto compila sin errores
- [ ] La ventana se abre en 1280x720
- [ ] Se ve el skybox (cielo)
- [ ] Se ven los modelos 3D del zoológico
- [ ] Se reproduce música de fondo
- [ ] Puedes moverte con WASD
- [ ] Puedes rotar la cámara con el mouse
- [ ] Presionar TAB cambia entre cámaras
- [ ] Las animaciones de animales funcionan (teclas V, J, L, etc.)

---

## 📚 Siguientes Pasos

Ahora que tienes el proyecto funcionando:

1. Lee la [Guía de Controles](Controles.md) para aprender a usar el programa
2. Explora el [Sistema de Animación](Sistema-Animacion.md) para ver todas las animaciones
3. Revisa la [Estructura del Código](Codigo-Estructura.md) si quieres modificar el proyecto

---

## 🆘 Ayuda Adicional

Si sigues teniendo problemas:

1. Revisa la sección [Troubleshooting](Troubleshooting.md)
2. Consulta las [Preguntas Frecuentes (FAQ)](FAQ.md)
3. Abre un issue en el [repositorio de GitHub](https://github.com/C4rlos316/ProyectoFinalGrafica/issues)

---

**¡Disfruta explorando el Zoológico Virtual! 🦁🐘🦜**
