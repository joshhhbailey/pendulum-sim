#ifndef PENDULUM_H
#define PENDULUM_H

#include <SDL2/SDL.h>

#include "Vec2.h"

class Pendulum
{
    public:
        Pendulum(Vec2 _armPos, int _armLength, int _weightRadius, SDL_Renderer* _renderer);
        void draw();
        void update(float _deltaTime);

    private:
        const float PI = 3.14159f;
        const float gravity = 0.1f;
        const float timestep = 0.1f;

        Vec2 m_armPos;
        int m_armLength = 256;
        float m_armAngle = PI / 4.0f;

        Vec2 m_weightPos;
        int m_weightRadius;
        float m_weightForce;
        float m_weightVelocity = 0.0f;
        float m_weightAcceleration;

        SDL_Renderer* m_renderer;
};

#endif