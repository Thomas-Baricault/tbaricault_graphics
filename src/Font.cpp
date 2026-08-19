/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include <tbaricault/colors.hpp>

#include "graphics/Font.hpp"


namespace tbaricault::graphics
{

    FT_Library Font::_ftLibrary = nullptr;


    void Font::init() noexcept
    {
        FT_Init_FreeType(&Font::_ftLibrary);
        return;
    }

    void Font::cleanup() noexcept
    {
        FT_Done_FreeType(Font::_ftLibrary);
        return;
    }

    Font::Font(FT_Face face) noexcept
        : _face(face)
    {
        return;
    }

    Font::Font(const std::string& path) noexcept
    {
        FT_New_Face(Font::_ftLibrary, path.c_str(), 0, &this->_face);
        return;
    }

    Font::Font(Font&& other) noexcept
        : _face(other._face)
        , _size(other._size)
        , _caches(std::move(other._caches))
    {
        other._face = nullptr;
        other._size = 0;
        return;
    }

    Font::~Font()
    {
        FT_Done_Face(this->_face);
        return;
    }

    Font::operator bool() const noexcept
    {
        return (this->_face != nullptr);
    }

    int Font::getLineHeight(int size)
    {
        if (size <= 0)
            return (0);
        this->_setSize(size);
        return (this->_caches.at(size).lineHeight);
    }

    const Font::Glyph* Font::getGlyph(FT_ULong code, int size)
    {
        if (size <= 0)
            return (nullptr);
        this->_setSize(size);
        Cache& cache = this->_caches.at(size);
        if (cache.lineHeight == 0)
            return (nullptr);
        auto it = cache.glyphs.find(code);
        if (it == cache.glyphs.end())
        {
            Glyph* glyph = &(cache.glyphs[code] = {});
            if (FT_Load_Char(this->_face, code, FT_LOAD_RENDER))
            {
                cache.glyphs.erase(code);
                return (nullptr);
            }
            else
            {
                int ascender = this->_face->size->metrics.ascender >> 6;
                int descender = abs(this->_face->size->metrics.descender >> 6);
                int size = this->_face->glyph->bitmap.width * this->_face->glyph->bitmap.rows;
                tbaricault::colors::RGBA* data = new tbaricault::colors::RGBA[size];
                for (int i = 0; i < size; i++)
                    data[i] = this->_face->glyph->bitmap.buffer[i] * 0x1010101;
                glyph->size = tbaricault::math::Vector2<int>(
                    this->_face->glyph->advance.x >> 6,
                    ascender + descender
                );
                glyph->offset = tbaricault::math::Vector2<int>(
                    this->_face->glyph->bitmap_left,
                    ascender - this->_face->glyph->bitmap_top
                );
                glyph->texture = Texture(tbaricault::images::Image(
                    tbaricault::math::Vector2<int>(
                        this->_face->glyph->bitmap.width,
                        this->_face->glyph->bitmap.rows
                    ),
                    data, false
                ));
                return (glyph);
            }
        }
        return (&it->second);
    }

    void Font::clear() noexcept
    {
        this->_caches.clear();
        return;
    }

    void Font::_setSize(int size)
    {
        if (size != this->_size)
        {
            this->_size = size;
            int error = FT_Set_Pixel_Sizes(this->_face, 0, size);
            if (!this->_caches.contains(size))
            {
                this->_caches[size] = {
                    error
                        ? 0
                        : (this->_face->size->metrics.ascender - this->_face->size->metrics.descender) >> 6,
                    {}
                };
            }
        }
        return;
    }

}
