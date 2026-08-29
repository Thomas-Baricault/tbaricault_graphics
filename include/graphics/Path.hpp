/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include <tbaricault/math.hpp>


namespace tbaricault::graphics
{

    /**
     * @brief Abstract base class representing an iterable geometric path
     * 
     * A Path generates a sequence of 2D points over time using an internal progression parameter.
     * 
     * Derived classes must implement the point generation logic.
     */
    class Path
    {

        public:

            /**
             * @brief Point type
             */
            using Point = tbaricault::math::Vector2<float>;


            /**
             * @brief Global resolution controlling sampling precision
             * 
             * Higher values generally produce smoother paths but increase computation cost.
             */
            static unsigned int RESOLUTION;


            /**
             * @brief Default constructor
             */
            Path() noexcept = default;

            /**
             * @brief Copy constructor
             * 
             * @param other Path to copy
             */
            Path(const Path& other) noexcept = default;

            /**
             * @brief Move constructor
             * 
             * @param other Path to move
             */
            Path(Path&& other) noexcept = default;

            /**
             * @brief Destructor
             */
            virtual ~Path() = default;

            /**
             * @brief Copy assignment operator
             * 
             * @param other Path to copy
             * 
             * @return Reference to this path
             */
            Path& operator=(const Path& other) noexcept = default;

            /**
             * @brief Move assignement operator
             * 
             * @param other Path to move
             * 
             * @return Reference to this path
             */
            Path& operator=(Path&& other) noexcept = default;

            /**
             * @brief Resets the path iteration state
             */
            void begin() noexcept;

            /**
             * @brief Checks whether the path has been fully traversed
             * 
             * @return `true` if iteration is complete, `false` otherwise
             */
            bool end() const noexcept;

            /**
             * @brief Sets the path iteration state
             * 
             * @param t State in the range [0; 1]
             */
            void set(float t) noexcept;

            /**
             * @brief Returns the next point on the path
             * 
             * @return Next sampled point
             */
            virtual Point next() = 0;


        protected:

            /**
             * @brief Current curve state in the range [0; 1]
             */
            float _t = 0;

    };

}
