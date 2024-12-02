#include "menu.hpp"
#include "ascii_renderer.hpp"
#include "3d_objects.hpp"
#include "projection.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

// ...existing code...

void rotateCube() {
    Object3D cube = createCube();
    glm::vec3 rotation(0.0f);
    bool running = true;
    
    // Настройка неблокирующего ввода для ncurses
    timeout(0);
    
    while (running) {
        // Обработка ввода
        int ch = getch();
        if (ch == 'q') {
            running = false;
            continue;
        }
        
        // Обновление вращения
        rotation.x += 0.03f;
        rotation.y += 0.02f;
        rotation.z += 0.01f;
        
        // Создаем копию куба для трансформации
        Object3D rotatedCube = cube;
        updateObjectRotation(rotatedCube, rotation);
        
        // Проецируем и рендерим
        std::vector<ScreenLine> lines = projectTo2D(rotatedCube);
        renderASCII(lines);
        
        // Пауза для контроля скорости анимации
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    // Восстанавливаем блокирующий ввод
    timeout(-1);
}

int main() {
    // Инициализация ncurses
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    std::vector<std::string> menuOptions = {
        "Start Animation",
        "Settings",
        "Exit"
    };
    
    int selected = 0;
    bool quit = false;
    
    while (!quit) {
        displayMenu(menuOptions, selected, quit);
        
        if (!quit) {
            switch (selected) {
                case 0: // Start Animation
                    rotateCube(); // Теперь эта функция показывает вращающийся куб
                    break;
                case 1: // Settings
                    displaySettings(RenderSettings::getInstance());
                    break;
                case 2: // Exit
                    quit = true;
                    break;
            }
        }
    }
    
    endwin();
    return 0;
}
