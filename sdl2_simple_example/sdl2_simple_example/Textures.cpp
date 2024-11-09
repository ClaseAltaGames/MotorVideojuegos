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
        ILenum error = ilGetError();

        // Generar textura de ajedrez en caso de error
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

        isTextureLoaded = true;  // Marcar que se ha cargado una textura de comprobaci�n
        std::cerr << "La textura BLANCO y NEGRO ha sido cargada correctamente" << std::endl;

    }
}

void Textures::DrawTexture() {
    if (!isTextureLoaded) {
        return;  // Si la textura no ha sido cargada, no hacer nada
    }

    glBindTexture(GL_TEXTURE_2D, textureID);  // Enlazar la textura
}
