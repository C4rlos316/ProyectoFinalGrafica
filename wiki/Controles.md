# 🎮 Controles y Características

## 🕹️ Controles Generales

### Movimiento de Cámara

| Acción | Tecla(s) | Descripción |
|--------|----------|-------------|
| **Mover adelante** | `W` o `↑` | Avanza en la dirección de la cámara |
| **Mover atrás** | `S` o `↓` | Retrocede en la dirección opuesta |
| **Mover izquierda** | `A` o `←` | Se mueve lateralmente a la izquierda |
| **Mover derecha** | `D` o `→` | Se mueve lateralmente a la derecha |
| **Rotar cámara** | **Mouse** | Mueve el mouse para mirar alrededor |
| **Salir** | `ESC` | Cierra la aplicación |

> 💡 **Tip:** El movimiento es independiente de los FPS gracias al sistema `deltaTime`, garantizando una experiencia fluida.

---

### Sistema de Cámara

| Acción | Tecla | Descripción |
|--------|-------|-------------|
| **Cambiar vista** | `TAB` | Alterna entre primera y tercera persona |

#### 🎥 Cámara en Primera Persona
- Vista directa desde los "ojos" del jugador
- Mayor inmersión
- Ideal para exploración detallada

#### 👤 Cámara en Tercera Persona
- Muestra el modelo del personaje (León "Alex")
- Cámara posicionada 5 unidades detrás del jugador
- Mejor orientación espacial
- Vista más cinemática

---

### Control de Iluminación

| Acción | Tecla | Descripción |
|--------|-------|-------------|
| **Luz central animada** | `ESPACIO` | Activa/desactiva la luz del centro con efecto senoidal |

> La luz central se mueve en patrón circular cuando está activada.

---

## 🐾 Animaciones de Animales

### Hábitat: Acuario (Cuadrante +X, -Z)

| Animal | Tecla | Tipo de Animación | Comportamiento |
|--------|-------|-------------------|----------------|
| **🐧 Pingüino** | *Automática* | Caminata circular | Se mueve continuamente en círculo con balanceo |
| **🐢 Tortuga Marina** | `T` | Natación | Nada en trayectoria predefinida moviendo aletas |
| **🦦 Nutria** | `N` | Salto y nado | Salta de la roca al agua y nada |

**Ubicaciones:**
- Pingüino: Base en `(7.25, 0.5, -9.8)`
- Tortuga: Base en `(7.25, -0.45, -4.625)`
- Nutria: Roca en `(5.5, 0.3, -9.8)`

---

### Hábitat: Selva (Cuadrante +X, +Z)

| Animal | Tecla | Tipo de Animación | Comportamiento |
|--------|-------|-------------------|----------------|
| **🦫 Capibara** | `B` | Caminata | Camina tranquilamente con movimiento de patas |
| **🐵 Mono** | `M` | Columpio | Se balancea en ramas con brazos extendidos |
| **🦜 Guacamaya** | `O` | Vuelo | Aletea y vuela en círculo |

**Ubicaciones:**
- Capibara: `(7.25, 0.0, 7.25)`
- Mono: En árbol de la selva
- Guacamaya: Área aérea de la selva

---

### Hábitat: Sabana (Cuadrante -X, -Z)

| Animal | Tecla | Tipo de Animación | Comportamiento |
|--------|-------|-------------------|----------------|
| **🐘 Elefante** | `V` | Caminata pesada | Camina lentamente balanceando trompa |
| **🦒 Jirafa** | `J` | Caminata alta | Camina con pasos largos moviendo cuello |
| **🦓 Cebra** | `L` | Trote | Corre con movimiento de galope |

**Ubicaciones:**
- Elefante: `(-7.25, 0.0, -7.25)`
- Jirafa: Cerca del elefante
- Cebra: Zona de sabana

---

### Hábitat: Desierto (Cuadrante -X, +Z)

