# 🎨 Modelos 3D y Assets

## 📦 Catálogo de Recursos

El Zoológico Virtual contiene **más de 80 modelos 3D** organizados por hábitat, junto con texturas y recursos visuales.

---

## 🗂️ Estructura de Directorios

```
ProyectoFinalGrafica/
├── Models/                    ← Modelos 3D (.obj)
│   ├── Acuario/
│   ├── Aviario/
│   ├── Selva/
│   ├── Sabana/
│   ├── Desierto/
│   ├── adornos/
│   ├── alex_leon/            ← Personaje 3ra persona
│   ├── arbol*/               ← Varios tipos de árboles
│   ├── cactus/
│   ├── [animales]/           ← Cada animal tiene su carpeta
│   └── ...
│
├── images/                    ← Texturas
│   ├── right.jpg             ← Skybox (6 caras)
│   ├── left.jpg
│   ├── top.jpg
│   ├── bottom.jpg
│   ├── front.jpg
│   ├── back.jpg
│   └── [texturas de pisos]
│
└── Shader/                    ← Shaders GLSL
    ├── lighting.vs/frag
    ├── lamp.vs/frag
    └── skybox.vs/frag
```

---

## 🦁 Modelos de Animales

### Hábitat: Acuario

| Animal | Carpeta | Partes | Escala | Descripción |
|--------|---------|--------|--------|-------------|
| **Pingüino** | `/Models/pinguino/` | Cuerpo, patas, aletas | 0.4 | Pingüino emperador con textura realista |
| **Tortuga Marina** | `/Models/tortuga_marina/` | Caparazón, cabeza, aletas (4) | 0.01 | Tortuga verde con detalles de caparazón |
| **Nutria** | `/Models/nutria/` | Cuerpo, patas (4), cola | 0.3 | Nutria de río con pelaje marrón |

### Hábitat: Selva

| Animal | Carpeta | Partes | Escala | Descripción |
|--------|---------|--------|--------|-------------|
| **Capibara** | `/Models/capibara/` | Cuerpo, patas (4), cabeza | 0.5 | El roedor más grande del mundo |
| **Mono** | `/Models/mono/` | Cuerpo, brazos (2), piernas (2), cola | 0.4 | Mono araña con cola prensil |
| **Guacamaya** | `/Models/guacamaya/` | Cuerpo, alas (2), cola | 0.3 | Guacamaya roja y azul |

### Hábitat: Sabana

| Animal | Carpeta | Partes | Escala | Descripción |
|--------|---------|--------|--------|-------------|
| **Elefante** | `/Models/elefante/` | Cuerpo, trompa, orejas (2), patas (4) | 1.0 | Elefante africano con colmillos |
| **Jirafa** | `/Models/jirafa/` | Cuerpo, cuello, cabeza, patas (4) | 1.2 | Jirafa con patrón de manchas |
| **Cebra** | `/Models/cebra/` | Cuerpo, patas (4), cola | 0.8 | Cebra con rayas blancas y negras |

### Hábitat: Desierto

| Animal | Carpeta | Partes | Escala | Descripción |
|--------|---------|--------|--------|-------------|
| **Camello** | `/Models/camello/` | Cuerpo, jorobas, cuello, patas (4) | 1.0 | Camello dromedario (1 joroba) |
| **Cóndor** | `/Models/condor/` | Cuerpo, alas (2), cola | 0.6 | Cóndor andino con envergadura grande |
| **Tortuga Terrestre** | `/Models/tortuga_terrestre/` | Caparazón, cabeza, patas (4) | 0.05 | Tortuga del desierto |

### Hábitat: Aviario

| Animal | Carpeta | Partes | Escala | Descripción |
|--------|---------|--------|--------|-------------|
| **Ave Central** | `/Models/aveSelva/` | Cuerpo, alas (2) | 0.3 | Ave tropical colorida |

---

## 🎭 Modelo del Personaje

### León "Alex"

**Carpeta:** `/Models/alex_leon/`  
**Escala:** 0.5  
**Uso:** Personaje visible en modo tercera persona

**Características:**
- Modelo de león de estilo cartoon
- Animación de idle (estático)
- Múltiples partes articuladas
- Textura con colores vibrantes

**Renderizado:**
```cpp
if (camera.cameraType == THIRD_PERSON) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, camera.position);
    model = glm::rotate(model, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    modelAlex.Draw(lightingShader);
}
```

---

## 🌳 Modelos de Vegetación

### Árboles

| Tipo | Carpeta | Cantidad | Ubicación |
|------|---------|----------|-----------|
| **Árbol de Sabana** | `/Models/arbolSabana/` | 3-4 | Hábitat de sabana |
| **Árbol de Selva** | `/Models/arbolSelva/` | 5-6 | Hábitat de selva |
| **Árbol con Ramas** | `/Models/arbolRama/` | 2-3 | Varios hábitats |

