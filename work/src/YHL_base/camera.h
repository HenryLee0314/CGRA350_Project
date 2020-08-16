#ifndef CGRA350_CAMERA_3D_H
#define CGRA350_CAMERA_3D_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace CGRA350 {

const float YAW         =  0.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
public:
    enum CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    virtual ~Camera();

    glm::mat4 GetViewMatrix();

    inline float getZoom() {
        return _zoom;
    }

    // void ProcessKeyboard(CameraMovement direction, float deltaTime);

    // virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // void ProcessMouseScroll(float yoffset);

    inline glm::vec3 getPosition() {
        return _position;
    }

    void setYaw(float yaw) {
        _yaw = yaw;
    }

    void setPitch(float pitch) {
        _pitch = pitch;
    }

    void setDistance(float distance) {
        _distance = distance;
    }

protected:
    void updateCameraVectors();

    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;

    float _yaw;
    float _pitch;
    float _distance;

    float _movementSpeed;
    float _mouseSensitivity;
    float _zoom;
};

} // namespace CGRA350

#endif // CGRA350_CAMERA_3D_H