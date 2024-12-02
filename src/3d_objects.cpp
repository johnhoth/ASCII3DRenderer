// 3d_objects.cpp
#include "3d_objects.hpp"
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/gtx/rotate_vector.hpp>



void Object3D::applyTransformation(const glm::mat4& transformation) {
    for (auto& vertex : vertices) {
        vertex.position = glm::vec3(transformation * glm::vec4(vertex.position, 1.0f));
    }
}


// Create a unit cube centered at the origin
Object3D createCube() {
    Object3D cube;
    // Get scale from settings
    float scale = RenderSettings::getInstance().cubeScale;
    
    cube.vertices = {
        {glm::vec3(-0.5f * scale, -0.5f * scale, -0.5f * scale)},
        {glm::vec3(0.5f * scale, -0.5f * scale, -0.5f * scale)},
        {glm::vec3(0.5f * scale, 0.5f * scale, -0.5f * scale)},
        {glm::vec3(-0.5f * scale, 0.5f * scale, -0.5f * scale)},
        {glm::vec3(-0.5f * scale, -0.5f * scale, 0.5f * scale)},
        {glm::vec3(0.5f * scale, -0.5f * scale, 0.5f * scale)},
        {glm::vec3(0.5f * scale, 0.5f * scale, 0.5f * scale)},
        {glm::vec3(-0.5f * scale, 0.5f * scale, 0.5f * scale)}
    };
    
    cube.edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},  // front face
        {4, 5}, {5, 6}, {6, 7}, {7, 4},  // back face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}   // connecting edges
    };
    
    return cube;
}

// Update the object's rotation
void updateObjectRotation(Object3D& object, const glm::vec3& rotationAngles) {
    // Apply rotation speed from settings
    float speed = RenderSettings::getInstance().rotationSpeed;
    glm::vec3 adjustedAngles = rotationAngles * speed;

    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), adjustedAngles.x, glm::vec3(1, 0, 0));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), adjustedAngles.y, glm::vec3(0, 1, 0));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), adjustedAngles.z, glm::vec3(0, 0, 1));

    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

    // Apply rotation to all vertices
    for (auto& vertex : object.vertices) {
        vertex.position = glm::vec3(rotationMatrix * glm::vec4(vertex.position, 1.0f));
    }
}
