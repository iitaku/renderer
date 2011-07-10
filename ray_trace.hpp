#ifndef RAY_TRACE_HPP
#define RAY_TRACE_HPP

#include "element_collection.hpp"
#include "helper.hpp"

#define LIGHT_NUM (1)
#define OBJECT_NUM (1+5)
#define REFLECT_NUM (5)

namespace gtc
{

extern bool flag;

struct Intersect
{
    bool result;
    float distance;
    Coord coord;
    Vector normal;
    
    Intersect(void)
        : result(false), distance(helper::make_inf()), coord(), normal() {}

    Intersect(bool _result)
        : result(_result), distance(helper::make_inf()), coord(), normal() {}
    
    Intersect(bool _result, float _distance)
        : result(_result), distance(_distance), coord(), normal() {}

    Intersect(bool _result, float _distance, 
              const Coord & _coord, const Vector & _normal)
        : result(_result), distance(_distance), coord(_coord), normal(_normal) {}
};

class Ray
{
private:
    Coord origin_;
    Vector direction_;

public:
    Ray(const Coord& origin, const Vector& direction)
        : origin_(origin), direction_(direction.normalize()) {}

    const Coord& Origin(void) const { return origin_; }
    const Vector& Direction(void) const { return direction_; }
};


class Object
{
protected:
    const unsigned int id_;
    RGBA8 color_;

public:    
    Object(unsigned int id)
        : id_(id), color_() {}

    Object(unsigned int id, const RGBA8& color)
        : id_(id), color_(color) {}

    virtual Intersect intersect(const Ray& ray) const = 0;
    virtual RGBA8 shading(Coord * lights, unsigned int light_num,
                          Object ** objs, unsigned int obj_num,
                          const Intersect & isect) const = 0;
};

class BackGround : public Object
{
public:
    BackGround()
        : Object(0) {}
    
    virtual Intersect intersect(const Ray& ray) const
    {
        return Intersect(false);
    }
    
    virtual RGBA8 shading(Coord * lights, unsigned int light_num,
                          Object ** objs, unsigned int obj_num,
                          const Intersect & isect) const
    {
        return color_;
    }
};

class Triangle : public Object
{
private:
    Coord v0_;
    Coord v1_;
    Coord v2_;

    Vector normal_;

    Vector calc_normal()
    {
        Vector v01 = v1_-v0_;
        Vector v02 = v2_-v0_;
        return v02.cross(v01).normalize();
    }

public:
    Triangle(unsigned int id)
        : Object(id), v0_(), v1_(), v2_() {}

    Triangle(unsigned int id,
             const Coord & v0,
             const Coord & v1, 
             const Coord & v2)
           : Object(id), v0_(v0), v1_(v1), v2_(v2)
    {
        normal_ = calc_normal();
    }

    Triangle(unsigned int id,
             const RGBA8 & color,
             const Coord & v0,
             const Coord & v1, 
             const Coord & v2)
           : Object(id, color), v0_(v0), v1_(v1), v2_(v2)
    {
        normal_ = calc_normal();
    }

    virtual Intersect intersect(const Ray& ray) const
    {
        float nume = (v0_ - ray.Origin()).dot(normal_);
        float deno = ray.Direction().dot(normal_);

        if (-0.0001f <= deno)
        {
            return Intersect(false);
        }
        
        float t = nume / deno;

        if (t < 0)
        {
            return Intersect(false);
        }
        
        Coord p = ray.Origin() + ray.Direction() * t;
        
        Vector d0p = p - v0_;
        Vector d01 = v1_ - v0_;
        if (d0p.cross(d01).dot(normal_) < 0)
        {
            return Intersect(false);
        }
        
        Vector d1p = p - v1_;
        Vector d12 = v2_ - v1_;
        if (d1p.cross(d12).dot(normal_) < 0)
        {
            return Intersect(false);
        }

        Vector d2p = p - v2_;
        Vector d20 = v0_ - v2_;
        if (d2p.cross(d20).dot(normal_) < 0)
        {
            return Intersect(false);
        }
        
        return Intersect(true, t, p, normal_);
    }

    virtual RGBA8 shading(Coord * lights, unsigned int light_num,
                          Object ** objs, unsigned int obj_num,
                          const Intersect & isect) const
    { 
        RGBA8 pixel;

        for (unsigned int i=0; i<light_num; ++i)
        {
            const Coord & light = lights[i];
            
            if (isect.normal.dot(light-isect.coord) <= 0.0f)
            {
                return RGBA8(0, 0, 0);
            }
 
            Ray ray(light, isect.coord - light);

            Intersect my_isect = objs[id_]->intersect(ray);

            for (unsigned int j=1; j<obj_num; ++j)
            {
                Intersect other_isect;
                const Object * obj = objs[j];

                if (id_ == j || NULL == obj)
                {
                    continue;
                }

                other_isect = obj->intersect(ray);
                
                if (other_isect.result && 
                    other_isect.distance < my_isect.distance)
                {
                    return RGBA8(0, 0, 0);
                }
            }

            float lambert = ray.Direction().dot(isect.normal);
            pixel = color_ * lambert;
        }

        return pixel;
    }
};

class Sphere : public Object
{
private:
    Coord center_;
    float radius_;

public:
    Sphere(unsigned int id)
        : Object(id), center_(), radius_(0) {}
    
