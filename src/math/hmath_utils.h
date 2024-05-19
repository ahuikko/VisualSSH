#ifndef MATH_UTILS_H
#define MATH_UTILS_H




constexpr float PI          = 3.14159265358979323846f;
constexpr float TWO_PI      = 6.28318530717958647692f;
constexpr float HALF_PI     = 1.57079632679489661923f;
constexpr float QUARTER_PI  = 0.78539816339744830961f;
constexpr float INV_PI      = 0.31830988618379067153f;
constexpr float INV_TWO_PI  = 0.15915494309189533576f;
constexpr float DEG2RAD     = 0.01745329251994329576f;
constexpr float RAD2DEG     = 57.2957795130823208767f;
constexpr float SQRT2       = 1.41421356237309504880f;
constexpr float SQRT3       = 1.73205080756887729352f;
constexpr float EULER       = 2.71828182845904523536f;





inline float radians(float degrees)
{
    return degrees * PI / 180.0;
}

inline float degrees(float radians)
{
    return  radians * (180.0 / PI);
}


#endif