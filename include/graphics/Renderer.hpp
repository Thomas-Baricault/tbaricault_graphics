/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include <stack>

#include <tbaricault/colors.hpp>
#include <tbaricault/math.hpp>
#include <tbaricault/str.hpp>

#include "Font.hpp"
#include "Texture.hpp"


namespace tbaricault::graphics
{

    /**
     * @brief 2D renderer
     */
    class Renderer final
    {

        public:

            /**
             * @brief Constructs an empty renderer
             */
            Renderer() noexcept = default;

            /**
             * @brief Copy constructor is disabled
             */
            Renderer(const Renderer&) = delete;

            /**
             * @brief Move constructor
             * 
             * @param other Renderer to move
             */
            Renderer(Renderer&& other) noexcept;

            /**
             * @brief Destructor
             */
            ~Renderer();

            /**
             * @brief Copy assignment operator is disabled
             */
            Renderer& operator=(const Renderer&) = delete;

            /**
             * @brief Move assignment operator
             * 
             * @param other Renderer to move
             * 
             * @return Reference to this renderer
             */
            Renderer& operator=(Renderer&& other) noexcept;

            /**
             * @brief Returns the size of the rendering target
             * 
             * @return Renderer size in pixels
             */
            const tbaricault::math::Vector2<int>& getSize() const noexcept;

            /**
             * @brief Resizes the rendering target
             * 
             * @param size New size in pixels
             */
            void resize(const tbaricault::math::Vector2<int>& size) noexcept;

            /**
             * @brief Begins a clipping region
             * 
             * Drawing operations are restricted to the specified area until clipEnd() is called.
             * 
             * @param area Clipping rectangle
             */
            void clipBegin(const tbaricault::math::Rect<int>& area);

            /**
             * @brief Ends the current clipping region
             */
            void clipEnd();

            /**
             * @brief Clears the rendering target
             */
            void clear() noexcept;

            /**
             * @brief Draws a filled rectangle
             * 
             * @param rect Destination rectangle
             * @param color Fill color
             */
            void drawRectangle(const tbaricault::math::Rect<int>& rect, const tbaricault::colors::Color& color) noexcept;

            /**
             * @brief Draws a filled ellipse
             * 
             * @param rect Bounding rectangle
             * @param color Fill color
             */
            void drawEllipse(const tbaricault::math::Rect<int>& rect, const tbaricault::colors::Color& color) noexcept;

            /**
             * @brief Draws a UTF-8 string
             * 
             * @param pos Text position
             * @param text Text to render
             * @param font Font used for rendering
             * @param size Font size in pixels
             * @param color Text color
             */
            void drawText(const tbaricault::math::Vector2<int> pos, const tbaricault::str::UTF8String& text, Font& font, int size, const tbaricault::colors::Color& color);

            /**
             * @brief Draws a texture
             * 
             * @param rect Destination rectangle
             * @param texture Texture to draw
             * @param srcRect Source rectangle within the texture (a value of 0 selects the entire texture)
             * @param mask Color multiplier applied during rendering
             */
            void drawTexture(const tbaricault::math::Rect<int>& rect, const Texture& texture, const tbaricault::math::Rect<int>& srcRect = 0, const tbaricault::colors::Color& mask = tbaricault::colors::Constant::White) noexcept;

            /**
             * @brief Draws the contents of another renderer
             * 
             * @param rect Destination rectangle
             * @param renderer Source renderer
             * @param srcRect Source rectangle within the renderer (a value of 0 selects the entire renderer)
             * @param mask Color multiplier applied during rendering
             */
            void drawRenderer(const tbaricault::math::Rect<int>& rect, const Renderer& renderer, const tbaricault::math::Rect<int>& srcRect = 0, const tbaricault::colors::Color& mask = tbaricault::colors::Constant::White) noexcept;

            /**
             * @brief Copies the renderer contents to the currently bound window
             * 
             * @param size Window size in pixels
             * @param rect Destination rectangle in the window
             */
            void drawToWindow(const tbaricault::math::Vector2<int>& size, const tbaricault::math::Rect<int>& rect) noexcept;


        private:

            /**
             * @brief Active renderer
             */
            static Renderer* _active;

            /**
             * @brief Whether the renderer is initialized
             */
            bool _initialized = false;

            /**
             * @brief Rendering target dimensions
             */
            tbaricault::math::Vector2<int> _size;

            /**
             * @brief Clipping regions stack
             */
            std::stack<tbaricault::math::Rect<int>> _clips;

            /**
             * @brief Current clipping region
             */
            tbaricault::math::Rect<int> _clip;

            /**
             * @brief Framebuffer identifier
             */
            GLuint _frameBuffer = 0;

            /**
             * @brief Texture identifier
             */
            GLuint _texture = 0;


            /**
             * @brief Initializes the renderer
             */
            void _init() noexcept;

            /**
             * @brief Releases renderer resources
             */
            void _cleanup() noexcept;

            /**
             * @brief Makes the renderer the current rendering target
             */
            void _bind() noexcept;

            /**
             * @brief Restores default rendering target
             */
            void _unbind() noexcept;

            /**
             * @brief Applies the current viewport and clipping region
             */
            void _applyViewport() noexcept;

            /**
             * @brief Sets the current drawing color
             * 
             * @param color Color to apply
             */
            void _color(const tbaricault::colors::Color& color) noexcept;

            /**
             * @brief Emits a vertex for the current primitive to draw on the renderer
             *
             * @param x X coordinate
             * @param y Y coordinate
             */
            void _drawingPoint(int x, int y) noexcept;

            /**
             * @brief Emits a vertex for the current primitive to draw on other surface
             *
             * @param x X coordinate
             * @param y Y coordinate
             * @param w Destination surface width
             * @param h Destination surface height
             */
            void _renderPoint(int x, int y, int w, int h) noexcept;

            /**
             * @brief Draws a texture
             * 
             * @param rect Destination rectangle
             * @param texture OpenGL texture identifier
             * @param textureSize Texture dimensions
             * @param srcRect Source rectangle within the texture
             * @param mask Color multiplier applied during rendering
             */
            void _drawTexture(const tbaricault::math::Rect<int>& rect, GLuint texture, const tbaricault::math::Vector2<int> size, const tbaricault::math::Rect<int>& srcRect, const tbaricault::colors::Color& mask) noexcept;

    };

}
