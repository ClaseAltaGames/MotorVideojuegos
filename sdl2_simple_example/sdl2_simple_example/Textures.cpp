#include "Textures.h"

Textures::Textures()
{
}

Textures::~Textures()
{
}


void Textures::DrawTextures(const char* filePath) {
    ILuint imageID;

    // Inicializa DevIL (esto debe hacerse solo una vez en el programa)
    ilInit();

    // Genera y enlaza un identificador de imagen de DevIL
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // Convertir const char* a const wchar_t*
    //std::wstring wideFilePath = charToWstring(filePath);

    // Carga la imagen con DevIL

    if (ilLoadImage((const wchar_t*)filePath)) {
        // Configura parámetros de la textura en OpenGL
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Transferencia de datos de imagen de DevIL a OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
            0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
        glGenerateMipmap(GL_TEXTURE_2D);

        // Borra la imagen de DevIL de la memoria
        ilDeleteImages(1, &imageID);
    }
    else {
        // Imprimir el error de DevIL
        ILenum error = ilGetError();
        std::cerr << "DevIL error: " << iluErrorString(error) << "\n";
        std::cerr << "Failed to load texture: " << filePath << "\n";
        std::cerr << "DevIL error: " << iluErrorString(error) << "\n";
    }
}
