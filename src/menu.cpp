#include "menu.hpp"

const int SELECTION_OFFSET = 3;  // offet for the selection pointer
const char* MENU_POINTER = "> ";

// Исправляем функцию formatSelectedLine
void formatSelectedLine(bool isSelected) {
    if (isSelected) {
        attron(COLOR_PAIR(1));  // Сначала включаем цвет
        attron(A_BOLD);         // Затем включаем жирность
    } else {
        attroff(COLOR_PAIR(1)); // Сначала выключаем цвет
        attroff(A_BOLD);        // Затем выключаем жирность
    }
}

std::string addSpaceBetweenChars(const std::string& input) {
    std::string result = " ";
    for (size_t i = 0; i < input.length(); ++i) {
        result += input[i];
        if (i < input.length() - 1) result += " ";
    }
    return result;
}

// Добавляем новую функцию для отрисовки пункта меню
void drawMenuItem(int row, int startCol, bool isSelected, const std::string& text) {
    if (isSelected) {
        formatSelectedLine(true);
        mvprintw(row, startCol - SELECTION_OFFSET, "%s%s", MENU_POINTER, text.c_str());
        formatSelectedLine(false);
    } else {
        mvprintw(row, startCol, "%s", text.c_str());
    }
}

// В функции drawSettingsItem меняем логику атрибутов:
void drawSettingsItem(int row, int startCol, int valueCol, int maxWidth, bool isSelected, 
                     bool isEditing, const std::string& text, float value = 0.0f, bool showValue = true) {
    if (isSelected) {
        formatSelectedLine(true);
        // mvprintw(row, startCol , "", );
        std::string displayText = isEditing ? addSpaceBetweenChars(text) : text;
        mvprintw(row, startCol - SELECTION_OFFSET, "%s%-*s", MENU_POINTER, maxWidth, displayText.c_str());
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
    // Добавляем правильную инициализацию ncurses в начале функции
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);  // Скрываем курсор
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    // Важно! Проверяем поддержку жирного текста
    if (!has_colors() || !can_change_color()) {
        // Можно добавить обработку ошибки
        printw("Your terminal does not support colors\n");
        refresh();
        return;
    }
    
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

        // Draw the options
        for (size_t i = 0; i < options.size(); ++i) {
            drawMenuItem(rows / 2 - options.size() + i, start_col, i == selected, options[i]);
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
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
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
        int settingsStartRow = rows / 2 - numSettings / 2;
        mvprintw(settingsStartRow - 4, (cols - title.size()) / 2, title.c_str());

        // Display terminal info
        mvprintw(settingsStartRow - 2, (cols - 20) / 2, "Terminal size: %dx%d", cols, rows);

        const int settingsStartCol = (cols - (maxTextWidth + 10)) / 2; // 10 для значения и отступов
        const int valueColumn = settingsStartCol + maxTextWidth + 2; // позиция для значений
        
        // Draw settings items
        drawSettingsItem(settingsStartRow, settingsStartCol, valueColumn, maxTextWidth,
                        selected == 0, isEditing && selected == 0,
                        settings.settingsOptions[0], settings.rotationSpeed);
        
        drawSettingsItem(settingsStartRow + 1, settingsStartCol, valueColumn, maxTextWidth,
                        selected == 1, isEditing && selected == 1,
                        settings.settingsOptions[1], settings.cubeScale);
        
        drawSettingsItem(settingsStartRow + 2, settingsStartCol, valueColumn, maxTextWidth,
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