### Plantas del Desierto

| Tipo | Carpeta | Cantidad | Descripción |
|------|---------|----------|-------------|
| **Cactus** | `/Models/cactus/` | 4-5 | Cactus Saguaro estilo |

---

## 🏗️ Estructuras y Decoración

### Estructuras Principales

| Estructura | Carpeta | Escala | Descripción |
|------------|---------|--------|-------------|
| **Aviario** | `/Models/Aviario/` | 2.0 | Jaula de vidrio central |
| **Acuario** | `/Models/Acuario/` | Varios | Tanque de agua, iglú |
| **Carrusel** | `/Models/carrusel/` | 1.0 | Carrusel decorativo |
| **CDMX** | `/Models/cdmx/` | Variable | Elementos de Ciudad de México |

### Adornos

**Carpeta:** `/Models/adornos/`

- Rocas
- Plantas decorativas
- Elementos de escenografía
- Señalética

---

## 🖼️ Texturas

### Skybox (Cielo 360°)

**Ubicación:** `/images/`

| Archivo | Cara del Cubo | Descripción |
|---------|---------------|-------------|
| `right.jpg` | +X | Lado derecho |
| `left.jpg` | -X | Lado izquierdo |
| `top.jpg` | +Y | Arriba (cielo) |
| `bottom.jpg` | -Y | Abajo (tierra) |
| `front.jpg` | +Z | Frente |
| `back.jpg` | -Z | Atrás |

**Tipo:** Cubemap (6 texturas formando un cubo)  
**Resolución:** 1024x1024 por cara (recomendado)  
**Formato:** JPG

---

### Texturas de Pisos

| Hábitat | Textura | Repetición | Descripción |
|---------|---------|------------|-------------|
| **Acuario** | `agua.jpg` | 8x8 | Textura de agua azul |
| **Selva** | `pasto_selva.jpg` | 6x6 | Pasto verde oscuro |
| **Sabana** | `pasto_sabana.jpg` | 6x6 | Pasto amarillento |
| **Desierto** | `arena.jpg` | 6x6 | Arena del desierto |
| **Centro** | `concreto.jpg` | 4x4 | Piso de concreto |

**Configuración de Repetición:**
```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
```

---

## 📐 Formato de Modelos

### Formato .obj (Wavefront)

**Características:**
- Formato de texto plano
- Ampliamente soportado
- Incluye vértices, normales, UVs
- Archivo .mtl para materiales

**Ejemplo de archivo .obj:**
```obj
# Vértices
v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 0.0 1.0 0.0

# Normales
vn 0.0 0.0 1.0

# Coordenadas de textura
vt 0.0 0.0
vt 1.0 0.0
vt 0.0 1.0

# Caras (triángulos)
f 1/1/1 2/2/1 3/3/1
```

---

## 🔧 Carga de Modelos

### Clase Model (Model.h)

```cpp
class Model {
public:
    // Constructor
    Model(GLchar* path) {
        this->loadModel(path);
    }
    
    // Dibujar modelo
    void Draw(Shader shader) {
        for (Mesh mesh : meshes) {
            mesh.Draw(shader);
        }
    }
    
private:
    vector<Mesh> meshes;
    string directory;
    
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};
```

### Uso en el Código

```cpp
// Cargar modelo
Model modelElefante((GLchar*)"Models/elefante/elefante.obj");

// En el bucle de renderizado
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, elefantePos);
model = glm::rotate(model, glm::radians(elefanteRotY), glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(1.0f));

glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
modelElefante.Draw(lightingShader);
```

---

## 🎨 Sistema de Materiales

### Archivo .mtl (Material)

Acompaña a cada .obj:

```mtl
newmtl Material
Ka 0.2 0.2 0.2        # Ambient color
Kd 0.8 0.8 0.8        # Diffuse color
Ks 1.0 1.0 1.0        # Specular color
Ns 32.0               # Shininess
map_Kd texture.jpg    # Diffuse texture
map_Ks specular.jpg   # Specular map
```

### Procesamiento con Assimp

Assimp carga automáticamente:
- ✅ Geometría (vértices, índices)
- ✅ Normales
- ✅ Coordenadas UV
- ✅ Materiales
- ✅ Texturas referenciadas

---

## 🔍 Optimización de Modelos

### Conteo de Polígonos

| Tipo de Modelo | Polígonos | Recomendación |
|----------------|-----------|---------------|
| **Animales** | 2,000-8,000 | Detalle medio |
| **Árboles** | 500-2,000 | Bajo detalle |
| **Estructuras** | 1,000-5,000 | Detalle variable |
| **Adornos** | 100-500 | Muy bajo |

**Total estimado:** ~150,000-300,000 polígonos en escena

