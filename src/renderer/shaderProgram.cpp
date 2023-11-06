# include "shaderProgram.hpp"

namespace renderer {
    shaderProgram::shaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
        GLuint vertexShaderID;
        if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) { 
            std::cerr << "VERTEX SHADER compile time error\n"
            return;
        }

        GLuint fragmentShaderID;
        if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) { 
            std::cerr << "FRAGMENT SHADER compile time error\n"
            return;
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShaderID);
        glAttachShader(m_ID, fragmentShaderID);
        glLinkProgram(m_ID);

        GLint success;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(m_ID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER:Link time error:\n" << infoLog << "\n";
            return false;
        }
        else {
            m_isCompiled = true;
        }

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    bool shaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID) {
        shaderID = glCreateShader(shaderType);
        const char* code = source.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Compile time error:\n" << infoLog << "\n";
            return false;
        }
        return true;
    }

    shaderProgram::~shaderProgram() {
        glDeleteProgram(m_ID);
    }

    void shaderProgram::use() const {
        glUseProgram(m_ID);
    }

    shaderProgram& shaderProgram::shaderProgram& operator= (shaderProgram&& shaderProgram) noexcept {
        glDeleteProgram(m_ID);
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;

        return *this
    }
    
    shaderProgram::shaderProgram (shaderProgram&& shaderProgram) noexcept {
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;
    }
}