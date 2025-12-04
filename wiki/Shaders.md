# 🎨 Shaders y Renderizado

## 🖌️ Visión General

El sistema de renderizado del Zoológico Virtual utiliza **shaders programables** (GLSL) con OpenGL 3.3 Core Profile, implementando el modelo de iluminación Phong y técnicas modernas de renderizado.

---

## 📁 Shaders Implementados

### Conjunto de Shaders

| Shader | Propósito | Archivos |
|--------|-----------|----------|
| **lighting** | Iluminación Phong para objetos | `lighting.vs`, `lighting.frag` |
| **lamp** | Renderizar cubos de luz | `lamp.vs`, `lamp.frag` |
| **skybox** | Renderizar cielo 360° | `skybox.vs`, `skybox.frag` |

**Ubicación:** `/ProyectoFinalGrafica/Shader/`

---

## 💡 Shader de Iluminación (lighting)

### Vertex Shader (lighting.vs)

**Propósito:** Transformar vértices y preparar datos para el fragment shader.

```glsl
#version 330 core

// Atributos de entrada (desde VAO)
layout (location = 0) in vec3 aPos;       // Posición del vértice
layout (location = 1) in vec3 aNormal;    // Normal del vértice
layout (location = 2) in vec2 aTexCoords; // Coordenadas de textura

// Salidas al fragment shader
out vec3 FragPos;      // Posición del fragmento en espacio mundo
out vec3 Normal;       // Normal transformada
out vec2 TexCoords;    // Coordenadas de textura

// Matrices de transformación
uniform mat4 model;        // Modelo (posición, rotación, escala del objeto)
uniform mat4 view;         // Vista (posición de la cámara)
uniform mat4 projection;   // Proyección (perspectiva)

void main() {
    // Posición final en espacio de clip
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Posición en espacio mundo (para cálculos de luz)
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transformar normal (inversa transpuesta para no-uniform scaling)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pasar coordenadas de textura
    TexCoords = aTexCoords;
}
```

**Transformaciones aplicadas:**
1. **Model:** Objeto local → Mundo
2. **View:** Mundo → Espacio de cámara
3. **Projection:** Cámara → Espacio de clip (NDC)

---

### Fragment Shader (lighting.frag)

**Propósito:** Calcular el color final de cada píxel usando iluminación Phong.

```glsl
#version 330 core

// Salida final
out vec4 FragColor;

// Entradas del vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Estructuras de datos

// Material del objeto
struct Material {
    sampler2D diffuse;     // Textura de color
    sampler2D specular;    // Mapa especular (brillo)
    float shininess;       // Exponente de brillo
};

// Luz direccional (sol)
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Luz puntual
struct PointLight {
    vec3 position;
    
    // Atenuación
    float constant;
    float linear;
    float quadratic;
    
    // Colores
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Spotlight (linterna)
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Uniforms
uniform vec3 viewPos;              // Posición de la cámara
uniform DirLight dirLight;         // Luz direccional
uniform PointLight pointLights[7]; // 7 luces puntuales
uniform SpotLight spotLight;       // Spotlight
uniform Material material;         // Material del objeto

// Prototipos de funciones
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    // Normalizar vectores
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Fase 1: Luz direccional
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    // Fase 2: Luces puntuales (7)
    for(int i = 0; i < 7; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    
    // Fase 3: Spotlight
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    
    // Color final
    FragColor = vec4(result, 1.0);
}

// ============================================
// FUNCIONES DE CÁLCULO DE LUZ
// ============================================

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    
    // Componente difusa
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Componente especular (Phong)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Combinar
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Atenuación
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // Combinar
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Atenuación
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // Intensidad del spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    // Combinar
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}
```

**Características:**
- ✅ Modelo Phong completo (ambient + diffuse + specular)
- ✅ Múltiples fuentes de luz (1 direccional + 7 puntuales + 1 spotlight)
- ✅ Atenuación realista de luz
- ✅ Mapas de textura y especular

---

## 🔦 Shader de Lámparas (lamp)

### Vertex Shader (lamp.vs)

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
```

### Fragment Shader (lamp.frag)

```glsl
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0); // Blanco puro (emisivo)
}
```

**Propósito:** Renderizar cubos de luz sin iluminación (emisivos).

---

## 🌌 Shader de Skybox (skybox)

### Vertex Shader (skybox.vs)

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = aPos;
    
    // Remover traslación de la view matrix
    mat4 viewNoTranslation = mat4(mat3(view));
    
    vec4 pos = projection * viewNoTranslation * vec4(aPos, 1.0);
    
    // Truco: z = w para que siempre esté en el fondo
    gl_Position = pos.xyww;
}
```

**Truco del Skybox:**
- Remover traslación de la view matrix → Skybox siempre centrado en cámara
- `gl_Position.z = w` → Depth test siempre falla (siempre al fondo)

---

### Fragment Shader (skybox.frag)

```glsl
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main() {
    FragColor = texture(skybox, TexCoords);
}
```

**Cubemap Texture:**
- Usa `samplerCube` (6 texturas en forma de cubo)
- Coordenadas de textura = vector dirección 3D

