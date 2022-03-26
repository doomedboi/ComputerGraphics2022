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

    friend T dot(const Vec2& left, const Vec2& right) {
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

    T& operator[] (int index) { return ((T*)this)[index]; }
    T operator[] (int index) const { return ((T*)this)[index]; }

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

    friend Vec3 operator-(const Vec3& left, const Vec3& right) {
        Vec3 base = left;
        base.x -= right.x;
        base.y -= right.y;
        base.z -= right.z;

        return  base;
    }

    Vec3& operator+=(const Vec3& left) {
        this->x += left.x;
        this->y += left.y;
        this->z += left.z;

        return *this;
    }

    Vec3& operator-=(const Vec3& left) {
        this->x -= left.x;
        this->y -= left.y;
        this->z -= left.z;

        return *this;
    }

    Vec3 operator*(const T& right) const {
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

    T dot(const Vec3& right) {
        // RVO
        T sum{};
        for (int i = 0; i < 3; i++)
            sum += right[i] * (*this)[i];

        return  sum;
    }

    [[nodiscard]] Vec3 GetNormalize() const {
        Vec3 base = *this;
        size_t len = base.GetLength();

        base.x /= len;
        base.y /= len;
        base.z /= len;

        return base;
    }

    friend Vec3 operator*(const Vec3& left, const Vec3& right) {
        Vec3 result = right;
        result[0] = left[1] * right[2] - left[2] * right[1];
        result[1] = left[2] * right[0] - left[0] * right[2];
        result[2] = left[0] * right[1] - left[1] * right[0];
        return result;
    }

    // TODO: replace it
    static Vec3 Forward() {
        static const Vec3 Forward = Vec3(0, 1, 0);
        return Forward;
    }

private:
    T x, y, z;
};


template <typename T>
class Matrix33 {
public:
    Matrix33(T m00, T m01, T m02,
             T m10, T m11, T m12,
             T m20, T m21, T m22)
    : m00(m00), m01(m01), m02(m02),
      m10(m10), m11(m11), m12(m12),
      m20(m20), m21(m21), m22(m22) {}

    Matrix33(Vec3<T> col1, Vec3<T> col2, Vec3<T> col3) :
        m00(col1[0]), m01(col1[1]), m02(col1[2]),
        m10(col2[0]), m11(col2[1]), m12(col2[2]),
        m20(col3[0]), m21(col3[1]), m22(col3[2]) {}

    bool operator==(const Matrix33& other) const
    {
        bool flag = true;
        for (int i = 0; i < 9 /*matrix 3 * 3*/; ++i) {
            if (*this[i] != other[i]) {
                flag = false;
                break;
            }
        }

        return flag;
    }

    Matrix33 operator+(const Matrix33& other) const {
        Matrix33 base = *this; //  pray to RVO + cache fr
        for (int i = 0; i < 9; ++i)
            base[i] += other[i];

        return base;
    }

    Matrix33 operator-(const Matrix33& other) const {
        Matrix33 base = *this;
        for (int i = 0; i < 9; ++i)
            base[i] += other[i];

        return base;
    }

    Matrix33 operator*(const Matrix33& other) const {
        //loop cause low perf
        Matrix33 base = *this;
        // calc process
        T lm00 = base.m00 * other.m00 + base.m01 * other.m10 + base.m02 * other.m20;
        T lm01 = base.m00 * other.m01 + base.m01 * other.m11 + base.m02 * other.m21;
        T lm02 = base.m00 * other.m02 + base.m01 * other.m12 + base.m02 * other.m22;

        T lm10 = base.m10 * other.m00 + base.m11 * other.m10 + base.m12 * other.m20;
        T lm11 = base.m10 * other.m01 + base.m11 * other.m11 + base.m12 * other.m21;
        T lm12 = base.m10 * other.m02 + base.m11 * other.m12 + base.m12 * other.m22;

        T lm20 = base.m20 * other.m00 + base.m21 * other.m10 + base.m22 * other.m20;
        T lm21 = base.m20 * other.m01 + base.m21 * other.m11 + base.m22 * other.m21;
        T lm22 = base.m20 * other.m02 + base.m21 * other.m12 + base.m22 * other.m22;

        // setting data
        base.m00 = lm00; base.m01 = lm01; base.m02 = lm02;
        base.m10 = lm10; base.m11 = lm11; base.m12 = lm12;
        base.m20 = lm20; base.m21 = lm21; base.m22 = lm22;

        return base;
    }

    Matrix33 operator*(const T other) const {
        Matrix33 base = *this;
        base.m00 *= other; base.m01 *= other; base.m02 *= other;
        base.m10 *= other; base.m11 *= other; base.m12 *= other;
        base.m20 *= other; base.m21 *= other; base.m22 *= other;

        return base;
    }

    [[nodiscard]] Matrix33 GetTranspose() const {
        Matrix33 base = *this;

        base.m10 = base.m01;
        base.m20 = base.m02;
        base.m01 = this->m10;
        base.m02 = this->m20;
        base.m21 = base.m12;
        base.m12 = this->m21;

        return base;
    }

    [[nodiscard]] T Determinant() const {
        T sum{};
        sum += (m00 * m11 * m22); sum += (m01 * m12 * m20);
        sum += (m02 * m10 * m21);
        sum -= (m02 * m11 * m20);
        sum -= (m00 * m12 * m21);
        sum -= (m01 * m10 * m22);

        return sum;
    }

    Matrix33 GetAdj() const {
        // if (Determinant() == 0) //throw
        Matrix33 base = GetTranspose();
        // hardcoded algo based on finding det of 2x2
        T rm00 = (base.m11 * base.m22) - (base.m21 * base.m12);
        T rm01 = (base.m10 * base.m22) - (base.m20 * base.m12);
        T rm02 = (base.m10 * base.m21) - (base.m20 * base.m11);
        T rm10 = (base.m01 * base.m22) - (base.m21 * base.m02);
        T rm11 = (base.m00 * base.m22) - (base.m20 * base.m02);
        T rm12 = (base.m00 * base.m21) - (base.m20 * base.m01);
        T rm20 = (base.m01 * base.m12) - (base.m11 * base.m02);
        T rm21 = (base.m00 * base.m12) - (base.m10 * base.m02);
        T rm22 = (base.m00 * base.m11) - (base.m10 * base.m01);

        base.m00 = rm00; base.m01 = rm01 *(-1); base.m02 = rm02;
        base.m10 = rm10 *(-1); base.m11 = rm11; base.m12 = rm12 * (-1);
        base.m20 = rm20; base.m21 = rm21 * (-1); base.m22 = rm22;

        return base;
    }

    Matrix33 GetInverted() const {
        const T det = Determinant();
        Matrix33 adjM = GetAdj();

        for (int i = 0; i < 9; ++i)
            adjM[i] /= det;
        return adjM;
    }

    T& operator[] (int index) {
        return ((T*)this)[index];
    }

    T operator[] (int index) const {
        return ((T*)this)[index];
    }

private:
    T m00, m01, m02;
    T m10, m11, m12;
    T m20, m21, m22;
};