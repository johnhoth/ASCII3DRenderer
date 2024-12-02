// ascii_renderer.hpp
#ifndef ASCII_RENDERER_HPP
#define ASCII_RENDERER_HPP

#include <vector>
#include "projection.hpp"
#include <ncurses.h>

void renderASCII(const std::vector<ScreenLine>& lines);
void drawLine(const ScreenPoint& start, const ScreenPoint& end);

#endif
