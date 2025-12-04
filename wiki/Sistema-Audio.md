# 🎵 Sistema de Audio

## 🔊 Visión General

El Zoológico Virtual integra un sistema de audio ambiental utilizando la librería **miniaudio**, proporcionando música de fondo continua para mejorar la experiencia inmersiva.

---

## 🏗️ Arquitectura del Sistema

### Librería: miniaudio

**Características:**
- Header-only (no requiere compilación separada)
- Multiplataforma (Windows, Linux, macOS)
- Bajo nivel de latencia
- Soporte para múltiples formatos (MP3, WAV, FLAC, etc.)
- Ligera y eficiente

**Versión:** 0.11.x

**Integración:**
```cpp
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
```

> ⚠️ El `#define` debe estar ANTES del `#include` y solo en UN archivo .cpp

---

## 🎼 Implementación

### Variables Globales

```cpp
ma_engine engine;      // Motor de audio principal
ma_sound sound;        // Objeto de sonido
```

### Inicialización

```cpp
// En la función main(), después de inicializar GLFW/OpenGL

// 1. Inicializar motor de audio
ma_result result = ma_engine_init(NULL, &engine);
if (result != MA_SUCCESS) {
    std::cout << "Error al inicializar el motor de audio" << std::endl;
    return -1;
}

// 2. Cargar archivo de audio
result = ma_sound_init_from_file(
    &engine,              // Motor
    "musica.mp3",         // Ruta del archivo
    MA_SOUND_FLAG_STREAM, // Flags (streaming para archivos grandes)
    NULL,                 // Grupo (NULL = predeterminado)
    NULL,                 // Fence (NULL = sin sincronización)
    &sound                // Objeto de salida
);

if (result != MA_SUCCESS) {
    std::cout << "Error al cargar el archivo de audio" << std::endl;
    ma_engine_uninit(&engine);
    return -1;
}

// 3. Configurar reproducción en bucle
ma_sound_set_looping(&sound, MA_TRUE);

// 4. Iniciar reproducción
ma_sound_start(&sound);
```

---

## 🎛️ Configuración

### Reproducción en Bucle

```cpp
ma_sound_set_looping(&sound, MA_TRUE);   // Activar bucle
ma_sound_set_looping(&sound, MA_FALSE);  // Desactivar bucle
```

### Control de Volumen

```cpp
// Volumen del motor (afecta todos los sonidos)
ma_engine_set_volume(&engine, 0.5f);  // 50% volumen (0.0 - 1.0)

// Volumen de un sonido específico
ma_sound_set_volume(&sound, 0.7f);    // 70% volumen
```

**Valores típicos:**
- `0.0f` - Silencio
- `0.5f` - Volumen medio
- `1.0f` - Volumen máximo
- `>1.0f` - Amplificación (puede causar distorsión)

### Control de Reproducción

```cpp
// Iniciar reproducción
ma_sound_start(&sound);

// Pausar
ma_sound_stop(&sound);

// Reiniciar desde el principio
ma_sound_seek_to_pcm_frame(&sound, 0);
ma_sound_start(&sound);
```

### Verificar Estado

```cpp
// ¿Está reproduciendo?
ma_bool32 isPlaying = ma_sound_is_playing(&sound);

// ¿Ha terminado?
ma_bool32 isAtEnd = ma_sound_at_end(&sound);
```

---

## 📁 Archivo de Audio

### Archivo Actual

**Nombre:** `musica.mp3`  
**Ubicación:** Raíz del proyecto (mismo directorio que el .exe)  
**Formato:** MP3  
**Comportamiento:** Reproducción continua en bucle

### Cambiar la Música

Para usar otro archivo de audio:

1. **Reemplazar el archivo:**
   - Coloca tu archivo MP3 en la raíz del proyecto
   - Renómbralo a `musica.mp3`
   - O modifica el código para usar otro nombre

