// 3d_objects.hpp
#ifndef OBJECTS_3D_HPP
#define OBJECTS_3D_HPP

#include <vector>
#include <glm/glm.hpp>
#include "menu.hpp" // Add this include for RenderSettings

struct Vertex {
    glm::vec3 position;
};

struct Edge {
    int start, end;
};

class Object3D {
public:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;

    void applyTransformation(const glm::mat4& transformation);
};

// Create a cube
Object3D createCube();

// Update the rotation of the object
void updateObjectRotation(Object3D& object, const glm::vec3& rotationAngles);


#endif
