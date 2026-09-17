/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include "graphics/CubicCurve.hpp"


namespace tbaricault::graphics
{

    CubicCurve::CubicCurve(CubicCurve::Point sp, CubicCurve::Point ep, CubicCurve::Point sc, CubicCurve::Point ec) noexcept
        : _sp(sp)
        , _ep(ep)
        , _sc(sc)
        , _ec(ec)
    {
        return;
    }

    CubicCurve::Point CubicCurve::next() noexcept
    {
        Point p0 = this->_sp + (this->_sc - this->_sp) * this->_t;
        Point p1 = this->_sc + (this->_ec - this->_sc) * this->_t;
        Point p2 = this->_ec + (this->_ep - this->_ec) * this->_t;
        p0 += (p1 - p0) * this->_t;
        p1 += (p2 - p1) * this->_t;
        Point r = p0 + (p1 - p0) * this->_t;
        this->_t += 1.0 / CubicCurve::RESOLUTION;
        return (r);
    }

}
