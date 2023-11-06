#pragma once 

#include <string>
#include <memory>
#include <map>

namespace renderer {
    class shaderProgram;
    class texture2D;
}


class resourceManager {
public:
    resourceManager(const std::string& executablePath);
    ~resourceManager() = default;

    resourceManager(const resourceManager&) = delete;
    resourceManager& operator=(const resourceManager&) = delete;
    resourceManager& operator=(resourceManager&&) = delete;
    resourceManager(resourceManager&&) = delete;

    std::shared_ptr<renderer::shaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
    std::shared_ptr<renderer::shaderProgram> getShaderProgram(const std::string& shaderName);
    std::shared_ptr<renderer::texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
    std::shared_ptr<renderer::texture2D> getTexture(const std::string& textureName);

private:
    std::string getFileString(const std::string& relativeFilePath) const;
    
    typedef std::map<const std::string, std::shared_ptr<renderer::shaderProgram>> shaderProgramsMap;
    shaderProgramsMap m_shaderPrograms;

    typedef std::map<const std::string, std::shared_ptr<renderer::texture2D>> texturesMap;
    texturesMap m_textures;

    std::string m_path;
};