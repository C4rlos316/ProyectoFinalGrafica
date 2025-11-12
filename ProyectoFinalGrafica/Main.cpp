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
//Skybox
//#include "Texture.h"
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
bool teclaTAB_presionada = false; // Para cambiar de cámara
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


glm::vec3 pinguBasePos = glm::vec3(7.25f, 0.5f, -9.8f); // El centro de la roca
glm::vec3 pinguPos = pinguBasePos; 
float pinguWalkDist = 0.5f; // Caminará 0.5 unidades hacia adelante y 0.5 hacia atrás
// --- FIN MODIFICADO ---

float pinguRotY = 0.0f; 
float pinguScale = 0.4f; 
// --- TORTUGA ACUARIO  ---
float rotTortugaAcuarioHead = 0.0f;
float rotTortugaAcuarioFL = 0.0f;
float rotTortugaAcuarioFR = 0.0f;
float rotTortugaAcuarioBL = 0.0f;
float rotTortugaAcuarioBR = 0.0f;
float rotTortugaAcuarioCuerpo = 0.0f; 


glm::vec3 tortugaBasePos = glm::vec3(7.25f, -0.45f, -4.625f); 
glm::vec3 tortugaAcuarioPos = tortugaBasePos; 
float tortugaSwimDist = 4.5f; 
// --- FIN MODIFICADO ---
float tortugaAcuarioRotY = 0.0f; // Rotación de la animación
float tortugaAcuarioScale = 0.01f;
bool animarTortugaAcuario = false;
bool teclaT_presionada = false; //tecla 'T'
float startTimeTortugaAcuario = 0.0f;
// --- FIN NUEVO ---

// ---  NUTRIA ACUARIO ---
float rotNutriaHead = 0.0f;
float rotNutriaCola = 0.0f;
float rotNutriaFL = 0.0f;
float rotNutriaFR = 0.0f;
float rotNutriaBL = 0.0f;
float rotNutriaBR = 0.0f;
float rotNutriaCuerpoX = 0.0f; // Para el giro en el clavado
float nutriaRotY = 0.0f; // Para la dirección
// Posiciones de la animación (¡AJUSTA ESTAS COORDENADAS!)
glm::vec3 nutriaPos1_Roca = glm::vec3(5.5f, 0.3f, -9.8f);   // Pos 1: Sobre la roca

// --- ¡MODIFICADO! (Arreglo 2) ---
// Sube el valor Y (segundo número) para que caiga ENCIMA del iglú, no adentro.
glm::vec3 nutriaPos = glm::vec3(5.5f, 0.2f, -9.8f);   // Pos 1: Sobre la roca
glm::vec3 nutriaPos2_Iglu = glm::vec3(11.2f, 1.0f, -9.0f);  // Pos 2: Sobre el iglú (Alto)

// --- ¡MODIFICADO! (Arreglo 2: Clavado) ---
// Bajamos la 'Y' final para que termine BAJO el agua
glm::vec3 nutriaPos3_Agua = glm::vec3(10.5f, -1.0f, -6.5f); //
float nutriaScale = 0.01f; // <-- ¡AJUSTA ESTA ESCALA!
bool animarNutria = false;
bool teclaN_presionada = false; // Usaremos la tecla 'N'
float startTimeNutria = 0.0f;
// --- FIN NUTRIA ---

// -----------------------------------------
//  DESIERTO
// -----------------------------------------

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

// GUACAMAYA (Cuadrante X, Z)
float guacamayaScale = 1.0f;
float rotGuacamaya = 270.0f;
float guacamayaAlaDer = 0.0f;
float guacamayaAlaIzq = 0.0f;
glm::vec3 guacamayaPos = glm::vec3(11.1f, 1.55f, 6.5f); 
bool animarGuacamaya = false;
float startTimeGuacamaya = 0.0f;
bool teclaO_presionada = false;                                                                                                                   

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

// JIRAFA (Cuadrante -X, -Z)
float jirafaScale = 0.35f;
float rotJirafa = 0.0f;
float jirafaCabezaRot = 0.0f;
float jirafaColaRot = 0.0f;
float jirafaPataDelDer = 0.0f;
float jirafaPataDelIzq = 0.0f;
float jirafaPataTrasDer = 0.0f;
float jirafaPataTrasIzq = 0.0f;
glm::vec3 jirafaPos = glm::vec3(-10.0f, 0.7f, -10.0f);
bool animarJirafa = false;
float startTimeJirafa = 0.0f;
bool teclaJ_presionada = false;

// --- CEBRA (-X,-Z) ---
float cebraScale = 0.027f;
float rotCebra = 180.0f; // Empieza mirando hacia abajo (-Z)
float cebraPataDelDer = 0.0f;
float cebraPataDelIzq = 0.0f;
float cebraPataTrasDer = 0.0f;
float cebraPataTrasIzq = 0.0f;
glm::vec3 cebraPos = glm::vec3(-2.8f, -0.4f, -3.5f); // Esquina cerca de la entrada
bool animarCebra = false;
float startTimeCebra = 0.0f;
bool teclaL_presionada = false;

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

	//Skybox
	
	////Skybox
