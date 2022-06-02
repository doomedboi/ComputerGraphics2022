#pragma once
#include <cmath>

template <typename T>
constexpr T pi()
{
    return static_cast<T>(3.141592653589793);
}

template <typename T>
constexpr T degToRad(T angle)
{
    return angle * (pi<T>() / T(180));
}

template <typename T>
class Vec2 {
public:
    Vec2(T x, T y) : x(x), y(y) {}

    [[nodiscard]] float Length() const { return sqrt(x * x + y * y); }

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

    // scalar * vector doesn't work need to throw out of class 
    Vec2 operator*(const T n) const {
        Vec2 base = *this;
        base.x * n;
        base.y * n;
        //  RVO
        return base;
    }

    Vec2 operator*(const Vec2& right) {
        auto tmp = *this;
        tmp.x *= right.x;
        tmp.y *= right.y;
        return tmp;
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

    static T dot(const Vec2& left, const Vec2& right) {
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
    Vec3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}

    T GetLength() { return static_cast<T>(sqrt(x * x + y * y + z * z)); }

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

    T dot(const Vec3& right) const {
        // RVO
        T sum{};
        for (int i = 0; i < 3; i++)
            sum += (right[i] * (*this)[i]);

        return  sum;
    }

    [[nodiscard]] Vec3 GetNormalize() const {
        return Normalize(*this);
    }

    static Vec3 Normalize(const Vec3<T>& v) {
        Vec3 base = v;
        T len = static_cast<T>(base.GetLength());

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
class Vec4 {
public:
    // func doesn't take ref bec normal usage with float ~ 4 bytes. ref actually sizes 8b
    Vec4(const T x, const T y, const T z, const T w) :
        x(x), y(y), z(z), w(w) {}

    Vec4() { Vec4(0.f, 0.f, 0.f, 0.f); }

    Vec4& operator[](const size_t index) { return (T*)(this)[index]; }

    Vec4 operator[](const size_t index) const { return (T*)(this)[index]; }
private:
    T x, y, z, w;
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

    Matrix33(Vec3<T>& col1, Vec3<T>& col2, Vec3<T>& col3) :
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

    [[nodiscard]] constexpr T Determinant() const {
        T sum{};
        sum += (m00 * m11 * m22); sum += (m01 * m12 * m20);
        sum += (m02 * m10 * m21);
        sum -= (m02 * m11 * m20);
        sum -= (m00 * m12 * m21);
        sum -= (m01 * m10 * m22);

        return sum;
    }

    Matrix33 GetAdj() const {
        /*if constexpr (Determinant() == (T)0)
            throw 0; */
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

template <typename T>
class Matrix44 {
public:
    Matrix44(
        const T _m00, const T _m01, const T _m02, const T _m03,
        const T _m10, const T _m11, const T _m12, const T _m13,
        const T _m20, const T _m21, const T _m22, const T _m23,
        const T _m30, const T _m31, const T _m32, const T _m33
    ) : m00(_m00), m01(_m01), m02(_m02), m03(_m03),
        m10(_m10), m11(_m11), m12(_m12), m13(_m13),
        m20(_m20), m21(_m21), m22(_m22), m23(_m23),
        m30(_m30), m31(_m31), m32(_m32), m33(_m33) {}

    Matrix44(const T number)
        :m00(number), m01(number), m02(number), m03(number),
        m10(number), m11(number), m12(number), m13(number),
        m20(number), m21(number), m22(number), m23(number),
        m30(number), m31(number), m32(number), m33(number) {}
    // TODO: realize it with forward reference
    Matrix44(const Vec4<T> row0,
             const Vec4<T> row1,
             const Vec4<T> row2,
             const Vec4<T> row3) {}

    Matrix44() = default;

    Matrix44 operator+(const Matrix44& other) const {
        Matrix44 base = *this;
        for (int i  = 0; i < MATRIX_SIZE; ++i)
            base(i) += other(i);

        return base;
    }

    Matrix44 operator-(const Matrix44& other) const {
        Matrix44 base = *this;
        for (int i = 0; i < MATRIX_SIZE; ++i)
            base[i] -= other(i);

        return base;
    }

    // A dubious decision. Think about that later.
    T& operator() (int i, int j = 0) {
        return ((T*)this)[i + j * MATRIX_ROW_SIZE];
    }

    T operator() (int i, int j = 0) const {
        return  ((T*)this)[i + j * MATRIX_ROW_SIZE];
    }

    Matrix44 operator*(const T num) const {
        Matrix44 base = *this;
        for (int i = 0; i < MATRIX_SIZE; ++i)
            base(i) *= num;

        return base;
    }

    [[nodiscard]] T Determinant() const{
        Matrix44 tmpMat = *this;
        T det{ 1 }; T buf{};
        for (int k = 0; k < MATRIX_ROW_SIZE - 1; k++) {
            for (int i = k + 1; i < MATRIX_ROW_SIZE; i++) {
                buf = -tmpMat(i,k) / tmpMat(k, k);
                for (int j = 0; j < MATRIX_ROW_SIZE; j++) {
                    tmpMat(i, j) += tmpMat(k, j) * buf;
                }
            }
        }

        for (int i = 0; i < MATRIX_ROW_SIZE; ++i)
            det *= tmpMat(i, i);

        return det;
    }

    Matrix44 GetInverse() const {
        Matrix44 orig = *this;
        T minor0 = orig(2+2) * orig(3, 3) - orig(3, 2) * orig(2,3);
        T minor1 = orig(2,1) * orig(3,3) - orig(3,1) * orig(2,3);
        T minor2 = orig(2,1) * orig(3,2) - orig(3,1) * orig(2,2);
        T minor3 = orig(2,0) * orig(3,3) - orig(3,0) * orig(2,3);
        T minor4 = orig(2,0) * orig(3,2) - orig(3,0) * orig(2,2);
        T minor5 = orig(2,0) * orig(3,1) - orig(3,0) * orig(2,1);
        T minor6 = orig(1,2) * orig(3,3) - orig(3,2) * orig(1,3);
        T minor7 = orig(1,1) * orig(3,3) - orig(3,1) * orig(1,3);
        T minor8 = orig(1,1) * orig(3,2) - orig(3,1) * orig(1, 2);
        T minor9 = orig(1,0) * orig(3,3) - orig(3,0) * orig(1,3);
        T minor10 = orig(1,0) * orig(3,2) - orig(3,0) * orig(1, 2);
        T minor11 = orig(1,0) * orig(3,1) - orig(3,0) * orig(1,1);
        T minor12 = orig(1,2) * orig(2,3) - orig(2,2) * orig(1, 3);
        T minor13 = orig(1,1) * orig(2,3) - orig(2,1) * orig(1,3);
        T minor14 = orig(1,1) * orig(2,2) - orig(2,1) * orig(1,2);
        T minor15 = orig(1,0) * orig(2,3) - orig(2,0) * orig(1,3);
        T minor16 = orig(1,0) * orig(2,2) - orig(2,0) * orig(1,2);
        T minor17 = orig(1,0) * orig(2,1) - orig(2,0) * orig(1,1);
        
        Matrix44 result;

        result(0,0) = +(orig(1,1) * minor0 - orig(1,2) * minor1 + orig(1,3) * minor2);
        result(0,1) = -(orig(1,0) * minor0 - orig(1,2) * minor3 + orig(1,3) * minor4);
        result(0,2) = +(orig(1,0) * minor1 - orig(1,1) * minor3 + orig(1,3) * minor5);
        result(0,3) = -(orig(1,0) * minor2 - orig(1,1) * minor4 + orig(1,2) * minor5);

        result(1,0) = -(orig(0,1) * minor0 - orig(0,2) * minor1 + orig(0,3) * minor2);
        result(1,1) = +(orig(0,0) * minor0 - orig(0,2) * minor3 + orig(0,3) * minor4);
        result(1,2) = -(orig(0,0) * minor1 - orig(0,1)* minor3 + orig(0,3) * minor5);
        result(1,3) = +(orig(0,0) * minor2 - orig(0,1) * minor4 + orig(0,2) * minor5);

        result(2,0) = +(orig(0,1) * minor6 - orig(0,2) * minor7 + orig(0,3) * minor8);
        result(2,1) = -(orig(0,0) * minor6 - orig(0,2) * minor9 + orig(0,3) * minor10);
        result(2,2) = +(orig(0,0) * minor7 - orig(0,1) * minor9 + orig(0,3) * minor11);
        result(2,3) = -(orig(0,0) * minor8 - orig(0,1) * minor10 + orig(0,2) * minor11);

        result(3,0) = -(orig(0,1) * minor12 - orig(0,2) * minor13 + orig(0,3) * minor14);
        result(3,1) = +(orig(0,0) * minor12 - orig(0,2) * minor15 + orig(0,3) * minor16);
        result(3,2) = -(orig(0,0) * minor13 - orig(0,1) * minor15 + orig(0,3) * minor17);
        result(3,3) = +(orig(0,0) * minor14 - orig(0,1) * minor16 + orig(0,2) * minor17);

        return result * (1.f / orig.Determinant());
}

    Matrix44 GetTransposed() const {
        Matrix44 base = *this;
        
        for (int i = 0; i < MATRIX_ROW_SIZE; i++)
            for (int j = 0; j < MATRIX_ROW_SIZE; j++)
                base(j, i) = (*this)(i, j);

        return base;
    }

    Matrix44 operator*(const Matrix44& other) const {
        Matrix44 base = *this;

        T lm00 = base.m00 * other.m00 + base.m01 * other.m10 + base.m02 * other.m20 + base.m03 * other.m30;
        T lm01 = base.m00 * other.m01 + base.m01 * other.m11 + base.m02 * other.m21 + base.m03 * other.m31;
        T lm02 = base.m00 * other.m02 + base.m01 * other.m12 + base.m02 * other.m22 + base.m03 * other.m32;
        T lm03 = base.m00 * other.m03 + base.m01 * other.m13 + base.m02 * other.m23 + base.m03 * other.m33;
                                                                                                            

        T lm10 = base.m10 * other.m00 + base.m11 * other.m10 + base.m12 * other.m20 + base.m13 * other.m30;
        T lm11 = base.m10 * other.m01 + base.m11 * other.m11 + base.m12 * other.m21 + base.m13 * other.m31;
        T lm12 = base.m10 * other.m02 + base.m11 * other.m12 + base.m12 * other.m22 + base.m13 * other.m32;
        T lm13 = base.m10 * other.m03 + base.m11 * other.m13 + base.m12 * other.m23 + base.m13 * other.m33;

        T lm20 = base.m20 * other.m00 + base.m21 * other.m10 + base.m22 * other.m20 + base.m23 * other.m30;
        T lm21 = base.m20 * other.m01 + base.m21 * other.m11 + base.m22 * other.m21 + base.m23 * other.m31;
        T lm22 = base.m20 * other.m02 + base.m21 * other.m12 + base.m22 * other.m22 + base.m23 * other.m32;
        T lm23 = base.m20 * other.m03 + base.m21 * other.m13 + base.m22 * other.m23 + base.m23 * other.m33;

        T lm30 = base.m30 * other.m00 + base.m31 * other.m10 + base.m32 * other.m20 + base.m33 * other.m30;
        T lm31 = base.m30 * other.m01 + base.m31 * other.m11 + base.m32 * other.m21 + base.m33 * other.m31;
        T lm32 = base.m30 * other.m02 + base.m31 * other.m12 + base.m32 * other.m22 + base.m33 * other.m32;
        T lm33 = base.m30 * other.m03 + base.m31 * other.m13 + base.m32 * other.m23 + base.m33 * other.m33;

        base.m00 = lm00; base.m01 = lm01; base.m02 = lm02; base.m03 = lm03;
        base.m10 = lm10; base.m11 = lm11; base.m12 = lm12; base.m13 = lm13;
        base.m20 = lm20; base.m21 = lm21; base.m22 = lm22; base.m23 = lm23;
        base.m30 = lm30; base.m31 = lm31; base.m32 = lm32; base.m33 = lm33;

        return base;

    }

    T& operator[] (size_t i) {
        return ((T*)this)[i];
    }

    T operator[] (size_t i) const {
        return ((T*)this)[i];
    }

    [[nodiscard]] Vec3<T> GetTranslation() const {
        return Vec3<T> (this->m03, this->m13, this->m23);
    }

    void SetTranslation(const Vec3<T>& t) {
        this[3] = t[0];
        this[7] = t[1];
        this[11] = t[2];
        this[15] = 1;
    }

    static Matrix44 CreateXRotate(const double degree) {
        const double rad = degToRad<double>(degree);
        const double cosq = cos(rad);
        const double sinq = sin(rad);

        return Matrix44(
            1, 0, 0, 0,
            0, cosq, -sinq, 0,
            0, sinq, cosq, 0,
            0, 0, 0, 1);

    }

    static Matrix44 CreateYRotate(const double degree) {
        const double rad = degToRad<double>(degree);
        const double cosq = cos(rad);
        const double sinq = sin(rad);

        return Matrix44(
            cosq, 0, sinq, 0,
            0, 1, 0, 0,
            -sinq, 0, cosq, 0,
            0, 0, 0, 1);
    }

    static Matrix44 CreateZRotate(const double degree) {
        const double rad = degToRad<double>(degree);
        const double cosq = cos(rad);
        const double sinq = sin(rad);

        return Matrix44(
            cosq, -sinq, 0, 0,
            sinq, cosq, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
    }

    Matrix44 CreateAxisRotate(const Vec3<T>& axis, const double degree) {
        const double rad = degToRad(degree);
        const double cosq = cos(rad);
        const double sinq = sin(rad);
        const double mult = 1 - cosq;

        const T m00 = cosq + axis[0] * axis[0] * mult;
        const T m01 = axis[0] * axis[1] * mult - sinq * axis[2];
        const T m02 = axis[0] * axis[2] * mult + sinq * axis[1];

        const T m10 = axis[1] * axis[1] * mult + axis[2] * sinq;
        const T m11 = cosq + axis[1] * axis[1] * mult;
        const T m12 = axis[1] * axis[2] * mult - sinq * axis[0];

        const T m20 = axis[0] * axis[2] * mult - sinq * axis[1];
        const T m21 = axis[1] * axis[2] * mult - axis[0] * sinq;
        const T m22 = axis[2] * axis[2] * mult + cosq;

        return Matrix44<T>(
            m01, m02, m03, 0,
            m10, m11, m12, 0,
            m20, m21, m22, 0,
            0,    0,    0, 1);
    }

    // probably it would be good to getting the unversal refernce...
    static Matrix44 CreateViewMatrix(const Vec3<T>& cameraPos, const Vec3<T>& cameraTarget, const Vec3<T>& worldUp) {
        const Vec3<T> forward = Vec3<T>::Normalize(cameraPos - cameraTarget);

        const Vec3<T> right = Vec3<T>::Normalize(worldUp.dot(forward));

        const Vec3 <T> up = Vec3<T>::Normalize(forward.dot(right));

        Matrix44 result{};
        result[0] = right[0]; result[1] = right[1]; result[2] = right[2]; result[3] = 0;
        result[4] = up[0]; result[5] = up[1]; result[6] = up[2]; result[7] = 0;
        result[8] = forward[0]; result[9] = forward[1]; result[10] = forward[2]; result[11] = 0;
        result[15] = 1;

        return result;
    }

    static Matrix44 perspective(const double fov, const double ratio,
        const double near, const double far) {
        Matrix44 proj;
        double fovTan = tan(fov / 2);
        proj[0] = 1 / ratio / fovTan;
        proj[5] = 1 / fovTan;
        proj[10] = (-near - far) / (near - far); proj[11] = (2 * far * near) / (near - far);
        proj[14] = 1;

        return proj;
    }

    static Matrix44 CreateScale(const Vec4<T>& s) {
        return Matrix44(s.x, 0, 0, 0,
                        0, s.y, 0, 0,
                        0, 0, s.z, 0,
                        0, 0, 0, s.w);
    }

    static Matrix44 CreateScale(const Vec3<T>& s, const Vec3<T>& t = Vec3<T>(0)) {
        return Matrix44(s[0], 0, 0, 0,
                        0, s[1], 0, 0,
                        0, 0, s[2], 0,
                        t[0], t[1], t[2], 1); // Am I correct?
    }


private:
    static constexpr int MATRIX_ROW_SIZE = 4;
    static constexpr int MATRIX_SIZE = 16;
private:
    T m00{}, m01{}, m02{}, m03{};
    T m10{}, m11{}, m12{}, m13{};
    T m20{}, m21{}, m22{}, m23{};
    T m30{}, m31{}, m32{}, m33{};
};


inline Vec3<double> CreateFrontVec(const double yaw, const double pitch, const double roll = 0) {
    return { cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch) };
}