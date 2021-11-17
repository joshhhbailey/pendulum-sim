#include "Pendulum.h"

#include <iostream>

Pendulum::Pendulum(Vec2 _armPos, int _armLength, int _weightRadius, int _colour, SDL_Renderer* _renderer)
{
    m_colour = _colour;
    m_renderer = _renderer;
    reset(_armPos, _armLength, _weightRadius);
}

void Pendulum::draw()
{
    // Set colour - white
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Draw arm
    SDL_RenderDrawLine(m_renderer, m_armPos.m_x, m_armPos.m_y, m_weightPos.m_x, m_weightPos.m_y);

    // Draw weight
    for (int x = m_weightPos.m_x - m_weightRadius; x <= m_weightPos.m_x + m_weightRadius; x++)
    {
        for (int y = m_weightPos.m_y - m_weightRadius; y <= m_weightPos.m_y + m_weightRadius; y++)
        {
            int x2 = (m_weightPos.m_x - x) * (m_weightPos.m_x - x);
            int y2 = (m_weightPos.m_y - y) * (m_weightPos.m_y - y);
            int r2 = m_weightRadius * m_weightRadius;

            // Inner colour
            if (x2 + y2 <= r2 - ((x2 + y2) / 5.0f))
            {
                SDL_SetRenderDrawColor(m_renderer, m_colour, m_colour, m_colour, 0xFF);
                SDL_RenderDrawPoint(m_renderer, x, y);
            }
            // Outer colour
            else if (x2 + y2 <= r2)
            {
                SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderDrawPoint(m_renderer, x, y);
            }
        }
    }
}

void Pendulum::update(float _deltaTime)
{
    m_weightForce = sin(m_armAngle) * m_gravity;
    m_weightAcceleration = (m_weightForce * -1.0f) / m_armLength;   // http://www.myphysicslab.com/pendulum1.html
    m_weightVelocity += m_weightAcceleration;
    //m_weightVelocity *= m_damping;
    m_armAngle += m_weightVelocity * _deltaTime;

    m_weightPos.m_x = (int)(sin(m_armAngle) * m_armLength) + m_armPos.m_x;
    m_weightPos.m_y = (int)(cos(m_armAngle) * m_armLength) + m_armPos.m_y;
}

void Pendulum::reset(Vec2 _armPos, int _armLength, int _weightRadius)
{
    m_armAngle = m_PI / 4.0f;
    m_weightVelocity = 0.0f;

    m_armPos = _armPos;
    m_armLength = _armLength;
    m_weightRadius = _weightRadius;

    m_weightPos.m_x = (int)(sin(m_armAngle) * m_armLength) + m_armPos.m_x;
    m_weightPos.m_y = (int)(cos(m_armAngle) * m_armLength) + m_armPos.m_y;
}