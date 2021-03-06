#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

class MathUtil {
    public:
        constexpr static float MARGIN_ERROR = 0.001f;

        constexpr static float PI = 3.1415926535897932f;

        static float degToRad(float degrees);
        static float radToDeg(float radians);
    
        static bool equalFloats(float val, float other);

        static int maxInt(int val, int other);
        static int minInt(int val, int other);
        static float maxFloat(float val, float other);
        static float minFloat(float val, float other);
        static int clampInt(int val, int min, int max);
        static float clampFloat(float val, float min, float max);

        static float absFloat(float val);
        static double absDouble(double val);

        static int floor(float val);
        static int ceil(float val);
};

#endif // MATH_H_INCLUDED
