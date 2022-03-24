#pragma once
#include <cmath>

template <typename T>
class Vec2 {
public:
    Vec2(T x, T y) : x(x), y(y) {}

    [[nodiscard]] size_t Length() const { return sqrt(x * x, y * y); }

    void operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
    }

    void operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
    }

    friend Vec2 operator+(const Vec2& left, const Vec2& right) {
        return Vec2(left.x + right.x, left.y + right.y);
    }

    friend Vec2 operator-(const Vec2 left, const Vec2& right) {
        return Vec2(left.x - right.x, left.y - right.y);
    }

    Vec2 operator*(const T n) const {
        Vec2 base = *this;
        base.x* n;
        base.y* n;
        //  RVO
        return base;
    }

    Vec2& operator*=(const T n) {
        *this.x *= n;
        *this.y *= n;
        return *this;
    }

    // radians
    [[nodiscard]] double GetAngle(const Vec2& other) const {
        return acos((*this * other) / (this->Length() * other.Length()));
    }

    friend T operator*(const Vec2& left, const Vec2& right) {
        return left.x * right.x + left.y * right.y;
    }

    [[nodiscard]] Vec2 Normalize() const {
        Vec2 base = *this;
        size_t len = base.Length();
        base.x /= len;
        base.y /= len;

        return base;
    }

    [[nodiscard]] Vec2 ReverseVector() const {
        return Vec2(-x, -y);
    }

    T& operator[] (int index) { return reinterpret_cast<T*>(this)[index]; }
    T operator[] (int index) const { return reinterpret_cast<T*>(this)[index]; }

private:
    T x, y;
};

template <typename T>
class Vec3 {

};

