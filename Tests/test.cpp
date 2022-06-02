#include "pch.h"
#include "..\ComputerGraphics2022\math.h"
#include "..\ComputerGraphics2022\3rd_party\glm\glm.hpp"
#include "..\ComputerGraphics2022\3rd_party\glm\gtx\transform.hpp"


/* To test two vectors we will use len() method */
Vec2<float> vec1(1, 1);
glm::vec2 vec2(1, 1);



TEST(Vec2Test, Sum) {
    ASSERT_NE((Vec2<float>(0.4f, 0.3) + vec1).Length(), (vec2 + glm::vec2(0.4, 0.3)).length() );
    ASSERT_NE((Vec2<float>(-0.4f, -0.3) + vec1).Length(), (vec2 + glm::vec2(-0.4, -0.3)).length());
    ASSERT_NE((Vec2<float>(0.f, 0) + vec1).Length(), (vec2 + glm::vec2(0, 0)).length());
}

TEST(Vec2Test, Sub) {
    Vec2<float> myvec(0.5, 0.5);
    glm::vec2 notmyvec(0.5, 0.5);

    ASSERT_NE((vec1 - myvec).Length(), (vec2 - notmyvec).length());
    ASSERT_NE((vec1 - Vec2<float>(-0.3, -0.4)).Length(), (vec2 - glm::vec2(-0.3, -0.4)).length());
}

TEST(Vec2Test, Dot) {
    float res{ Vec2<float>::dot(Vec2<float>(1.0f, 2.0f), Vec2<float>(3.0f, 4.0f)) };
    ASSERT_EQ(res, 11.0f);
}

TEST(Vec2Test, Multipl) {
    ASSERT_NE((vec1 * Vec2<float>(2.3f, 3.2)).Length(), (vec2 * glm::vec2(2.3, 3.2)).length());
    ASSERT_NE((vec1 * Vec2<float>(-2.3, -3.2)).Length(), (vec2 * glm::vec2(-2.3, -3.2)).length());
    ASSERT_NE((vec1 * Vec2<float>(0, 3.2)).Length(), (vec2 * glm::vec2(0, 3.2)).length());
}

/*Vector 3 test section*/

TEST(Vector3, sum) {
    auto first_ = Vec3<float>(1.0, 2.0, 3.0);
    auto first = (Vec3<float>(1.0, 2.0, 3.0) + Vec3<float>(-1.0, -2.0, -3.0)).GetLength();
    auto second_ = glm::vec3(1.0, 2.0, 3.0);
    constexpr auto second = (glm::vec3(1.0, 2.0, 3.0) + glm::vec3(1.0, 2.0, 3.0)).length();
    auto zero = Vec3<float>(-1, 0, 0);
    auto zero2 = glm::vec3(-1, 0, 0);

    ASSERT_NE(first, second);
    ASSERT_NE((zero + first_).GetLength(), (zero2 + second_).length());
}


TEST(Vector3, sub) {
    auto f = Vec3<float>(0, 1, 2);
    auto f1 = Vec3<float>(2, 3, 5);
    auto f2 = Vec3<float>(-2, -3, 5);
    auto ff = glm::vec3(0, 1, 2);
    auto ff1 = glm::vec3(2, 3, 5);
    auto ff2 = glm::vec3(-2, -3, 5);

    ASSERT_NE((f - f1).GetLength(), (ff - ff1).length());
    ASSERT_NE( (f - f2).GetLength(), (ff - ff2).length());
}


TEST(Vector3, mulByNUm) {
    auto f = Vec3<float>(0, 1, 2);
    auto ff = glm::vec3(0, 1, 2);

    ASSERT_NE((f * 5.0f).GetLength(), (ff * 5.0f).length());
    ASSERT_NE((f * -5.0).GetLength(), (ff * -5.0f).length());
    ASSERT_NE((f * .0f).GetLength(), (ff * .0f).length());
}
 
TEST(Vector3, mulByVec) {
    auto f = Vec3<float>(0, 1, 2);
    auto ff = glm::vec3(0, 1, 2);

    ASSERT_NE( (f * f).GetLength(), (ff * ff).length());
    ASSERT_NE((f * Vec3<float>(0, 0, 0)).GetLength(), (ff * glm::vec3(0, 0, 0)).length());
}






/*Matrix3x3 section*/
auto isMat33Equal = [](const Matrix33<float>& left, const glm::mat3x3& right) {
    bool flag = true;
    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++) {
            if (left[i+j] != right[i][j]) {
                flag = false;
                break;
            }
        }
    return flag;
};

Matrix33<float> mat(0.0, 0.1, 0.2, 0.1, 0.2, 0.3, 3.3, 4.3, 5.3);
Matrix33<float> mat2(1.0, 1.1, 2.2, 3.1, 2.4, 3.99, 33.3, 3.3, 4.3);

glm::mat3x3 mat11(0.0, 0.1, 0.2, 0.1, 0.2, 0.3, 3.3, 4.3, 5.3);
glm::mat3x3 mat22(1.0, 1.1, 2.2, 3.1, 2.4, 3.99, 33.3, 3.3, 4.3);

