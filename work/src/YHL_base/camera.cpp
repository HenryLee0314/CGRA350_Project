#include "camera.h"

namespace CGRA350 {

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : _front(glm::vec3(0.0f, 0.0f, 0.0f))
    , _movementSpeed(SPEED)
    , _mouseSensitivity(SENSITIVITY)
    , _zoom(ZOOM)
{
    _position = position;
    _worldUp = up;
    _yaw = yaw;
    _pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : _front(glm::vec3(0.0f, 0.0f, 0.0f))
    , _movementSpeed(SPEED)
    , _mouseSensitivity(SENSITIVITY)
    , _zoom(ZOOM)
{
    _position = glm::vec3(posX, posY, posZ);
    _worldUp = glm::vec3(upX, upY, upZ);
    _yaw = yaw;
    _pitch = pitch;
    updateCameraVectors();
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix()
{
    updateCameraVectors();
    return glm::lookAt(_position, glm::vec3(0.0f, 0.0f, 0.0f), _up);
}

// void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
// {
//     float velocity = _movementSpeed * deltaTime;
//     if (direction == FORWARD){
//         _position += _front * velocity;
//     }
//     if (direction == BACKWARD){
//         _position -= _front * velocity;
//     }
//     if (direction == LEFT){
//         _position -= _right * velocity;
//     }
//     if (direction == RIGHT){
//         _position += _right * velocity;
//     }
// }

// void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
// {
//     xoffset *= _mouseSensitivity;
//     yoffset *= _mouseSensitivity;

//     _yaw   += xoffset;
//     _pitch += yoffset;

//     if (constrainPitch)
//     {
//         if (_pitch > 89.0f)
//             _pitch = 89.0f;
//         if (_pitch < -89.0f)
//             _pitch = -89.0f;
//     }

//     updateCameraVectors();
// }

// void Camera::ProcessMouseScroll(float yoffset)
// {
//     if (_zoom >= 1.0f && _zoom <= 45.0f)
//         _zoom -= yoffset;
//     if (_zoom <= 1.0f)
//         _zoom = 1.0f;
//     if (_zoom >= 45.0f)
//         _zoom = 45.0f;
// }

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    // front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    // front.y = sin(glm::radians(_pitch));
    // front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

    front.x = cos((_yaw)) * cos((_pitch));
    front.y = sin((_pitch));
    front.z = sin((_yaw)) * cos((_pitch));


    _front = glm::normalize(front);
    _position = _distance * glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    _up    = glm::normalize(glm::cross(_right, _front));
}

} // namespace CGRA350