#pragma once

#include "shape.hpp"

/**
 * @brief Represents a model that gets rendered into the world
 * 
 */
class Model {
    Shape shape;

    public:
        /**
         * @brief Constructs a new Model object from a given Shape
         * 
         * @param shape The given shape
         */
        Model(Shape shape);

        /**
         * @brief Draws the model by calling glut's static functions
         * 
         */
        void draw();
};