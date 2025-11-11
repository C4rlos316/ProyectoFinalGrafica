#pragma once

// Std. Includes
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Tipos de cámara
enum Camera_Type
{
	FIRST_PERSON,
	THIRD_PERSON
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;
const GLfloat THIRD_PERSON_DISTANCE = 5.0f; // Distancia en tercera persona

// Camera class with first and third person support
class Camera
{
public:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	// Camera options
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	// Tipo de cámara y distancia para tercera persona
	Camera_Type cameraType;
	GLfloat thirdPersonDistance;
	glm::vec3 targetPosition; // Posición del "personaje" que seguimos

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM),
		cameraType(FIRST_PERSON), thirdPersonDistance(THIRD_PERSON_DISTANCE)
	{
		this->position = position;
		this->targetPosition = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM),
		cameraType(FIRST_PERSON), thirdPersonDistance(THIRD_PERSON_DISTANCE)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->targetPosition = this->position;
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Returns the view matrix
	glm::mat4 GetViewMatrix()
	{
		if (cameraType == FIRST_PERSON)
		{
			// Primera persona: cámara mira desde position hacia front
			return glm::lookAt(this->position, this->position + this->front, this->up);
		}
		else // THIRD_PERSON
		{
			// Tercera persona: cámara está detrás del personaje mirando hacia él
			glm::vec3 cameraPos = targetPosition - front * thirdPersonDistance;
			cameraPos.y += 1.5f; // Elevar un poco la cámara
			return glm::lookAt(cameraPos, targetPosition, this->up);
		}
	}

	// Procesa movimiento del teclado
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->movementSpeed * deltaTime;

		// Calcular dirección de movimiento en el plano horizontal
		glm::vec3 frontFlat = glm::normalize(glm::vec3(this->front.x, 0.0f, this->front.z));
		glm::vec3 rightFlat = glm::normalize(glm::vec3(this->right.x, 0.0f, this->right.z));

		if (cameraType == FIRST_PERSON)
		{
			// Primera persona: mover la cámara directamente
			if (direction == FORWARD)
				this->position += frontFlat * velocity;
			if (direction == BACKWARD)
				this->position -= frontFlat * velocity;
			if (direction == LEFT)
				this->position -= rightFlat * velocity;
			if (direction == RIGHT)
				this->position += rightFlat * velocity;

			this->targetPosition = this->position;
		}
		else // THIRD_PERSON
		{
			// Tercera persona: mover el target (personaje)
			if (direction == FORWARD)
				this->targetPosition += frontFlat * velocity;
			if (direction == BACKWARD)
				this->targetPosition -= frontFlat * velocity;
			if (direction == LEFT)
				this->targetPosition -= rightFlat * velocity;
			if (direction == RIGHT)
				this->targetPosition += rightFlat * velocity;

			// Actualizar position para que siga al target
			this->position = this->targetPosition;
		}
	}

	// Procesa movimiento del mouse
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
	{
		xOffset *= this->mouseSensitivity;
		yOffset *= this->mouseSensitivity;

		this->yaw += xOffset;
		this->pitch += yOffset;

		// Limitar pitch
		if (constrainPitch)
		{
			if (this->pitch > 89.0f)
				this->pitch = 89.0f;
			if (this->pitch < -89.0f)
				this->pitch = -89.0f;
		}

		this->updateCameraVectors();
	}

	// Cambia entre primera y tercera persona
	void ToggleCameraType()
	{
		if (cameraType == FIRST_PERSON)
		{
			cameraType = THIRD_PERSON;
			targetPosition = position;
		}
		else
		{
			cameraType = FIRST_PERSON;
			position = targetPosition;
		}
	}

	// Ajusta la distancia en tercera persona
	void AdjustThirdPersonDistance(GLfloat delta)
	{
		thirdPersonDistance += delta;
		if (thirdPersonDistance < 2.0f) thirdPersonDistance = 2.0f;
		if (thirdPersonDistance > 15.0f) thirdPersonDistance = 15.0f;
	}

	GLfloat GetZoom()
	{
		return this->zoom;
	}

	glm::vec3 GetPosition()
	{
		if (cameraType == FIRST_PERSON)
			return this->position;
		else
			return this->targetPosition;
	}

	glm::vec3 GetFront()
	{
		return this->front;
	}

	Camera_Type GetCameraType()
	{
		return this->cameraType;
	}

private:
	// Calcula los vectores de la cámara
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);

		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
};