#include "texture2D.hpp"

namespace renderer {
    texture2D::texture2D(const GLuint width, GLuint height, 
                    const unsigned char* data, 
                    const unsigned int channels, 
                    const GLenum filter, //NEAREST
                    const GLenum wrapMode) 
    {
        switch (channels) {
            case 4: 
                m_mode = GL_RGBA;
            case 3:
                m_mode = GL_RGB;
            default:
                m_mode = GL_RGBA;
                break; 
        }
        
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, wrapMode);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    texture2D& texture2D::operator=(texture2D&& texture2d) {
        glDeleteTextures(1, &m_ID);        
        m_ID = texture2d.m_ID;
        texture2d.m_ID = 0;
        m_mode = texture2d.m_mode;
        m_width = texture2d.m_width;
        m_height = texture2d.m_height;
        return *this;
    }

    texture2D::texture2D(texture2D&& texture2d) {
        m_ID = texture2d.m_ID;
        texture2d.m_ID = 0;
        m_mode = texture2d.m_mode;
        m_width = texture2d.m_width;
        m_height = texture2d.m_height;
    }

    texture2D::~texture2D() {
        glDeleteTextures(1, &m_ID);
    }

    void texture2D::bind() const {

    }
}
