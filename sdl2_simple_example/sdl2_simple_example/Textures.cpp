#include "Textures.h"
#include <iostream>
#include <string>
#include "Component.h"

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

std::wstring charToWstring(const char* text) {
    size_t length = strlen(text);
    std::wstring wtext(length, L'\0');  // Reservar espacio para los caracteres

    // Usar mbstowcs_s para una conversión segura
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, &wtext[0], length + 1, text, length);

    return wtext;
}

Textures::Textures() : Component() {
}

Textures::~Textures() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);  // Liberar memoria de la textura en OpenGL
    }
}


void Textures::DrawTextures(const char* filePath) {
    ILuint imageID;

    ilInit();  // Inicializar DevIL

    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // Convierte filePath de const char* a const wchar_t*
    std::wstring wideFilePath = charToWstring(filePath);

    std::cout << "Intentando cargar la textura desde: " << filePath << std::endl; // Mensaje de depuración

    if (ilLoadImage((wchar_t *)filePath)) {
        std::cout << "Textura cargada exitosamente: " << filePath << std::endl; // Mensaje de depuración
        glGenTextures(1, &textureID);  // Generar textura en OpenGL
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
            0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
        glGenerateMipmap(GL_TEXTURE_2D);

        ilDeleteImages(1, &imageID);  // Liberar la imagen en DevIL
    }
    else {
        ILenum error = ilGetError();
        std::cerr << "DevIL error: " << iluErrorString(error) << "\n";
        std::cerr << "Error al cargar la textura: " << filePath << "\n";
        GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
        for (int i = 0; i < CHECKERS_HEIGHT; i++) {
            for (int j = 0; j < CHECKERS_WIDTH; j++) {
                int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
                checkerImage[i][j][0] = (GLubyte)c;
                checkerImage[i][j][1] = (GLubyte)c;
                checkerImage[i][j][2] = (GLubyte)c;
                checkerImage[i][j][3] = (GLubyte)255;
            }
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
    }
}


void Textures::BindTexture() {
    glBindTexture(GL_TEXTURE_2D, textureID);  // Enlazar la textura
}
