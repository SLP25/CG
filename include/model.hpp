#pragma once

#include "shape.hpp"

class Model {
    Shape shape;

    public:
        Model(Shape shape);

        void draw();
};