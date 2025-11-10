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
void ConfigurarVAO(GLuint& VAO, GLuint& VBO, float* vertices, size_t size);
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
};



// =================================================================================
// 						ANIMACIÓN Y POSICIONES BASE DE ANIMALES
// =================================================================================

// -----------------------------------------
//  SELVA (X,Z)
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

//  CAPIBARA (Cuadrante X, Z)
float capibaraScale = 1.0f;
float rotCapibara = 180.0f;
float capibaraCabezaRot = 0.0f;
float capibaraNaranjaRot = 0.0f;
float capibaraPataDelDer = 0.0f;
float capibaraPataDelIzq = 0.0f;
float capibaraPataTrasDer = 0.0f;
float capibaraPataTrasIzq = 0.0f;
glm::vec3 capibaraPos = glm::vec3(11.0f, 0.0f, 8.0f);
bool animarCapibara = false;
float startTimeCapibara = 0.0f;
bool teclaP_presionada = false;

//  MONO (Cuadrante X, Z)
float monoScale = 1.0f;
float rotMono = 180.0f;
float monoColaRot = 0.0f;
float monoPataDelDer = 0.0f;
float monoPataDelIzq = 0.0f;
float monoPataTrasDer = 0.0f;
float monoPataTrasIzq = 0.0f;
glm::vec3 monoPos = glm::vec3(11.0f, 0.0f, 11.0f); 
bool animarMono = false;
float startTimeMono = 0.0f;
bool teclaM_presionada = false;

// -----------------------------------------
//  ACUARIO (X,-Z)
// -----------------------------------------

// -----------------------------------------
//  SABANA (-X,-Z)
// -----------------------------------------

//  ELEFANTE (Cuadrante -X, -Z)
float rotElefante = 90.0f;
float rotElefanteLado = 0.0f;
float elefanteLegFL = 0.0f;
float elefanteLegFR = 0.0f;
float elefanteLegBL = 0.0f;
float elefanteLegBR = 0.0f;
float elefanteTrompa = 0.0f;
float elefanteScale = 0.50f;
glm::vec3 elefantePos = glm::vec3(-11.0f, -0.4f, -10.5f);
bool animarElefante = false;
float startTimeElefante = 0.0f;
bool teclaV_presionada = false;


// -----------------------------------------
//  DESIERTO (-X,Z)
// -----------------------------------------

//  CAMELLO (Cuadrante -X, Z)
float rotCamel = 180.0f;
float camelLegFL = 0.0f;
float camelLegFR = 0.0f;
float camelLegBL = 0.0f;
float camelLegBR = 0.0f;
float camelHead = 0.0f;
float camelTail = 0.0f;
float camelScale = 0.65f;
glm::vec3 camelPos = glm::vec3(-6.0f, -0.5f, 10.0f);
bool animarCamello = false;
float startTimeCamello = 0.0f;
bool teclaC_presionada = false;


//  TORTUGA (Cuadrante -X, Z)
float rotTortuga = 0.0f;
float tortugaLegFL = 0.0f;
float tortugaLegFR = 0.0f;
float tortugaScale = 0.20f;
glm::vec3 tortugaPos = glm::vec3(-7.8f, -0.18f, 9.5f);
bool animarTortuga = false;
float startTimeTortuga = 0.0f;
bool teclaX_presionada = false;
// -----------------------------------------
//  AVE (Cuadrante Central)
// -----------------------------------------
float rotAlaIzq = 0.0f;
float rotAlaDer = 0.0f;
float rotCabeza = 0.0f;
glm::vec3 avePos = glm::vec3(0.0f, 1.0f, 0.0f); // Posición del ave en el centro

//  CÓNDOR (Cuadrante -X, Z)
float rotCondor = 90.0f;
float condorHead = 0.0f;
float condorAlaIzq = 0.0f;
float condorAlaDer = 0.0f;
float condorScale = 0.70f;
glm::vec3 condorPos = glm::vec3(-6.7f, 0.5f, 6.0f);
bool animarCondor = false;
bool teclaZ_presionada = false;


// Vértices para el piso
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


