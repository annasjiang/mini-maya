#include "camera.h"

#include <la.h>
#include <iostream>


Camera::Camera():
    Camera(400, 400)
{
    look = glm::vec3(0,0,-1);
    up = glm::vec3(0,1,0);
    right = glm::vec3(1,0,0);
    theta = 0.f;
    phi = 0.f;
    zoom = 12.f;
}

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp):
    fovy(45),
    width(w),
    height(h),
    near_clip(0.1f),
    far_clip(1000),
    eye(e),
    ref(r),
    world_up(worldUp),
    theta(0.f),
    phi(0.f),
    zoom(12.f)
{
    RecomputeAttributesPolar();
}

Camera::Camera(const Camera &c):
    fovy(c.fovy),
    width(c.width),
    height(c.height),
    near_clip(c.near_clip),
    far_clip(c.far_clip),
    aspect(c.aspect),
    eye(c.eye),
    ref(c.ref),
    look(c.look),
    up(c.up),
    right(c.right),
    world_up(c.world_up),
    V(c.V),
    H(c.H),
    theta(c.theta),
    phi(c.phi),
    zoom(c.zoom)
{}

// OG CAMERA
void Camera::RecomputeAttributes()
{
    look = glm::normalize(ref - eye);
    right = glm::normalize(glm::cross(look, world_up));
    up = glm::cross(right, look);

    float tan_fovy = tan(glm::radians(fovy/2));
    float len = glm::length(ref - eye);
    aspect = width / static_cast<float>(height);
    V = up*len*tan_fovy;
    H = right*len*aspect*tan_fovy;
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(glm::radians(fovy), width / (float)height, near_clip, far_clip) * glm::lookAt(eye, ref, up);
}

void Camera::RotateAboutUp(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(deg), up);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    RecomputeAttributes();
}
void Camera::RotateAboutRight(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(deg), right);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    RecomputeAttributes();
}

void Camera::TranslateAlongLook(float amt)
{
    glm::vec3 translation = look * amt;
    eye += translation;
    ref += translation;
}

void Camera::TranslateAlongRight(float amt)
{
    glm::vec3 translation = right * amt;
    eye += translation;
    ref += translation;
}
void Camera::TranslateAlongUp(float amt)
{
    glm::vec3 translation = up * amt;
    eye += translation;
    ref += translation;
}

// POLAR CAMERA
void Camera::RecomputeAttributesPolar()
{
    mat4 mat1 = mat4(1.0f);

    mat4 polarMat = rotate(mat1, radians(theta), vec3(0.f, 1.f, 0.f)) *
                    rotate(mat1, radians(phi), vec3(1.f, 0.f, 0.f)) *
                    translate(mat1, vec3(0.f, 0.f, zoom));

    eye = vec3(polarMat * vec4(0.f, 0.f, 0.f, 1.f));
    look = vec3(polarMat * vec4(0.f, 0.f, 1.f, 0.f));
    up = vec3(polarMat * vec4(0.f, 1.f, 0.f, 0.f));
    right = vec3(polarMat * vec4(1.f, 0.f, 0.f, 0.f));

    float tan_fovy = tan(glm::radians(fovy/2));
    float len = glm::length(ref - eye);
    aspect = width / static_cast<float>(height);
    V = up*len*tan_fovy;
    H = right*len*aspect*tan_fovy;
}

void Camera::RotateAboutUpPolar(float deg)
{
    theta += deg;
}
void Camera::RotateAboutRightPolar(float deg)
{
    phi += deg;
}

void Camera::TranslateAlongLookPolar(float amt)
{
    zoom += amt;
}

void Camera::TranslateAlongRightPolar(float amt)
{
    glm::vec3 translation = right * amt;
    eye += translation;
    ref += translation;
}
void Camera::TranslateAlongUpPolar(float amt)
{
    glm::vec3 translation = up * amt;
    eye += translation;
    ref += translation;
}

