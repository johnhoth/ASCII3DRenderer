// projection.cpp
#include "projection.hpp"
#include <glm/gtc/matrix_transform.hpp>

std::vector<ScreenLine> projectTo2D(const Object3D& object, float fov) {
    std::vector<ScreenLine> screenLines;
    
    // Создаем матрицу проекции
    glm::mat4 projection = glm::perspective(
        glm::radians(fov),
        1.0f,  // aspect ratio 1:1
        0.1f,  // near
        100.0f // far
    );
    
    // Матрица вида (отодвигаем камеру назад по Z)
    float scale = RenderSettings::getInstance().cubeScale;
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f - scale));
    
    glm::mat4 vp = projection * view;
    
    // Проецируем каждое ребро
    for (const auto& edge : object.edges) {
        glm::vec4 start = vp * glm::vec4(object.vertices[edge.start].position, 1.0f);
        glm::vec4 end = vp * glm::vec4(object.vertices[edge.end].position, 1.0f);
        
        // Перспективное деление
        start /= start.w;
        end /= end.w;
        
        // Convert to screen coordinates (centered)
        ScreenLine line {
            {static_cast<int>(start.x * 30), static_cast<int>(-start.y * 15)},
            {static_cast<int>(end.x * 30), static_cast<int>(-end.y * 15)}
        };
        
        screenLines.push_back(line);
    }
    
    return screenLines;
}
