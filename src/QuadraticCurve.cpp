/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include "graphics/QuadraticCurve.hpp"


namespace tbaricault::graphics
{

    QuadraticCurve::QuadraticCurve(QuadraticCurve::Point s, QuadraticCurve::Point e, QuadraticCurve::Point c) noexcept
        : _s(s)
        , _e(e)
        , _c(c)
    {
        return;
    }

    QuadraticCurve::Point QuadraticCurve::next() noexcept
    {
        Point p0 = this->_s + (this->_c - this->_s) * this->_t;
        Point p1 = this->_c + (this->_e - this->_c) * this->_t;
        Point r = p0 + (p1 - p0) * this->_t;
        this->_t += 1.0 / QuadraticCurve::RESOLUTION;
        return (r);
    }

}
