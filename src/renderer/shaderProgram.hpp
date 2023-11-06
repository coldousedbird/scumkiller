#pragma once 

#include <string>
#include <glad/glad.h>

namespace renderer {
   class shaderProgram {
   public:
      shaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
      ~shaderProgram();
      bool isCompiled() const { return m_isCompiled; };
      void use() const;

      shaderProgram() = delete;
      shaderProgram(shaderProgram&) = delete;
      shaderProgram& operator= (const shaderProgram&) = delete;
      shaderProgram& operator= (shaderProgram&& shaderProgram) noexcept;
      shaderProgram (shaderProgram&& shaderProgram) noexcept;

   private:
      bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
      
      bool m_isCompiled = false;
      GLuint m_ID = 0;
   }
}