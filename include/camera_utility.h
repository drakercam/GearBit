#ifndef CAMERA_UTILITY_H
#define CAMERA_UTILITY_H

#include "math_utility.h"
#include "input_utility.h"
#include "window_utility.h"

struct Camera
{
    float speed;
    float rotationSpeed;
    Vector3 position;
    Quaternion orientation;

    Vector3 BASE_SIDE;
    Vector3 BASE_UP;
    Vector3 BASE_FORWARD;
};

// default camera initialization
inline void Camera_Default(Camera& camera)
{
    camera.speed = 0.0f;
    camera.rotationSpeed = Math_DegToRad(45.0f);
    camera.position = {0.0f,0.0f,0.0f};
    camera.orientation = {1.0f,0.0f,0.0f,0.0f};
    camera.BASE_SIDE = {1.0f,0.0f,0.0f};
    camera.BASE_UP = {0.0f,1.0f,0.0f};
    camera.BASE_FORWARD = {0.0f,0.0f,1.0f};
}

// initialize the camera with speed and rotation speed
inline void Camera_Init(Camera& camera, float speed, float theta, Vector3 position)
{
    camera.speed = speed;
    camera.rotationSpeed = Math_DegToRad(theta);
    camera.position = position;
    camera.orientation = {1.0f,0.0f,0.0f,0.0f};
    camera.BASE_SIDE = {1.0f,0.0f,0.0f};
    camera.BASE_UP = {0.0f,1.0f,0.0f};
    camera.BASE_FORWARD = {0.0f,0.0f,1.0f};
}

inline const Vector3 Camera_GetForward(const Camera& camera)
{
    Vector3 forward = Math_Vec3Normalize(Math_Mat3MultiplyVec3(
                                        Math_QuatConvertToMat3(camera.orientation),
                                        camera.BASE_FORWARD));

    return {-forward.x, -forward.y, -forward.z};
}

inline const Vector3 Camera_GetSide(const Camera& camera)
{
    Vector3 forward = Math_Vec3Normalize(Math_Mat3MultiplyVec3(Math_QuatConvertToMat3(camera.orientation), camera.BASE_FORWARD));
    Vector3 up = Math_Vec3Normalize(Math_Mat3MultiplyVec3(Math_QuatConvertToMat3(camera.orientation), camera.BASE_UP));
    Vector3 side = Math_Vec3Normalize(Math_Vec3Cross(up, forward));

    return side;
}

inline const Vector3 Camera_GetUp(const Camera& camera)
{
    Vector3 forward = Math_Vec3Normalize(Math_Mat3MultiplyVec3(Math_QuatConvertToMat3(camera.orientation), camera.BASE_FORWARD));
    Vector3 side = Math_Vec3Normalize(Math_Mat3MultiplyVec3(Math_QuatConvertToMat3(camera.orientation), camera.BASE_SIDE));
    Vector3 up = Math_Vec3Normalize(Math_Vec3Cross(side, forward));

    return up;
}

inline void Camera_Pitch(Camera& camera, float theta)  // rotate about x axis
{
    Vector3 axis = Math_Vec3Normalize(Camera_GetSide(camera));
    Quaternion rotQuat = Math_QuatRotate(axis, theta);
    camera.orientation = Math_QuatNormalize(Math_QuatMultiply(rotQuat, camera.orientation));
}

inline void Camera_Yaw(Camera& camera, float theta)    // rotate about y axis
{
    Vector3 axis = Math_Vec3Normalize(Camera_GetUp(camera));
    Quaternion rotQuat = Math_QuatRotate(axis, theta);
    camera.orientation = Math_QuatNormalize(Math_QuatMultiply(rotQuat, camera.orientation));
}

inline void Camera_Roll(Camera& camera, float theta)   // rotate about z axis
{
    Vector3 axis = Math_Vec3Normalize(Camera_GetForward(camera));
    Quaternion rotQuat = Math_QuatRotate(axis, theta);
    camera.orientation = Math_QuatNormalize(Math_QuatMultiply(rotQuat, camera.orientation));
}

inline const Matrix4 Camera_ViewMatrix(const Camera& camera)
{
    // Convert orientation quaternion to a 4x4 rotation matrix
    Matrix4 rotationMat = Math_QuatConvertToMat4(camera.orientation);

    // Transpose rotation (invert rotation)
    Matrix4 rotationInv = 
    {
        rotationMat.m[0], rotationMat.m[4], rotationMat.m[8], 0,
        rotationMat.m[1], rotationMat.m[5], rotationMat.m[9], 0,
        rotationMat.m[2], rotationMat.m[6], rotationMat.m[10], 0,
        0,                0,                0,               1
    };

    // Translation by negative camera position
    Matrix4 translation = Math_Mat4Translate({-camera.position.x, -camera.position.y, -camera.position.z});

    return Math_Mat4Multiply(rotationInv, translation);
}

inline void Camera_Update(const Window& window, Camera& camera, float dt)
{
    Vector3 moveDir = {0.0f, 0.0f, 0.0f};

    if (IsKeyPressed(window, GLFW_KEY_W)) moveDir = Math_Vec3Add(moveDir, Camera_GetForward(camera));
    if (IsKeyPressed(window, GLFW_KEY_S)) moveDir = Math_Vec3Add(moveDir, Math_Vec3Scale(Camera_GetForward(camera), -1.0f));

    if (IsKeyPressed(window, GLFW_KEY_A)) moveDir = Math_Vec3Add(moveDir, Math_Vec3Scale(Camera_GetSide(camera), -1.0f));
    if (IsKeyPressed(window, GLFW_KEY_D)) moveDir = Math_Vec3Add(moveDir, Camera_GetSide(camera));

    if (IsKeyPressed(window, GLFW_KEY_Q)) moveDir = Math_Vec3Add(moveDir, Math_Vec3Scale(Camera_GetUp(camera), -1.0f));
    if (IsKeyPressed(window, GLFW_KEY_E)) moveDir = Math_Vec3Add(moveDir, Camera_GetUp(camera));

    if (Math_Vec3Length(moveDir) > 0.0f)
    {
        moveDir = Math_Vec3Normalize(moveDir);
        camera.position = Math_Vec3Add(camera.position, Math_Vec3Scale(moveDir, camera.speed * dt));
    }

    float rotationAmt = camera.rotationSpeed * dt;

    // Pitch
    if (IsKeyPressed(window, GLFW_KEY_I))
        Camera_Pitch(camera, rotationAmt);

    if (IsKeyPressed(window, GLFW_KEY_K))
        Camera_Pitch(camera, -rotationAmt);

    // Yaw
    if (IsKeyPressed(window, GLFW_KEY_J))
        Camera_Yaw(camera, -rotationAmt);

    if (IsKeyPressed(window, GLFW_KEY_L))
        Camera_Yaw(camera, rotationAmt);

    // Roll
    if (IsKeyPressed(window, GLFW_KEY_U))
        Camera_Roll(camera, -rotationAmt);

    if (IsKeyPressed(window, GLFW_KEY_O))
        Camera_Roll(camera, rotationAmt);

    camera.orientation = Math_QuatNormalize(camera.orientation);
}

#endif