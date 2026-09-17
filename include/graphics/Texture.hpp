/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include <GL/glew.h>

#include <tbaricault/images.hpp>
#include <tbaricault/math.hpp>


namespace tbaricault::graphics
{

    /**
     * @brief Wrapper around an OpenGL texture
     */
    class Texture final
    {

        public:

            /**
             * @brief Constructs an invalid texture
             */
            Texture() noexcept = default;

            /**
             * @brief Copy constructor
             * 
             * @param other Image to copy
             */
            Texture(const Texture& other);

            /**
             * @brief Move constructor
             * 
             * @param other Texture to move
             */
            Texture(Texture&& other) noexcept;

            /**
             * @brief Constructs a texture from an image
             * 
             * @param image Source image
             */
            Texture(const tbaricault::images::Image& image);

            /**
             * @brief Destructor
             */
            ~Texture();

            /**
             * @brief Copy assignment operator
             * 
             * @param other Texture to copy
             * 
             * @return Reference to this texture
             */
            Texture& operator=(const Texture& other);

            /**
             * @brief Move assignement operator
             * 
             * @param other Texture to move
             * 
             * @return Reference to this texture
             */
            Texture& operator=(Texture&& other) noexcept;

            /**
             * @brief Returns whether the texture is in a valid state
             */
            explicit operator bool() const noexcept;

            /**
             * @brief Converts the texture to image
             */
            operator tbaricault::images::Image() const;

            /**
             * @brief Returns the OpenGL texture identifier
             * 
             * @return Texture identifier
             */
            GLuint getGLElement() const noexcept;

            /**
             * @brief Returns the texture dimensions
             * 
             * @return Texture size (width, height)
             */
            const tbaricault::math::Vector2<int>& getSize() const noexcept;


        protected:

            /**
             * @brief OpenGL texture identifier
             */
            GLuint _glElement = 0;

            /**
             * @brief Texture dimensions
             */
            tbaricault::math::Vector2<int> _size;


            /**
             * @brief Constructs an OpenGL texture from pixel data
             * 
             * @param size Texture dimensions
             * @param data Pointer to pixel data (RGBA format)
             * 
             * @return OpenGL texture identifier
             */
            static GLuint _createTexture(const tbaricault::math::Vector2<int>& size, const tbaricault::colors::RGBA* data) noexcept;

            /**
             * @brief Returns texture pixel data
             * 
             * @return Pointer to pixel data (RGBA format)
             */
            tbaricault::colors::RGBA* _extractData() const;

    };

}