---

## 🛠️ Clase Shader (Shader.h)

### Funcionalidad

```cpp
class Shader {
public:
    GLuint Program;  // ID del programa shader
    
    // Constructor: compila y vincula shaders
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    
    // Usar el shader
    void Use() {
        glUseProgram(Program);
    }
};
```

### Compilación de Shaders

```cpp
// 1. Leer archivos de shader
std::ifstream vShaderFile(vertexPath);
std::ifstream fShaderFile(fragmentPath);
std::string vertexCode = /* leer archivo */;
std::string fragmentCode = /* leer archivo */;

// 2. Compilar vertex shader
GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertex, 1, &vShaderCode, NULL);
glCompileShader(vertex);
// Verificar errores...

// 3. Compilar fragment shader
GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragment, 1, &fShaderCode, NULL);
glCompileShader(fragment);
// Verificar errores...

// 4. Vincular programa
Program = glCreateProgram();
glAttachShader(Program, vertex);
glAttachShader(Program, fragment);
glLinkProgram(Program);
// Verificar errores...

// 5. Limpiar
glDeleteShader(vertex);
glDeleteShader(fragment);
```

---

## 🎨 Pipeline de Renderizado

### Orden de Renderizado

```cpp
// 1. Limpiar buffers
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 2. Activar shader de iluminación
lightingShader.Use();

// 3. Configurar uniforms (matrices, luces)
glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
// ... configurar luces ...

// 4. Renderizar objetos
for (cada objeto) {
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    objeto.Draw(lightingShader);
}

// 5. Renderizar luces (sin iluminación)
lampShader.Use();
for (cada luz) {
    // ... renderizar cubo de luz ...
}

// 6. Renderizar skybox (al final, sin depth write)
glDepthFunc(GL_LEQUAL);  // Cambiar depth test
skyboxShader.Use();
// ... renderizar skybox ...
glDepthFunc(GL_LESS);    // Restaurar depth test
```

---

## 🔧 Configuración de Uniforms

### Enviar Datos al Shader

```cpp
// Obtener location del uniform
GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");

// Enviar matriz
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

// Enviar vec3
glUniform3f(posLoc, x, y, z);
// O con glm::vec3
glUniform3fv(posLoc, 1, glm::value_ptr(position));

// Enviar float
glUniform1f(shineLoc, 32.0f);

// Enviar int (texture unit)
glUniform1i(texLoc, 0);  // Texture unit 0
```

---

## 🖼️ Sistema de Texturas

### Bindeo de Texturas

```cpp
// Activar unidad de textura
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, diffuseMap);

glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, specularMap);

// En el shader
glUniform1i(glGetUniformLocation(shader.Program, "material.diffuse"), 0);
glUniform1i(glGetUniformLocation(shader.Program, "material.specular"), 1);
```

### Cubemap (Skybox)

```cpp
// Cargar 6 texturas en un cubemap
GLuint cubemapTexture;
glGenTextures(1, &cubemapTexture);
glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

std::vector<std::string> faces {
    "right.jpg",  "left.jpg",
    "top.jpg",    "bottom.jpg",
    "front.jpg",  "back.jpg"
};

for (GLuint i = 0; i < faces.size(); i++) {
    unsigned char* image = SOIL_load_image(faces[i].c_str(), ...);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
}
```

---

## ⚙️ Optimizaciones

### Frustum Culling
❌ No implementado - Posible mejora futura

### Instanced Rendering
❌ No implementado - Para objetos repetidos (árboles, etc.)

### Level of Detail (LOD)
❌ No implementado - Modelos más simples a distancia

### Batching
✅ Parcialmente - Modelos se renderizan por tipo

---

## 🐛 Debugging de Shaders

### Verificar Errores de Compilación

```cpp
GLint success;
GLchar infoLog[512];

glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
}
```

### Verificar Errores de Linkado

```cpp
glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
}
```

---

## 🎓 Conceptos Técnicos

### Espacio de Coordenadas

1. **Local Space** - Coordenadas del modelo
2. **World Space** - Posición en el mundo (model matrix)
3. **View Space** - Relativo a la cámara (view matrix)
4. **Clip Space** - Después de proyección (projection matrix)
5. **Screen Space** - Píxeles en pantalla

### Normal Matrix

```cpp
mat3(transpose(inverse(model)))
```

**Razón:** Las normales no se transforman igual que las posiciones cuando hay non-uniform scaling.

### Depth Testing

```cpp
glEnable(GL_DEPTH_TEST);  // Activar
glDepthFunc(GL_LESS);     // Pasar si z < depth buffer
```

### Blending (Transparencias)

```cpp
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```

---

## 🔗 Referencias

- [Sistema de Iluminación](Sistema-Iluminacion.md) - Configuración de luces
- [Modelos 3D](Modelos-3D.md) - Carga y renderizado de modelos
- [Arquitectura](Arquitectura.md) - Pipeline de renderizado

---

**¡Los shaders dan vida visual al mundo 3D! 🎨✨**
