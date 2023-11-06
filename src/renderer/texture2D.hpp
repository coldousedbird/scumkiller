#pragma once

#include <glad/glad.h>
#include <string>

namespace renderer {
    class texture2D {
    public:
        texture2D(const GLuint width, GLuint height, 
                const unsigned char* data, 
                const unsigned int channels = 4, 
                const GLenum filter = GL_LINEAR, 
                const GLenum wrapMode = GL_CLAMP_TO_EDGE);
        
        texture2D() = delete;
        texture2D(const texture2D&) = delete;
        texture2D& operator=(const texture2D&) = delete;
        texture2D& operator=(texture2D&& texture2d);
        texture2D(texture2D&& texture2d);        
        ~texture2D();

        void bind() const;

    private:
        GLuint m_ID;
        GLenum m_mode;
        unsigned int m_width;
        unsigned int m_height;
    };
}