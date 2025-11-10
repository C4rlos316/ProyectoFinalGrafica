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
//  ACUARIO
// -----------------------------------------
float rotAletaIzqPingu = 0.0f;
float rotAletaDerPingu = 0.0f;
float rotPataIzqPingu = 0.0f;
float rotPataDerPingu = 0.0f;
float rotCuerpoPingu = 0.0f;

// --- ¡MODIFICADO! ---
// 'pinguBasePos' es el centro de la roca. 'pinguPos' se actualizará en el loop.
glm::vec3 pinguBasePos = glm::vec3(7.25f, 0.5f, -9.8f); // El centro de la roca
glm::vec3 pinguPos = pinguBasePos; // La posición actual, se animará
float pinguWalkDist = 0.5f; // Caminará 0.5 unidades hacia adelante y 0.5 hacia atrás
// --- FIN MODIFICADO ---

float pinguRotY = 0.0f; // Para la rotación del pingüino
float pinguScale = 0.4f; // <-- ¡AJUSTA ESTA ESCALA!


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


// -----------------------------------------
//  AVE (Cuadrante Central)
// -----------------------------------------
float rotAlaIzq = 0.0f;
float rotAlaDer = 0.0f;
float rotCabeza = 0.0f;
glm::vec3 avePos = glm::vec3(0.0f, 1.0f, 0.0f); // Posición del ave en el centro