2. **Modificar el código:**
```cpp
// Cambiar la ruta del archivo
result = ma_sound_init_from_file(
    &engine,
    "tu_musica.mp3",      // ← Cambiar aquí
    MA_SOUND_FLAG_STREAM,
    NULL,
    NULL,
    &sound
);
```

### Formatos Soportados

miniaudio soporta los siguientes formatos:
- ✅ **MP3** (más común)
- ✅ **WAV** (sin compresión)
- ✅ **FLAC** (compresión sin pérdida)
- ✅ **Vorbis/OGG**

**Recomendación:** MP3 para balance entre calidad y tamaño.

---

## 🔧 Flags de Configuración

### MA_SOUND_FLAG_STREAM

```cpp
MA_SOUND_FLAG_STREAM  // Streaming (recomendado para archivos grandes)
```

**Ventajas:**
- Bajo uso de memoria
- Ideal para música de fondo
- Carga progresiva

**Desventajas:**
- Ligera latencia al iniciar (imperceptible en música)

### Alternativa: Cargar en Memoria

```cpp
// Para sonidos cortos (efectos de sonido)
ma_sound_init_from_file(&engine, "efecto.wav", 0, NULL, NULL, &sound);
```

**Ventajas:**
- Sin latencia
- Reproducción instantánea

**Desventajas:**
- Alto uso de memoria para archivos grandes

---

## 🎮 Integración en el Bucle del Juego

### Sin Código Adicional Necesario

Una vez iniciado, el motor de miniaudio maneja automáticamente:
- Thread separado para el audio
- Buffer de audio
- Sincronización con el hardware

```cpp
// En el bucle principal - NO es necesario código adicional
while (!glfwWindowShouldClose(window)) {
    // ... renderizado ...
    
    // El audio continúa automáticamente en segundo plano
    
    // ... swap buffers, poll events ...
}
```

---

## 🧹 Limpieza de Recursos

### Al Finalizar el Programa

```cpp
// Antes de terminar GLFW
ma_sound_uninit(&sound);    // Liberar sonido
ma_engine_uninit(&engine);  // Liberar motor

// Luego limpiar GLFW
glfwTerminate();
```

**Orden importante:**
1. Desinit sonidos
2. Desinit motor
3. Terminar GLFW

---

## 🎨 Características Avanzadas (No Implementadas)

### Efectos de Audio

miniaudio soporta efectos (para implementación futura):

```cpp
// Ejemplo: Fade in/out
for (float t = 0.0f; t < 1.0f; t += 0.01f) {
    ma_sound_set_volume(&sound, t);
    sleep(10);  // 10ms
}

// Ejemplo: Pitch shifting
ma_sound_set_pitch(&sound, 1.2f);  // Más agudo
ma_sound_set_pitch(&sound, 0.8f);  // Más grave
```

### Audio Espacial (3D)

```cpp
// Configurar audio posicional (para implementación futura)
ma_sound_set_position(&sound, x, y, z);
ma_engine_listener_set_position(&engine, 0, camX, camY, camZ);
```

**Uso potencial:**
- Sonidos de animales en sus posiciones
- Agua fluyendo en el acuario
- Pájaros cantando en la selva

### Múltiples Sonidos

```cpp
// Ejemplo: Sistema completo de audio
ma_sound musicaFondo;
ma_sound sonidoPinguin;
ma_sound sonidoAgua;
ma_sound sonidoSelva;

// Cargar todos
ma_sound_init_from_file(&engine, "musica.mp3", 
                        MA_SOUND_FLAG_STREAM, NULL, NULL, &musicaFondo);
ma_sound_init_from_file(&engine, "pinguino.wav", 
                        0, NULL, NULL, &sonidoPinguin);
// ...

// Reproducir según contexto
if (cameraEnAcuario) {
    ma_sound_start(&sonidoAgua);
}
```

---

## 🐛 Solución de Problemas

### El audio no se reproduce

**Posibles causas:**

