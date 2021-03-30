#include "pch.hpp"
#include "Texture.hpp"

void lwvl::Texture::bind() {
    if (m_id != 0) {
        glActiveTexture(GL_TEXTURE0 + m_slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}

uint32_t lwvl::Texture::slot() const {
    return m_slot;
}

void lwvl::Texture::slot(uint32_t value) {
    m_slot = value;
    //    int32_t maxTextureUnits;
    //    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);
    //
    //    if (static_cast<uint32_t>(maxTextureUnits) < value) {
    //        m_slot = value;
    //    } else {
    //        throw std::exception("Max Texture Units Exceeded.");
    //    }
}

void lwvl::Texture::construct(
    uint32_t width, uint32_t height, const void *data,
    ChannelLayout internalFormat, ChannelOrder format,
    ByteFormat type
) {
    glTexImage2D(
        GL_TEXTURE_2D, 0, static_cast<GLint>(internalFormat), width, height, 0,
        static_cast<GLenum>(format), static_cast<GLenum>(type), data
    );
}

void lwvl::Texture::filter(lwvl::Filter value) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(value));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(value));
}
