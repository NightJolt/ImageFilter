#pragma once

#include "globals.h"

namespace fun {
    class R {
    public:
        static void LoadResources();

        static std::vector <sf::Texture> textures;
        static std::vector <sf::Font> fonts;
        static std::vector <sf::Shader*> shaders;

        // audio
        // lua scripts

    private:
        static const std::string textures_to_load[];
        static const std::string fonts_to_load[];
        static const std::string shaders_to_load[];

        static const std::string resources_directory;
        static const std::string textures_directory;
        static const std::string fonts_directory;
        static const std::string shaders_directory;
    };
}