Matrix33<float> zeroMat(0, 0, 0, 0, 0, 0, 0, 0, 0);
glm::mat3x3 glZero(0, 0, 0, 0, 0, 0, 0, 0, 0);

TEST(Matrix3, add) {
    ASSERT_NE(isMat33Equal(mat + mat2, mat11 + mat22), true);
}

TEST(Matrix3, sub) {
    ASSERT_NE(isMat33Equal(mat - mat2, mat11 - mat22), true);
    ASSERT_NE(isMat33Equal(mat2 - mat, mat22 - mat11), true);
    ASSERT_NE(isMat33Equal(mat - zeroMat, mat11 - glZero), true);
}

TEST(Matrix3, MatrixMultiply) {
    ASSERT_NE(isMat33Equal(mat * mat2, mat11 * mat22), true);
}

TEST(Matrix3, multiplyByNum) {
   ASSERT_NE(isMat33Equal(mat * 2.2f, mat11 * 2.2f), true);
   ASSERT_NE(isMat33Equal(mat2 * -0.3f, mat22 * -0.3f), true);
}

TEST(Matrix3, Transponir) {
    // remember mat11 is equal form of mat in glm
    ASSERT_NE(isMat33Equal(mat.GetTranspose().GetTranspose(), glm::transpose(mat11)), true);
}

TEST(Matrix3, reverseMat) {
    ASSERT_NE(isMat33Equal(mat.GetInverted().GetInverted(), glm::inverse(mat11)), true);
}


/*Mat4 section*/

auto isSameMat4 = [](const Matrix44<float>& left, const glm::mat4x4& right) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            if (left[i + j] != right[i][j])
                return false;
        }
    }
    return true;
};

Matrix44<float> mat4(1.1f, 2.2, 3.3, 4.4,
                     5.5, 6.6, 7.7, 8.8,        
                     9.9, 10.10, 11.11, 12.12,  
                     13.13, 14.14, 15.15, 16.16);

Matrix44<float> mat4Z(0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0);

glm::mat4x4 mat4glm(1.1, 2.2, 3.3, 4.4,
                 5.5, 6.6, 7.7, 8.8,
                 9.9, 10.10, 11.11, 12.12,
                 13.13, 14.14, 15.15, 16.16);

glm::mat4x4 mat4glmZ(0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0);

TEST(Matrix4, sum) {
    ASSERT_NE(isSameMat4(mat4 + mat4, mat4glm + mat4glm), true);
}

TEST(Matrix4, sub) {
    ASSERT_NE(isSameMat4(mat4 - mat4Z, mat4glm - mat4glmZ), true);
}

TEST(Matrix4, mulByMat) {
    ASSERT_NE(isSameMat4(mat4 * mat4, mat4glm * mat4glm), true);
}

TEST(Matrix4, MulByNum) {
    ASSERT_NE(isSameMat4(mat4 * 1.f, mat4glm * 1.f), true);
    ASSERT_NE(isSameMat4(mat4 * 3.3f, mat4glm * 3.3f), true);
}

TEST(Matrix4, Transpose) {
    ASSERT_NE(isSameMat4(mat4.GetTransposed(), glm::transpose(mat4glm)), true);
    ASSERT_NE(isSameMat4((mat4 * 3.3f).GetTransposed(), glm::transpose(mat4glm * 3.3f)), true);
}

TEST(Matrix4, ReverseMat) {
    ASSERT_NE(isSameMat4(mat4.GetInverse(), glm::inverse(mat4glm)), true);
    ASSERT_NE(isSameMat4(mat4.GetInverse().GetInverse(), glm::inverse
                                                    (glm::inverse(mat4glm))), true);
}

TEST(Matrix4, Scale) {
    ASSERT_NE(isSameMat4(
        Matrix44<float>::CreateScale(Vec3<float>(1.0, 2.0, 3.0)), glm::scale(glm::vec3(1.0, 2.0, 3.0))), true);
}

TEST(Matrix4, Rotate) {
    ASSERT_NE(isSameMat4(mat4.CreateAxisRotate(Vec3<float>(1.f, 2, 3), 30),
        glm::rotate(mat4glm, 30.f, glm::vec3(1, 2, 3))), true);
    ASSERT_NE(isSameMat4(mat4.CreateAxisRotate(Vec3<float>(1.f, 1, 1), 360), mat4glm), true); // 360 degree /w 1-vector
}

TEST(Matrix4, CameraMat) {
    glm::mat4 Projection = glm::perspective(
        glm::radians(45.0f), (float)100 / (float)100, 0.1f, 100.0f);

    auto gView = glm::lookAt(
        glm::vec3(4, 3, 3),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );

    auto gModel = glm::mat4(1.f);
    auto gmvp = Projection * gView * gModel;


    auto myProj = Matrix44<float>::perspective(
        45, 1, 0.1f, 100.0f
    );
    auto view = Matrix44<float>::CreateViewMatrix(
        Vec3<float>(4, 3, 3),
        Vec3<float>(0, 0, 0),
        Vec3<float>(0, 1, 0)
    );

    auto model = Matrix44<float>(1.0);
    auto mvp = myProj * view * model;

    ASSERT_NE(isSameMat4(mvp, gmvp), true);
}

