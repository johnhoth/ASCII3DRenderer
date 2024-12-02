// projection.hpp
#ifndef PROJECTION_HPP
#define PROJECTION_HPP

#include <glm/glm.hpp>
#include <vector>
#include "3d_objects.hpp"

struct ScreenPoint {
    int x, y;
};

struct ScreenLine {
    ScreenPoint start;
    ScreenPoint end;
};

std::vector<ScreenLine> projectTo2D(const Object3D& object, float fov = 60.0f);

#endif
