/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include <GL/glew.h>

#include "graphics/Font.hpp"
#include "graphics/utils.hpp"


namespace tbaricault::graphics
{

    void init() noexcept
    {
        glewInit();
        Font::init();
        return;
    }

    void cleanup() noexcept
    {
        Font::cleanup();
        return;
    }

}
