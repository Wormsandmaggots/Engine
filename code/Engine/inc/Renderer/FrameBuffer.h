//
// Created by Radek on 04.04.2024.
//

#ifndef ENGINE_FRAMEBUFFER_H
#define ENGINE_FRAMEBUFFER_H

#include <vector>
#include "glad/glad.h"

enum class FramebufferTextureFormat
{
    None = 0,

    // Color
    RGBA8,
    RED_INTEGER,

    // Depth/stencil
    DEPTH24STENCIL8,

    // Defaults
    Depth = DEPTH24STENCIL8
};

struct Attachment{
    FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
};

struct FramebufferSpecification
{
    FramebufferSpecification() = default;
    FramebufferSpecification(std::initializer_list<Attachment> attachments)
    : attachments(attachments) {}

    int width = 0, height = 0;
    std::vector<Attachment> attachments;

    uint32_t Samples = 1;

    bool SwapChainTarget = false;
};

class FrameBuffer {
public:
    FramebufferSpecification Specification;

    FrameBuffer(int,int,int = 0);

    void bind();
    void unbind();

    int readPixel(int, int, int);

    void drawQuad() const;

    int getTexture() const;
    int getBloomTexture() const;
private:
    unsigned int fbo;
    unsigned int texture;
    unsigned int bloomTexture;
    unsigned int quadVAO, quadVBO;
};


#endif //ENGINE_FRAMEBUFFER_H
