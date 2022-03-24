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

    // TODO: rename to dot
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
public:
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    size_t GetLength() { return sqrt(x * x + y * y + z * z); }

    friend Vec3 operator+(const Vec3& left, const Vec3& right) {
        Vec3 base = left;
        base.x -= right.x;
        base.y -= right.y;
        base.z -= right.z;

        return base;
    }

    friend Vec3 operator-(const Vec3& left, const Vec3& right)
    {
        Vec3 base = left;
        base.x -= right.x;
        base.y -= right.y;
        base.z -= right.z;

        return  base;
    }

    Vec3& operator+=(const Vec3& left)
    {
        this->x += left.x;
        this->y += left.y;
        this->z += left.z;

        return *this;
    }

    Vec3& operator-=(const Vec3& left)
    {
        this->x -= left.x;
        this->y -= left.y;
        this->z -= left.z;

        return *this;
    }

    Vec3 operator*(const T& right) const 
    {
        Vec3 base = *this;
        base.x *= right;
        base.y *= right;
        base.z *= right;

        return base;
    }

    T operator[] (int index) const {
        return ((T*)(this))[index];
    }

    T& operator[] (int index) {
        return ((T*)(this))[index];
    }

    T dot(const Vec3& right)
    {
        // RVO
        T sum{};
        for (int i = 0; i < 3; i++)
            sum += right[i] * (*this)[i];

        return  sum;
    }

    Vec3 GetNormalize() const
    {
        Vec3 base = *this;
        size_t len = base.GetLength();

        base.x /= len;
        base.y /= len;
        base.z /= len;

        return base;
    }

    friend Vec3 operator*(const Vec3& left, const Vec3& right)
    {
        Vec3 result = right;
        result[0] = left[1] * right[2] - left[2] * right[1];
        result[1] = left[2] * right[0] - left[0] * right[2];
        result[2] = left[0] * right[1] - left[1] * right[0];
        return result;
    }

    static Vec3 Forward() {
        static const Vec3 Forward = Vec3(0, 1, 0);
        return Forward;
    }

private:
    T x, y, z;
};

