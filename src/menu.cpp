#include "menu.hpp"

// Display the menu at the center of the screen
void displayMenu(const std::vector<std::string>& options, int& selected, bool& quit) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    while (true) {
        clear();
        
        // Draw the menu title
        std::string title = "Main Menu";
        mvprintw(rows / 2 - options.size() / 2 - 2, (cols - title.size()) / 2, title.c_str());

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
            if (i == selected) {
                attron(A_BOLD);
                mvprintw(rows / 2 - options.size() / 2 + i, start_col - 2, "> %s", options[i].c_str());
                attroff(A_BOLD);
            } else {
                mvprintw(rows / 2 - options.size() / 2 + i, start_col, "%s", options[i].c_str());
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
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    bool exitSettings = false;
    int selected = 0;
    const int numSettings = 2;
    EditState editState = EditState::NONE;
    auto lastBlink = std::chrono::steady_clock::now();
    bool showValue = true;

    while (!exitSettings) {
        clear();
        std::string title = "Settings";
        mvprintw(rows / 2 - 4, (cols - title.size()) / 2, title.c_str());

        // Display terminal info
        mvprintw(rows / 2 - 2, (cols - 20) / 2, "Terminal size: %dx%d", cols, rows);

        const int settingsStartCol = (cols - 30) / 2;
        
        // Display rotation speed
        if (selected == 0) {
            mvprintw(rows / 2, settingsStartCol - 2, "> ");
        }
        mvaddstr(rows / 2, settingsStartCol, "Rotation Speed: ");
        if (editState == EditState::EDITING_SPEED && !showValue) {
            mvaddstr(rows / 2, settingsStartCol + 15, "     ");
        } else {
            mvprintw(rows / 2, settingsStartCol + 15, "%.1f", settings.rotationSpeed);
        }

        // Display cube scale
        if (selected == 1) {
            mvprintw(rows / 2 + 2, settingsStartCol - 2, "> ");
        }
        mvaddstr(rows / 2 + 2, settingsStartCol, "Cube Scale: ");
        if (editState == EditState::EDITING_SCALE && !showValue) {
            mvaddstr(rows / 2 + 2, settingsStartCol + 12, "     ");
        } else {
            mvprintw(rows / 2 + 2, settingsStartCol + 12, "%.1f", settings.cubeScale);
        }

        // Update instructions based on state
        if (editState == EditState::NONE) {
            mvprintw(rows - 2, 2, "Use UP/DOWN to select, ENTER to edit, 'q' to return");
        } else {
            mvprintw(rows - 2, 2, "Use LEFT/RIGHT to adjust, ENTER to save");
        }
        
        refresh();

        // Handle blinking effect
        auto now = std::chrono::steady_clock::now();
        if (editState != EditState::NONE && 
            std::chrono::duration_cast<std::chrono::milliseconds>(now - lastBlink).count() >= 500) {
            showValue = !showValue;
            lastBlink = now;
        }

        // Input handling
        if (editState == EditState::NONE) {
            int ch = getch();
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
                    editState = (selected == 0) ? EditState::EDITING_SPEED : EditState::EDITING_SCALE;
                    showValue = true;
                    lastBlink = std::chrono::steady_clock::now();
                    break;
            }
        } else {
            int ch = getch();
            if (ch == '\n') {
                editState = EditState::NONE;
                showValue = true;
            } else if (ch == KEY_LEFT || ch == KEY_RIGHT) {
                float& value = (editState == EditState::EDITING_SPEED) ? 
                              settings.rotationSpeed : settings.cubeScale;
                value += (ch == KEY_LEFT) ? -0.1f : 0.1f;
                value = std::max(0.1f, std::min(5.0f, value));
                showValue = true;
                lastBlink = std::chrono::steady_clock::now();
            }
        }
    }
}