1. **Archivo no encontrado**
   ```cpp
   // Verificar que musica.mp3 está en el directorio correcto
   // Usar ruta absoluta para debugging:
   result = ma_sound_init_from_file(&engine, 
       "C:/ruta/completa/musica.mp3", ...);
   ```

2. **Formato no soportado**
   - Verificar que el archivo MP3 es válido
   - Intentar con un WAV simple

3. **Dispositivo de audio no disponible**
   ```cpp
   // Verificar inicialización del motor
   if (result != MA_SUCCESS) {
       printf("Error code: %d\n", result);
   }
   ```

### Audio entrecortado o con clicks

**Soluciones:**

1. **Aumentar el buffer:**
```cpp
ma_engine_config config = ma_engine_config_init();
config.periodSizeInFrames = 1024;  // Aumentar buffer
ma_engine_init(&config, &engine);
```

2. **Usar streaming:**
```cpp
// Asegurar que se usa MA_SOUND_FLAG_STREAM
```

### Volumen muy bajo o muy alto

```cpp
// Ajustar volumen
ma_engine_set_volume(&engine, 0.3f);  // Más bajo
ma_engine_set_volume(&engine, 0.8f);  // Más alto
```

---

## 📊 Comparación de Alternativas

| Librería | Ventajas | Desventajas |
|----------|----------|-------------|
| **miniaudio** | Simple, header-only, multiplataforma | Menos features que alternativas grandes |
| **OpenAL** | Potente, audio 3D, estándar | Más complejo de configurar |
| **FMOD** | Profesional, muchas features | Licencia comercial costosa |
| **SDL_mixer** | Fácil, bien documentado | Requiere SDL como dependencia |
| **irrKlang** | Muy fácil de usar | Licencia comercial |

**Elección del proyecto:** miniaudio es ideal para este caso por:
- ✅ Simplicidad de integración
- ✅ No requiere DLLs adicionales
- ✅ Bajo overhead
- ✅ Suficiente para música de fondo

---

## 🎓 Conceptos Técnicos

### Streaming vs Loading

**Streaming:**
- Audio se lee del disco en tiempo real
- Bajo uso de memoria RAM
- Ideal para música larga

**Loading:**
- Todo el audio se carga en RAM
- Alto uso de memoria
- Reproducción instantánea sin latencia

### Sample Rate y Bit Depth

**Sample Rate típicos:**
- 44.1 kHz - Calidad CD
- 48 kHz - Estándar profesional

**Bit Depth:**
- 16-bit - Estándar
- 24-bit - Alta calidad

> miniaudio maneja automáticamente la conversión.

### Threading

miniaudio crea automáticamente:
- Thread de audio separado
- No bloquea el renderizado
- Sincronización automática con hardware de audio

---

## 🔗 Referencias

- [Documentación oficial de miniaudio](https://miniaud.io/)
- [GitHub de miniaudio](https://github.com/mackron/miniaudio)
- [Instalación](Instalacion.md) - Cómo compilar con miniaudio
- [Arquitectura](Arquitectura.md) - Integración en el sistema

---

## 📝 Código Completo de Ejemplo

```cpp
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

int main() {
    // ... Inicializar GLFW y OpenGL ...
    
    // Inicializar audio
    ma_engine engine;
    ma_sound sound;
    
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        return -1;
    }
    
    if (ma_sound_init_from_file(&engine, "musica.mp3", 
                                MA_SOUND_FLAG_STREAM, 
                                NULL, NULL, &sound) != MA_SUCCESS) {
        ma_engine_uninit(&engine);
        return -1;
    }
    
    ma_sound_set_looping(&sound, MA_TRUE);
    ma_engine_set_volume(&engine, 0.6f);  // 60% volumen
    ma_sound_start(&sound);
    
    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        // ... renderizado ...
    }
    
    // Limpieza
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
    glfwTerminate();
    
    return 0;
}
```

---

**¡El audio ambiental completa la inmersión! 🎵🔊**
