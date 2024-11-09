#include "Textures.h"
#include <iostream>
#include <string>
#include "Component.h"

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64


Textures::Textures() : Component(), textureID(0), isTextureLoaded(false) {
}

Textures::~Textures() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);  // Liberar memoria de la textura en OpenGL
    }
}

void Textures::LoadTexture(const char* filePath) {
    // Liberar textura previa si está cargada
    if (isTextureLoaded) {
        glDeleteTextures(1, &textureID);  // Liberar la textura previa en OpenGL
        textureID = 0;
        isTextureLoaded = false;
    }

    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    std::cout << "Intentando cargar la textura desde: " << filePath << std::endl;

    if (ilLoadImage((wchar_t*)filePath)) {
        std::cout << "Textura cargada exitosamente: " << filePath << std::endl;
        glGenTextures(1, &textureID);  // Generar nueva textura
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
            0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
        glGenerateMipmap(GL_TEXTURE_2D);

        isTextureLoaded = true;  // Marcar que la textura fue cargada exitosamente

        ilDeleteImages(1, &imageID);  // Liberar la imagen en DevIL
    }
    else {
        // Código para generar textura de comprobación
        // ...
    }
}

void Textures::DrawTexture() {
    if (!isTextureLoaded) {
        return;  // Si la textura no ha sido cargada, no hacer nada
    }

    glBindTexture(GL_TEXTURE_2D, textureID);  // Enlazar la textura
}