// Vértices del cubo
float vertices[] = {
	// Posiciones         // Normales          // Coordenadas de Textura (U, V)
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
	// (Asegúrate de que la ruta 'Models/Acuario/escenarioAcuario.obj' sea correcta)
	Model EscenarioAcuario((char*)"Models/Acuario/escenarioacuario.obj");

	// --- Cargar Pinguino ---
	// (Asegúrate de crear la carpeta 'Models/Acuario/Pinguino/' y poner los .obj ahí)
	Model PinguCuerpo((char*)"Models/Acuario/cuerpopinguno.obj");
	Model PinguPataDer((char*)"Models/Acuario/pataderechapingu.obj");
	Model PinguPataIzq((char*)"Models/Acuario/pataizquierdapingu.obj");
	Model PinguAletaDer((char*)"Models/Acuario/aletaderechapingu.obj");
	Model PinguAletaIzq((char*)"Models/Acuario/aletaizquierdapingu.obj");
	// IGLU
	Model IgluModel((char*)"Models/Acuario/IGLU.obj");

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
	// 						CARGA DE MODELOS - AVIARIO (CENTRO)
	// =================================================================================

	Model AviarioMadera((char*)"Models/Aviario/Aviariobase.obj");
	Model AviarioVidrio((char*)"Models/Aviario/AviarioVidrio.obj");

	// ---  Cargar aves ---
	Model AveCuerpo((char*)"Models/Aviario/cuerpoave1.obj");
	Model AveCabeza((char*)"Models/Aviario/cabezaave1.obj");
	Model AveAlaIzq((char*)"Models/Aviario/alaizquierdaave1.obj");
	Model AveAlaDer((char*)"Models/Aviario/aladerechaave1.obj");
	Model AvePatas((char*)"Models/Aviario/patasave1.obj");
	Model AveCola((char*)"Models/Aviario/colaave1.obj");


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

	// *** TEXTURA PARA EL PISO ACUARIO (DIVIDIDO) ***

	//
	GLuint pisoPiedraTextureID = TextureFromFile("images/rocacafe.jpg", ".");
	ConfigurarTexturaRepetible(pisoPiedraTextureID);

	GLuint pisoAguaTextureID = TextureFromFile("images/agua2.jpg", ".");
	ConfigurarTexturaRepetible(pisoAguaTextureID);


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

		//glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0); // Esta línea estaba causando problemas
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

		//... (código de texturas comentado) ...


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

		// --- ¡MODIFICADO! ---
		// **** DIBUJOS DEL PISO Y ACCESORIOS DE ACUARIO ****
		// El cuadrante total es: centro(7.25f, -0.49f, -7.25f), escala(10.5f, 0.1f, 10.5f)

		// 1. Mitad trasera (Piedra)
		// Escalamos el piso a la mitad de profundidad (5.25f)
		// y lo movemos 1/4 hacia atrás (centro Z = -9.875f)
		DibujarPiso(pisoPiedraTextureID, glm::vec3(7.25f, -0.49f, -9.875f), glm::vec3(10.5f, 0.1f, 5.25f), VAO_Cubo, modelLoc);

		// 2. Mitad delantera (Agua)
		// Escalamos el piso a la mitad de profundidad (5.25f)
		// y lo movemos 1/4 hacia adelante (centro Z = -4.625f)
		DibujarPiso(pisoAguaTextureID, glm::vec3(7.25f, -0.49f, -4.625f), glm::vec3(10.5f, 0.1f, 5.25f), VAO_Cubo, modelLoc);


		// ---  DIBUJAR EL FONDO DEL ACUARIO ---
		model = glm::mat4(1.0f);
		// El centro X del cuadrante es 7.25f
		// El fondo Z del cuadrante es -7.25f - (10.5f / 2) = -12.5f
		model = glm::translate(model, glm::vec3(5.25f, -0.5f, -12.5f)); // <-- Corregí el X de 5.25 a 7.25
		model = glm::scale(model, glm::vec3(2.2f, 2.0f, 1.5f)); // <-- ¡AJUSTA ESTA ESCALA!
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		EscenarioAcuario.Draw(lightingShader);
	
		// --- DIBUJO IGLU ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(11.2f, -0.4f, -9.0f));
		model = glm::rotate(model, glm::radians(220.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));  
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		IgluModel.Draw(lightingShader);


	



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



		// =================================================================================
		// 							ANIMACIÓN DE ANIMALES - AVIARIO (CENTRO)
		// =================================================================================

		// Hacemos que el ave aletee y mueva la cabeza
		float t_aves = glfwGetTime();

		// Aleteo (de -1 a 1 y de vuelta)
		float aleteo = sin(t_aves * 6.0f); // 6.0 = aleteo rápido
		rotAlaIzq = aleteo * 45.0f; // Sube y baja 45 grados
		rotAlaDer = -aleteo * 45.0f; // Sube y baja 45 grados (opuesto)

		// Movimiento de cabeza (más lento)
		rotCabeza = sin(t_aves * 1.5f) * 15.0f; // Gira 15 grados a los lados


		// --- ¡MODIFICADO! ---
		// =================================================================================
		// 							ANIMACIÓN DE ANIMALES - ACUARIO (PINGUINO)
		// =================================================================================

		// 1. Definir el ciclo
		float cycleDuration = 10.0f; // 10 segundos en total
		float walkDuration = 4.0f;   // 4 seg para caminar
		float turnDuration = 1.0f;   // 1 seg para girar

		// fmod nos da un tiempo que se repite de 0.0 a 10.0
		float t_pingu = fmod(glfwGetTime(), cycleDuration);

		// 2. Definir las fases del ciclo
		float turn1StartTime = walkDuration; // 4.0s
		float walkBackStartTime = turn1StartTime + turnDuration; // 5.0s
		float turn2StartTime = walkBackStartTime + walkDuration; // 9.0s

		// --- ¡MODIFICADO! ---
		// 3. Definir las posiciones de inicio y fin (AHORA CAMINANDO EN EJE X)
		// Camina desde 'pinguBasePos.x - pinguWalkDist' hasta 'pinguBasePos.x + pinguWalkDist'
		glm::vec3 pinguStartPos = glm::vec3(pinguBasePos.x - pinguWalkDist, pinguBasePos.y, pinguBasePos.z);
		glm::vec3 pinguEndPos = glm::vec3(pinguBasePos.x + pinguWalkDist, pinguBasePos.y, pinguBasePos.z);
		// --- FIN MODIFICADO ---

		// 4. Lógica de Estado
		if (t_pingu < turn1StartTime) // Fase 1: Caminar Adelante (0s a 4s)
		{
			// Interpolar la posición
			float t_interp = t_pingu / walkDuration; // 0.0 a 1.0
			pinguPos = glm::mix(pinguStartPos, pinguEndPos, t_interp);
			pinguRotY = 90.0f; // --- ¡MODIFICADO! Mirando en dirección +X (flecha roja)

			// Animar movimiento
			float pasoPingu = sin(t_pingu * 8.0f); // Aleteo y patas (más rápido)
			rotPataIzqPingu = pasoPingu * 20.0f;
			rotPataDerPingu = -pasoPingu * 20.0f;
			rotCuerpoPingu = pasoPingu * 5.0f; // Balanceo
			rotAletaIzqPingu = abs(pasoPingu) * 25.0f;
			rotAletaDerPingu = abs(pasoPingu) * 25.0f;
		}
		else if (t_pingu < walkBackStartTime) // Fase 2: Girar 180 (4s a 5s)
		{
			pinguPos = pinguEndPos; // Quedarse quieto en el punto final

			// Interpolar la rotación
			float t_interp = (t_pingu - turn1StartTime) / turnDuration; // 0.0 a 1.0
			pinguRotY = 90.0f + (t_interp * 180.0f); // --- ¡MODIFICADO! Gira de 90 a 270

			// Detener animación de extremidades
			rotPataIzqPingu = 0.0f; rotPataDerPingu = 0.0f; rotCuerpoPingu = 0.0f; rotAletaIzqPingu = 0.0f; rotAletaDerPingu = 0.0f;
		}
		else if (t_pingu < turn2StartTime) // Fase 3: Caminar de Regreso (5s a 9s)
		{
			// Interpolar la posición (al revés)
			float t_interp = (t_pingu - walkBackStartTime) / walkDuration; // 0.0 a 1.0
			pinguPos = glm::mix(pinguEndPos, pinguStartPos, t_interp);
			pinguRotY = 270.0f; // --- ¡MODIFICADO! Mirando en dirección -X

			// Animar movimiento
			float pasoPingu = sin(t_pingu * 8.0f); // Aleteo y patas
			rotPataIzqPingu = pasoPingu * 20.0f;
			rotPataDerPingu = -pasoPingu * 20.0f;
			rotCuerpoPingu = pasoPingu * 5.0f; // Balanceo
			rotAletaIzqPingu = abs(pasoPingu) * 25.0f;
			rotAletaDerPingu = abs(pasoPingu) * 25.0f;
		}
		else // Fase 4: Girar de Regreso (9s a 10s)
		{
			pinguPos = pinguStartPos; // Quedarse quieto en el punto inicial

			// Interpolar la rotación
			float t_interp = (t_pingu - turn2StartTime) / turnDuration; // 0.0 a 1.0
			pinguRotY = 270.0f + (t_interp * 180.0f); // --- ¡MODIFICADO! Gira de 270 a 450 (que es 90)

			// Detener animación de extremidades
			rotPataIzqPingu = 0.0f; rotPataDerPingu = 0.0f; rotCuerpoPingu = 0.0f; rotAletaIzqPingu = 0.0f; rotAletaDerPingu = 0.0f;
		}


		// =================================================================================
		// 							DIBUJO DE MODELOS - AVIARIO (CENTRO)
		// =================================================================================

		// --- ¡MODIFICADO! ---
		// Esta es la forma correcta de manejar modelos con partes opacas y transparentes.

		// --- 1. DIBUJAR LA PARTE OPACA (Madera) ---
		// No necesita GL_BLEND. Usa 'transparency = 0' (que es el valor por defecto
		// después de que lo reseteamos al final del bucle).

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		AviarioMadera.Draw(lightingShader); // <-- Dibuja solo la madera


		// --- 2. DIBUJAR LA PARTE TRANSPARENTE (Vidrio) ---

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// "Encendemos" el interruptor 'transparency' en el shader
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);

		// Volvemos a aplicar la misma transformación para el vidrio
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		AviarioVidrio.Draw(lightingShader); // <-- Dibuja solo el vidrio


		// --- DIBUJAR EL AVE ---
		// (La dibujamos aquí para que esté dentro del vidrio transparente)

		// ¡IMPORTANTE! Tus texturas de ave NO son transparentes.
		// Le decimos al shader que las dibuje OPACAS (transparency = 0)
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// Cuerpo: Esta es la matriz "padre"
		model = glm::mat4(1.0f);
		model = glm::translate(model, avePos);
		model = glm::scale(model, glm::vec3(0.5f)); // <-- AJUSTA EL TAMAÑO
		modelTemp = model; // Guardamos la matriz del cuerpo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AveCuerpo.Draw(lightingShader);

		// Cabeza
		// --- ¡DEBES ENCONTRAR ESTE PIVOTE! ---
		glm::vec3 pivoteCabeza(0.0f, 0.5f, 0.1f); // (X,Y,Z) desde el centro del cuerpo (INVENTADO)
		model = modelTemp;
		model = glm::translate(model, pivoteCabeza);
		model = glm::rotate(model, glm::radians(rotCabeza), glm::vec3(0.0f, 1.0f, 0.0f)); // Rota en Y
		model = glm::translate(model, -pivoteCabeza);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AveCabeza.Draw(lightingShader);

		// Ala Izquierda
		// --- ¡DEBES ENCONTRAR ESTE PIVOTE! ---
		glm::vec3 pivoteAlaIzq(0.3f, 0.3f, 0.0f); // (X,Y,Z) desde el centro del cuerpo (INVENTADO)
		model = modelTemp;
		model = glm::translate(model, pivoteAlaIzq);
		model = glm::rotate(model, glm::radians(rotAlaIzq), glm::vec3(0.0f, 0.0f, 1.0f)); // Rota en Z (aleteo)
		model = glm::translate(model, -pivoteAlaIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AveAlaIzq.Draw(lightingShader);

		// Ala Derecha
		// --- ¡DEBES ENCONTRAR ESTE PIVOTE! ---
		glm::vec3 pivoteAlaDer(-0.3f, 0.3f, 0.0f); // (X,Y,Z) desde el centro del cuerpo (INVENTADO)
		model = modelTemp;
		model = glm::translate(model, pivoteAlaDer);
		model = glm::rotate(model, glm::radians(rotAlaDer), glm::vec3(0.0f, 0.0f, 1.0f)); // Rota en Z
		model = glm::translate(model, -pivoteAlaDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AveAlaDer.Draw(lightingShader);

		// Cola
		// --- ¡DEBES ENCONTRAR ESTE PIVOTE! ---
		glm::vec3 pivoteCola(0.0f, 0.1f, -0.4f); // (INVENTADO)
		model = modelTemp;
		model = glm::translate(model, pivoteCola);
		// (No hay rotación para la cola en este ejemplo, pero aquí iría)
		model = glm::translate(model, -pivoteCola);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AveCola.Draw(lightingShader);

		// Patas
		// --- ¡DEBES ENCONTRAR ESTE PIVOTE! ---
		glm::vec3 pivotePatas(0.0f, -0.2f, 0.0f); // (INVENTADO)
		model = modelTemp;
		model = glm::translate(model, pivotePatas);
		// (No hay rotación para las patas)
		model = glm::translate(model, -pivotePatas);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AvePatas.Draw(lightingShader);


		// Desactivamos todo para el resto de la escena
		glDisable(GL_BLEND);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);


		// --- DIBUJAR PINGUINO ---

		// Cuerpo
		model = glm::mat4(1.0f);
		model = glm::translate(model, pinguPos); // Posición global del pinguino
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));// Rotación global en Y
		model = glm::rotate(model, glm::radians(pinguRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(pinguScale)); // Escala global
		modelTemp = model; // Guardamos matriz del cuerpo

		// Aplicar balanceo del cuerpo
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f)); // Mover origen al centro del cuerpo (aprox)
		model = glm::rotate(model, glm::radians(rotCuerpoPingu), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotar en Z
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // Volver a la posición original

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PinguCuerpo.Draw(lightingShader);

		// Aleta Izquierda
		glm::vec3 pivoteAletaIzq(0.18f, 0.5f, 0.0f); // AJUSTADO A MANO
		model = modelTemp; // Usa la matriz base sin el balanceo del cuerpo para los apéndices
		model = glm::translate(model, pivoteAletaIzq);
		model = glm::rotate(model, glm::radians(rotAletaIzqPingu), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota en X
		model = glm::translate(model, -pivoteAletaIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PinguAletaIzq.Draw(lightingShader);

		// Aleta Derecha
		glm::vec3 pivoteAletaDer(-0.18f, 0.5f, 0.0f); // AJUSTADO A MANO
		model = modelTemp; // Usa la matriz base sin el balanceo del cuerpo para los apéndices
		model = glm::translate(model, pivoteAletaDer);
		model = glm::rotate(model, glm::radians(rotAletaDerPingu), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota en X
		model = glm::translate(model, -pivoteAletaDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PinguAletaDer.Draw(lightingShader);

		// Pata Izquierda
		glm::vec3 pivotePataIzq(0.06f, -0.4f, 0.02f); // AJUSTADO A MANO
		model = modelTemp; // Usa la matriz base sin el balanceo del cuerpo para los apéndices
		model = glm::translate(model, pivotePataIzq);
		model = glm::rotate(model, glm::radians(rotPataIzqPingu), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota en X
		model = glm::translate(model, -pivotePataIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PinguPataIzq.Draw(lightingShader);

		// Pata Derecha
		glm::vec3 pivotePataDer(-0.06f, -0.4f, 0.02f); // AJUSTADO A MANO
		model = modelTemp; // Usa la matriz base sin el balanceo del cuerpo para los apéndices
		model = glm::translate(model, pivotePataDer);
		model = glm::rotate(model, glm::radians(rotPataDerPingu), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota en X
		model = glm::translate(model, -pivotePataDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PinguPataDer.Draw(lightingShader);


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