// Vértices para PAREDES 
float verticesPared[] = {
	// Cara Trasera (-Z) - CORREGIDO: más repeticiones horizontales
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   15.0f, 0.0f,  // Aumentado de 3.0 a 10.0
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   15.0f, 5.0f,  // Aumentado de 3.0 a 10.0
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   15.0f, 5.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 5.0f,
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

	// Cara Frontal (+Z)
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   15.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   15.0f, 5.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   15.0f, 5.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 5.0f,
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

	// Cara Izquierda (-X)
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   5.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   5.0f, 15.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 15.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 15.0f,
	-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   5.0f, 0.0f,

	// Cara Derecha (+X)
	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   5.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   5.0f, 15.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 15.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 15.0f,
	 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   5.0f, 0.0f,

	 // Cara Inferior (-Y)
	 -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 5.0f,
	  0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   5.0f, 5.0f,
	  0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   5.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   5.0f, 0.0f,
	 -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 5.0f,

	 // Cara Superior (+Y)
	 -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 5.0f,
	  0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   5.0f, 5.0f,
	  0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   5.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   5.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 5.0f
};
glm::vec3 Light1 = glm::vec3(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturaID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturaID);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame


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

	// ====== ESCENARIO ======
	Model ArbolSelva((char*)"Models/arbolSelva/arbolSelva.obj");
	glm::vec3 arbolSelvaPos(11.0f, -0.5f, 3.1f);
	glm::vec3 arbolSelvaScale(0.2f, 0.2f, 0.2f);
	float arbolSelvaRot = 0.0f;

	Model Sandia((char*)"Models/sandia/sandia.obj");
	glm::vec3 sandiaPos(3.0f, -0.2f, 8.0f); 
	glm::vec3 sandiaScale(1.0f, 1.0f, 1.0f);
	float sandiaRot = 180.0f;

	Model TroncoSelva1((char*)"Models/troncoSelva/troncoSelva.obj");
	glm::vec3 troncoSelva1Pos(10.0f, -0.2f, 11.0f);
	glm::vec3 troncoSelva1Scale(1.0f, 0.7f, 1.0f);
	float troncoSelva1Rot = 0.0f;

	Model TroncoSelva2((char*)"Models/troncoSelva/troncoSelva.obj");
	glm::vec3 troncoSelva2Pos(5.5f, -0.2f, 11.0f);
	glm::vec3 troncoSelva2Scale(1.0f, 0.5f, 1.0f);
	float troncoSelva2Rot = 0.0f;

	// ====== CAPIBARA ======
	Model Capibara_Cabeza((char*)"Models/capibara/cabezaCapi.obj");
	Model Capibara_Cuerpo((char*)"Models/capibara/cuerpoCapi.obj");
	Model Capibara_Naranja((char*)"Models/capibara/naranjaCapi.obj");
	Model Capibara_PataDelDer((char*)"Models/capibara/pataDelDerCapi.obj");
	Model Capibara_PataDelIzq((char*)"Models/capibara/pataDelIzqCapi.obj");
	Model Capibara_PataTrasDer((char*)"Models/capibara/pataTrasDerCapi.obj");
	Model Capibara_PataTrasIzq((char*)"Models/capibara/pataTrasIzqCapi.obj");

	// ====== MONO ======
	Model Mono_Cuerpo((char*)"Models/mono/cuerpoMono.obj");
	Model Mono_Cola((char*)"Models/mono/colaMono.obj");
	Model Mono_PataDelDer((char*)"Models/mono/pataDelDerMono.obj");
	Model Mono_PataDelIzq((char*)"Models/mono/pataDelIzqMono.obj");
	Model Mono_PataTrasDer((char*)"Models/mono/pataTasDerMono.obj");
	Model Mono_PataTrasIzq((char*)"Models/mono/pataTrasIzqMono.obj");

	// =================================================================================
	// 						CARGA DE MODELOS - DESIERTO (-X,Z)
	// =================================================================================
	std::cout << "Cargando modelos..." << std::endl;

	//  ************ Codigo comentado para no cargar todo desde el inicio se descomenta al final *******
	// ====== CAMELLO ======
	Model CamelBody((char*)"Models/camello/CamelBody.obj");
	Model CamelHead((char*)"Models/camello/CamelCabeza.obj");
	Model CamelLeg_FL((char*)"Models/camello/CamelPataizqEnfr.obj");
	Model CamelLeg_FR((char*)"Models/camello/CamelPataEnfreDer.obj");
	Model CamelLeg_BL((char*)"Models/camello/CamelPataizqAtras.obj");
	Model CamelLeg_BR((char*)"Models/camello/CamelPataAtrasDer.obj");


	// ====== ESCENARIO ======
	//Model Oasis((char*)"Models/oasis/oasis.obj");
	//glm::vec3 oasisPos(-9.5f, -0.64f, 9.5f);
	//glm::vec3 oasisScale(20.0f, 20.0f, 20.0f);
	//float oasisRot = 270.0f;

	//Model Huesos((char*)"Models/huesos/huesos.obj");
	//glm::vec3 huesosPos(-8.5f, -0.6f, 4.0f);
	//glm::vec3 huesosScale(0.3f, 0.25f, 0.25f);
	//float huesosRot = 90.0f;

	//Model Tronco((char*)"Models/tronco/tronco.obj");
	//glm::vec3 troncoPos(-6.8f, -0.5f, 6.0f);
	//glm::vec3 troncoScale(0.7f, 0.7f, 0.7f);
	//float troncoRot = 0.0f;


	//Model Cactus((char*)"Models/cactus/Cactus.obj");
	//glm::vec3 cactusPos(-6.0f, -0.5f, 3.7f);
	//glm::vec3 cactusScale(0.04f, 0.04f, 0.04f);
	//float cactusRot = 0.0f;

	//	// ====== CAMELLO ======
	//Model CamelBody((char*)"Models/camello/CamelBody.obj");
	//Model CamelHead((char*)"Models/camello/CamelCabeza.obj");
	//Model CamelLeg_FL((char*)"Models/camello/CamelPataizqEnfr.obj");
	//Model CamelLeg_FR((char*)"Models/camello/CamelPataEnfreDer.obj");
	//Model CamelLeg_BL((char*)"Models/camello/CamelPataizqAtras.obj");
	//Model CamelLeg_BR((char*)"Models/camello/CamelPataAtrasDer.obj");

	//// ====== TORTUGA ======
	//Model TortugaBody((char*)"Models/tortuga/tortuga_cuerpo.obj");
	//Model TortugaLeg_FL((char*)"Models/tortuga/tortuga_pata_izq.obj");
	//Model TortugaLeg_FR((char*)"Models/tortuga/tortuga_pata_der.obj");


	//// ====== CÓNDOR ======
	//Model CondorBody((char*)"Models/condor/condor_cuerpo.obj");
	//Model CondorHead((char*)"Models/condor/condor_cabeza.obj");
	//Model CondorAla_Der((char*)"Models/condor/condor_ala_der.obj");
	//Model CondorAla_Izq((char*)"Models/condor/condor_ala_izq.obj");

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

	// ====== ESCENARIO ======

	// ROCA
	Model Roca((char*)"Models/roca/roca.obj");
	glm::vec3 RocaPos(-7.25f, -0.5f, -11.8f);
	glm::vec3 RocaEScale(0.06f, 0.06f, 0.06f);
	float RocaRot = 270.0f;

	//ELEFANTE
	 
	Model ElefanteBody((char*)"Models/elefante/elefante_cuerpo.obj");
	Model ElefanteTrompa((char*)"Models/elefante/elefante_trompa.obj");
	Model ElefanteLeg_FL((char*)"Models/elefante/elefante_pata_izq_enfr.obj");
	Model ElefanteLeg_FR((char*)"Models/elefante/elefante_pata_der_enfr.obj");
	Model ElefanteLeg_BL((char*)"Models/elefante/elefante_pata_izq_atras.obj");
	Model ElefanteLeg_BR((char*)"Models/elefante/elefante_pata_der_atras.obj");
	
	//JIRAFA


	//

	// =================================================================================
	// 						Carga de Texturas para los pisos
	// =================================================================================


	// PARA ESTA PARTE YA HAY UNA FUNCION LA CUAL ES ConfigurarTexturaRepetible para que la usen
	// y no tengan que poner todo el codigo repetido ConfigurarTexturaRepetible(variablePiso);

	// ***TEXTURA GENERAL PARA EL PISO GENERAL***
	GLuint pisoTextureID = TextureFromFile("images/ladrillo.png", ".");
	ConfigurarTexturaRepetible(pisoTextureID);
	
	// *** TEXTURA PARA EL PISO ENTRADA ***
	GLuint pisoEntradaID = TextureFromFile("images/pasto.jpg", ".");
	ConfigurarTexturaRepetible(pisoEntradaID);

	// *** TEXTURA PARA LAS PAREDES ***
	GLuint paredTextureID = TextureFromFile("images/muro.jpg", ".");
	ConfigurarTexturaRepetible(paredTextureID);

	// *** TEXTURA PARA EL PISO ACUARIO ***
	GLuint pisoAcuarioTextureID = TextureFromFile("images/acuario.jpg", ".");
	ConfigurarTexturaRepetible(pisoAcuarioTextureID);
	// *** TEXTURA PARA EL PISO ACUARIO (DIVIDIDO) ***

	//
	GLuint pisoPiedraTextureID = TextureFromFile("images/rocacafe.jpg", ".");
	ConfigurarTexturaRepetible(pisoPiedraTextureID);

	GLuint pisoAguaTextureID = TextureFromFile("images/agua2.jpg", ".");
	ConfigurarTexturaRepetible(pisoAguaTextureID);


	// *** TEXTURA PARA EL PISO SELVA ***
	GLuint pisoSelvaTextureID = TextureFromFile("images/selva.png", ".");
	ConfigurarTexturaRepetible(pisoSelvaTextureID);

	// *** TEXTURA PARA EL PISO SABANA ***
	GLuint pisoSabanaTextureID = TextureFromFile("images/sabana.jpg", ".");
	ConfigurarTexturaRepetible(pisoSabanaTextureID);

	// *** TEXTURA PARA EL PISO DESIERTO ***
	GLuint pisoArenaTextureID = TextureFromFile("images/sand.jpg", ".");
	ConfigurarTexturaRepetible(pisoArenaTextureID);

	// Altura de la pared
	float alturaPared = 3.0f;
	// Escala general del área
	float tamanoBase = 25.0f;

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);


	// =================================================================================
	// 		CONFIGURACIÓN DE VÉRTICES PARA PISO GENERAL
	// =================================================================================
	GLuint VBO_Cubo, VAO_Cubo;
	ConfigurarVAO(VAO_Cubo, VBO_Cubo, vertices, sizeof(vertices));

	// =================================================================================
	// 		CONFIGURACIÓN DE VÉRTICES PARA PAREDES
	// =================================================================================
	GLuint VBO_Pared, VAO_Pared;
	ConfigurarVAO(VAO_Pared, VBO_Pared, verticesPared, sizeof(verticesPared));

	// =================================================================================
	// 		CONFIGURACIÓN DE VÉRTICES PARA PISO DE ENTRADA
	// =================================================================================
	GLuint VBO_Entrada, VAO_Entrada;
	ConfigurarVAO(VAO_Entrada, VBO_Entrada, vertices, sizeof(vertices));


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
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.4f, -1.0f, -0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.15f, 0.13f, 0.10f); 
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.9f, 0.85f, 0.75f); 
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 0.95f, 0.85f);

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
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

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
		DibujarPiso(pisoEntradaID, glm::vec3(0.0f, -0.5f, 17.5f), glm::vec3(25.0f, 0.1f, 10.0f), VAO_Cubo, modelLoc);

		//Dibujo de las paredes 
		
		// Pared trasera (Z negativa)
		DibujarPiso(paredTextureID, glm::vec3(0.0f, alturaPared / 2 - 0.5f, -tamanoBase / 2),
			glm::vec3(tamanoBase, alturaPared, 0.2f), VAO_Pared, modelLoc);



		// Pared izquierda (X negativa)
		DibujarPiso(paredTextureID, glm::vec3(-tamanoBase / 2, alturaPared / 2 - 0.5f, 0.0f),
			glm::vec3(0.2f, alturaPared, tamanoBase), VAO_Pared, modelLoc);

		// Pared derecha (X positiva)
		DibujarPiso(paredTextureID, glm::vec3(tamanoBase / 2, alturaPared / 2 - 0.5f, 0.0f),
			glm::vec3(0.2f, alturaPared, tamanoBase), VAO_Pared, modelLoc);


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

		// --- ÁRBOL ---
		model = glm::mat4(1);
		model = glm::translate(model, arbolSelvaPos);
		model = glm::scale(model, arbolSelvaScale);
		model = glm::rotate(model, glm::radians(arbolSelvaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ArbolSelva.Draw(lightingShader);

		// --- SANDÍA ---
		model = glm::mat4(1);
		model = glm::translate(model, sandiaPos);
		model = glm::scale(model, sandiaScale);
		model = glm::rotate(model, glm::radians(sandiaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sandia.Draw(lightingShader);

		// --- TRONCO 1 ---
		model = glm::mat4(1);
		model = glm::translate(model, troncoSelva1Pos);
		model = glm::scale(model, troncoSelva1Scale);
		model = glm::rotate(model, glm::radians(troncoSelva1Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TroncoSelva1.Draw(lightingShader);

		// --- TRONCO 2 ---
		model = glm::mat4(1);
		model = glm::translate(model, troncoSelva2Pos);
		model = glm::scale(model, troncoSelva2Scale);
		model = glm::rotate(model, glm::radians(troncoSelva2Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TroncoSelva2.Draw(lightingShader);

		// **** DIBUJO DEL CAPIBARA ****
		model = glm::mat4(1);
		model = glm::translate(model, capibaraPos);
		model = glm::rotate(model, glm::radians(rotCapibara), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(capibaraScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_Cuerpo.Draw(lightingShader);

		// Cabeza
		glm::vec3 capibaraPivotCabeza(0.0f, 0.5f, 0.4f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotCabeza);
		model = glm::rotate(model, glm::radians(capibaraCabezaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, -capibaraPivotCabeza);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_Cabeza.Draw(lightingShader);

		// Pata delantera derecha
		glm::vec3 capibaraPivotPataDelDer(0.2f, 0.3f, 0.3f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotPataDelDer);
		model = glm::rotate(model, glm::radians(capibaraPataDelDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -capibaraPivotPataDelDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_PataDelDer.Draw(lightingShader);

		// Pata delantera izquierda
		glm::vec3 capibaraPivotPataDelIzq(-0.2f, 0.3f, 0.3f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotPataDelIzq);
		model = glm::rotate(model, glm::radians(capibaraPataDelIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -capibaraPivotPataDelIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_PataDelIzq.Draw(lightingShader);

		// Pata trasera derecha
		glm::vec3 capibaraPivotPataTrasDer(0.2f, 0.3f, -0.3f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotPataTrasDer);
		model = glm::rotate(model, glm::radians(capibaraPataTrasDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -capibaraPivotPataTrasDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_PataTrasDer.Draw(lightingShader);

		// Pata trasera izquierda
		glm::vec3 capibaraPivotPataTrasIzq(-0.2f, 0.3f, -0.3f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotPataTrasIzq);
		model = glm::rotate(model, glm::radians(capibaraPataTrasIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -capibaraPivotPataTrasIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_PataTrasIzq.Draw(lightingShader);

		// Naranja
		glm::vec3 capibaraPivotNaranja(0.0f, 0.0f, 0.0f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotNaranja);
		model = glm::rotate(model, glm::radians(capibaraNaranjaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, -capibaraPivotNaranja);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_Naranja.Draw(lightingShader);

		// CAPIBARA - ANIMACIÓN
		if (animarCapibara)
		{
			float t = glfwGetTime() - startTimeCapibara;

			// FASE 1: CAMINANDO (10 segundos)
			if (t < 10.0f)
			{
				float totalDist = 7.0f;
				capibaraPos.x = 11.0f - (t * (totalDist / 10.0f));

				// Movimiento de patas (caminata de cuadrúpedo)
				float paso = sin(t * 6.0f);
				capibaraPataDelDer = paso * 1.5f;
				capibaraPataTrasDer = paso * 1.5f;
				capibaraPataDelIzq = -paso * 1.5f;
				capibaraPataTrasIzq = -paso * 1.5f;

				// Naranja rotando mientras camina
				capibaraNaranjaRot = t * 180.0f;

				capibaraCabezaRot = 0.0f;
				rotCapibara = 180.0f;
			}
			// FASE 2: DETENIDO, COMIENDO (6 segundos)
			else if (t < 16.0f)
			{
				float t2 = t - 10.0f;
				capibaraPos.x = 4.0f;

				// Patas deteniéndose gradualmente
				capibaraPataDelDer = sin(t2 * 0.5f) * 2.0f;
				capibaraPataDelIzq = -capibaraPataDelDer;
				capibaraPataTrasDer = -capibaraPataDelDer;
				capibaraPataTrasIzq = capibaraPataDelDer;

				// Naranja regresa a posición inicial
				capibaraNaranjaRot = 1800.0f - (t2 * 300.0f);

				capibaraCabezaRot = 0.0f;
				rotCapibara = 180.0f;
			}
			// FASE 3: QUIETO
			else
			{
				capibaraPos.x = 4.0f;
				capibaraCabezaRot = 0.0f;
				capibaraPataDelDer = capibaraPataDelIzq = 0.0f;
				capibaraPataTrasDer = capibaraPataTrasIzq = 0.0f;
				capibaraNaranjaRot = 0.0f;
				rotCapibara = 180.0f;
			}
		}

		// **** DIBUJO DEL MONO ****
		model = glm::mat4(1);
		model = glm::translate(model, monoPos);
		model = glm::rotate(model, glm::radians(rotMono), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(monoScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_Cuerpo.Draw(lightingShader);

		// Cola
		glm::vec3 monoPivotCola(0.0f, 0.5f, -0.4f);
		model = modelTemp;
		model = glm::translate(model, monoPivotCola);
		model = glm::rotate(model, glm::radians(monoColaRot), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -monoPivotCola);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_Cola.Draw(lightingShader);

		// Pata delantera derecha
		glm::vec3 monoPivotPataDelDer(0.2f, 0.3f, 0.3f);
		model = modelTemp;
		model = glm::translate(model, monoPivotPataDelDer);
		model = glm::rotate(model, glm::radians(monoPataDelDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -monoPivotPataDelDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_PataDelDer.Draw(lightingShader);

		// Pata delantera izquierda
		glm::vec3 monoPivotPataDelIzq(-0.2f, 0.3f, 0.3f);
		model = modelTemp;
		model = glm::translate(model, monoPivotPataDelIzq);
		model = glm::rotate(model, glm::radians(monoPataDelIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -monoPivotPataDelIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_PataDelIzq.Draw(lightingShader);

		// Pata trasera derecha
		glm::vec3 monoPivotPataTrasDer(0.2f, 0.3f, -0.3f);
		model = modelTemp;
		model = glm::translate(model, monoPivotPataTrasDer);
		model = glm::rotate(model, glm::radians(monoPataTrasDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -monoPivotPataTrasDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_PataTrasDer.Draw(lightingShader);

		// Pata trasera izquierda
		glm::vec3 monoPivotPataTrasIzq(-0.2f, 0.3f, -0.3f);
		model = modelTemp;
		model = glm::translate(model, monoPivotPataTrasIzq);
		model = glm::rotate(model, glm::radians(monoPataTrasIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -monoPivotPataTrasIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_PataTrasIzq.Draw(lightingShader);

		// MONO - ANIMACIÓN
		if (animarMono)
		{
			float t = glfwGetTime() - startTimeMono;

			// FASE 1: PRIMER BRINCO (1.5 segundos)
			if (t < 1.5f)
			{
				float saltoCiclo = abs(sin(t * 2.094f));
				monoPos.y = 0.0f + (saltoCiclo * 1.0f);
				monoPos.x = 11.0f - (t * 1.5f);

				// Patas recogidas durante el salto
				monoPataDelDer = saltoCiclo * 8.0f;
				monoPataDelIzq = saltoCiclo * 8.0f;
				monoPataTrasDer = saltoCiclo * 8.0f;
				monoPataTrasIzq = saltoCiclo * 8.0f;

				// Cola levantada durante el salto
				monoColaRot = 2.0f + (saltoCiclo * 1.5f);
				rotMono = 180.0f;
			}
			// PAUSA 1: ATERRIZA (0.2 segundos)
			else if (t < 1.7f)
			{
				monoPos.y = 0.0f;
				monoPos.x = 8.75f;
				monoPataDelDer = monoPataDelIzq = 0.0f;
				monoPataTrasDer = monoPataTrasIzq = 0.0f;
				monoColaRot = 0.5f;
				rotMono = 180.0f;
			}
			// FASE 2: SEGUNDO BRINCO (1.5 segundos)
			else if (t < 3.2f)
			{
				float t2 = t - 1.7f;
				float saltoCiclo = abs(sin(t2 * 2.094f));
				monoPos.y = 0.0f + (saltoCiclo * 1.0f);
				monoPos.x = 8.75f - (t2 * 1.5f);

				// Patas recogidas durante el salto
				monoPataDelDer = saltoCiclo * 8.0f;
				monoPataDelIzq = saltoCiclo * 8.0f;
				monoPataTrasDer = saltoCiclo * 8.0f;
				monoPataTrasIzq = saltoCiclo * 8.0f;

				// Cola levantada durante el salto
				monoColaRot = 2.0f + (saltoCiclo * 1.5f);
				rotMono = 180.0f;
			}
			// PAUSA 2: ATERRIZA (0.2 segundos)
			else if (t < 3.4f)
			{
				monoPos.y = 0.0f;
				monoPos.x = 6.5f;
				monoPataDelDer = monoPataDelIzq = 0.0f;
				monoPataTrasDer = monoPataTrasIzq = 0.0f;
				monoColaRot = 0.5f;
				rotMono = 180.0f;
			}
			// FASE 3: TERCER BRINCO (1.5 segundos)
			else if (t < 4.9f)
			{
				float t3 = t - 3.4f;
				float saltoCiclo = abs(sin(t3 * 2.094f));
				monoPos.y = 0.0f + (saltoCiclo * 1.0f);
				monoPos.x = 6.5f - (t3 * 1.5f);

				// Patas recogidas durante el salto
				monoPataDelDer = saltoCiclo * 8.0f;
				monoPataDelIzq = saltoCiclo * 8.0f;
				monoPataTrasDer = saltoCiclo * 8.0f;
				monoPataTrasIzq = saltoCiclo * 8.0f;

				// Cola levantada durante el salto
				monoColaRot = 2.0f + (saltoCiclo * 1.5f);
				rotMono = 180.0f;
			}
			// FASE 4: CAMINANDO (1 segundo)
			else if (t < 5.9f)
			{
				float t4 = t - 4.9f;
				monoPos.y = 0.0f;
				monoPos.x = 4.25f - (t4 * 1.0f);

				// Movimiento de patas (caminata de cuadrúpedo)
				float paso = sin(t4 * 6.0f);
				monoPataDelDer = paso * 2.0f;
				monoPataTrasDer = paso * 2.0f;
				monoPataDelIzq = -paso * 2.0f;
				monoPataTrasIzq = -paso * 2.0f;

				// Cola moviéndose mientras camina
				monoColaRot = sin(t4 * 2.0f) * 2.0f;
				rotMono = 180.0f;
			}
			// FASE 5: QUIETO

			else
			{
				monoPos.x = 3.25f;
				monoPos.y = 0.0f;
				monoColaRot = 0.0f;
				monoPataDelDer = monoPataDelIzq = 0.0f;
				monoPataTrasDer = monoPataTrasIzq = 0.0f;
				rotMono = 180.0f;
			}
		}

		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE MODELOS SABANA (-x,-z)
		// ---------------------------------------------------------------------------------

		// **** DIBUJO DEL PISO SABANA Y ACCESORIOS SABANA ****
		DibujarPiso(pisoSabanaTextureID, glm::vec3(-7.25f, -0.49f, -7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);

		// --- Roca  ---
		model = glm::mat4(1);
		model = glm::translate(model, RocaPos);
		model = glm::scale(model, RocaEScale);
		model = glm::rotate(model, glm::radians(RocaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Roca.Draw(lightingShader);


		// **** DIBUJO DE ANIMALES SABANA ****


		//ELEFANTE

		model = glm::mat4(1);
		model = glm::translate(model, elefantePos);
		model = glm::rotate(model, glm::radians(rotElefante), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotación en Y
		model = glm::rotate(model, glm::radians(rotElefanteLado), glm::vec3(0.0f, 0.0f, 1.0f));  // NUEVA: Rotación lateral (acostarse)
		model = glm::scale(model, glm::vec3(elefanteScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ElefanteBody.Draw(lightingShader);

		// Trompa (con rotación en X para moverla arriba/abajo)
		glm::vec3 elefantePivotTrompa(0.0f, 1.0f, 0.5f);
		model = modelTemp;
		model = glm::translate(model, elefantePivotTrompa);
		model = glm::rotate(model, glm::radians(elefanteTrompa), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -elefantePivotTrompa);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ElefanteTrompa.Draw(lightingShader);

		// Pierna delantera izquierda
		glm::vec3 elefantePivotFL(0.3f, 1.2f, 0.5f);
		model = modelTemp;
		model = glm::translate(model, elefantePivotFL);
		model = glm::rotate(model, glm::radians(elefanteLegFL), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -elefantePivotFL);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ElefanteLeg_FL.Draw(lightingShader);

		// Pierna delantera derecha
		glm::vec3 elefantePivotFR(-0.3f, 1.2f, 0.5f);
		model = modelTemp;
		model = glm::translate(model, elefantePivotFR);
		model = glm::rotate(model, glm::radians(elefanteLegFR), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -elefantePivotFR);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ElefanteLeg_FR.Draw(lightingShader);

		// Pierna trasera izquierda
		glm::vec3 elefantePivotBL(0.3f, 1.2f, -0.5f);
		model = modelTemp;
		model = glm::translate(model, elefantePivotBL);
		model = glm::rotate(model, glm::radians(elefanteLegBL), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -elefantePivotBL);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ElefanteLeg_BL.Draw(lightingShader);

		// Pierna trasera derecha
		glm::vec3 elefantePivotBR(-0.3f, 1.2f, -0.5f);
		model = modelTemp;
		model = glm::translate(model, elefantePivotBR);
		model = glm::rotate(model, glm::radians(elefanteLegBR), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -elefantePivotBR);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ElefanteLeg_BR.Draw(lightingShader);

		//ELEFANTE - ANIMACIÓN

		if (animarElefante)
		{
			float t = glfwGetTime() - startTimeElefante;

			// FASE 1: CAMINANDO (0s - 8s)
			if (t < 8.0f)
			{
				// Movimiento en EJE X (de -11 a -6)
				float totalDist = 5.0f;
				elefantePos.x = -11.0f + (t * (totalDist / 8.0f));

				// Movimiento de patas
				float paso = sin(t * 2.0f);
				elefanteLegFL = paso * 10.0f;
				elefanteLegBR = paso * 15.0f;
				elefanteLegFR = -paso * 10.0f;
				elefanteLegBL = -paso * 15.0f;

				// Trompa en movimiento natural
				elefanteTrompa = sin(t * 0.5f) * 5.0f;
				rotElefante = 90.0f;
				rotElefanteLado = 0.0f;
				elefantePos.y = -0.4f;
				elefantePos.z = -10.5f;  
			}

			// FASE 2: SE ACUESTA DE LADO (8s - 12s)
			else if (t < 12.0f)
			{
				float t2 = t - 8.0f;
				elefantePos.x = -6.0f;
				elefantePos.z = -10.5f; 

				// Rotación lateral se acuesta de lado
				rotElefanteLado = t2 * 22.5f;

				// SUBE de -0.4 a 0.0 mientras se acuesta
				elefantePos.y = -0.4f + (t2 * 0.1f);

				// Patas se mantienen
				elefanteLegFL = 0.0f;
				elefanteLegFR = 0.0f;
				elefanteLegBL = 0.0f;
				elefanteLegBR = 0.0f;

				// Trompa
				elefanteTrompa = 5.0f - (t2 * 1.25f);
				rotElefante = 90.0f;
			}

			// FASE FINAL: ACOSTADO DE LADO
			else
			{
				elefantePos.x = -6.0f;
				elefantePos.z = -10.5f;  //  CAMBIADO: Posición final cerca de la pared
				elefantePos.y = 0.0f;
				rotElefanteLado = 90.0f;

				// Patas
				elefanteLegFL = 0.0f;
				elefanteLegFR = 0.0f;
				elefanteLegBL = 0.0f;
				elefanteLegBR = 0.0f;

				// Respiración solo trompa
				elefanteTrompa = sin(glfwGetTime() * 0.5f) * 2.0f;
				rotElefante = 90.0f;
			}
		}



		// ---------------------------------------------------------------------------------
	// 							DIBUJO DE MODELOS DESIERTO (-x,z)
	// ---------------------------------------------------------------------------------

	// **** DIBUJO DEL PISO DESIERTO  Y COMPONENTES ****

		DibujarPiso(pisoArenaTextureID, glm::vec3(-7.25f, -0.49f, 7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);

		//  ************ Codigo comentado para no cargar todo desde el inicio se descomenta al final *******

		//// --- OASIS ---
		//model = glm::mat4(1);
		//model = glm::translate(model, oasisPos);
		//model = glm::scale(model, oasisScale);
		//model = glm::rotate(model, glm::radians(oasisRot), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Oasis.Draw(lightingShader);

		//// --- HUESOS ---
		//model = glm::mat4(1);
		//model = glm::translate(model, huesosPos);
		//model = glm::scale(model, huesosScale);
		//model = glm::rotate(model, glm::radians(huesosRot), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Huesos.Draw(lightingShader);

		//// --- TRONCO ---
		//model = glm::mat4(1);
		//model = glm::translate(model, troncoPos);
		//model = glm::scale(model, troncoScale);
		//model = glm::rotate(model, glm::radians(troncoRot), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Tronco.Draw(lightingShader);

		//// --- CACTUS ---
		//model = glm::mat4(1);
		//model = glm::translate(model, cactusPos);
		//model = glm::scale(model, cactusScale);
		//model = glm::rotate(model, glm::radians(cactusRot), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Cactus.Draw(lightingShader);

		//// **** DIBUJO DE ANIMALES DESIERTO ****

		////CAMELLO


		//if (animarCamello)
		//{
		//	float t = glfwGetTime() - startTimeCamello;

		//	// CAMINANDO HACIA EL CACTUS
		//	if (t < 8.0f)
		//	{
		//		// Movimiento de avance 
		//		float totalDist = 10.0f - 5.0f; // distancia = 5 unidades
		//		camelPos.z = 10.0f - (t * (totalDist / 8.0f)); // avanza desde Z=10 hasta Z=5

		//		// Movimiento de patas (alternadas)
		//		float paso = sin(t * 2.0f); // frecuencia lenta
		//		camelLegFL = paso * 15.0f;  // pata frontal izq
		//		camelLegBR = paso * 15.0f;  // pata trasera der
		//		camelLegFR = -paso * 15.0f; // pata frontal der (opuesta)
		//		camelLegBL = -paso * 15.0f; // pata trasera izq (opuesta)

		//		// Cabeza 
		//		camelHead = sin(t * 0.5f) * 1.3f; // leve movimiento
		//		rotCamel = 180.0f;
		//	}

		//	// FASE 2: LLEGA AL CACTUS
		//	else if (t < 14.0f)
		//	{
		//		float t2 = t - 8.0f;
		//		camelPos.z = 5.0f; // está en el cactus 

		//		// Detiene patas lentamente
		//		camelLegFL = sin(t2 * 1.0f) * 5.0f;
		//		camelLegFR = -camelLegFL;
		//		camelLegBL = -camelLegFR;
		//		camelLegBR = camelLegFR;

		//		// Movimiento de cabeza simulando comer
		//		camelHead = abs(sin(t2 * 1.5f)) * 2.9f; // baja y sube lento
		//		rotCamel = 180.0f;
		//	}

		//	
		//	else
		//	{
		//		camelPos.z = 5.0f;
		//		camelHead = 0.0f;
		//		camelLegFL = camelLegFR = camelLegBL = camelLegBR = 0.0f;
		//		rotCamel = 180.0f;
		//	}
		//}


		//model = glm::mat4(1);
		//model = glm::translate(model, camelPos);
		//model = glm::rotate(model, glm::radians(rotCamel), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(camelScale));
		//modelTemp = model;
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//CamelBody.Draw(lightingShader);

		//// Cabeza
		//model = modelTemp;
		//model = glm::rotate(model, glm::radians(camelHead), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//CamelHead.Draw(lightingShader);

		//// Pierna delantera izquierda
		//glm::vec3 camelPivotFL(0.3f, 1.2f, 0.5f);
		//model = modelTemp;
		//model = glm::translate(model, camelPivotFL);
		//model = glm::rotate(model, glm::radians(camelLegFL), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, -camelPivotFL);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//CamelLeg_FL.Draw(lightingShader);

		//// Pierna delantera derecha
		//glm::vec3 camelPivotFR(-0.3f, 1.2f, 0.5f);
		//model = modelTemp;
		//model = glm::translate(model, camelPivotFR);
		//model = glm::rotate(model, glm::radians(camelLegFR), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, -camelPivotFR);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//CamelLeg_FR.Draw(lightingShader);

		//// Pierna trasera izquierda
		//glm::vec3 camelPivotBL(0.3f, 1.2f, -0.5f);
		//model = modelTemp;
		//model = glm::translate(model, camelPivotBL);
		//model = glm::rotate(model, glm::radians(camelLegBL), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, -camelPivotBL);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//CamelLeg_BL.Draw(lightingShader);

		//// Pierna trasera derecha
		//glm::vec3 camelPivotBR(-0.3f, 1.2f, -0.5f);
		//model = modelTemp;
		//model = glm::translate(model, camelPivotBR);
		//model = glm::rotate(model, glm::radians(camelLegBR), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, -camelPivotBR);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//CamelLeg_BR.Draw(lightingShader);



		////CONDOR

		//if (animarCondor)
		//{
		//	float t = glfwGetTime();
		//	condorAlaIzq = sin(t * 10.0f) * 1.5f;
		//	condorAlaDer = -condorAlaIzq;
		//	condorHead = sin(t * 8.0f) * 1.0f;
		//	condorPos.y = 0.7f + sin(t * 0.8f) * 0.15f;
		//}


		//model = glm::mat4(1);
		//model = glm::translate(model, condorPos);
		//model = glm::rotate(model, glm::radians(rotCondor), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(condorScale));
		//modelTemp = model;
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//CondorBody.Draw(lightingShader);

		//// Cabeza
		//model = modelTemp;
		//model = glm::rotate(model, glm::radians(condorHead), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//CondorHead.Draw(lightingShader);

		//// Ala izquierda
		//glm::vec3 condorPivotAlaIzq(0.5f, 0.5f, 0.0f);
		//model = modelTemp;
		//model = glm::translate(model, condorPivotAlaIzq);
		//model = glm::rotate(model, glm::radians(condorAlaIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, -condorPivotAlaIzq);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//CondorAla_Izq.Draw(lightingShader);

		//// Ala derecha
		//glm::vec3 condorPivotAlaDer(-0.5f, 0.5f, 0.0f);
		//model = modelTemp;
		//model = glm::translate(model, condorPivotAlaDer);
		//model = glm::rotate(model, glm::radians(condorAlaDer), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, -condorPivotAlaDer);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//CondorAla_Der.Draw(lightingShader);

		////TORTUGA
		//if (animarTortuga)
		//{
		//	float t = glfwGetTime() - startTimeTortuga; // tiempo de animación

		//	//FASE 1: Camina hacia el agua
		//	if (t < 2.5f)
		//	{
		//		tortugaPos.x = -7.8f - (t * 0.08f);
		//		tortugaPos.y = -0.18f;
		//		rotTortuga = 0.0f;
		//		tortugaScale = 0.20f;
		//	}
		//	//FASE 2: Empieza a sumergirse
		//	else if (t < 6.0f)
		//	{
		//		float t2 = t - 2.5f;
		//		tortugaPos.x = -8.0f - (t2 * 0.4f);    // avanza en el agua 
		//		tortugaPos.y = -0.18f - (t2 * 0.07f);  // baja más suave
		//		tortugaScale = 0.20f - (t2 * 0.012f);  // se achica 
		//		rotTortuga = sin(t2 * 0.5f) * 5.0f;
		//	}
		//	//FASE 3: Sale del agua
		//	else if (t < 9.0f)
		//	{
		//		float t3 = t - 6.0f;
		//		tortugaPos.x = -9.4f - (t3 * 0.7f);              // sale del agua
		//		tortugaPos.y = -0.425f + (t3 * 0.0483f);
		//		tortugaScale = 0.19f + (t3 * 0.0033f);
		//		rotTortuga = t3 * 60.0f;                         // empieza a girar 
		//	}
		//	//FASE FINAL: gira
		//	else
		//	{
		//		tortugaPos.x = -11.5f;
		//		tortugaPos.y = -0.28f;
		//		tortugaScale = 0.20f;
		//		rotTortuga = 180.0f;
		//	}
		//}

		//model = glm::mat4(1);
		//model = glm::translate(model, tortugaPos);
		//model = glm::rotate(model, glm::radians(rotTortuga), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(tortugaScale));
		//modelTemp = model;
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//TortugaBody.Draw(lightingShader);


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

		//// Pata delantera izquierda
		//glm::vec3 tortugaPivotFL(0.2f, 0.3f, 0.3f);
		//model = modelTemp;
		//model = glm::translate(model, tortugaPivotFL);
		//model = glm::rotate(model, glm::radians(tortugaLegFL), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, -tortugaPivotFL);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//TortugaLeg_FL.Draw(lightingShader);

		//// Pata delantera derecha
		//glm::vec3 tortugaPivotFR(-0.2f, 0.3f, 0.3f);
		//model = modelTemp;
		//model = glm::translate(model, tortugaPivotFR);
		//model = glm::rotate(model, glm::radians(tortugaLegFR), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, -tortugaPivotFR);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//TortugaLeg_FR.Draw(lightingShader);

		lightingShader.Use(); // shader de iluminación 

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}


// --- Función para configurar VAO/VBO genérico ---
void ConfigurarVAO(GLuint& VAO, GLuint& VBO, float* vertices, size_t size)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	// Atributo de Posición (Location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Atributo de Normal (Location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Atributo de Coordenadas de Textura (Location 2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
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

	//ELEFANTE
	if (keys[GLFW_KEY_V])
	{
		if (!teclaV_presionada)
		{
			animarElefante = !animarElefante;
			startTimeElefante = glfwGetTime();
			teclaV_presionada = true;
		}
	}
	else
	{
		teclaV_presionada = false;
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

	//CONDOR
	if (keys[GLFW_KEY_Z])
	{
		if (!teclaZ_presionada)
		{
			animarCondor = !animarCondor;
			teclaZ_presionada = true;
		}
	}
	else
	{
		teclaZ_presionada = false;
	}

	//TORTUGA
	if (keys[GLFW_KEY_X])
	{
		if (!teclaX_presionada) // solo la primera vez que se presiona
		{
			animarTortuga = !animarTortuga;       
			startTimeTortuga = glfwGetTime();     // reinicia animación
			teclaX_presionada = true;             // evita repeticiones
		}
	}
	else
	{
		teclaX_presionada = false; // se suelta la tecla
	}
	
	//CAPIBARA
	if (keys[GLFW_KEY_B])
	{
		if (!teclaP_presionada) // Reutilizas la variable que ya tienes
		{
			animarCapibara = !animarCapibara;
			startTimeCapibara = glfwGetTime();
			teclaP_presionada = true;
		}
	}
	else
	{
		teclaP_presionada = false;
	}

	//MONO
	if (keys[GLFW_KEY_M])
	{
		if (!teclaM_presionada)
		{
			animarMono = !animarMono;
			startTimeMono = glfwGetTime();
			teclaM_presionada = true;
		}
	}
	else
	{
		teclaM_presionada = false;
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