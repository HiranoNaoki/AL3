#pragma once
#include <Vector3.h>
#include <Affine.h>


Vector3& operator+=(Vector3& lhv, const Vector3& rhv);


const Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3& operator*=(Vector3& v, float s);


const Vector3 operator*(const Vector3& v,float s);

float EaseInOut(float x1, float x2, float t);
float Lerp(float x1, float x2, float t);
Vector3 Leap(const Vector3& v1, const Vector3& v2, float t);

Matrix4x4 PlayerAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);