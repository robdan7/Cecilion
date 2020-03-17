#define MATH_EXPORTS

#ifdef MATH_EXPORTS
#define MATH_API __declspec(dllexport)
#else
#define MATH_API __declspec(dllimport)
#endif

class MATH_API Math {
public:
    static double add(double a, double b);
    static double mul(double a, double b);
};