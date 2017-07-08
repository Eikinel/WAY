#pragma once

# include <SFML\Graphics.hpp>
# include <vector>
# include <iostream>

// Directories
# define ASSETS_DIR		"./Assets"
# define FONTS_DIR		ASSETS_DIR"/Fonts"

# define GAME_NAME	"W . A . Y"

// Global variable for screens, containing menu, game, etc.
class IScreen;
extern std::vector<IScreen *> all_screens;