#ifndef RAY_TRACE_HPP
#define RAY_TRACE_HPP

#include "four_element.hpp"

namespace gtc
{

typedef FourElement<float> Coord;
typedef FourElement<float> Vector;
typedef FourElement<unsigned char> RGBA8;

class Ray
{
private:
    Coord origin_;
    Vector direction_;

public:
    Ray(const Coord& origin, const Vector& direction)
        : origin_(origin), direction_(direction.norm()) {}

    const Coord& Origin(void) const { return origin_; }
    const Vector& Direction(void) const { return direction_; }
};

class Board
{
private:
    Coord lb_;
    Coord rb_;
    Coord rt_;
    Coord lt_;
public:
    Board(const Coord & lb,
          const Coord & rb, 
          const Coord & rt, 
          const Coord & lt)
        : lb_(lb), rb_(rb), rt_(rt), lt_(lt) {}

    bool intersect(const Ray& ray)
    {
        return false;
    }
};

class Sphere
{
private:
    Coord center_;
    float radius_;

public:
    Sphere() {}
    
    Sphere(const Coord & center,
           float radius)
        : center_(center), radius_(radius) {}

    bool intersect(const Ray& ray, float& intersect_distance)
    {
        float a = ray.Direction().self_inner_product();
        float b = ray.Direction().inner_product((ray.Origin() - center_));
        float c = (ray.Origin() - center_).self_inner_product() - (radius_*radius_);

        float d = b * b - a * c;

        if (0>d) 
        {
            intersect_distance = 0;
            return false;
        }
        
        /* とりあえずゼロ */
        intersect_distance = 0;
        return true;
    }
};

class Scene
{
private:
    int width_;
    int height_;
    
    Coord view_point_;
    Coord screen_[4];

    Sphere obj0;

public:
    Scene(int width, int height)
        : width_(width), height_(height), view_point_(0, 0, 1.0)
    {
        screen_[0] = Coord(-1.0, -1.0, 0.0);
        screen_[1] = Coord(+1.0, -1.0, 0.0);
        screen_[2] = Coord(+1.0, +1.0, 0.0);
        screen_[3] = Coord(-1.0, +1.0, 0.0);

        obj0 = Sphere(Coord(0, 0, -1.0), 0.5);
    }

    RGBA8 render(int i, int j)
    {
        Coord screen_coord = Coord(static_cast<float>(i-(width_/2))/static_cast<float>(width_/2),
                                   static_cast<float>(j-(height_/2))/static_cast<float>(height_/2),
                                   0.0);

        Vector direction = screen_coord - view_point_;
        Ray ray(view_point_, direction);
        
        float distance;
        
        if (obj0.intersect(ray, distance))
        {
            return RGBA8(255, 255, 255, 0);
        }
        else
        {
            return RGBA8(0, 0, 0, 0);
        }
    }
};

} /* namespace gtc */

#endif  /* RAY_TRACe_HPP */