| Animal | Tecla | Tipo de Animación | Comportamiento |
|--------|-------|-------------------|----------------|
| **🐫 Camello** | `C` | Caminata desértica | Camina balanceando jorobas |
| **🦅 Cóndor** | `Z` | Vuelo majestuoso | Planea con alas extendidas |
| **🐢 Tortuga Terrestre** | `X` | Caminata lenta | Avanza lentamente con movimiento de patas |

**Ubicaciones:**
- Camello: `(-7.25, 0.0, 7.25)`
- Cóndor: Área aérea del desierto
- Tortuga: Zona del oasis

---

### Hábitat: Aviario (Centro)

| Animal | Tecla | Tipo de Animación | Comportamiento |
|--------|-------|-------------------|----------------|
| **🕊️ Ave Central** | *Automática* | Vuelo enjaulado | Vuela en círculos dentro del aviario de vidrio |

**Ubicación:**
- Centro del zoológico: `(0.0, 2.5, 0.0)`

---

## 🌟 Características Especiales

### Sistema de Iluminación Dinámica

El zoológico cuenta con **9 fuentes de luz**:

#### Luz Direccional (Sol)
- Simula la luz solar global
- Afecta a toda la escena
- Color: Blanco cálido

#### Luces Puntuales (7 luces)

| # | Ubicación | Posición | Color | Propósito |
|---|-----------|----------|-------|-----------|
| 0 | Centro | `(2.0, 0.2, 2.0)` | Blanco | Luz animada central |
| 1 | Entrada | `(4.1, 5.5, 12.5)` | Amarillo | Ilumina letrero |
| 2 | Desierto | `(-6.8, -0.5, 6.0)` | Naranja | Oasis |
| 3 | Sabana | `(-7.25, 2.0, -7.25)` | Amarillo cálido | Iluminación general |
| 4 | Acuario | `(11.2, 1.2, -9.0)` | Azul | Ambiente acuático |
| 5 | Aviario | `(0.0, 2.5, 0.0)` | Blanco | Luz interna |
| 6 | Selva | `(7.25, 1.5, 7.25)` | Verde | Ambiente selvático |

#### Spotlight (Linterna)
- Foco direccional desde la posición de la cámara
- Cono de luz ajustable
- Activada automáticamente

**Parámetros de Atenuación:**
- Constante: 1.0
- Lineal: 0.09
- Cuadrática: 0.032

---

### Skybox 360°

El cielo panorámico está compuesto por 6 texturas:
- `right.jpg` - Lado derecho
- `left.jpg` - Lado izquierdo
- `top.jpg` - Arriba
- `bottom.jpg` - Abajo
- `front.jpg` - Frente
- `back.jpg` - Atrás

> El skybox se renderiza siempre al fondo usando depth testing especial.

---

### Sistema de Audio

**Música Ambiental:**
- Archivo: `musica.mp3`
- Reproducción en bucle continuo
- Activación automática al iniciar
- Volumen ajustable en el código

**Implementación:**
- Motor de audio: **miniaudio**
- Sin latencia
- Formato soportado: MP3

---

## 🎯 Técnicas de Interacción

### Cómo Activar las Animaciones

1. **Presiona la tecla correspondiente** al animal que quieres animar
2. **Mantén presionada** o **presiona nuevamente** para continuar/detener la animación
3. Cada animación tiene un ciclo completo que se ejecuta

**Ejemplo: Animar al Elefante**
```
1. Ubica el hábitat de la Sabana (cuadrante -X, -Z)
2. Presiona la tecla 'V'
3. Observa cómo el elefante camina balanceando su trompa
4. Presiona 'V' nuevamente para detener
```

### Exploración Recomendada

**Recorrido Sugerido:**

1. **Inicio** - Posición `(0, 1, 21)`
   - Observa la entrada con el letrero
   - Presiona `ESPACIO` para ver la luz central animada

2. **Acuario** - Camina hacia `+X, -Z`
   - Observa el pingüino caminando automáticamente
   - Presiona `T` para activar la tortuga
   - Presiona `N` para que la nutria salte

