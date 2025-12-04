# 💡 Sistema de Iluminación

## 🌟 Visión General

El Zoológico Virtual implementa un sistema de iluminación completo basado en el **modelo de reflexión Phong**, con múltiples tipos de fuentes de luz para crear una atmósfera realista e inmersiva.

---

## 🔦 Tipos de Luz Implementados

### 1. Luz Direccional (Sol)

Simula la luz solar global que ilumina toda la escena.

**Características:**
- No tiene posición, solo dirección
- Afecta a todos los objetos por igual
- Simula luz infinitamente distante

**Configuración:**
```cpp
// Dirección del sol (hacia abajo y ligeramente lateral)
glm::vec3 dirLight_direction(-0.2f, -1.0f, -0.3f);

// Colores
glm::vec3 dirLight_ambient(0.3f, 0.3f, 0.3f);    // Luz ambiental suave
glm::vec3 dirLight_diffuse(0.8f, 0.8f, 0.8f);    // Luz difusa principal
glm::vec3 dirLight_specular(1.0f, 1.0f, 1.0f);   // Brillo especular
```

**En el Fragment Shader:**
```glsl
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    
    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Combinar
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    return (ambient + diffuse + specular);
}
```

---

### 2. Luces Puntuales (Point Lights)

7 luces puntuales distribuidas estratégicamente en el zoológico.

**Características:**
- Emiten luz en todas direcciones desde un punto
- Tienen atenuación (se debilitan con la distancia)
- Cada una puede tener color diferente

#### 📍 Posiciones y Propósitos

| ID | Nombre | Posición | Color | Atenuación | Propósito |
|----|--------|----------|-------|------------|-----------|
| 0 | **Luz Central** | (2.0, 0.2, 2.0) | Blanco | Media | Iluminación general, animable |
| 1 | **Entrada** | (4.1, 5.5, 12.5) | Amarillo cálido | Baja | Ilumina letrero de entrada |
| 2 | **Desierto (Oasis)** | (-6.8, -0.5, 6.0) | Naranja | Media | Ambiente cálido desértico |
| 3 | **Sabana** | (-7.25, 2.0, -7.25) | Amarillo dorado | Media | Simula sol de sabana |
| 4 | **Acuario (Iglú)** | (11.2, 1.2, -9.0) | Azul | Media | Ambiente acuático |
| 5 | **Aviario** | (0.0, 2.5, 0.0) | Blanco | Alta | Luz interna del aviario |
| 6 | **Selva** | (7.25, 1.5, 7.25) | Verde | Media | Ambiente selvático |

#### Configuración de Atenuación

```cpp
// Parámetros de atenuación (fórmula: 1.0 / (constant + linear*d + quadratic*d²))
float constant = 1.0f;      // Factor constante
float linear = 0.09f;       // Factor lineal
float quadratic = 0.032f;   // Factor cuadrático
```

**Distancias Efectivas:**
- Radio de 50 unidades: Luz suave y extendida
- Radio de 32 unidades: Luz moderada
- Radio de 20 unidades: Luz concentrada

#### En el Fragment Shader:

```glsl
struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}
```

---

### 3. Spotlight (Linterna)

Una luz focal que sigue la posición de la cámara.

**Características:**
- Emite luz en forma de cono
- Sigue la posición y dirección de la cámara
- Ideal para efecto de linterna

**Configuración:**
```cpp
// Posición y dirección (actualizadas cada frame)
glm::vec3 spotLight_position = camera.position;
glm::vec3 spotLight_direction = camera.front;

// Ángulos del cono
float cutOff = cos(glm::radians(12.5f));        // Ángulo interno
float outerCutOff = cos(glm::radians(17.5f));   // Ángulo externo (suavizado)

// Colores
glm::vec3 spotLight_ambient(0.0f, 0.0f, 0.0f);    // Sin ambiental
glm::vec3 spotLight_diffuse(1.0f, 1.0f, 1.0f);    // Luz blanca intensa
glm::vec3 spotLight_specular(1.0f, 1.0f, 1.0f);   // Brillo completo

// Atenuación (más fuerte que las point lights)
float constant = 1.0f;
float linear = 0.09f;
float quadratic = 0.032f;
```

**En el Fragment Shader:**
```glsl
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

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Intensidad del spotlight (suavizado en bordes)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    // Cálculos estándar
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Atenuación
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // Combinar con intensidad del spotlight
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}
```

---

## 🎨 Modelo de Iluminación Phong

### Componentes del Modelo

El modelo Phong descompone la luz en tres componentes:

#### 1. **Ambient (Ambiental)**
- Luz constante que ilumina todo por igual
- Simula luz reflejada del entorno
- No depende de la posición de la cámara

```glsl
vec3 ambient = light.ambient * material.ambient;
```

#### 2. **Diffuse (Difusa)**
- Luz que se dispersa en todas direcciones
- Depende del ángulo entre la normal y la luz
- Da la apariencia de "mate"

```glsl
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = light.diffuse * (diff * material.diffuse);
```

#### 3. **Specular (Especular)**
- Brillo/reflejo de la luz
- Depende del ángulo de vista
- Crea puntos brillantes

```glsl
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec3 specular = light.specular * (spec * material.specular);
```

### Fórmula Final

```glsl
vec3 result = ambient + diffuse + specular;
```

---

## 🎭 Materiales

Cada objeto tiene propiedades de material que definen cómo interactúa con la luz.

### Estructura de Material

