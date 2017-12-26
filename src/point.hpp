#pragma once

struct Point 
{
    Point()
        : x(0)
        , y(0)
    { }

    Point(int x_, int y_)
        : x(x_)
        , y(y_)
    { }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point operator+(const Point& other) const {
        Point result(*this);
        result += other;
        return result; 
    }

    Point& operator-=(const Point& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Point operator-(const Point& other) const {
        Point result(*this);
        result -= other;
        return result;
    }

    Point operator-() const {
        return *this*-1; 
    }

    Point& operator*=(const Point& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Point operator*(int value) const {
        Point result(value, value);
        result *= *this;
        return result;
    }

    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }

    int x, y; 

    static Point up() { return Point(0, 1); }
    static Point down() { return Point(0, -1); }
    static Point right() { return Point(1, 0); }
    static Point left() { return Point(-1, 0); }
}; 
