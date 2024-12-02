#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include <string>
#include <ncurses.h>
#include <iostream>
#include <thread>
#include <chrono>

// Добавляем новую функцию для форматирования текста
std::string addSpaceBetweenChars(const std::string& input);

// Добавляем структуру для настроек
struct RenderSettings {
    float rotationSpeed;
    float cubeScale;
    std::vector<std::string> settingsOptions = {"Rotation Speed", "Cube Scale"};
    static RenderSettings& getInstance() {
        static RenderSettings instance;
        return instance;
    }
private:
    RenderSettings() : rotationSpeed(1.0f), cubeScale(1.0f) {}
};

// Добавляем функцию форматирования выбранной строки
void formatSelectedLine(bool isSelected);

void displayMenu(const std::vector<std::string>& options, int& selected, bool& quit);
void displaySettings(RenderSettings& settings);

#endif // MENU_HPP
