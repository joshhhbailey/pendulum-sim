#ifndef VEC2_H
#define VEC2_H

class Vec2
{
    public:
        int m_x;
        int m_y;

        Vec2() = default;
        Vec2(int _x, int _y) {m_x = _x; m_y = _y; }
};

#endif