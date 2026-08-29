/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include "graphics/CubicCurve.hpp"
#include "graphics/Renderer.hpp"


namespace tbaricault::graphics
{

    Renderer* Renderer::_active = nullptr;


    Renderer::Renderer(Renderer&& other) noexcept
        : _initialized(other._initialized)
        , _size(other._size)
        , _clips(std::move(other._clips))
        , _frameBuffer(other._frameBuffer)
        , _texture(other._texture)
    {
        other._initialized = false;
        other._size = 0;
        other._frameBuffer = 0;
        other._texture = 0;
        return;
    }

    Renderer::~Renderer()
    {
        this->_cleanup();
        return;
    }

    Renderer& Renderer::operator=(Renderer&& other) noexcept
    {
        if (&other == this)
            return (*this);
        this->_cleanup();
        this->_initialized = other._initialized;
        this->_size = other._size;
        this->_clips = std::move(other._clips);
        this->_frameBuffer = other._frameBuffer;
        this->_texture = other._texture;
        other._initialized = false;
        other._size = 0;
        other._frameBuffer = 0;
        other._texture = 0;
        return (*this);
    }

    const tbaricault::math::Vector2<int>& Renderer::getSize() const noexcept
    {
        return (this->_size);
    }

    void Renderer::resize(const tbaricault::math::Vector2<int>& size) noexcept
    {
        if (size != this->_size)
        {
            if (Renderer::_active)
                Renderer::_active->_unbind();
            this->_init();
            this->_size = size;
            glBindTexture(GL_TEXTURE_2D, this->_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->_size.x, this->_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        return;
    }

    void Renderer::clipBegin(const tbaricault::math::Rect<int>& rect)
    {
        this->_bind();
        this->_clips.push(
            (
                this->_clips.empty()
                    ? tbaricault::math::Rect<int>(this->_size)
                    : this->_clips.top()
            ) & rect
        );
        this->_applyViewport();
        return;
    }

    void Renderer::clipEnd()
    {
        this->_bind();
        this->_clips.pop();
        this->_applyViewport();
        return;
    }

    void Renderer::clear() noexcept
    {
        this->_bind();
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_QUADS);
        glColor4f(0, 0, 0, 1);
        glVertex2f(-1, -1);
        glVertex2f(1, -1);
        glVertex2f(1, 1);
        glVertex2f(-1, 1);
        glEnd();
        return;
    }

    void Renderer::drawRectangle(const tbaricault::math::Rect<int>& rect, const tbaricault::colors::Color& color) noexcept
    {
        if (color.a > 0 && rect.w > 0 && rect.h > 0)
        {
            this->_bind();
            glBegin(GL_QUADS);
            this->_color(color);
            this->_drawingPoint(rect.x, rect.y);
            this->_drawingPoint(rect.x + rect.w, rect.y);
            this->_drawingPoint(rect.x + rect.w, rect.y + rect.h);
            this->_drawingPoint(rect.x, rect.y + rect.h);
            glEnd();
        }
        return;
    }

    void Renderer::drawEllipse(const tbaricault::math::Rect<int>& rect, const tbaricault::colors::Color& color) noexcept
    {
        if (color.a > 0 && rect.w > 0 && rect.h > 0)
        {
            this->_bind();
            glBegin(GL_TRIANGLE_FAN);
            this->_color(color);
            tbaricault::math::Vector2<float> c = rect.size() * 0.2761423749f;
            CubicCurve::Point point(rect.x + rect.w / 2, rect.y);
            CubicCurve curve(
                point,
                CubicCurve::Point(rect.x + rect.w, rect.y + rect.h / 2),
                CubicCurve::Point(rect.x + rect.w / 2 + c.x, rect.y),
                CubicCurve::Point(rect.x + rect.w, rect.y + rect.h / 2 - c.y)
            );
            this->_drawingPoint(point.x, point.y);
            while (!curve.end())
            {
                point = curve.next();
                this->_drawingPoint(point.x, point.y);
            }
            curve = CubicCurve(
                point,
                CubicCurve::Point(rect.x + rect.w / 2, rect.y + rect.h),
                CubicCurve::Point(rect.x + rect.w, rect.y + rect.h / 2 + c.y),
                CubicCurve::Point(rect.x + rect.w / 2 + c.x, rect.y + rect.h)
            );
            this->_drawingPoint(point.x, point.y);
            while (!curve.end())
            {
                point = curve.next();
                this->_drawingPoint(point.x, point.y);
            }
            curve = CubicCurve(
                point,
                CubicCurve::Point(rect.x, rect.y + rect.h / 2),
                CubicCurve::Point(rect.x + rect.w / 2 - c.x, rect.y + rect.h),
                CubicCurve::Point(rect.x, rect.y + rect.h / 2 + c.y)
            );
            this->_drawingPoint(point.x, point.y);
            while (!curve.end())
            {
                point = curve.next();
                this->_drawingPoint(point.x, point.y);
            }
            curve = CubicCurve(
                point,
                CubicCurve::Point(rect.x + rect.w / 2, rect.y),
                CubicCurve::Point(rect.x, rect.y + rect.h / 2 - c.y),
                CubicCurve::Point(rect.x + rect.w / 2 - c.x, rect.y)
            );
            this->_drawingPoint(point.x, point.y);
            while (!curve.end())
            {
                point = curve.next();
                this->_drawingPoint(point.x, point.y);
            }
            glEnd();
        }
        return;
    }

    void Renderer::drawText(const tbaricault::math::Vector2<int> pos, const tbaricault::str::UTF8String& text, Font& font, int size, const tbaricault::colors::Color& color)
    {
        if (font)
        {
            tbaricault::math::Vector2<int> origin = pos;
            for (char32_t c : text)
            {
                const Font::Glyph* glyph = font.getGlyph(c, size);
                if (glyph)
                {
                    this->drawTexture(tbaricault::math::Rect(origin + glyph->offset, glyph->texture.getSize()), glyph->texture, 0, color);
                    origin.x += glyph->size.x;
                }
            }
        }
        return;
    }

    void Renderer::drawTexture(const tbaricault::math::Rect<int>& rect, const Texture& texture, const tbaricault::math::Rect<int>& srcRect, const tbaricault::colors::Color& mask) noexcept
    {
        this->_drawTexture(rect, texture.getGLElement(), texture.getSize(), srcRect, mask);
        return;
    }

    void Renderer::drawRenderer(const tbaricault::math::Rect<int>& rect, const Renderer& renderer, const tbaricault::math::Rect<int>& srcRect, const tbaricault::colors::Color& mask) noexcept
    {
        this->_drawTexture(rect, renderer._texture, renderer._size, srcRect, mask);
        return;
    }

    void Renderer::drawToWindow(const tbaricault::math::Vector2<int>& size, const tbaricault::math::Rect<int>& rect) noexcept
    {
        if (Renderer::_active)
            Renderer::_active->_unbind();
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, this->_texture);
        glViewport(0, 0, size.x, size.y);
        glBegin(GL_QUADS);
        glColor4f(1, 1, 1, 1);
        glTexCoord2f(0, 0);
        this->_renderPoint(rect.x, rect.y, size.x, size.y);
        glTexCoord2f(1, 0);
        this->_renderPoint(rect.x + rect.w, rect.y, size.x, size.y);
        glTexCoord2f(1, 1);
        this->_renderPoint(rect.x + rect.w, rect.y + rect.h, size.x, size.y);
        glTexCoord2f(0, 1);
        this->_renderPoint(rect.x, rect.y + rect.h, size.x, size.y);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        return;
    }