//Shader skyboxShader("Shader/skybox.vs", "Shader/skybox.frag");
//// Vértices de cubo del skybox
//float skyboxVertices[] = {
//	-1.0f,  1.0f, -1.0f,
//	-1.0f, -1.0f, -1.0f,
//	 1.0f, -1.0f, -1.0f,
//	 1.0f, -1.0f, -1.0f,
//	 1.0f,  1.0f, -1.0f,
//	-1.0f,  1.0f, -1.0f,
//	-1.0f, -1.0f,  1.0f,
//	-1.0f, -1.0f, -1.0f,
//	-1.0f,  1.0f, -1.0f,
//	-1.0f,  1.0f, -1.0f,
//	-1.0f,  1.0f,  1.0f,
//	-1.0f, -1.0f,  1.0f,
//	 1.0f, -1.0f, -1.0f,
//	 1.0f, -1.0f,  1.0f,
//	 1.0f,  1.0f,  1.0f,
//	 1.0f,  1.0f,  1.0f,
//	 1.0f,  1.0f, -1.0f,
//	 1.0f, -1.0f, -1.0f,
//	-1.0f, -1.0f,  1.0f,
//	-1.0f,  1.0f,  1.0f,
//	 1.0f,  1.0f,  1.0f,
//	 1.0f,  1.0f,  1.0f,
//	 1.0f, -1.0f,  1.0f,
//	-1.0f, -1.0f,  1.0f,
//	-1.0f,  1.0f, -1.0f,
//	 1.0f,  1.0f, -1.0f,
//	 1.0f,  1.0f,  1.0f,
//	 1.0f,  1.0f,  1.0f,
//	-1.0f,  1.0f,  1.0f,
//	-1.0f,  1.0f, -1.0f,
//	-1.0f, -1.0f, -1.0f,
//	-1.0f, -1.0f,  1.0f,
//	 1.0f, -1.0f, -1.0f,
//	 1.0f, -1.0f, -1.0f,
//	-1.0f, -1.0f,  1.0f,
//	 1.0f, -1.0f,  1.0f
//};
// 
	// =================================================================================
	// 						CARGA DE MODELOS - Acuario (X,-Z)
	// =================================================================================


	// =================================================================================
	// 						CARGA DE MODELO - Personaje camara
	// =================================================================================
	Model PersonajeAlex((char*)"Models/alex_leon/alex_leon.obj");

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
	// tortuga2Acuario
	Model TortugaModel((char*)"Models/tortuga2/tortuga1.obj");
	Model TortugaBody((char*)"Models/tortuga2/tortuga1cuerpo.obj");
	Model TortugaPataFL((char*)"Models/tortuga2/tortuga1patadelanteraizquierda.obj");
	Model TortugaPataFR((char*)"Models/tortuga2/tortuga1patadelanteraderecha.obj");
	Model TortugaPataBL((char*)"Models/tortuga2/tortuga1patatraseraizquierda.obj");
	Model TortugaPataBR((char*)"Models/tortuga2/tortuga1patatraseraderecha.obj");
	// --- Cargar Nutria ---
	
	Model NutriaBody((char*)"Models/nutriaacuario/nutriabody.obj");
	Model NutriaHead((char*)"Models/nutriaacuario/nutriahead.obj");
	Model NutriaCola((char*)"Models/nutriaacuario/nutriacola.obj");
	Model NutriaPataFL((char*)"Models/nutriaacuario/nutriapatadelanteraizquierda.obj");
	Model NutriaPataFR((char*)"Models/nutriaacuario/nutriapatadelanteraderecha.obj");
	Model NutriaPataBL((char*)"Models/nutriaacuario/nutriapatatraseraizquierda.obj");
	Model NutriaPataBR((char*)"Models/nutriaacuario/nutriapatatraseraderecha.obj");
	// --- FIN NUEVO ---
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

	Model Pelota((char*)"Models/pelota/pelota.obj");
	glm::vec3 pelotaPos(5.5f, -0.2f, 11.0f);
	glm::vec3 pelotaScale(0.5f, 0.5f, 0.5f);
	float pelotaRot = 0.0f;

	Model Platano1((char*)"Models/platano/platano.obj");
	glm::vec3 platano1Pos(11.0f, -0.4f, 11.8f);
	glm::vec3 platano1Scale(5.0f, 5.0f, 5.0f);
	float platano1Rot = 0.0f;

	Model Platano2((char*)"Models/platano/platano.obj");
	glm::vec3 platano2Pos(7.6f, -0.4f, 9.8f);
	glm::vec3 platano2Scale(5.0f, 5.0f, 5.0f);
	float platano2Rot = 0.0f;

	Model Gato((char*)"Models/platano/gato.obj");
	glm::vec3 gatoPos(3.2f, 0.0f, 11.9f);
	glm::vec3 gatoScale(0.8f, 0.8f, 0.8f);
	float gatoRot = 90.0f;

	Model ArbolRama((char*)"Models/arbolRama/arbolRama.obj");
	glm::vec3 arbolRamaPos(11.0f, 0.6f, 6.0f);
	glm::vec3 arbolRamaScale(2.5f, 2.5f, 2.5f);
	float arbolRamaRot = 0.0f;

	Model RamaSelva((char*)"Models/ramaSelva/ramaSelva.obj");
	glm::vec3 ramaSelvaPos(3.0f, 0.15f, 6.0f);
	glm::vec3 ramaSelvaScale(2.5f, 2.5f, 4.0f);
	float ramaSelvaRot = 0.0f;

	Model PlantaSelva1((char*)"Models/plantaSelva/planta_selva.obj");
	glm::vec3 plantaSelva1Pos(8.5f, -0.3f, 3.1f);
	glm::vec3 plantaSelva1Scale(0.4f, 0.4f, 0.4f);
	float plantaSelva1Rot = 0.0f;

	Model PlantaSelva2((char*)"Models/plantaSelva/planta_selva.obj");
	glm::vec3 plantaSelva2Pos(11.0f, -0.3f, 9.5f);
	glm::vec3 plantaSelva2Scale(0.4f, 0.4f, 0.4f);
	float plantaSelva2Rot = 0.0f;
	
	Model Loto1((char*)"Models/loto/loto.obj");
	glm::vec3 loto1Pos(6.5f, -0.21f, 3.1f);
	glm::vec3 loto1Scale(1.0f, 1.0f, 1.0f);
	float loto1Rot = 0.0f;

	Model Loto2((char*)"Models/loto/loto.obj");
	glm::vec3 loto2Pos(5.0f, -0.21f, 3.1f);
	glm::vec3 loto2Scale(1.0f, 1.0f, 1.0f);
	float loto2Rot = 0.0f;

	Model Loto3((char*)"Models/loto/loto.obj");
	glm::vec3 loto3Pos(3.5f, -0.21f, 3.1f);
	glm::vec3 loto3Scale(1.0f, 1.0f, 1.0f);
	float loto3Rot = 0.0f;

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

	// ====== GUACAMAYA ======
	Model Ave_Cuerpo((char*)"Models/aveSelva/cuerpoAve.obj");
	Model Ave_AlaDer((char*)"Models/aveSelva/alaDerAve.obj");
	Model Ave_AlaIzq((char*)"Models/aveSelva/alaIzqAve.obj");

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
	// 						CARGA DE MODELOS - ENTRADA // ADORNOS
	// =================================================================================
	Model LetreroZoo((char*)"Models/adornos/zooletre.obj");
	Model BancaModel((char*)"Models/adornos/banca.obj");
	// =================================================================================
	// 						CARGA DE MODELOS - Sabana (-X,-Z)
	// =================================================================================

	Model ArbolSabana((char*)"Models/arbolSabana/arbol.obj");
	glm::vec3 arbolSabanaPos(-11.0f, 1.0f, -3.2f);
	glm::vec3 arbolSabanaScale(3.5f, 3.5f, 3.5f);
	float arbolSabanaRot = 0.0f;
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

	Model Jirafa_Cabeza((char*)"Models/jirafa/cabezaJirafa.obj");
	Model Jirafa_Cola((char*)"Models/jirafa/colaJirafa.obj");
	Model Jirafa_Cuerpo((char*)"Models/jirafa/cuerpoJirafa.obj");
	Model Jirafa_PataDelDer((char*)"Models/jirafa/pataDelDerJirafa.obj");
	Model Jirafa_PataDelIzq((char*)"Models/jirafa/pataDelIzqJirafa.obj");
	Model Jirafa_PataTrasDer((char*)"Models/jirafa/pataTrasDerJirafa.obj");
	Model Jirafa_PataTrasIzq((char*)"Models/jirafa/pataTrasIzqJirafa.obj");

	//CEBRA
	Model Cebra_Cuerpo((char*)"Models/cebra/cebra_cuerpo.obj");
	Model Cebra_PataDelDer((char*)"Models/cebra/cebra_pata_der_enfr.obj");
	Model Cebra_PataDelIzq((char*)"Models/cebra/cebra_pata_izq_enfr.obj");
	Model Cebra_PataTrasDer((char*)"Models/cebra/cebra_pata_der_atras.obj");
	Model Cebra_PataTrasIzq((char*)"Models/cebra/cebra_pata_izq_atras.obj");

	// =================================================================================
    // 						CARGA DE MODELOS - ENTRADA
	// =================================================================================

	Model Taquilla((char*)"Models/taquilla/taquilla.obj");
	glm::vec3 taquillaPos(3.5f, 0.4f, 14.5f);
	glm::vec3 taquillaScale(3.5f, 3.0f, 3.5f);
	float taquillaRot = 270.0f;

	Model Kitty((char*)"Models/hellokitty/hellokitty.obj");
	glm::vec3 kittyPos(-2.5f, 0.4f, 14.5f);
	glm::vec3 kittyScale(1.5f, 1.5f, 1.5f);
	float kittyRot = 270.0f;

	Model Monito((char*)"Models/monito/monito.obj");
	glm::vec3 monitoPos(-5.0f, 0.4f, 14.5f);
	glm::vec3 monitoScale(1.5f, 1.5f, 1.5f);
	float monitoRot = 270.0f;

	Model CDMX((char*)"Models/cdmx/cdmx.obj");
	glm::vec3 cdmxPos(-9.0f, 0.4f, 20.0f);
	glm::vec3 cdmxScale(5.0f, 5.0f, 4.0f);
	float cdmxRot = 270.0f;

	Model Carrusel((char*)"Models/carrusel/carrusel.obj");
	glm::vec3 carruselPos(9.0f, 1.0f, 19.0f);
	glm::vec3 carruselScale(4.0f, 3.0f, 3.5f);
	float carruselRot = 0.0f;

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

	////SKYBOX
