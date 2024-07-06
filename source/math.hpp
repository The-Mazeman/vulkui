#pragma once
#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#define PI (float)3.14159265358979323846

struct vec2
{
    float cells[2];
};
struct vec3
{
    float cells[3];
};
struct vec4
{
    float cells[4];
};
struct mat2
{
    float cells[2][2];
};
struct mat4
{
    float cells[4][4];
};

void rotate(mat2* matrix, float radians);
void identity(mat2* matrix);
void scale(mat2* matrix, float width, float height);
void multiply(vec2* vec, mat2* mat);
float squareRoot(float x);
float square(float x);
float tanInverse(float x);
void calculateIntersection(Window* windowA, Window* windowB, Window* intersection);
