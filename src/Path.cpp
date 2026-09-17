/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include <algorithm>

#include "graphics/Path.hpp"


namespace tbaricault::graphics
{

    unsigned int Path::RESOLUTION = 20;


    void Path::begin() noexcept
    {
        this->_t = 0;
        return;
    }

    bool Path::end() const noexcept
    {
        return (this->_t >= 1);
    }

    void Path::set(float t) noexcept
    {
        this->_t = std::clamp(t, 0.0f, 1.0f);
    }

}
