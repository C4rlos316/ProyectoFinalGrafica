#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


// Funciones prototipo para callbacks
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// =================================================================================
// 	CONFIGURACIÓN INICIAL Y VARIABLES GLOBALES
// =================================================================================
//Configurar funciones para repetir textura de piso
void ConfigurarTexturaRepetible(GLuint textureID);
void DibujarPiso(GLuint textureID, glm::vec3 posicion, glm::vec3 escala, GLuint VAO_Cubo, GLint modelLoc);


// Propiedades de la ventana
const GLuint WIDTH = 1024, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;


// Configuración de la cámara
Camera  camera(glm::vec3(0.0f, 1.0f, 21.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

bool keys[1024];
bool firstMouse = true;
// Posiciones de las luces
glm::vec3 lightPos(1.5f, 2.0f, -2.5f);
glm::vec3 lightPos2(-1.5f, 2.0f, -2.5f);
bool active;

// Posiciones para luces puntuales 
glm::vec3 pointLightPositions[] = {
	glm::vec3(2.0f, 0.2f,  2.0f),  // Esquina 1 (Positivo X, Positivo Z)
	glm::vec3(-2.0f, 0.2f,  2.0f),  // Esquina 2 (Negativo X, Positivo Z)
	glm::vec3(2.0f, 0.2f, -2.0f),  // Esquina 3 (Positivo X, Negativo Z)
	glm::vec3(-2.0f, 0.2f, -2.0f)   // Esquina 4 (Negativo X, Negativo Z)
};



// =================================================================================
// 						ANIMACIÓN Y POSICIONES BASE DE ANIMALES
// =================================================================================

// -----------------------------------------
//  DESIERTO
// -----------------------------------------

// -----------------------------------------
//  CAMELLO (Cuadrante -X, Z)
// -----------------------------------------
float rotCamel = 180.0f;
float camelLegFL = 0.0f;
float camelLegFR = 0.0f;
float camelLegBL = 0.0f;
float camelLegBR = 0.0f;
float camelHead = 0.0f;
float camelTail = 0.0f;
float camelScale = 0.65f;
glm::vec3 camelPos = glm::vec3(-6.0f, -0.5f, 10.0f); //-8,13

bool animarCamello = false;
float startTimeCamello = 0.0f;
bool teclaC_presionada = false;




// Vértices del cubo
float vertices[] = {
	// Posiciones           // Normales           // Coordenadas de Textura (U, V)
	// Cara Trasera (-Z)
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	// Cara Frontal (+Z)
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	// Cara Izquierda (-X)
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	// Cara Derecha (+X)
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 // Cara Inferior (-Y) 
	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 10.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  10.0f, 10.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  10.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  10.0f, 0.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 10.0f,
	 // Cara Superior (+Y) 
	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 10.0f,
	  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  10.0f, 10.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  10.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  10.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 10.0f
};


glm::vec3 Light1 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame


void pataDraw(glm::mat4 modelo, glm::vec3 escala, glm::vec3 traslado, GLint uniformModel, GLuint VAO, GLuint texturaID)
{
	// 1. Configurar la matriz del modelo para esta parte
	modelo = glm::mat4(1);
	modelo = glm::scale(modelo, escala); // tamaño
	modelo = glm::translate(modelo, traslado);// posición
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelo));

	// 2. Activar y enlazar la textura específica para esta parte

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturaID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturaID); 

	// 3. Enlazar el VAO del cubo
	glBindVertexArray(VAO);

	// 4. Habilitar TODOS los atributos que usa lightingShader
	// (El lampShader los deshabilita, así que hay que volver a habilitarlos)
	glEnableVertexAttribArray(0); // Posición
	glEnableVertexAttribArray(1); // Normal
	glEnableVertexAttribArray(2); // TexCoords

	// 5. Dibujar el cubo
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 6. Desvincular
	glBindVertexArray(0);


}



