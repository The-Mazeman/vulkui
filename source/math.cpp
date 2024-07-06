#include "include.hpp"
#include "math.hpp"

void multiply(vec2* vec, mat2* mat)
{
    float vecX = vec->cells[0];
    float vecY = vec->cells[1];

    vec->cells[0] = (vecX * mat->cells[0][0]) + (vecY * mat->cells[0][1]); 
    vec->cells[1] = (vecX * mat->cells[1][0]) + (vecY * mat->cells[1][1]); 
}
void multiply(mat2* a, mat2* b, mat2* c)
{
    c->cells[0][0] = (a->cells[0][0] * b->cells[0][0]) + (a->cells[0][1] * b->cells[1][0]);
    c->cells[0][1] = (a->cells[0][0] * b->cells[0][1]) + (a->cells[0][1] * b->cells[1][1]);
    c->cells[1][0] = (a->cells[1][0] * b->cells[0][0]) + (a->cells[1][1] * b->cells[1][0]);
    c->cells[1][1] = (a->cells[1][0] * b->cells[0][1]) + (a->cells[1][1] * b->cells[1][1]);
}
void rotate(mat2* matrix, float radians)
{
    matrix->cells[0][0] = cosf(radians);
    matrix->cells[0][1] = sinf(radians) * -1.0f;
    matrix->cells[1][0] = sinf(radians);
    matrix->cells[1][1] = cosf(radians);
}
void identity(mat2* matrix)
{
    matrix->cells[0][0] = 1.0f;
    matrix->cells[1][1] = 1.0f;
}
void scale(mat2* matrix, float width, float height)
{
    mat2 scaleMatrix = {};
    scaleMatrix.cells[0][0] = width;
    scaleMatrix.cells[1][1] = height;
    
    mat2 result = {};
    multiply(matrix, &scaleMatrix, &result);
    *matrix = result;

}

float square(float x)
{
    return x * x;
}
float squareRoot(float x)
{
    return (float)sqrt(x);
}
float tanInverse(float x)
{
    return (float)atan(x);
}
void calculateIntersection(Window* windowA, Window* windowB, Window* intersection)
{
    float x0 = max(windowA->x, windowB->x);
    float y0 = max(windowA->y, windowB->y);
    float x1 = min(windowA->x + windowA->width, windowB->x + windowB->width);
    float y1 = min(windowA->y + windowA->height, windowB->y + windowB->height);

    if(x1 < x0)
    {
        x0 = 0.0f;
        x1 = x0;
    }
    if(y1 < y0)
    {
        y0 = 0.0f;
        y1 = y0;
    }

    intersection->x = x0;
    intersection->y = y0;
    intersection->width = x1 - x0;
    intersection->height = y1 - y0;
}
