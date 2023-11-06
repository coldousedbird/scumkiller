#include "resourceManager.hpp"
#include "../renderer/shaderProgram.hpp"
#include "../renderer/texture2D.hpp"

#include "sstream"
#include "fstream"
#include "iostream"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


resourceManager::resourceManager(const std::string& executablePath) {
    size_t found = executablePath.find_last_of("/\\");
    m_path = executablePath.substr(0, found);
}

std::string resourceManager::getFileString(const std::string& relativeFilePath) const {
    std::ifstream f;
    f.open(m_path+"/"+relativeFilePath.c_str(), std::ios::in | std::ios::binary);
    if (!f.is_open()) {
        std::cerr << "failed to open file: " << relativeFilePath << "\n";
        return std::string{};
    }

    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();

}

std::shared_ptr<renderer::shaderProgram> resourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexString = getFileString(vertexPath);
    if (vertexString.empty()) {
        std::cerr << "No vertex shader!\n";  
        return nullptr;
    }
    
    std::string fragmentString = getFileString(fragmentPath);
    if (fragmentString.empty()) {
        std::cerr << "No fragment shader!\n";  
        return nullptr;
    }

    std::shared_ptr<renderer::shaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<renderer::shaderProgram>(vertexString, fragmentString)).first->second;
    if (newShader->isCompiled())
        return newShader;
    
    std::cerr << "Can't load shader program:\n"
              << "Vertex: " << vertexPath << "\n"
              << "Fragment: " << fragmentPath << "\n";
    
    return nullptr;
}

std::shared_ptr<renderer::shaderProgram> resourceManager::getShaderProgram(const std::string& shaderName) {
    shaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
    if (it != m_shaderPrograms.end())
        return it->second;

    std::cerr << "Can't find the shader program: " << shaderName << std::endl;
    return nullptr;
}

std::shared_ptr<renderer::texture2D> resourceManager::loadTexture(const std::string& textureName, const std::string& texturePath) {
    int channels = 0;
    int width = 0;
    int height = 0;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(std::string(m_path+"/"+texturePath).c_str(), &width, &height, &channels, 0);

    if (!pixels) {
        std::cerr << "Can't load image: " << texturePath << "\n";
        return nullptr;
    }
    
    std::shared_ptr<renderer::texture2D> newTexture 
                                    = m_textures.emplace(
                                        textureName, 
                                        std::make_shared<renderer::texture2D>(
                                        width, height, pixels, channels, GL_NEAREST, 
                                        GL_CLAMP_TO_EDGE)).first->second;

    stbi_image_free(pixels);
    return newTexture;
}

std::shared_ptr<renderer::texture2D> resourceManager::getTexture(const std::string& textureName) {
    texturesMap::const_iterator it = m_textures.find(textureName);
    if (it != m_textures.end())
        return it->second;

    std::cerr << "Can't find the texture: " << textureName << std::endl;
    return nullptr;
}