    Sphere(unsigned int id,
           const Coord & center,
           float radius)
        : Object(id), center_(center), radius_(radius) {}
    
    Sphere(unsigned int id,
           const RGBA8 & color,
           const Coord & center,
           float radius)
        : Object(id, color), center_(center), radius_(radius) {}

    virtual Intersect intersect(const Ray& ray) const
    {
        float a = ray.Direction().self_dot();
        float b = ray.Direction().dot((ray.Origin() - center_));
        float c = (ray.Origin() - center_).self_dot() - (radius_*radius_);

        float d = b * b - a * c;

        if (0 > d) 
        {
            return Intersect(false);
        }

        float tn = -b - sqrtf(d);
        float tp = -b + sqrtf(d);

        if (tn < 0 && tp < 0)
        {
            return Intersect(false);
        }

        float t = MAX(tn, tp);
        Coord p = ray.Origin() + ray.Direction() * t;
        Vector n = (p - center_).normalize();
        
        return Intersect(true, t, p, n);
    }

    virtual RGBA8 shading(Coord * lights, unsigned int light_num,
                          Object ** objs, unsigned int obj_num,
                          const Intersect & isect) const
    {
        RGBA8 pixel = color_;

        for (unsigned int i=0; i<light_num; ++i)
        {
            const Coord & light = lights[i];

            if (isect.normal.dot(light-isect.coord) <= 0.0f)
            {
                return RGBA8(0, 0, 0);
            }

            Ray ray(light, isect.coord - light);

            Intersect my_isect = objs[id_]->intersect(ray);

            for (unsigned int j=0; j<obj_num; ++j)
            {
                Intersect other_isect;
                const Object * obj = objs[j];

                if (id_ == j || NULL == obj)
                {
                    continue;
                }

                other_isect = obj->intersect(ray);
                
                if (other_isect.result && 
                    other_isect.distance < my_isect.distance)
                {
                    return RGBA8(0, 0, 0);
                }
            }

            float lambert = ray.Direction().dot(isect.normal);
            pixel = color_ * lambert;
        }

        return pixel;
    }
};

class Scene
{
private:
    int width_;
    int height_;
    
    Coord view_point_;
    Coord screen_[4];
    
    Coord lights_[LIGHT_NUM];

    Object* objs_[OBJECT_NUM];
   
public:
    Scene(int width, int height)
        : width_(width), height_(height), 
          view_point_(0, 0, 1.0)
    {
        screen_[0] = Coord(-5.0, -5.0, 0.0);
        screen_[1] = Coord(+5.0, -5.0, 0.0);
        screen_[2] = Coord(+5.0, +5.0, 0.0);
        screen_[3] = Coord(-5.0, +5.0, 0.0);
        
        lights_[0] = Coord(0.0, 5.0, 1.0);

        objs_[0] = new BackGround();
        objs_[1] = new Sphere(1, RGBA8(255, 0, 0), Coord(-0.7, 0.0, -1.5), 0.7);
        objs_[2] = new Sphere(2, RGBA8(0, 255, 0), Coord(+0.7, 0.0, -1.5), 0.7);
        objs_[3] = new Sphere(3, RGBA8(0, 0, 255), Coord(+0.0, 1.2, -1.5), 0.7);

        objs_[4] = new Triangle(4, RGBA8(255, 255, 255),
                               Coord(-2.0, -2.0, -1.0), 
                               Coord(-2.0, -2.0, -20.0),
                               Coord(+2.0, -2.0, -1.0));

        objs_[5] = new Triangle(5, RGBA8(255, 255, 255),
                               Coord(-2.0, -2.0, -20.0), 
                               Coord(+2.0, -2.0, -20.0),
                               Coord(+2.0, -2.0, -1.0));

    }

    ~Scene()
    {
        for (int i=0; i<OBJECT_NUM; ++i)
        {
            delete objs_[i];
        }
    }

    RGBA8 render(int x, int y)
    {
        Coord screen_coord = 
            Coord(static_cast<float>(x-(width_/2))/static_cast<float>(width_/2),
                  static_cast<float>(y-(height_/2))/static_cast<float>(height_/2),
                  0.0);

        Vector direction = screen_coord - view_point_;
        Ray ray(view_point_, direction);
        Intersect isect;

        unsigned int obj_idx = 0;
        RGBA8 pixel;
        
        for (unsigned int i=1; i<OBJECT_NUM; ++i)
        {
            Intersect tmp_isect;
            
            if (NULL == objs_[i])
            {
                continue;
            }
            
            tmp_isect = objs_[i]->intersect(ray);

            if (tmp_isect.result)
            {
                if (tmp_isect.distance < isect.distance)
                {
                    isect = tmp_isect;
                    obj_idx = i;
                }
            }
        }
        
        pixel = objs_[obj_idx]->shading(&lights_[0], LIGHT_NUM, 
                                        &objs_[0], OBJECT_NUM, 
                                        isect);

        return pixel;
    }

    void displace_view(const Vector& displacement)
    {
        view_point_ = view_point_ + displacement;
    }
};

} /* namespace gtc */

#endif  /* RAY_TRACe_HPP */