3. **Selva** - Dirígete a `+X, +Z`
   - Presiona `B` para el capibara
   - Presiona `M` para el mono
   - Presiona `O` para la guacamaya

4. **Sabana** - Navega a `-X, -Z`
   - Presiona `V` para el elefante
   - Presiona `J` para la jirafa
   - Presiona `L` para la cebra

5. **Desierto** - Ve a `-X, +Z`
   - Presiona `C` para el camello
   - Presiona `Z` para el cóndor
   - Presiona `X` para la tortuga

6. **Aviario** - Centro `(0, 0, 0)`
   - Observa el ave volando automáticamente

---

## 🎨 Efectos Visuales

### Transparencias
- El aviario usa materiales con transparencia (vidrio)
- Blending habilitado: `GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA`

### Materiales Phong
Cada objeto tiene propiedades:
- **Ambient** - Luz ambiental base
- **Diffuse** - Luz difusa (texturas)
- **Specular** - Brillo especular
- **Shininess** - Intensidad del brillo (típicamente 32.0)

### Texturas Repetibles
Los pisos de cada hábitat usan texturas repetidas:
- Agua en el acuario
- Pasto en la selva y sabana
- Arena en el desierto
- Concreto en el centro

---

## ⌨️ Resumen de Teclas Rápidas

```
╔═══════════════════════════════════════════════════════╗
║           GUÍA RÁPIDA DE CONTROLES                    ║
╠═══════════════════════════════════════════════════════╣
║ MOVIMIENTO                                            ║
║  W/A/S/D o Flechas  → Mover cámara                   ║
║  Mouse              → Rotar vista                     ║
║  TAB                → Cambiar cámara (1ra/3ra)       ║
║  ESC                → Salir                           ║
║                                                       ║
║ ILUMINACIÓN                                           ║
║  ESPACIO            → Luz central animada            ║
║                                                       ║
║ ANIMACIONES - ACUARIO                                 ║
║  T                  → Tortuga marina                 ║
║  N                  → Nutria                         ║
║  (Pingüino automático)                               ║
║                                                       ║
║ ANIMACIONES - SELVA                                   ║
║  B                  → Capibara                       ║
║  M                  → Mono                           ║
║  O                  → Guacamaya                      ║
║                                                       ║
║ ANIMACIONES - SABANA                                  ║
║  V                  → Elefante                       ║
║  J                  → Jirafa                         ║
║  L                  → Cebra                          ║
║                                                       ║
║ ANIMACIONES - DESIERTO                                ║
║  C                  → Camello                        ║
║  Z                  → Cóndor                         ║
║  X                  → Tortuga terrestre              ║
║                                                       ║
║ (Ave del aviario es automática)                      ║
╚═══════════════════════════════════════════════════════╝
```

---

## 🔍 Tips y Trucos

### Para Mejor Experiencia Visual
1. Usa modo **tercera persona** (TAB) para vistas cinemáticas
2. Muévete **lentamente** para apreciar los detalles
3. Observa los **reflejos especulares** en los modelos
4. Prueba diferentes **ángulos de cámara** en cada hábitat

### Para Desarrolladores
1. El código de controles está en la función `DoMovement()` en `Main.cpp`
2. Los callbacks están en `KeyCallback()` y `MouseCallback()`
3. Cada animal tiene variables de estado (líneas 200-800 en Main.cpp)

### Rendimiento
1. Si experimentas lag, compila en modo **Release**
2. Reduce la resolución en `Main.cpp` (línea 147)
3. Cierra otras aplicaciones que usen la GPU

---

## 📚 Más Información

- [Sistema de Cámara](Sistema-Camara.md) - Detalles técnicos de las cámaras
- [Sistema de Animación](Sistema-Animacion.md) - Cómo funcionan las animaciones
- [Sistema de Iluminación](Sistema-Iluminacion.md) - Configuración de luces
- [FAQ](FAQ.md) - Preguntas frecuentes

---

**¡Explora y disfruta del Zoológico Virtual! 🎮🦁**
