#include "menu.hpp"

const int SELECTION_OFFSET = 3;  // Добавляем константу для отступа
const char* MENU_POINTER = "> ";

// Добавляем реализацию функции форматирования
void formatSelectedLine(bool isSelected) {
    if (isSelected) {
        attron(COLOR_PAIR(1) | A_BOLD);
    } else {
        attroff(COLOR_PAIR(1) | A_BOLD);
        attron(A_NORMAL);
    }
}

std::string addSpaceBetweenChars(const std::string& input) {
    std::string result;
    for (size_t i = 0; i < input.length(); ++i) {
        result += input[i];
        if (i < input.length() - 1) result += " ";
    }
    return result;
}

// В функции drawSettingsItem меняем логику атрибутов:
void drawSettingsItem(int row, int startCol, int valueCol, int maxWidth, bool isSelected, 
                     bool isEditing, const std::string& text, float value = 0.0f, bool showValue = true) {
    if (isSelected) {
        formatSelectedLine(true);
        mvprintw(row, startCol - SELECTION_OFFSET, "%s", MENU_POINTER);
        std::string displayText = isEditing ? addSpaceBetweenChars(text) : text;
        mvprintw(row, startCol, "%-*s", maxWidth, displayText.c_str());
        if (showValue) {
            mvprintw(row, valueCol, ": %.1f", value);
        }
        formatSelectedLine(false);
    } else {
        mvprintw(row, startCol, "%-*s", maxWidth, text.c_str());
        if (showValue) {
            mvprintw(row, valueCol, ": %.1f", value);
        }
    }
}

// Display the menu at the center of the screen
void displayMenu(const std::vector<std::string>& options, int& selected, bool& quit) {
    start_color();  // Добавляем инициализацию цветов
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    while (true) {
        clear();
        
        // Draw the menu title
        std::string title = "Main Menu";
        mvprintw(rows / 2 - options.size() - 2, (cols - title.size()) / 2, title.c_str());

        // Calculate the starting position for the options
        int max_option_length = 0;
        for (const auto& option : options) {
            if (option.size() > max_option_length) {
                max_option_length = option.size();
            }
        }
        int start_col = (cols - max_option_length) / 2;

        // Draw the options with spacing between lines
        for (size_t i = 0; i < options.size(); ++i) {
            if (i == selected) {
                formatSelectedLine(true);
                mvprintw(rows / 2 - options.size() + i, start_col - SELECTION_OFFSET, "%s%s", 
                        MENU_POINTER, options[i].c_str());
                formatSelectedLine(false);
            } else {
                mvprintw(rows / 2 - options.size() + i, start_col, "%s", options[i].c_str());
            }
        }

        // Instructions
        mvprintw(rows - 2, 2, "Use arrow keys to navigate. Press Enter to select. Press 'q' to quit.");
        refresh();

        int ch = getch();
        if (ch == 'q') {
            quit = true;
            break;
        } else if (ch == KEY_UP) {
            selected = (selected == 0) ? options.size() - 1 : selected - 1;
        } else if (ch == KEY_DOWN) {
            selected = (selected + 1) % options.size();
        } else if (ch == '\n') {
            break;
        }
    }
}

void displaySettings(RenderSettings& settings) {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    bool exitSettings = false;
    int selected = 0;
    const int numSettings = 3; // Увеличиваем количество пунктов
    bool isEditing = false;

    // Рассчитываем максимальную длину названия настройки с пробелами
    int maxTextWidth = 0;
    for (const auto& option : settings.settingsOptions) {
        int spacedLength = option.length() * 2 - 1; // длина строки с пробелами
        maxTextWidth = std::max(maxTextWidth, spacedLength);
    }
    
    while (!exitSettings) {
        clear();
        std::string title = "Settings";
        mvprintw(rows / 2 - 4, (cols - title.size()) / 2, title.c_str());

        // Display terminal info
        mvprintw(rows / 2 - 2, (cols - 20) / 2, "Terminal size: %dx%d", cols, rows);

        const int settingsStartCol = (cols - (maxTextWidth + 10)) / 2; // 10 для значения и отступов
        const int valueColumn = settingsStartCol + maxTextWidth + 2; // позиция для значений
        
        // Draw settings items
        drawSettingsItem(rows / 2, settingsStartCol, valueColumn, maxTextWidth,
                        selected == 0, isEditing && selected == 0,
                        settings.settingsOptions[0], settings.rotationSpeed);
        
        drawSettingsItem(rows / 2 + 1, settingsStartCol, valueColumn, maxTextWidth,
                        selected == 1, isEditing && selected == 1,
                        settings.settingsOptions[1], settings.cubeScale);
        
        drawSettingsItem(rows / 2 + 2, settingsStartCol, valueColumn, maxTextWidth,
                        selected == 2, false, "Back to Main Menu", 0, false);

        // Update instructions based on state
        mvprintw(rows - 2, 2, isEditing ? 
                "Use LEFT/RIGHT to adjust, ENTER to save" :
                "Use UP/DOWN to select, ENTER to edit, 'q' to return");
        
        refresh();

        // Input handling
        int ch = getch();
        if (!isEditing) {
            switch (ch) {
                case 'q':
                    exitSettings = true;
                    break;
                case KEY_UP:
                    selected = (selected - 1 + numSettings) % numSettings;
                    break;
                case KEY_DOWN:
                    selected = (selected + 1) % numSettings;
                    break;
                case '\n':
                    if (selected == 2) {  // Если выбран пункт "Back to Main Menu"
                        exitSettings = true;
                    } else {
                        isEditing = true;
                    }
                    break;
            }
        } else {
            if (ch == '\n') {
                isEditing = false;
            } else if (ch == KEY_LEFT || ch == KEY_DOWN) {
                float& value = (selected == 0) ? settings.rotationSpeed : settings.cubeScale;
                value = std::max(0.1f, value - 0.1f);
            } else if (ch == KEY_RIGHT || ch == KEY_UP) {
                float& value = (selected == 0) ? settings.rotationSpeed : settings.cubeScale;
                value = std::min(5.0f, value + 0.1f);
            }
        }
    }
}

