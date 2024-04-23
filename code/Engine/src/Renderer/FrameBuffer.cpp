//
// Created by Radek on 04.04.2024.
//

#include "Renderer/FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height) {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    Specification.width = width;
    Specification.height = height;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Specification.width, Specification.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, Specification.width, Specification.height);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int FrameBuffer::readPixel(int attachmentIndex, int x, int y) {
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixelData;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
}