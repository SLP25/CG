#pragma once

#include "utils.hpp"
#include "parser.hpp"
#include <iostream>

class Camera;

template<class... Cameras> struct helper {
    static std::unique_ptr<Camera> parseCamera(std::string type, XMLParser parser);
};

class Camera {

    public:
        static std::unique_ptr<Camera> parse(XMLParser parser);

        virtual ~Camera();

        virtual void initScene(WindowSize windowSize) = 0;
        virtual void setupScene() = 0;
        virtual void handleKey(unsigned char key, int x, int y);
        virtual void handleSpecialKey(int key, int x, int y);

    protected:
        static void defaultInitScene(WindowSize windowSize, float fov, float near, float far);

    private:
        template<class... Cameras>
        static std::unique_ptr<Camera> parseCamera(std::string type, XMLParser parser) {
            return helper<Cameras...>::parseCamera(type, parser);
        }
};

template<class Derived, class... Cameras> struct helper<Derived, Cameras...> {
    static std::unique_ptr<Camera> parseCamera(std::string type, XMLParser parser) {
        
        if (Derived::className() == type) {
            return std::make_unique<Derived>(parser);
        }

        return helper<Cameras...>::parseCamera(type, parser);
    }
};     

template<> struct helper<> {
    static std::unique_ptr<Camera> parseCamera(
        __attribute__((unused)) std::string type,
         __attribute__((unused)) XMLParser parser) {
        throw exception(); //string("No listed camera matched the received type '") + type + string("'")
    }
};

class PolarCamera : public Camera {
    Point position;
    Point lookAt;
    Vector up;
    float fov;
    float near;
    float far;

    public:
        static string className() { return "PolarCamera"; }

        PolarCamera(XMLParser parser);

        void initScene(WindowSize windowSize);
        void setupScene();
        void handleSpecialKey(int key, int x, int y);
};

class FPSCamera : public Camera {
    Point position;
    float angleXZ;
    float angleZY;
    Vector up;
    float fov;
    float near;
    float far;

    public:
        static string className() { return "FPSCamera"; }

        FPSCamera(XMLParser parser);

        void initScene(WindowSize windowSize);
        void setupScene();
        void handleKey(unsigned char key, int x, int y);
        void handleSpecialKey(int key, int x, int y);

    private:
        FPSCamera(Point,float,float,Vector,float,float,float);
        Vector getLookAtVector();
};