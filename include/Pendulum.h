#ifndef PENDULUM_H
#define PENDULUM_H

#include <SDL2/SDL.h>

#include "Vec2.h"

class Pendulum
{
    public:
        Pendulum(Vec2 _armPos, int _armLength, int _weightRadius, int _alpha, SDL_Renderer* _renderer);
        void draw();
        void update(float _deltaTime);
        void reset(Vec2 _armPos, int _armLength, int _weightRadius);

    private:
        const float m_PI = 3.14159f;
        const float m_gravity = 9.8f;
        const float m_damping = 0.995f;

        // Arm
        Vec2 m_armPos;
        int m_armLength;
        float m_armAngle;

        // Weight
        Vec2 m_weightPos;
        int m_weightRadius;
        float m_weightForce;
        float m_weightVelocity;
        float m_weightAcceleration;

        int m_colour;
        SDL_Renderer* m_renderer;
};

#endif