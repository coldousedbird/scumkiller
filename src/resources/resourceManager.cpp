#include "resourceManager.hpp"
#include "../renderer/shaderProgram.hpp"

#include "sstream"
#include "fstream"
#include "iostream"

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