```glsl
struct Material {
    sampler2D diffuse;      // Textura de color
    sampler2D specular;     // Mapa especular (brillo)
    float shininess;        // Intensidad del brillo (1-256)
};
```

### Valores Típicos de Shininess

| Material | Shininess | Apariencia |
|----------|-----------|------------|
| Plástico mate | 8.0 | Brillo mínimo |
| Plástico | 32.0 | Brillo moderado |
| Metal pulido | 128.0 | Muy brillante |
| Espejo | 256.0 | Reflejo perfecto |

**En el proyecto:**
```cpp
// Configurar material en el shader
GLint matShineLoc = glGetUniformLocation(lightingShader.Program, "material.shininess");
glUniform1f(matShineLoc, 32.0f);  // Brillo estándar
```

---

## ✨ Luz Central Animada

La luz en el centro del zoológico tiene una animación especial.

### Activación
- **Tecla:** ESPACIO
- **Comportamiento:** Movimiento circular con variación de altura

### Código de Animación

```cpp
if (active) {
    // Movimiento circular
    float radius = 2.0f;
    float speed = 1.0f;
    float t = glfwGetTime() * speed;
    
    pointLightPositions[0].x = 2.0f + cos(t) * radius;
    pointLightPositions[0].z = 2.0f + sin(t) * radius;
    
    // Variación de altura (onda senoidal)
    pointLightPositions[0].y = 0.2f + sin(t * 2.0f) * 0.5f;
}
```

### Visualización

Se renderiza un cubo blanco en la posición de cada luz para visualizarlas:

```cpp
// Shader sin iluminación para las lámparas
Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

// Renderizar cada luz
for (GLuint i = 0; i < 7; i++) {
    model = glm::mat4(1.0f);
    model = glm::translate(model, pointLightPositions[i]);
    model = glm::scale(model, glm::vec3(0.2f));  // Cubos pequeños
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
```

---

## 🌈 Colores por Hábitat

Cada hábitat tiene una paleta de color específica en su iluminación:

### Acuario (Azul)
```cpp
ambient:  vec3(0.0, 0.1, 0.2)   // Azul oscuro
diffuse:  vec3(0.2, 0.4, 0.8)   // Azul brillante
specular: vec3(0.8, 0.8, 1.0)   // Brillo azulado
```

### Selva (Verde)
```cpp
ambient:  vec3(0.1, 0.2, 0.1)   // Verde oscuro
diffuse:  vec3(0.3, 0.6, 0.3)   // Verde vivo
specular: vec3(0.5, 1.0, 0.5)   // Brillo verdoso
```

### Desierto (Naranja)
```cpp
ambient:  vec3(0.2, 0.15, 0.0)  // Naranja oscuro
diffuse:  vec3(0.8, 0.5, 0.2)   // Naranja cálido
specular: vec3(1.0, 0.8, 0.5)   // Brillo cálido
```

### Sabana (Amarillo)
```cpp
ambient:  vec3(0.2, 0.2, 0.1)   // Amarillo oscuro
diffuse:  vec3(0.8, 0.7, 0.3)   // Amarillo dorado
specular: vec3(1.0, 1.0, 0.7)   // Brillo dorado
```

---

## 🔧 Configuración en el Código

### Pasar Luces al Shader

```cpp
// Luz direccional
glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 
            -0.2f, -1.0f, -0.3f);
glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 
            0.3f, 0.3f, 0.3f);
// ... más uniforms

// Luces puntuales (en bucle)
for (GLuint i = 0; i < 7; i++) {
    std::string number = std::to_string(i);
    
    glUniform3f(glGetUniformLocation(lightingShader.Program, 
                ("pointLights[" + number + "].position").c_str()), 
                pointLightPositions[i].x, 
                pointLightPositions[i].y, 
                pointLightPositions[i].z);
    
    glUniform3f(glGetUniformLocation(lightingShader.Program, 
                ("pointLights[" + number + "].ambient").c_str()), 
                0.05f, 0.05f, 0.05f);
    // ... más propiedades
}

// Spotlight
glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 
            camera.position.x, camera.position.y, camera.position.z);
glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 
            camera.front.x, camera.front.y, camera.front.z);
```

---

## 📊 Resumen del Sistema

| Tipo de Luz | Cantidad | Uso Principal |
|-------------|----------|---------------|
| Direccional | 1 | Iluminación global (sol) |
| Puntual | 7 | Iluminación local por hábitat |
| Spotlight | 1 | Linterna de la cámara |
| **TOTAL** | **9** | Iluminación completa |

### Costo Computacional

Cada fragmento (píxel) calcula:
- 1 luz direccional
- 7 luces puntuales
- 1 spotlight
= **9 cálculos de iluminación por píxel**

**Optimización:** El fragment shader es eficiente gracias a la GPU.

---

## 🎓 Conceptos Técnicos

### Normal Mapping (No implementado actualmente)
Para mejoras futuras, se podría añadir normal mapping para detalles extra.

### Shadow Mapping (No implementado actualmente)
Las sombras proyectadas requieren pases adicionales de renderizado.

### HDR y Tone Mapping (No implementado actualmente)
Para manejar rangos dinámicos de luz muy altos.

---

## 🔗 Referencias

- [Shaders y Materiales](Shaders.md) - Código completo de los shaders de iluminación
- [Controles](Controles.md) - Cómo activar la luz central animada
- [Arquitectura](Arquitectura.md) - Pipeline de renderizado

---

**¡La iluminación da vida y profundidad al mundo 3D! 💡✨**
