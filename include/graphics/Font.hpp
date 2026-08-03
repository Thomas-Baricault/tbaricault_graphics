/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <tbaricault/math.hpp>

#include "Texture.hpp"


namespace tbaricault::graphics
{

    /**
     * @brief Wrapper around FreeType font face
     */
    class Font final
    {

        public:

            /**
             * @brief Represents a rasterized glyph
             */
            struct Glyph
            {

                /**
                 * @brief Glyph bitmap dimensions in pixels
                 */
                tbaricault::math::Vector2<int> size;

                /**
                 * @brief Offset from the text baseline to the glyph origin.
                 */
                tbaricault::math::Vector2<int> offset;

                /**
                 * @brief Texture containing the glyph bitmap.
                 */
                Texture texture;

            };


            /**
             * @brief Initializes the FreeType library
             * 
             * This function must be called before any font operation.
             */
            static void init() noexcept;

            /**
             * @brief Shuts down the FreeType library
             * 
             * After calling this function, no other font functions should be used.
             */
            static void cleanup() noexcept;

            /**
             * @brief Constructs an invalid font
             */
            Font() noexcept = default;

            /**
             * @brief Copy constructor is disabled
             */
            Font(const Font&) = delete;

            /**
             * @brief Move constructor
             * 
             * @param other Font to move
             */
            Font(Font&&) noexcept;

            /**
             * @brief Constructs a font from an existing FreeType face
             * 
             * @param face FreeType font face
             */
            Font(FT_Face face) noexcept;

            /**
             * @brief Loads a font from a file
             * 
             * @param path Path to the font file
             */
            Font(const std::string& path) noexcept;

            /**
             * @brief Destructor
             */
            ~Font();

            /**
             * @brief Copy assignment is disabled
             */
            Font& operator=(const Font&) = delete;

            /**
             * @brief Move assignment operator
             * 
             * @param other Font to move
             * 
             * @return Reference to this font
             */
            Font& operator=(Font&& other) noexcept;

            /**
             * @brief Returns whether the font is in a valid state
             */
            explicit operator bool() const noexcept;

            /**
             * @brief Returns the line height for a given font size
             * 
             * The value is cached after the first request.
             * 
             * @param size Font size in pixels
             * 
             * @return Line height in pixels
             */
            int getLineHeight(int size);

            /**
             * @brief Returns the glyph corresponding to a Unicode code point
             * 
             * The glyph is rasterized and cached on first use.
             * 
             * @param code Unicode code point
             * @param size Font size in pixels
             * 
             * @return Pointer to the requested glyph, or nullptr on error
             */
            const Glyph* getGlyph(FT_ULong code, int size);

            /**
             * @brief Removes all cached glyphs
             */
            void clear() noexcept;


        private:

            /**
             * @brief Represents cached data for specific font size
             */
            struct Cache
            {

                /**
                 * @brief Line height in pixels
                 */
                int lineHeight = 0;

                /**
                 * @brief Cached glyphs
                 */
                std::unordered_map<FT_ULong, Glyph> glyphs;
            };


            /**
             * @brief FreeType library handle
             */
            static FT_Library _ftLibrary;

            /**
             * @brief FreeType face handle
             */
            FT_Face _face = nullptr;

            /**
             * @brief Active font size
             */
            int _size = 0;

            /**
             * @brief Cached data
             */
            std::unordered_map<int, Cache> _caches;


            /**
             * @brief Sets active font size
             * 
             * @param size Font size
             */
            void _setSize(int size);

    };

}
