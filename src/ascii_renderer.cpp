// ascii_renderer.cpp
#include "ascii_renderer.hpp"
#include <cmath>

void drawLine(const ScreenPoint& start, const ScreenPoint& end) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    
    // Calculate center offset
    int offsetX = cols / 2;
    int offsetY = rows / 2;
    
    int dx = abs(end.x - start.x);
    int dy = abs(end.y - start.y);
    int sx = start.x < end.x ? 1 : -1;
    int sy = start.y < end.y ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    
    int x = start.x;
    int y = start.y;
    
    while (true) {
        // Add offsets to center the drawing
        int screenX = x + offsetX;
        int screenY = y + offsetY;
        
        // Check if point is within screen bounds
        if (screenX >= 0 && screenX < cols && screenY >= 0 && screenY < rows) {
            mvaddch(screenY, screenX, '#');
        }
        
        if (x == end.x && y == end.y) break;
        int e2 = err;
        if (e2 > -dx) { err -= dy; x += sx; }
        if (e2 < dy) { err += dx; y += sy; }
    }
}

void renderASCII(const std::vector<ScreenLine>& lines) {
    clear();
    
    // Get terminal size for bounds checking
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    
    // Draw a border around the rendering area (optional)
    box(stdscr, 0, 0);
    
    // Render all lines
    for (const auto& line : lines) {
        drawLine(line.start, line.end);
    }
    
    refresh();
}
