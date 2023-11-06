#pragma once 

#include <string>
#include <memory>
#include <map>

namespace renderer {
    class shaderProgram;
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

private:
    std::string getFileString(const std::string& relativeFilePath) const;
    
    typedef std::map<const std::string, std::shared_ptr<renderer::shaderProgram>> shaderProgramsMap;
    shaderProgramsMap m_shaderPrograms;

    std::string m_path;
};