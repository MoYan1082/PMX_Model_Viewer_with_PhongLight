#include "camera.h"

Camera::Camera(Config& camConfig) {
    cv::Vec3f tmpvec;
    camConfig.get("Position", tmpvec);
    Position = glm::vec3(tmpvec[0], tmpvec[1], tmpvec[2]);
    camConfig.get("WorldUp", tmpvec);
    WorldUp = glm::vec3(tmpvec[0], tmpvec[1], tmpvec[2]);
    camConfig.get("Front", tmpvec);
    Front = glm::vec3(tmpvec[0], tmpvec[1], tmpvec[2]);

    camConfig.get("Yaw", Yaw);
    camConfig.get("Pitch", Pitch);

    camConfig.get("MovementSpeed", MovementSpeed);
    camConfig.get("MouseSensitivity", MouseSensitivity);
    camConfig.get("Zoom", Zoom);

    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction) {
    float velocity = MovementSpeed * 0.0001;
    switch (direction) {
    case FORWARD:
        Position += Front * velocity;
        break;
    case BACKWARD:
        Position -= Front * velocity;
        break;
    case LEFT:
        Position -= Right * velocity;
        break;
    case RIGHT:
        Position += Right * velocity;
        break;
    case UP:
        Position += Up * velocity;
        break;
    case DOWN:
        Position -= Up * velocity;
        break;
    default:
        assert(false);
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, 
    // screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
}

void Camera::updateCameraVectors() {
    // calculates the front vector from the Camera's (updated) Euler Angles
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}