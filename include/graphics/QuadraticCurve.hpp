/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include "Path.hpp"


namespace tbaricault::graphics
{

    /**
     * @brief Quadratic Bézier curve
     * 
     * A quadratic Bézier curve is defined by two end points and a single control point.
     */
    class QuadraticCurve final
        : public Path
    {

        public:

            /**
             * @brief Default constructor is disabled
             */
            QuadraticCurve() noexcept = delete;

            /**
             * @brief Constructs a quadratic Bézier curve
             *
             * @param s Starting point of the curve
             * @param e Ending point of the curve
             * @param c Control point influencing the shape of the curve
             */
            QuadraticCurve(Point s, Point e, Point c) noexcept;

            /**
             * @brief Copy constructor
             * 
             * @param other Curve to copy
             */
            QuadraticCurve(const QuadraticCurve& other) noexcept = default;

            /**
             * @brief Move constructor
             * 
             * @param other Curve to move
             */
            QuadraticCurve(QuadraticCurve&& other) noexcept = default;

            /**
             * @brief Destructor
             */
            virtual ~QuadraticCurve() = default;

            /**
             * @brief Copy assignement operator
             * 
             * @param other Curve to copy
             * 
             * @return Reference to this curve
             */
            QuadraticCurve& operator=(const QuadraticCurve& other) noexcept = default;

            /**
             * @brief Move assignement operator
             * 
             * @param other Curve to move
             * 
             * @return Reference to this curve
             */
            QuadraticCurve& operator=(QuadraticCurve&& other) noexcept = default;

            /**
             * @brief Returns the next point on the curve
             * 
             * @return Next sampled point
             */
            Point next() noexcept override;


        protected:

            /**
             * @brief Starting point of the curve
             */
            Point _s;

            /**
             * @brief Ending point of the curve
             */
            Point _e;

            /**
             * @brief Control point influencing the shape of the curve
             */
            Point _c;

    };

}
