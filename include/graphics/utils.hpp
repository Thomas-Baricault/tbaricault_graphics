/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


namespace tbaricault::graphics
{

    /**
     * @brief Initializes the graphics module
     * 
     * This function must be called before any other function in the graphics module.
     */
    void init() noexcept;

    /**
     * @brief Cleans up the graphics module
     * 
     * After calling this function, no other graphics function should be used.
     */
    void cleanup() noexcept;

}
