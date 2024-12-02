#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include <string>
#include <ncurses.h>
#include <iostream>
#include <thread>
#include <chrono>

// Добавляем структуру для настроек
struct RenderSettings {
    float rotationSpeed;
    float cubeScale;
    static RenderSettings& getInstance() {
        static RenderSettings instance;
        return instance;
    }
private:
    RenderSettings() : rotationSpeed(1.0f), cubeScale(1.0f) {}
};

// Add new enum for edit state
enum class EditState {
    NONE,
    EDITING_SPEED,
    EDITING_SCALE
};

void displayMenu(const std::vector<std::string>& options, int& selected, bool& quit);
void displaySettings(RenderSettings& settings);

#endif // MENU_HPP