//GLuint skyboxVBO, skyboxVAO;
//glGenVertexArrays(1, &skyboxVAO);
//glGenBuffers(1, &skyboxVBO);
//glBindVertexArray(skyboxVAO);
//glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
//glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
//glEnableVertexAttribArray(0);
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//vector <const GLchar*> faces;
//faces.push_back("images/skybox/right.jpg");
//faces.push_back("images/skybox/left.jpg");
//faces.push_back("images/skybox/top.jpg");
//faces.push_back("images/skybox/bottom.jpg");
//faces.push_back("images/skybox/back.jpg");
//faces.push_back("images/skybox/front.jpg");
//GLuint cubeMapTexture = TextureLoading::LoadCubemap(faces);

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

		// =================================================================================
		// 							DIBUJO DE MODELOS - ENTRADA
		// =================================================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.1f, 2.8f, 8.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LetreroZoo.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LetreroZoo.Draw(lightingShader);

		// =================================================================================
		// 							DIBUJO DE MODELOS - BANCAS
		// =================================================================================
		// --- Banca 1 (Fondo) ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -11.7f)); 
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BancaModel.Draw(lightingShader);

		// --- Banca 2 (Camino Izquierdoo) ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-11.5f, -0.5f, 0.0f)); 
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BancaModel.Draw(lightingShader);

		// --- Banca 3 (Camino Derechoo) ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(11.5f, -0.5f, 0.0f)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BancaModel.Draw(lightingShader);
		// --- FIN DE BANCAS ---
	
		
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

		// Dibujar personaje en tercera persona
		if (camera.GetCameraType() == THIRD_PERSON)
		{
			glm::vec3 personPos = camera.GetPosition();

			// --- Calcular rotación basada en la dirección de la cámara ---
			glm::vec3 cameraFront = camera.GetFront();
			float yawAngle = glm::degrees(atan2(cameraFront.x, cameraFront.z)); // Ángulo en grados

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(personPos.x, personPos.y - 1.4f, personPos.z));

			// --Rotar según la dirección de la cámara ---
			model = glm::rotate(model, glm::radians(yawAngle), glm::vec3(0.0f, 1.0f, 0.0f));

			model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			PersonajeAlex.Draw(lightingShader);
		}


		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE ESCENARIO ACUARIO (x,-z)
		// ---------------------------------------------------------------------------------

		
		// **** DIBUJOS DEL PISO Y ACCESORIOS DE ACUARIO ****
		// El cuadrante total es: centro(7.25f, -0.49f, -7.25f), escala(10.5f, 0.1f, 10.5f)

		// 1. Mitad trasera (Piedra)
		
		
		DibujarPiso(pisoPiedraTextureID, glm::vec3(7.25f, -0.49f, -9.875f), glm::vec3(10.5f, 0.1f, 5.25f), VAO_Cubo, modelLoc);

		// 2. Mitad delantera (Agua)
		
		DibujarPiso(pisoAguaTextureID, glm::vec3(7.25f, -0.49f, -4.625f), glm::vec3(10.5f, 0.1f, 5.25f), VAO_Cubo, modelLoc);


		// ---  DIBUJAR EL FONDO DEL ACUARIO ---
		model = glm::mat4(1.0f);
		// El centro X del cuadrante es 7.25f
		// El fondo Z del cuadrante es -7.25f - (10.5f / 2) = -12.5f
		model = glm::translate(model, glm::vec3(5.25f, -0.5f, -12.5f)); 
		model = glm::scale(model, glm::vec3(2.2f, 2.0f, 1.5f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		EscenarioAcuario.Draw(lightingShader);
		// --- DIBUJAR TORTUGA ACUARIO ---

		// Shell (Cáscara/Cuerpo principal)
		model = glm::mat4(1.0f);
		model = glm::translate(model, tortugaAcuarioPos); 
		model = glm::rotate(model, glm::radians(tortugaAcuarioRotY), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(tortugaAcuarioScale));
		modelTemp = model; 
		model = glm::rotate(model, glm::radians(rotTortugaAcuarioCuerpo), glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
		TortugaModel.Draw(lightingShader);

		// Body (Cabeza y cuello)
		glm::vec3 pivoteTortugaHead(0.0f, 0.1f, 0.2f); 
		model = modelTemp;
		model = glm::translate(model, pivoteTortugaHead);
		model = glm::rotate(model, glm::radians(rotTortugaAcuarioHead), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, -pivoteTortugaHead);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TortugaBody.Draw(lightingShader);

		// Pata Delantera Izquierda
		
		glm::vec3 pivoteTortugaFL(0.2f, 0.0f, 0.1f); 
		model = modelTemp;
		model = glm::translate(model, pivoteTortugaFL);
		model = glm::rotate(model, glm::radians(rotTortugaAcuarioFL), glm::vec3(1.0f, 0.0f, 1.0f)); 
		model = glm::translate(model, -pivoteTortugaFL);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TortugaPataFL.Draw(lightingShader);

		// Pata Delantera Derecha
		
		glm::vec3 pivoteTortugaFR(-0.2f, 0.0f, 0.1f); 
		model = modelTemp;
		model = glm::translate(model, pivoteTortugaFR);
		model = glm::rotate(model, glm::radians(rotTortugaAcuarioFR), glm::vec3(1.0f, 0.0f, -1.0f)); 
		model = glm::translate(model, -pivoteTortugaFR);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TortugaPataFR.Draw(lightingShader);

		// Pata Trasera Izquierda
		
		glm::vec3 pivoteTortugaBL(0.15f, 0.0f, -0.2f); 
		model = modelTemp;
		model = glm::translate(model, pivoteTortugaBL);
		model = glm::rotate(model, glm::radians(rotTortugaAcuarioBL), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::translate(model, -pivoteTortugaBL);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TortugaPataBL.Draw(lightingShader);

		// Pata Trasera Derecha
		
		glm::vec3 pivoteTortugaBR(-0.15f, 0.0f, -0.2f); 
		model = modelTemp;
		model = glm::translate(model, pivoteTortugaBR);
		model = glm::rotate(model, glm::radians(rotTortugaAcuarioBR), glm::vec3(1.0f, 0.0f, -1.0f));
		model = glm::translate(model, -pivoteTortugaBR);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TortugaPataBR.Draw(lightingShader);
		// --- FIN BLOQUE TORTUGA ---
		
		// --- ¡NUEVO! DIBUJAR NUTRIA ---
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, nutriaPos);
		model = glm::rotate(model, glm::radians(nutriaRotY), glm::vec3(0.0f, 1.0f, 0.0f)); // Dirección
		model = glm::scale(model, glm::vec3(nutriaScale));

		// Aplicamos el giro del clavado (rotNutriaCuerpoX) a la matriz 'model'
		// ANTES de guardarla en modelTemp. Así, todas las partes heredarán el giro.
		model = glm::rotate(model, glm::radians(rotNutriaCuerpoX), glm::vec3(1.0f, 0.0f, 0.0f));
		
		modelTemp = model; // Guardar matriz base (AHORA CON EL GIRO DEL CLAVADO)
		
		// Dibujar Cuerpo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		NutriaBody.Draw(lightingShader);

		// Cabeza
		glm::vec3 pivoteNutriaHead(0.0f, 0.2f, 0.3f); // <-- ¡AJUSTA ESTE PIVOTE!
		model = modelTemp; // Usa la matriz base (CON giro)
		model = glm::translate(model, pivoteNutriaHead);
		model = glm::rotate(model, glm::radians(rotNutriaHead), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, -pivoteNutriaHead);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		NutriaHead.Draw(lightingShader);

		// Cola
		glm::vec3 pivoteNutriaCola(0.0f, 0.1f, -0.4f); // <-- ¡AJUSTA ESTE PIVOTE!
		model = modelTemp;
		model = glm::translate(model, pivoteNutriaCola);
		model = glm::rotate(model, glm::radians(rotNutriaCola), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -pivoteNutriaCola);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		NutriaCola.Draw(lightingShader);
		
		// --- ¡MODIFICADO! (Arreglo 1: Patas desfasadas) ---
		// Subí el valor Y (segundo número) de los pivotes para pegarlos al cuerpo.
		
		// Pata Delantera Izquierda
		glm::vec3 pivoteNutriaFL(0.1f, 0.6f, 0.2f); // <-- ¡AJUSTA! (Subí Y de 0.3 a 0.4)
		model = modelTemp;
		model = glm::translate(model, pivoteNutriaFL);
		model = glm::rotate(model, glm::radians(rotNutriaFL), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -pivoteNutriaFL);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		NutriaPataFL.Draw(lightingShader);

		// Pata Delantera Derecha
		glm::vec3 pivoteNutriaFR(-0.1f, 0.6f, 0.2f); // <-- ¡AJUSTA! (Subí Y de 0.3 a 0.4)
		model = modelTemp;
		model = glm::translate(model, pivoteNutriaFR);
		model = glm::rotate(model, glm::radians(rotNutriaFR), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -pivoteNutriaFR);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		NutriaPataFR.Draw(lightingShader);

		// Pata Trasera Izquierda
		glm::vec3 pivoteNutriaBL(0.1f, 0.3f, -0.3f); // <-- (Y es 0.3)
		model = modelTemp;
		model = glm::translate(model, pivoteNutriaBL);
		model = glm::rotate(model, glm::radians(rotNutriaBL), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -pivoteNutriaBL);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		NutriaPataBL.Draw(lightingShader);

		// Pata Trasera Derecha
		glm::vec3 pivoteNutriaBR(-0.1f, 0.3f, -0.3f); // <-- (Y es 0.3)
		model = modelTemp;
		model = glm::translate(model, pivoteNutriaBR);
		model = glm::rotate(model, glm::radians(rotNutriaBR), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -pivoteNutriaBR);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		NutriaPataBR.Draw(lightingShader);
		// --- FIN BLOQUE NUTRIA ---
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

		// --- PELOTA ---
		model = glm::mat4(1);
		model = glm::translate(model, pelotaPos);
		model = glm::scale(model, pelotaScale);
		model = glm::rotate(model, glm::radians(pelotaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pelota.Draw(lightingShader);

		// --- PLATANO 1 ---
		model = glm::mat4(1);
		model = glm::translate(model, platano1Pos);
		model = glm::scale(model, platano1Scale);
		model = glm::rotate(model, glm::radians(platano1Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Platano1.Draw(lightingShader);

		// --- PLATANO 2 ---
		model = glm::mat4(1);
		model = glm::translate(model, platano2Pos);
		model = glm::scale(model, platano2Scale);
		model = glm::rotate(model, glm::radians(platano2Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Platano2.Draw(lightingShader);

		// --- GATO ---
		model = glm::mat4(1);
		model = glm::translate(model, gatoPos);
		model = glm::scale(model, gatoScale);
		model = glm::rotate(model, glm::radians(gatoRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Gato.Draw(lightingShader);

		// --- ÁRBOL MUERTO ---
		model = glm::mat4(1);
		model = glm::translate(model, arbolRamaPos);
		model = glm::scale(model, arbolRamaScale);
		model = glm::rotate(model, glm::radians(arbolRamaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ArbolRama.Draw(lightingShader);

		// --- RAMA ---
		model = glm::mat4(1);
		model = glm::translate(model, ramaSelvaPos);
		model = glm::scale(model, ramaSelvaScale);
		model = glm::rotate(model, glm::radians(ramaSelvaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RamaSelva.Draw(lightingShader);

		//--- PLANTA 1 ---
		model = glm::mat4(1);
		model = glm::translate(model, plantaSelva1Pos);
		model = glm::scale(model, plantaSelva1Scale);
		model = glm::rotate(model, glm::radians(plantaSelva1Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PlantaSelva1.Draw(lightingShader);

		//--- PLANTA 2 ---
		model = glm::mat4(1);
		model = glm::translate(model, plantaSelva2Pos); 
		model = glm::scale(model, plantaSelva2Scale);
		model = glm::rotate(model, glm::radians(plantaSelva2Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PlantaSelva2.Draw(lightingShader);

		// --- LOTO 1 ---
		model = glm::mat4(1);
		model = glm::translate(model, loto1Pos);
		model = glm::scale(model, loto1Scale);
		model = glm::rotate(model, glm::radians(loto1Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Loto1.Draw(lightingShader);

		// --- LOTO 2 ---
		model = glm::mat4(1);
		model = glm::translate(model, loto2Pos);
		model = glm::scale(model, loto2Scale);
		model = glm::rotate(model, glm::radians(loto2Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Loto2.Draw(lightingShader);

		// --- LOTO 3 ---
		model = glm::mat4(1);
		model = glm::translate(model, loto3Pos);
		model = glm::scale(model, loto3Scale);
		model = glm::rotate(model, glm::radians(loto3Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Loto3.Draw(lightingShader);

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
				monoPos.x = 3.f;
				monoPos.y = 0.0f;
				monoColaRot = 0.0f;
				monoPataDelDer = monoPataDelIzq = 0.0f;
				monoPataTrasDer = monoPataTrasIzq = 0.0f;
				rotMono = 180.0f;
			}
		}

		// **** DIBUJO DE LA GUACAMAYA ****
		model = glm::mat4(1);
		model = glm::translate(model, guacamayaPos);
		model = glm::rotate(model, glm::radians(rotGuacamaya), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(guacamayaScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ave_Cuerpo.Draw(lightingShader);

		// Ala derecha
		glm::vec3 guacamayaPivotAlaDer(0.3f, 0.0f, 0.0f);
		model = modelTemp;
		model = glm::translate(model, guacamayaPivotAlaDer);
		model = glm::rotate(model, glm::radians(guacamayaAlaDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -guacamayaPivotAlaDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ave_AlaDer.Draw(lightingShader);

		// Ala izquierda
		glm::vec3 guacamayaPivotAlaIzq(-0.3f, 0.0f, 0.0f);
		model = modelTemp;
		model = glm::translate(model, guacamayaPivotAlaIzq);
		model = glm::rotate(model, glm::radians(guacamayaAlaIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -guacamayaPivotAlaIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ave_AlaIzq.Draw(lightingShader);

		// GUACAMAYA - ANIMACIÓN
		if (animarGuacamaya)
		{
			float t = glfwGetTime() - startTimeGuacamaya;

			// FASE 1: Volando (8 segundos)
			if (t < 8.0f)
			{
				// Vuelo en línea recta
				guacamayaPos.x = 11.1f - (t * 1.0f);
				guacamayaPos.y = 1.55f;
				guacamayaPos.z = 6.5f;

				// Aleteo suave
				float aleteo = sin(t * 2.5f);
				guacamayaAlaDer = aleteo * 5.0f;
				guacamayaAlaIzq = -aleteo * 5.0f;
				rotGuacamaya = 270.0f;
			}
			// FASE 2: Descendiendo hacia la rama (4 segundos)
			else if (t < 12.0f)
			{
				float t2 = t - 8.0f;
				guacamayaPos.x = 3.1f - (t2 * 0.025f);
				guacamayaPos.y = 1.55f - (t2 * 0.2625f);
				guacamayaPos.z = 6.5f;

				// Alas casi quietas al planear
				guacamayaAlaDer = 3.0f;
				guacamayaAlaIzq = -3.0f;
				rotGuacamaya = 270.0f;
			}
			// FASE 3: Posada en la rama
			else
			{
				guacamayaPos.x = 3.0f;
				guacamayaPos.y = 0.5f;
				guacamayaPos.z = 6.5f;
				guacamayaAlaDer = 0.0f;
				guacamayaAlaIzq = 0.0f;
				rotGuacamaya = 270.0f;
			}
		}

		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE MODELOS SABANA (-x,-z)
		// ---------------------------------------------------------------------------------

		// **** DIBUJO DEL PISO SABANA Y ACCESORIOS SABANA ****
		DibujarPiso(pisoSabanaTextureID, glm::vec3(-7.25f, -0.49f, -7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);

		// --- Arbol ---
		model = glm::mat4(1);
		model = glm::translate(model, arbolSabanaPos);
		model = glm::scale(model, arbolSabanaScale);
		model = glm::rotate(model, glm::radians(arbolSabanaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ArbolSabana.Draw(lightingShader);
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

		// **** DIBUJO DE LA JIRAFA ****
		model = glm::mat4(1);
		model = glm::translate(model, jirafaPos);
		model = glm::rotate(model, glm::radians(rotJirafa), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(jirafaScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Jirafa_Cuerpo.Draw(lightingShader);

		// Cabeza
		glm::vec3 jirafaPivotCabeza(0.0f, 1.5f, 0.3f);
		model = modelTemp;
		model = glm::translate(model, jirafaPivotCabeza);
		model = glm::rotate(model, glm::radians(jirafaCabezaRot), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -jirafaPivotCabeza);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Jirafa_Cabeza.Draw(lightingShader);

		// Cola
		glm::vec3 jirafaPivotCola(0.0f, 0.8f, -0.5f);
		model = modelTemp;
		model = glm::translate(model, jirafaPivotCola);
		model = glm::rotate(model, glm::radians(jirafaColaRot), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -jirafaPivotCola);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Jirafa_Cola.Draw(lightingShader);

		// Pata delantera derecha
		glm::vec3 jirafaPivotPataDelDer(0.3f, 0.8f, 0.4f);
		model = modelTemp;
		model = glm::translate(model, jirafaPivotPataDelDer);
		model = glm::rotate(model, glm::radians(jirafaPataDelDer), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -jirafaPivotPataDelDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Jirafa_PataDelDer.Draw(lightingShader);

		// Pata delantera izquierda
		glm::vec3 jirafaPivotPataDelIzq(-0.3f, 0.8f, 0.4f);
		model = modelTemp;
		model = glm::translate(model, jirafaPivotPataDelIzq);
		model = glm::rotate(model, glm::radians(jirafaPataDelIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -jirafaPivotPataDelIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Jirafa_PataDelIzq.Draw(lightingShader);

		// Pata trasera derecha
		glm::vec3 jirafaPivotPataTrasDer(0.3f, 0.8f, -0.4f);
		model = modelTemp;
		model = glm::translate(model, jirafaPivotPataTrasDer);
		model = glm::rotate(model, glm::radians(jirafaPataTrasDer), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -jirafaPivotPataTrasDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Jirafa_PataTrasDer.Draw(lightingShader);

		// Pata trasera izquierda
		glm::vec3 jirafaPivotPataTrasIzq(-0.3f, 0.8f, -0.4f);
		model = modelTemp;
		model = glm::translate(model, jirafaPivotPataTrasIzq);
		model = glm::rotate(model, glm::radians(jirafaPataTrasIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -jirafaPivotPataTrasIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Jirafa_PataTrasIzq.Draw(lightingShader);

		// JIRAFA - ANIMACIÓN
		if (animarJirafa)
		{
			float t = glfwGetTime() - startTimeJirafa;

			// FASE 1: Caminando (4 segundos)
			if (t < 4.0f)
			{
				// Avanza 4 unidades en Z
				float totalDist = 4.0f;
				jirafaPos.z = -10.0f + (t * (totalDist / 4.0f));

				// Ciclo de caminata (patas se mueven por pares)
				float paso = sin(t * 5.5f);
				jirafaPataDelDer = paso * 2.5f;
				jirafaPataTrasDer = paso * 2.5f;
				jirafaPataDelIzq = -paso * 2.5f;
				jirafaPataTrasIzq = -paso * 2.5f;

				// Cabeza se balancea suavemente
				jirafaCabezaRot = sin(t * 0.5f) * 2.0f;

				// Cola se mueve al caminar
				jirafaColaRot = sin(t * 1.5f) * 5.0f;

				rotJirafa = 0.0f;
			}
			// FASE 2: Comiendo hojas (4 segundos)
			else if (t < 8.0f)
			{
				float t2 = t - 4.0f;
				jirafaPos.z = -6.0f;

				// Patas se detienen gradualmente
				jirafaPataDelDer = sin(t2 * 0.5f) * 0.5f;
				jirafaPataDelIzq = -jirafaPataDelDer;
				jirafaPataTrasDer = -jirafaPataDelDer;
				jirafaPataTrasIzq = jirafaPataDelDer;

				// Cabeza baja para comer
				jirafaCabezaRot = sin(t2 * 1.0f) * 3.0f;

				// Cola se mueve suavemente
				jirafaColaRot = sin(t2 * 0.8f) * 2.0f;

				rotJirafa = 0.0f;
			}
			// FASE 3: Quieta
			else
			{
				jirafaPos.z = -6.0f;
				jirafaCabezaRot = 0.0f;
				jirafaColaRot = 0.0f;
				jirafaPataDelDer = jirafaPataDelIzq = 0.0f;
				jirafaPataTrasDer = jirafaPataTrasIzq = 0.0f;
				rotJirafa = 0.0f;
			}
		}

		// --- ¡NUEVO! **** DIBUJO DE LA CEBRA **** ---
		model = glm::mat4(1);
		model = glm::translate(model, cebraPos);
		model = glm::rotate(model, glm::radians(rotCebra), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(cebraScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cebra_Cuerpo.Draw(lightingShader);

		// Pata delantera derecha
		glm::vec3 cebraPivotPataDelDer(0.3f, 0.8f, 0.4f); // Ajusta estos valores según tu modelo
		model = modelTemp;
		model = glm::translate(model, cebraPivotPataDelDer);
		model = glm::rotate(model, glm::radians(cebraPataDelDer), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -cebraPivotPataDelDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cebra_PataDelDer.Draw(lightingShader);

		// Pata delantera izquierda
		glm::vec3 cebraPivotPataDelIzq(-0.3f, 0.8f, 0.4f);
		model = modelTemp;
		model = glm::translate(model, cebraPivotPataDelIzq);
		model = glm::rotate(model, glm::radians(cebraPataDelIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -cebraPivotPataDelIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cebra_PataDelIzq.Draw(lightingShader);

		// Pata trasera derecha
		glm::vec3 cebraPivotPataTrasDer(0.3f, 0.8f, -0.4f);
		model = modelTemp;
		model = glm::translate(model, cebraPivotPataTrasDer);
		model = glm::rotate(model, glm::radians(cebraPataTrasDer), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -cebraPivotPataTrasDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cebra_PataTrasDer.Draw(lightingShader);

		// Pata trasera izquierda
		glm::vec3 cebraPivotPataTrasIzq(-0.3f, 0.8f, -0.4f);
		model = modelTemp;
		model = glm::translate(model, cebraPivotPataTrasIzq);
		model = glm::rotate(model, glm::radians(cebraPataTrasIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -cebraPivotPataTrasIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cebra_PataTrasIzq.Draw(lightingShader);


		// ---  CEBRA - ANIMACIÓN  ---
		if (animarCebra)
		{
			float t = glfwGetTime() - startTimeCebra;

			// FASE 1: Caminar hacia abajo (Z: -3.5 → -10.0) - 7 segundos
			if (t < 7.0f)
			{
				cebraPos.x = -2.8f;
				float totalDist = 6.5f; // Distancia en Z (de -3.5 a -10.0)
				cebraPos.z = -3.5f - (t * (totalDist / 7.0f)); // Avanza hacia -Z

				// Ciclo de caminata
				float paso = sin(t * 5.5f);
				cebraPataDelDer = paso * 4.0f;
				cebraPataTrasDer = paso * 4.0f;
				cebraPataDelIzq = -paso * 4.0f;
				cebraPataTrasIzq = -paso * 4.0f;

				rotCebra = 180.0f;
			}
			// FASE 2: Girar hacia la izquierda 
			else if (t < 8.0f)
			{
				float t2 = t - 7.0f;
				cebraPos.x = -2.8f;
				cebraPos.z = -10.0f;

				// Rotación
				rotCebra = 180.0f + (t2 * 90.0f);

				// Detener patas
				cebraPataDelDer = 0.0f;
				cebraPataDelIzq = 0.0f;
				cebraPataTrasDer = 0.0f;
				cebraPataTrasIzq = 0.0f;
			}
			// FASE 3: Caminar hacia la izquierda (X: -2.8 → -10.5) - 8 segundos
			else if (t < 16.0f)
			{
				float t3 = t - 8.0f;
				float totalDist = 7.7f; // Distancia en X (de -2.8 a -10.5)
				cebraPos.x = -2.8f - (t3 * (totalDist / 8.0f)); // Avanza hacia -X
				cebraPos.z = -10.0f;

				// Ciclo de caminata
				float paso = sin(t3 * 5.5f);
				cebraPataDelDer = paso * 4.0f;
				cebraPataTrasDer = paso * 4.0f;
				cebraPataDelIzq = -paso * 4.0f;
				cebraPataTrasIzq = -paso * 4.0f;

				rotCebra = 270.0f;
			}
			// FASE 4: Girar hacia arriba
			else if (t < 17.0f)
			{
				float t4 = t - 16.0f;
				cebraPos.x = -10.5f;
				cebraPos.z = -10.0f;

				// rotación
				rotCebra = 270.0f + (t4 * 90.0f);

				// Detener patas
				cebraPataDelDer = 0.0f;
				cebraPataDelIzq = 0.0f;
				cebraPataTrasDer = 0.0f;
				cebraPataTrasIzq = 0.0f;
			}
			// FASE 5: Caminar hacia arriba (Z: -10.0 → -3.5) - 7 segundos
			else if (t < 24.0f)
			{
				float t5 = t - 17.0f;
				cebraPos.x = -10.5f;
				float totalDist = 6.5f;
				cebraPos.z = -10.0f + (t5 * (totalDist / 7.0f)); // Avanza hacia +Z

				// Ciclo de caminata
				float paso = sin(t5 * 5.5f);
				cebraPataDelDer = paso * 4.0f;
				cebraPataTrasDer = paso * 4.0f;
				cebraPataDelIzq = -paso * 4.0f;
				cebraPataTrasIzq = -paso * 4.0f;

				rotCebra = 0.0f;
			}
			// FASE 6: Giro final hacia la derecha (1 segundo)
			else if (t < 25.0f)
			{
				float t6 = t - 24.0f;
				cebraPos.x = -10.5f;
				cebraPos.z = -3.5f;

				// rotación de 0° a 90°
				rotCebra = t6 * 90.0f;

				// Detener patas
				cebraPataDelDer = 0.0f;
				cebraPataDelIzq = 0.0f;
				cebraPataTrasDer = 0.0f;
				cebraPataTrasIzq = 0.0f;
			}
			// FASE 7: Caminar de regreso a la posición inicial (X: -10.5 → -2.8) - 8 segundos
			else if (t < 33.0f)
			{
				float t7 = t - 25.0f;
				float totalDist = 7.7f; // Distancia en X
				cebraPos.x = -10.5f + (t7 * (totalDist / 8.0f)); // Avanza hacia +X
				cebraPos.z = -3.5f;

				// Ciclo de caminata
				float paso = sin(t7 * 5.5f);
				cebraPataDelDer = paso * 4.0f;
				cebraPataTrasDer = paso * 4.0f;
				cebraPataDelIzq = -paso * .0f;
				cebraPataTrasIzq = -paso * 4.0f;

				rotCebra = 90.0f; // Mirando hacia la derecha (+X)
			}
			// FASE 8: Giro final
			else if (t < 34.0f)
			{
				float t8 = t - 33.0f;
				cebraPos.x = -2.8f;
				cebraPos.z = -3.5f;

				// rotación
				rotCebra = 90.0f + (t8 * 90.0f);

				// Detener patas
				cebraPataDelDer = 0.0f;
				cebraPataDelIzq = 0.0f;
				cebraPataTrasDer = 0.0f;
				cebraPataTrasIzq = 0.0f;
			}
			// FASE 9: Quieta en posición inicial
			else
			{
				cebraPos.x = -2.8f;
				cebraPos.z = -3.5f;
				cebraPataDelDer = 0.0f;
				cebraPataDelIzq = 0.0f;
				cebraPataTrasDer = 0.0f;
				cebraPataTrasIzq = 0.0f;
				rotCebra = 180.0f; // Mirando hacia abajo
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


		
		// =================================================================================
		// 							ANIMACIÓN DE ANIMALES - ACUARIO (PINGUINO)
		// =================================================================================

		// 1.ciclo
		float cycleDuration = 10.0f; // 10 segundos en total
		float walkDuration = 4.0f;   // 4 seg para caminar
		float turnDuration = 1.0f;   // 1 seg para girar

		// fmod da un tiempo que se repite de 0.0 a 10.0
		float t_pingu = fmod(glfwGetTime(), cycleDuration);

		// 2. Definir las fases del ciclo
		float turn1StartTime = walkDuration; // 4.0s
		float walkBackStartTime = turn1StartTime + turnDuration; // 5.0s
		float turn2StartTime = walkBackStartTime + walkDuration; // 9.0s

		
		glm::vec3 pinguStartPos = glm::vec3(pinguBasePos.x - pinguWalkDist, pinguBasePos.y, pinguBasePos.z);
		glm::vec3 pinguEndPos = glm::vec3(pinguBasePos.x + pinguWalkDist, pinguBasePos.y, pinguBasePos.z);
		

		// 4. Lógica de Estado
		if (t_pingu < turn1StartTime) // Fase 1: Caminar Adelante (0s a 4s)
		{
			// Interpolar la posición
			float t_interp = t_pingu / walkDuration; // 0.0 a 1.0
			pinguPos = glm::mix(pinguStartPos, pinguEndPos, t_interp);
			pinguRotY = 90.0f; // --- 

			// Animar movimiento
			float pasoPingu = sin(t_pingu * 8.0f); // Aleteo y patas 
			rotPataIzqPingu = pasoPingu * 20.0f;
			rotPataDerPingu = -pasoPingu * 20.0f;
			rotCuerpoPingu = pasoPingu * 5.0f; // Balanceo
			rotAletaIzqPingu = abs(pasoPingu) * 25.0f;
			rotAletaDerPingu = abs(pasoPingu) * 25.0f;
		}
		else if (t_pingu < walkBackStartTime) // Fase 2: Girar 180 
		{
			pinguPos = pinguEndPos; // Quedarse quieto en el punto final

			// Interpolar la rotación
			float t_interp = (t_pingu - turn1StartTime) / turnDuration; // 0.0 a 1.0
			pinguRotY = 90.0f + (t_interp * 180.0f); 

			// Detener animación 
			rotPataIzqPingu = 0.0f; rotPataDerPingu = 0.0f; rotCuerpoPingu = 0.0f; rotAletaIzqPingu = 0.0f; rotAletaDerPingu = 0.0f;
		}
		else if (t_pingu < turn2StartTime) // Fase 3: Caminar de Regreso
		{
			// Interpolar la posición 
			float t_interp = (t_pingu - walkBackStartTime) / walkDuration; // 0.0 a 1.0
			pinguPos = glm::mix(pinguEndPos, pinguStartPos, t_interp);
			pinguRotY = 270.0f; 

			// Animar movimiento
			float pasoPingu = sin(t_pingu * 8.0f); // Aleteo y patas
			rotPataIzqPingu = pasoPingu * 20.0f;
			rotPataDerPingu = -pasoPingu * 20.0f;
			rotCuerpoPingu = pasoPingu * 5.0f; // Balanceo
			rotAletaIzqPingu = abs(pasoPingu) * 25.0f;
			rotAletaDerPingu = abs(pasoPingu) * 25.0f;
		}
		else // Fase 4: Girar de Regreso 
		{
			pinguPos = pinguStartPos; // Quedarse quieto en el punto inicial

			// Interpolar la rotación
			float t_interp = (t_pingu - turn2StartTime) / turnDuration; // 0.0 a 1.0
			pinguRotY = 270.0f + (t_interp * 180.0f); 

			// Detener animación 
			rotPataIzqPingu = 0.0f; rotPataDerPingu = 0.0f; rotCuerpoPingu = 0.0f; rotAletaIzqPingu = 0.0f; rotAletaDerPingu = 0.0f;
		}
		
		// =================================================================================
		// 							ANIMACIÓN DE ANIMALES - ACUARIO (TORTUGA)
		// =================================================================================
		

		// 3. Definir las posiciones de inicio y fin 
		
		glm::vec3 tortugaStartPos = glm::vec3(tortugaBasePos.x - tortugaSwimDist, tortugaBasePos.y, tortugaBasePos.z);
		glm::vec3 tortugaEndPos = glm::vec3(tortugaBasePos.x + tortugaSwimDist, tortugaBasePos.y, tortugaBasePos.z);

		if (animarTortugaAcuario)
		{
			// 1. Definir el ciclo
			float cycleDuration = 12.0f; // 12 segundos en total
			float swimDuration = 5.0f;   // 5 seg para nadar
			float turnDuration = 1.0f;   // 1 seg para girar

			float t_turt = fmod(glfwGetTime() - startTimeTortugaAcuario, cycleDuration);

			// 2. Definir las fases
			float turn1StartTime = swimDuration; // 5.0s
			float swimBackStartTime = turn1StartTime + turnDuration; // 6.0s
			float turn2StartTime = swimBackStartTime + swimDuration; // 11.0s

			// 4. Lógica de Estado
			float nado = sin(t_turt * 4.0f); // Frecuencia de nado
			if (t_turt < turn1StartTime) // Fase 1: Nadar Adelante
			{
				float t_interp = t_turt / swimDuration;
				tortugaAcuarioPos = glm::mix(tortugaStartPos, tortugaEndPos, t_interp);
				tortugaAcuarioRotY = 90.0f; // 

				//  Aleteo y balanceo ---
				rotTortugaAcuarioFL = nado * 25.0f;
				rotTortugaAcuarioFR = -nado * 25.0f;
				rotTortugaAcuarioBL = -nado * 15.0f;
				rotTortugaAcuarioBR = nado * 15.0f;
				rotTortugaAcuarioHead = nado * 10.0f; 
				rotTortugaAcuarioCuerpo = nado * 5.0f;
			}
			else if (t_turt < swimBackStartTime) // Fase 2: Girar 180 
			{
				tortugaAcuarioPos = tortugaEndPos;
				float t_interp = (t_turt - turn1StartTime) / turnDuration;
				tortugaAcuarioRotY = 90.0f - (t_interp * 180.0f); 

				// Detener nado
				rotTortugaAcuarioHead = 0.0f; rotTortugaAcuarioFL = 0.0f; rotTortugaAcuarioFR = 0.0f;
				rotTortugaAcuarioBL = 0.0f; rotTortugaAcuarioBR = 0.0f; rotTortugaAcuarioCuerpo = 0.0f;
			}
			else if (t_turt < turn2StartTime) // Fase 3: Nadar de Regreso 
			{
				float t_interp = (t_turt - swimBackStartTime) / swimDuration;
				tortugaAcuarioPos = glm::mix(tortugaEndPos, tortugaStartPos, t_interp);
				tortugaAcuarioRotY = -90.0f; 

				//  Aleteo y balanceo ---
				rotTortugaAcuarioFL = nado * 25.0f;
				rotTortugaAcuarioFR = -nado * 25.0f;
				rotTortugaAcuarioBL = -nado * 15.0f;
				rotTortugaAcuarioBR = nado * 15.0f;
				rotTortugaAcuarioHead = nado * 10.0f; 
				rotTortugaAcuarioCuerpo = nado * 5.0f; 
			}
			else // Fase 4: Girar de Regreso 
			{
				tortugaAcuarioPos = tortugaStartPos;
				float t_interp = (t_turt - turn2StartTime) / turnDuration;
				tortugaAcuarioRotY = -90.0f - (t_interp * 180.0f); 

				// Detener nado
				rotTortugaAcuarioHead = 0.0f; rotTortugaAcuarioFL = 0.0f; rotTortugaAcuarioFR = 0.0f;
				rotTortugaAcuarioBL = 0.0f; rotTortugaAcuarioBR = 0.0f; rotTortugaAcuarioCuerpo = 0.0f;
			}
		}
		else
		{
			// Resetear a la posición incial
			tortugaAcuarioPos = tortugaStartPos;
			tortugaAcuarioRotY = 90.0f; 

			// Detener aletas y cabeza
			rotTortugaAcuarioHead = 0.0f;
			rotTortugaAcuarioFL = 0.0f;
			rotTortugaAcuarioFR = 0.0f;
			rotTortugaAcuarioBL = 0.0f;
			rotTortugaAcuarioBR = 0.0f;
			rotTortugaAcuarioCuerpo = 0.0f; 
		}
		// --- FIN ---
		
		// =================================================================================
		// 							ANIMACIÓN DE ANIMALES - ACUARIO (NUTRIA)
		// =================================================================================
		if (animarNutria)
		{
			// Duración de cada etapa
			float t_nutria = glfwGetTime() - startTimeNutria;
			float t_impulso = 0.5f; // 0.5s para agacharse
			float t_salto1 = 3.0f;  // 3 seg de roca a iglú
			float t_salto2 = 3.0f;  // 3 seg de iglú a agua

			float fase0End = t_impulso;
			float fase1End = fase0End + t_salto1;
			float fase2End = fase1End + t_salto2;

			// --- FASE 0: Impulso (Agacharse) ---
			if (t_nutria < fase0End)
			{
				nutriaPos = nutriaPos1_Roca; // No te muevas
				// --- ¡CORREGIDO! Rotar para ver al iglú ---
				nutriaRotY = 45.0f; // (Ajusta este ángulo)
				rotNutriaCuerpoX = 15.0f; // Inclínate un poco

				// Mueve las patas hacia atrás (agacharse)
				float crouch = (t_nutria / t_impulso) * -20.0f;
				rotNutriaFL = crouch; rotNutriaFR = crouch;
				rotNutriaBL = crouch; rotNutriaBR = crouch;
				rotNutriaCola = crouch / 2.0f;
				rotNutriaHead = 10.0f; // Mirar un poco arriba
			}
			// --- FASE 1: Salto de Roca a Iglú ---
			else if (t_nutria < fase1End)
			{
				float t_interp = (t_nutria - fase0End) / t_salto1;
				nutriaPos = glm::mix(nutriaPos1_Roca, nutriaPos2_Iglu, t_interp);
				nutriaPos.y += 1.5f * sin(glm::radians(t_interp * 180.0f));

				nutriaRotY = 45.0f; // Mantiene la rotación al iglú
				rotNutriaCuerpoX = 0.0f; // Cuerpo derecho

				// Patas se encogen durante el salto
				rotNutriaFL = 30.0f; rotNutriaFR = 30.0f;
				rotNutriaBL = 30.0f; rotNutriaBR = 30.0f;
				rotNutriaCola = 10.0f; rotNutriaHead = 0.0f;
			}
			// --- FASE 2: Clavado de Iglú a Agua ---
			else if (t_nutria < fase2End)
			{
				float t_interp = (t_nutria - fase1End) / t_salto2;
				nutriaPos = glm::mix(nutriaPos2_Iglu, nutriaPos3_Agua, t_interp);
				nutriaPos.y += 2.0f * sin(glm::radians(t_interp * 180.0f));

				// --- ¡CORREGIDO! Orientación del salto ---
				nutriaRotY = 135.0f; // (Ajusta este ángulo para que mire al agua)
				rotNutriaCuerpoX = t_interp * 360.0f; // Un giro completo en el aire

				// --- ¡CORREGIDO! Reseteamos las patas/cabeza a 0 ---
				// Dejamos que el giro del cuerpo (rotNutriaCuerpoX) haga todo el trabajo.
				rotNutriaFL = 0.0f; rotNutriaFR = 0.0f; rotNutriaBL = 0.0f; rotNutriaBR = 0.0f;
				rotNutriaHead = 0.0f; rotNutriaCola = 0.0f;
			}
			// --- FASE 3: En el agua ---
			else
			{
				nutriaPos = nutriaPos3_Agua;
				nutriaRotY = 135.0f;
				rotNutriaCuerpoX = 360.0f; // Mantener rotación final

				// Detener animación de patas
				rotNutriaFL = 0.0f; rotNutriaFR = 0.0f; rotNutriaBL = 0.0f; rotNutriaBR = 0.0f;
			}
		}
		else
		{
			// Resetear a la posición inicial (la roca)
			nutriaPos = nutriaPos1_Roca;
			nutriaRotY = 45.0f; // --- ¡CORREGIDO! Que mire al iglú al resetear
			rotNutriaCuerpoX = 0.0f;
			// Resetear partes del cuerpo
			rotNutriaHead = 0.0f; rotNutriaCola = 0.0f; rotNutriaFL = 0.0f;
			rotNutriaFR = 0.0f; rotNutriaBL = 0.0f; rotNutriaBR = 0.0f;
		}
		// --- FIN---

		// =================================================================================
		// 							DIBUJO DE MODELOS - AVIARIO (CENTRO)
		// =================================================================================

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		AviarioMadera.Draw(lightingShader); 


		//  (Vidrio)

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//  interruptor 'transparency' en el shader
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);

		// Volvemos a aplicar la misma transformación para el vidrio
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		AviarioVidrio.Draw(lightingShader); 


		// --- DIBUJAR EL AVE ---
	
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// Cuerpo
		model = glm::mat4(1.0f);
		model = glm::translate(model, avePos);
		model = glm::scale(model, glm::vec3(0.5f)); 
		modelTemp = model; 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AveCuerpo.Draw(lightingShader);

		// Cabeza
		
		glm::vec3 pivoteCabeza(0.0f, 0.5f, 0.1f); 
		model = modelTemp;
		model = glm::translate(model, pivoteCabeza);
		model = glm::rotate(model, glm::radians(rotCabeza), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::translate(model, -pivoteCabeza);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AveCabeza.Draw(lightingShader);

		// Ala Izquierda
	
		glm::vec3 pivoteAlaIzq(0.3f, 0.3f, 0.0f); 
		model = modelTemp;
		model = glm::translate(model, pivoteAlaIzq);
		model = glm::rotate(model, glm::radians(rotAlaIzq), glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::translate(model, -pivoteAlaIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AveAlaIzq.Draw(lightingShader);

		// Ala Derecha
		
		glm::vec3 pivoteAlaDer(-0.3f, 0.3f, 0.0f); 
		model = modelTemp;
		model = glm::translate(model, pivoteAlaDer);
		model = glm::rotate(model, glm::radians(rotAlaDer), glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::translate(model, -pivoteAlaDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AveAlaDer.Draw(lightingShader);

		// Cola
		
		glm::vec3 pivoteCola(0.0f, 0.1f, -0.4f); 
		model = modelTemp;
		model = glm::translate(model, pivoteCola);
		
		model = glm::translate(model, -pivoteCola);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AveCola.Draw(lightingShader);

		// Patas
		
		glm::vec3 pivotePatas(0.0f, -0.2f, 0.0f); 
		model = modelTemp;
		model = glm::translate(model, pivotePatas);
		
		model = glm::translate(model, -pivotePatas);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		AvePatas.Draw(lightingShader);


		
		glDisable(GL_BLEND);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);


		// --- DIBUJAR PINGUINO ---

		// Cuerpo
		model = glm::mat4(1.0f);
		model = glm::translate(model, pinguPos); // Posición global del pinguino
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));// Rotación global en Y
		model = glm::rotate(model, glm::radians(pinguRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(pinguScale)); // Escala global
		modelTemp = model; 

		// Aplicar balanceo del cuerpo
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f)); 
		model = glm::rotate(model, glm::radians(rotCuerpoPingu), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotar en Z
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // Volver a la posición original

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PinguCuerpo.Draw(lightingShader);

		// Aleta Izquierda
		glm::vec3 pivoteAletaIzq(0.18f, 0.5f, 0.0f); 
		model = modelTemp; 
		model = glm::translate(model, pivoteAletaIzq);
		model = glm::rotate(model, glm::radians(rotAletaIzqPingu), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota en X
		model = glm::translate(model, -pivoteAletaIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PinguAletaIzq.Draw(lightingShader);

		// Aleta Derecha
		glm::vec3 pivoteAletaDer(-0.18f, 0.5f, 0.0f); 
		model = modelTemp; 
		model = glm::translate(model, pivoteAletaDer);
		model = glm::rotate(model, glm::radians(rotAletaDerPingu), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota en X
		model = glm::translate(model, -pivoteAletaDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PinguAletaDer.Draw(lightingShader);

		// Pata Izquierda
		glm::vec3 pivotePataIzq(0.06f, -0.4f, 0.02f); 
		model = modelTemp; 
		model = glm::translate(model, pivotePataIzq);
		model = glm::rotate(model, glm::radians(rotPataIzqPingu), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota en X
		model = glm::translate(model, -pivotePataIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PinguPataIzq.Draw(lightingShader);

		// Pata Derecha
		glm::vec3 pivotePataDer(-0.06f, -0.4f, 0.02f); 
		model = modelTemp; 
		model = glm::translate(model, pivotePataDer);
		model = glm::rotate(model, glm::radians(rotPataDerPingu), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota en X
		model = glm::translate(model, -pivotePataDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PinguPataDer.Draw(lightingShader);

		// ---------------------------------------------------------------------------------
// 							DIBUJO DE MODELOS ENTRADA
// ---------------------------------------------------------------------------------
// --- TAQUILLA ---
		model = glm::mat4(1);
		model = glm::translate(model, taquillaPos);
		model = glm::scale(model, taquillaScale);
		model = glm::rotate(model, glm::radians(taquillaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Taquilla.Draw(lightingShader);

		// --- HELLO KITTY ---
		model = glm::mat4(1);
		model = glm::translate(model, kittyPos);
		model = glm::scale(model, kittyScale);
		model = glm::rotate(model, glm::radians(kittyRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Kitty.Draw(lightingShader);

		// --- MONITO ---
		model = glm::mat4(1);
		model = glm::translate(model, monitoPos);
		model = glm::scale(model, monitoScale);
		model = glm::rotate(model, glm::radians(monitoRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Monito.Draw(lightingShader);

		// --- CDMX ---
		model = glm::mat4(1);
		model = glm::translate(model, cdmxPos);
		model = glm::scale(model, cdmxScale);
		model = glm::rotate(model, glm::radians(cdmxRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CDMX.Draw(lightingShader);

		// --- CARRUSEL ---
		model = glm::mat4(1);
		model = glm::translate(model, carruselPos);
		model = glm::scale(model, carruselScale);
		model = glm::rotate(model, glm::radians(carruselRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Carrusel.Draw(lightingShader);

		lightingShader.Use(); // shader de iluminación 

//// ========================================================================
//// DIBUJAR SKYBOX
//// ========================================================================
//glDepthFunc(GL_LEQUAL);
//skyboxShader.Use();
//view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
//glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
//glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

//glBindVertexArray(skyboxVAO);
//glActiveTexture(GL_TEXTURE0);
//glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
//glDrawArrays(GL_TRIANGLES, 0, 36);
//glBindVertexArray(0);
//glDepthFunc(GL_LESS);
////IMPORTANTE: Desenlazar la textura del skybox
//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

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

	// CAMBIAR TIPO DE CÁMARA (Primera/Tercera persona)
	if (keys[GLFW_KEY_TAB])
	{
		if (!teclaTAB_presionada)
		{
			camera.ToggleCameraType();
			teclaTAB_presionada = true;

			// Mensaje en consola (opcional)
			if (camera.GetCameraType() == FIRST_PERSON)
				std::cout << "Camara: PRIMERA PERSONA" << std::endl;
			else
				std::cout << "Camara: TERCERA PERSONA" << std::endl;
		}
	}
	else
	{
		teclaTAB_presionada = false;
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

	//JIRAFA
	if (keys[GLFW_KEY_J])
	{
		if (!teclaJ_presionada)
		{
			animarJirafa = !animarJirafa;
			startTimeJirafa = glfwGetTime();
			teclaJ_presionada = true;
		}
	}
	else
	{
		teclaJ_presionada = false;
	}

	// --- ¡NUEVO! CEBRA (Tecla L) ---
	if (keys[GLFW_KEY_L])
	{
		if (!teclaL_presionada)
		{
			animarCebra = !animarCebra;
			startTimeCebra = glfwGetTime();
			teclaL_presionada = true;
		}
	}
	else
	{
		teclaL_presionada = false;
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
	// --- ¡NUEVO! NUTRIA ACUARIO (Tecla N) ---
	if (keys[GLFW_KEY_N])
	{
		if (!teclaN_presionada)
		{
			animarNutria = !animarNutria;
			startTimeNutria = glfwGetTime();
			teclaN_presionada = true;
		}
	}
	else
	{
		teclaN_presionada = false;
	}
	// --- FIN NUEVO ---

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	//  TORTUGA ACUARIO (Tecla T) ---
	if (keys[GLFW_KEY_T])
	{
		if (!teclaT_presionada)
		{
			animarTortugaAcuario = !animarTortugaAcuario;
			startTimeTortugaAcuario = glfwGetTime();
			teclaT_presionada = true;
		}
	}
	else
	{
		teclaT_presionada = false;
	}
	// --- FIN ---

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}

	//GUACAMAYA
	if (keys[GLFW_KEY_O])
	{
		if (!teclaO_presionada)
		{
			animarGuacamaya = !animarGuacamaya;
			startTimeGuacamaya = glfwGetTime();
			teclaO_presionada = true;
		}
	}
	else
	{
		teclaO_presionada = false;
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