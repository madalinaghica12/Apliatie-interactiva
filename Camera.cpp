#include "Camera.hpp"
namespace gps {
    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        this->lastX = 800.0f / 2.0;
        this->lastY = 600.0 / 2.0;
        this->firstMouse = true;
    }
    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(this->cameraPosition, this->cameraPosition + this->cameraTarget, this->cameraUpDirection);
    }
    glm::vec3 Camera::getCameraPosition() const {
        return this->cameraPosition;
    }
    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        switch (direction) {
        case MOVE_FORWARD:
            this->cameraPosition += speed * this->cameraTarget;
            break;
        case MOVE_BACKWARD:
            this->cameraPosition -= speed * this->cameraTarget;
            break;
        case MOVE_RIGHT:
            this->cameraPosition += glm::normalize(glm::cross(this->cameraTarget, this->cameraUpDirection)) * speed;
            break;
        case MOVE_LEFT:
            this->cameraPosition -= glm::normalize(glm::cross(this->cameraTarget, this->cameraUpDirection)) * speed;
            break;
        }
        if (this->cameraPosition.x > 30.0f) {
            this->cameraPosition.x = 30.0f;
        }
        if (this->cameraPosition.x < -30.0f) {
            this->cameraPosition.x = -30.0f;
        }

        if (this->cameraPosition.y < 1.0f) {
            this->cameraPosition.y = 1.0f;
        }
        if (this->cameraPosition.y > 30.0f) {
            this->cameraPosition.y = 30.0f;
        }

        // Adaugă orice alte verificări necesare pentru a limita mișcarea în spațiu.
        // Exemplu: Limitarea pe axa Z dacă este cazul.
        if (this->cameraPosition.z < -55.0f) {
            this->cameraPosition.z = -55.0f;
        }
        if (this->cameraPosition.z > 50.0f) {
            this->cameraPosition.z = 50.0f;
        }
    }
    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float yaw, float pitch) {
        this->cameraTarget.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->cameraTarget.y = sin(glm::radians(pitch));
        this->cameraTarget.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->cameraTarget = glm::normalize(this->cameraTarget);
    }
    void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
    {
        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        this->yaw += xoffset;
        this->pitch += yoffset;
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->pitch > 89.0f)
                this->pitch = 89.0f;
            if (this->pitch < -89.0f)
                this->pitch = -89.0f;
        }
        // Update Front, Right and Up Vectors using the updated Euler angles
        this->updateCameraVectors();
    }
    void Camera::updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->cameraTarget = glm::normalize(front);
    }
}