### Técnicas de Optimización

1. **Level of Detail (LOD)**
   - ❌ No implementado
   - Posible mejora: Modelos más simples a distancia

2. **Frustum Culling**
   - ❌ No implementado
   - Posible mejora: No renderizar objetos fuera de vista

3. **Occlusion Culling**
   - ❌ No implementado
   - Posible mejora: No renderizar objetos ocluidos

---

## 🛠️ Herramientas Recomendadas

### Modelado 3D

| Software | Tipo | Recomendado para |
|----------|------|------------------|
| **Blender** | Gratuito | Modelado general, exportación .obj |
| **3ds Max** | Comercial | Modelado profesional |
| **Maya** | Comercial | Animación avanzada |
| **SketchUp** | Freemium | Modelado arquitectónico |

### Edición de Texturas

| Software | Tipo | Uso |
|----------|------|-----|
| **GIMP** | Gratuito | Edición de imágenes |
| **Photoshop** | Comercial | Edición profesional |
| **Paint.NET** | Gratuito | Edición básica |

### Visualización de Modelos

| Software | Propósito |
|----------|-----------|
| **MeshLab** | Ver y analizar .obj |
| **Blender** | Vista previa completa |

---

## 📊 Estadísticas de Recursos

### Resumen de Assets

| Tipo | Cantidad | Tamaño Aprox. |
|------|----------|---------------|
| **Modelos .obj** | 80+ | ~50 MB |
| **Texturas** | 15+ | ~20 MB |
| **Shaders** | 6 archivos | <10 KB |
| **Audio** | 1 archivo | ~5 MB |
| **Total** | ~100 archivos | ~75 MB |

---

## 🎯 Agregar Nuevos Modelos

### Paso a Paso

1. **Preparar el modelo en Blender:**
   - Asegurar escala apropiada
   - Triangular mallas (Modifier → Triangulate)
   - Aplicar texturas
   - UV unwrap correcto

2. **Exportar a .obj:**
   - File → Export → Wavefront (.obj)
   - Opciones:
     - ✅ Include Normals
     - ✅ Include UVs
     - ✅ Write Materials
     - ✅ Triangulate Faces

3. **Organizar archivos:**
   ```
   Models/
   └── nuevo_animal/
       ├── modelo.obj
       ├── modelo.mtl
       └── textura.jpg
   ```

4. **Cargar en el código:**
   ```cpp
   Model modelNuevo((GLchar*)"Models/nuevo_animal/modelo.obj");
   ```

5. **Renderizar:**
   ```cpp
   glm::mat4 model = glm::mat4(1.0f);
   model = glm::translate(model, posicion);
   model = glm::scale(model, glm::vec3(escala));
   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
   modelNuevo.Draw(lightingShader);
   ```

---

## 🎨 Buenas Prácticas

### Organización

- ✅ Un directorio por modelo
- ✅ Nombres descriptivos
- ✅ Texturas en el mismo directorio que el .obj
- ✅ Escala consistente (1 unidad = 1 metro)

### Optimización

- ✅ Reducir polígonos innecesarios
- ✅ Usar texturas de tamaño apropiado (1024x1024 o 2048x2048)
- ✅ Compartir texturas cuando sea posible
- ✅ Comprimir texturas (JPG para color, PNG para alpha)

### Compatibilidad

- ✅ Triangular todas las mallas (no quads o n-gons)
- ✅ Calcular normales
- ✅ UV unwrap apropiado
- ✅ Probar en MeshLab antes de integrar

---

## 🔗 Referencias

- [Sistema de Animación](Sistema-Animacion.md) - Cómo animar modelos
- [Shaders](Shaders.md) - Renderizado de modelos
- [Troubleshooting](Troubleshooting.md) - Problemas con modelos

---

## 🎨 Galería de Modelos

### Hábitats

```
🌊 ACUARIO
├─ Pingüino (animado)
├─ Tortuga marina (animada)
├─ Nutria (animada)
└─ Elementos: Iglú, rocas, agua

🌴 SELVA
├─ Capibara (animado)
├─ Mono (animado)
├─ Guacamaya (animada)
└─ Elementos: Árboles tropicales, plantas

🌾 SABANA
├─ Elefante (animado)
├─ Jirafa (animada)
├─ Cebra (animada)
└─ Elementos: Árboles de acacia, pasto

🏜️ DESIERTO
├─ Camello (animado)
├─ Cóndor (animado)
├─ Tortuga terrestre (animada)
└─ Elementos: Cactus, rocas, oasis

🏛️ AVIARIO
├─ Ave central (animada)
└─ Elementos: Estructura de vidrio

🎡 CENTRO
├─ León Alex (personaje)
├─ Carrusel
└─ Elementos decorativos
```

---

**¡Los modelos 3D dan forma al mundo virtual! 🎨🦁**