int main()
{
	// =================================================================================
	// INICIALIZACIÓN DE GLFW, GLEW Y VENTANA
	// =================================================================================

	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// =================================================================================
	// 						CARGA DE MODELOS - Acuario (X,-Z)
	// =================================================================================



	// =================================================================================
	// 						CARGA DE MODELOS - Selva (X,Z)
	// =================================================================================




	// =================================================================================
	// 						CARGA DE MODELOS - DESIERTO (-X,Z)
	// =================================================================================
	std::cout << "Cargando modelos..." << std::endl;

		// ====== CAMELLO ======
	Model CamelBody((char*)"Models/camello/CamelBody.obj");
	Model CamelHead((char*)"Models/camello/CamelCabeza.obj");
	Model CamelLeg_FL((char*)"Models/camello/CamelPataizqEnfr.obj");
	Model CamelLeg_FR((char*)"Models/camello/CamelPataEnfreDer.obj");
	Model CamelLeg_BL((char*)"Models/camello/CamelPataizqAtras.obj");
	Model CamelLeg_BR((char*)"Models/camello/CamelPataAtrasDer.obj");

	std::cout << "Modelos cargados!" << std::endl;


	// =================================================================================
	// 						CARGA DE MODELOS - Sabana (-X,-Z)
	// =================================================================================



	// =================================================================================
	// 						Carga de Texturas para los pisos
	// =================================================================================


	// PARA ESTA PARTE YA HAY UNA FUNCION LA CUAL ES ConfigurarTexturaRepetible para que la usen
	// y no tengan que poner todo el codigo repetido ConfigurarTexturaRepetible(variablePiso);

	// ***TEXTURA GENERAL PARA EL PISO ***
	GLuint pisoTextureID = TextureFromFile("images/ladrillo.png", ".");
	ConfigurarTexturaRepetible(pisoTextureID);

	GLuint pisoEntradaID = TextureFromFile("images/pasto.jpg", ".");
	ConfigurarTexturaRepetible(pisoEntradaID);

	// *** TEXTURA PARA EL PISO ACUARIO ***
	GLuint pisoAcuarioTextureID = TextureFromFile("images/acuario.jpg", ".");
	ConfigurarTexturaRepetible(pisoAcuarioTextureID);

	// *** TEXTURA PARA EL PISO SELVA ***
	GLuint pisoSelvaTextureID = TextureFromFile("images/selva.jpg", ".");
	ConfigurarTexturaRepetible(pisoSelvaTextureID);

	// *** TEXTURA PARA EL PISO SABANA ***
	GLuint pisoSabanaTextureID = TextureFromFile("images/sabana.jpg", ".");
	ConfigurarTexturaRepetible(pisoSabanaTextureID);

	// *** TEXTURA PARA EL PISO DESIERTO ***
	GLuint pisoArenaTextureID = TextureFromFile("images/sand.jpg", ".");
	ConfigurarTexturaRepetible(pisoArenaTextureID);


	// =================================================================================
	// 					CONFIGURACIÓN DE VÉRTICES PARA PRIMITIVAS
	// =================================================================================

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// =================================================================================
		// 					CONFIGURACIÓN DE VÉRTICES PARA PRIMITIVAS (CUBO)
		// =================================================================================
	GLuint VBO_Cubo, VAO_Cubo;
	glGenVertexArrays(1, &VAO_Cubo);
	glGenBuffers(1, &VBO_Cubo);
	glBindVertexArray(VAO_Cubo);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cubo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO_Cubo); // Enlazar el VAO

	// Atributo de Posición (Location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	// Atributo de Normal (Location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Atributo de Coordenadas de Textura (Location 2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Desvincular el VAO



	// =================================================================================
	// 		CONFIGURACIÓN DE VÉRTICES PARA PISO DE ENTRADA
	// =================================================================================
	GLuint VBO_Entrada, VAO_Entrada;
	glGenVertexArrays(1, &VAO_Entrada);
	glGenBuffers(1, &VBO_Entrada);
	glBindVertexArray(VAO_Entrada);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Entrada);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Atributo de Posición (Location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Atributo de Normal (Location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Atributo de Coordenadas de Textura (Location 2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Desvincular el VAO


	// =================================================================================
	// 								CICLO DE RENDERIZADO
	// =================================================================================

	while (!glfwWindowShouldClose(window))
	{

		// Delta time, Eventos, Limpieza
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.6f, 0.7f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Luz Direccional (dirLight)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.6f, 0.6f, 0.6f);

		// Luces Puntuales (pointLights) - Usando los colores de las lámparas
		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);
		// SpotLight  //una luz tipo linterna en la cámara
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(6.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(10.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 2.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		GLint lampColorLoc = glGetUniformLocation(lampShader.Program, "lampColor");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 modelTemp = glm::mat4(1.0f);

		// ---------------------------------------------------------------------------------
		// 							CARGA DE TEXTURAS
		// ---------------------------------------------------------------------------------

		//GLuint texMadera, texMetal, texPlastico;
		//int textureWidth, textureHeight, nrChannels;
		//unsigned char* image;

		//// Función lambda para cargar texturas
		//auto cargarTextura = [&](const char* path, GLuint& tex) {
		//	glGenTextures(1, &tex);
		//	glBindTexture(GL_TEXTURE_2D, tex);
		//	stbi_set_flip_vertically_on_load(true);
		//	image = stbi_load(path, &textureWidth, &textureHeight, &nrChannels, 0);
		//	if (image)
		//	{
		//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		//		glGenerateMipmap(GL_TEXTURE_2D);
		//	}
		//	else
		//	{
		//		std::cout << "Failed to load texture: " << path << std::endl;
		//	}
		//	stbi_image_free(image);

		//	// Parámetros de textura
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	};

		//// Cargar las tres texturas
		//cargarTextura("images/madera.jpg", texMadera);
		//cargarTextura("images/madera.jpg", texMetal);
		//cargarTextura("images/madera.jpg", texPlastico);


		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE ESCENARIOS
		// ---------------------------------------------------------------------------------

		// DIBUJO DEL PISO GENERAL LADRILLO
			
		lightingShader.Use();
		DibujarPiso(pisoTextureID, glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(25.0f, 0.1f, 25.0f), VAO_Cubo, modelLoc);

		// DIBUJO DEL PASTO ENTRADA
		DibujarPiso(pisoEntradaID, glm::vec3(0.0f, -0.5f, 17.6f), glm::vec3(25.0f, 0.1f, 10.0f), VAO_Cubo, modelLoc);




		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE ESCENARIO ACUARIO (x,-z)
		// ---------------------------------------------------------------------------------

		// **** DIBUJOS DEL PISO Y ACCESORIOS DE ACUARIO ****
		DibujarPiso(pisoAcuarioTextureID, glm::vec3(7.25f, -0.49f, -7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);



		// **** DIBUJO DE ANIMALES ACUARIO ****




		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE MODELOS SELVA (x,z)
		// ---------------------------------------------------------------------------------

		// **** DIBUJO DEL PISO SELVA Y ACCESORIOS SELVA ****
		DibujarPiso(pisoSelvaTextureID, glm::vec3(7.25f, -0.49f, 7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);




		// **** DIBUJO DE ANIMALES SELVA ****



		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE MODELOS SABANA (-x,-z)
		// ---------------------------------------------------------------------------------

		// **** DIBUJO DEL PISO SABANA Y ACCESORIOS SABANA ****
		DibujarPiso(pisoSabanaTextureID, glm::vec3(-7.25f, -0.49f, -7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);



		// **** DIBUJO DE ANIMALES SABANA ****



		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE MODELOS DESIERTO (-x,z)
		// ---------------------------------------------------------------------------------

		// **** DIBUJO DEL PISO DESIERTO  Y COMPONENTES ****

		DibujarPiso(pisoArenaTextureID, glm::vec3(-7.25f, -0.49f, 7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);




		// **** DIBUJO DE ANIMALES DESIERTO ****

		//CAMELLO


		if (animarCamello)
		{
			float t = glfwGetTime() - startTimeCamello;

			// CAMINANDO HACIA EL CACTUS
			if (t < 8.0f)
			{
				// Movimiento de avance 
				float totalDist = 10.0f - 5.0f; // distancia = 5 unidades
				camelPos.z = 10.0f - (t * (totalDist / 8.0f)); // avanza desde Z=10 hasta Z=5

				// Movimiento de patas (alternadas)
				float paso = sin(t * 2.0f); // frecuencia lenta
				camelLegFL = paso * 15.0f;  // pata frontal izq
				camelLegBR = paso * 15.0f;  // pata trasera der
				camelLegFR = -paso * 15.0f; // pata frontal der (opuesta)
				camelLegBL = -paso * 15.0f; // pata trasera izq (opuesta)

				// Cabeza 
				camelHead = sin(t * 0.5f) * 1.3f; // leve movimiento
				rotCamel = 180.0f;
			}

			// FASE 2: LLEGA AL CACTUS
			else if (t < 14.0f)
			{
				float t2 = t - 8.0f;
				camelPos.z = 5.0f; // ya está en el cactus (AJUSTADO)

				// Detiene patas lentamente
				camelLegFL = sin(t2 * 1.0f) * 5.0f;
				camelLegFR = -camelLegFL;
				camelLegBL = -camelLegFR;
				camelLegBR = camelLegFR;

				// Movimiento de cabeza simulando comer
				camelHead = abs(sin(t2 * 1.5f)) * 2.9f; // baja y sube lento
				rotCamel = 180.0f;
			}

			
			else
			{
				camelPos.z = 5.0f;
				camelHead = 0.0f;
				camelLegFL = camelLegFR = camelLegBL = camelLegBR = 0.0f;
				rotCamel = 180.0f;
			}
		}


		model = glm::mat4(1);
		model = glm::translate(model, camelPos);
		model = glm::rotate(model, glm::radians(rotCamel), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(camelScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelBody.Draw(lightingShader);

		// Cabeza
		model = modelTemp;
		model = glm::rotate(model, glm::radians(camelHead), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelHead.Draw(lightingShader);

		// Pierna delantera izquierda
		glm::vec3 camelPivotFL(0.3f, 1.2f, 0.5f);
		model = modelTemp;
		model = glm::translate(model, camelPivotFL);
		model = glm::rotate(model, glm::radians(camelLegFL), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -camelPivotFL);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelLeg_FL.Draw(lightingShader);

		// Pierna delantera derecha
		glm::vec3 camelPivotFR(-0.3f, 1.2f, 0.5f);
		model = modelTemp;
		model = glm::translate(model, camelPivotFR);
		model = glm::rotate(model, glm::radians(camelLegFR), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -camelPivotFR);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelLeg_FR.Draw(lightingShader);

		// Pierna trasera izquierda
		glm::vec3 camelPivotBL(0.3f, 1.2f, -0.5f);
		model = modelTemp;
		model = glm::translate(model, camelPivotBL);
		model = glm::rotate(model, glm::radians(camelLegBL), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -camelPivotBL);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelLeg_BL.Draw(lightingShader);

		// Pierna trasera derecha
		glm::vec3 camelPivotBR(-0.3f, 1.2f, -0.5f);
		model = modelTemp;
		model = glm::translate(model, camelPivotBR);
		model = glm::rotate(model, glm::radians(camelLegBR), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -camelPivotBR);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelLeg_BR.Draw(lightingShader);

		//CONDOR

		//TORTUGA


		lightingShader.Use(); // shader de iluminación 

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}



// --- Función para configurar los parametros del piso de textura ---
void ConfigurarTexturaRepetible(GLuint textureID)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}


// --- Función para dibujar pisos con textura ---
void DibujarPiso(GLuint textureID, glm::vec3 posicion, glm::vec3 escala, GLuint VAO_Cubo, GLint modelLoc)
{
	// Activar y enlazar la textura
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Enlazar el VAO del cubo
	glBindVertexArray(VAO_Cubo);

	// Habilitar los atributos necesarios
	glEnableVertexAttribArray(0); // Posición
	glEnableVertexAttribArray(1); // Normal
	glEnableVertexAttribArray(2); // TexCoords

	// Crear matriz de transformación para el piso
	glm::mat4 model_piso = glm::mat4(1.0f);
	model_piso = glm::translate(model_piso, posicion);
	model_piso = glm::scale(model_piso, escala);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_piso));

	// Dibujar el cubo (piso)
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	//CAMELLO
	if (keys[GLFW_KEY_C])
	{
		if (!teclaC_presionada)
		{
			animarCamello = !animarCamello;
			startTimeCamello = glfwGetTime();
			teclaC_presionada = true;
		}
	}
	else
	{
		teclaC_presionada = false;
	}


	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos; 

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}