    void Renderer::_init() noexcept
    {
        if (!this->_initialized)
        {
            if (Renderer::_active)
                Renderer::_active->_unbind();
            this->_initialized = true;
            glGenTextures(1, &this->_texture);
            glBindTexture(GL_TEXTURE_2D, this->_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->_size.x, this->_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
            glGenFramebuffers(1, &this->_frameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, this->_frameBuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_texture, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        return;
    }

    void Renderer::_cleanup() noexcept
    {
        if (this->_initialized)
        {
            this->_unbind();
            glDeleteTextures(1, &this->_texture);
            glDeleteFramebuffers(1, &this->_frameBuffer);
        }
        return;
    }

    void Renderer::_bind() noexcept
    {
        this->_init();
        if (Renderer::_active != this)
        {
            if (Renderer::_active)
                Renderer::_active->_unbind();
            Renderer::_active = this;
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindFramebuffer(GL_FRAMEBUFFER, this->_frameBuffer);
            this->_applyViewport();
        }
        return;
    }

    void Renderer::_unbind() noexcept
    {
        if (Renderer::_active == this)
        {
            Renderer::_active = nullptr;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
        }
        return;
    }

    void Renderer::_applyViewport() noexcept
    {
        if (Renderer::_active == this)
        {
            this->_clip = (
                this->_clips.empty()
                ? tbaricault::math::Rect<int>(this->_size)
                : this->_clips.top()
            );
            glViewport(
                this->_clip.x,
                this->_clip.y,
                this->_clip.w,
                this->_clip.h
            );
        }
        return;
    }

    void Renderer::_color(const tbaricault::colors::Color& color) noexcept
    {
        glColor4f(
            color.r / 255.0,
            color.g / 255.0,
            color.b / 255.0,
            color.a / 255.0
        );
        return;
    }

    void Renderer::_drawingPoint(int x, int y) noexcept
    {
        glVertex2f(
            2.0 / this->_clip.w * (x - this->_clip.x) - 1,
            2.0 / this->_clip.h * (y - this->_clip.y) - 1
        );
        return;
    }

    void Renderer::_renderPoint(int x, int y, int w, int h) noexcept
    {
        glVertex2f(
            2.0 / w * x - 1,
            2.0 / h * (h - y - 1) - 1
        );
        return;
    }

    void Renderer::_drawTexture(const tbaricault::math::Rect<int>& rect, GLuint texture, const tbaricault::math::Vector2<int> size, const tbaricault::math::Rect<int>& srcRect, const tbaricault::colors::Color& mask) noexcept
    {
        if (texture && mask.a > 0)
        {
            this->_bind();
            glBindTexture(GL_TEXTURE_2D, texture);
            glBegin(GL_QUADS);
            this->_color(mask);
            if (srcRect.area() == 0)
            {
                glTexCoord2f(0, 0);
                this->_drawingPoint(rect.x, rect.y);
                glTexCoord2f(1, 0);
                this->_drawingPoint(rect.x + rect.w, rect.y);
                glTexCoord2f(1, 1);
                this->_drawingPoint(rect.x + rect.w, rect.y + rect.h);
                glTexCoord2f(0, 1);
                this->_drawingPoint(rect.x, rect.y + rect.h);
            }
            else
            {
                glTexCoord2f(srcRect.x / static_cast<float>(size.x), srcRect.y / static_cast<float>(size.y));
                this->_drawingPoint(rect.x, rect.y);
                glTexCoord2f((srcRect.x + srcRect.w) / static_cast<float>(size.x), srcRect.y / static_cast<float>(size.y));
                this->_drawingPoint(rect.x + rect.w, rect.y);
                glTexCoord2f((srcRect.x + srcRect.w) / static_cast<float>(size.x), (srcRect.y + srcRect.h) / static_cast<float>(size.y));
                this->_drawingPoint(rect.x + rect.w, rect.y + rect.h);
                glTexCoord2f(srcRect.x / static_cast<float>(size.x), (srcRect.y + srcRect.h) / static_cast<float>(size.y));
                this->_drawingPoint(rect.x, rect.y + rect.h);
            }
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        return;
    }

}
