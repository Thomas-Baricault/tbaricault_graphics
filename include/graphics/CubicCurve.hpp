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
     * @brief Cubic Bézier curve
     *
     * A cubic Bézier curve is defined by two end points and two control points
     */
    class CubicCurve final
        : public Path
    {

        public:

            /**
             * @brief Default constructor is disabled
             */
            CubicCurve() noexcept = delete;

             /**
              * @brief Constructs a cubic Bézier curve
              *
              * @param sp Starting point of the curve
              * @param ep Ending point of the curve
              * @param sc Control point influencing the beginning of the curve
              * @param ec Control point influencing the end of the curve
              */
            CubicCurve(Point sp, Point ep, Point sc, Point ec) noexcept;

            /**
             * @brief Copy constructor
             * 
             * @param other Curve to copy
             */
            CubicCurve(const CubicCurve& other) noexcept = default;

            /**
             * @brief Move constructor
             * 
             * @param other Curve to move
             */
            CubicCurve(CubicCurve&& other) noexcept = default;

            /**
             * @brief Destructor
             */
            virtual ~CubicCurve() = default;

            /**
             * @brief Copy assignement operator
             * 
             * @param other Curve to copy
             * 
             * @return Reference to this curve
             */
            CubicCurve& operator=(const CubicCurve& other) noexcept = default;

            /**
             * @brief Move assignement operator
             * 
             * @param other Curve to move
             * 
             * @return Reference to this curve
             */
            CubicCurve& operator=(CubicCurve&& other) noexcept = default;

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
            Point _sp;

            /**
             * @brief Ending point of the curve
             */
            Point _ep;

            /**
             * @brief Control point influencing the beginning of the curve
             */
            Point _sc;

            /**
             * @brief Control point influencing the end of the curve
             */
            Point _ec;

    };

}
