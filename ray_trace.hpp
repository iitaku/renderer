#ifndef RAY_TRACE_HPP
#define RAY_TRACE_HPP

#include "element_collection.hpp"
#include "helper.hpp"
#include "porting.hpp"

#define LIGHT_NUM (1)
#define OBJECT_NUM (1+5)
#define REFLECT_NUM (5)

namespace gtc
{

typedef RGBA8U RGBA;

static int deb_x;
static int deb_y;

struct Ray
{
    Coord origin;
    Vector direction;
    float strong;

    FUNC_DECL
    Ray(void)
        : origin(), direction(), strong(0.0f) {}
    
    FUNC_DECL
    Ray(const Coord& _origin, const Vector& _direction, float _strong)
        : origin(_origin), direction(_direction.normalize()), strong(_strong) {}

    FUNC_DECL
    bool operator==(const Ray & rhs)
    {
        if (this->origin == rhs.origin &&
            this->direction == rhs.direction &&
            this->strong == rhs.strong)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

struct Intersect
{
    bool result;
    float distance;
    Coord coord;
    Vector normal;
    Ray ray;
    const void * obj;
    
    FUNC_DECL
    Intersect(void)
        : result(false), distance(helper::make_inf()), coord(), normal(), ray(), obj(NULL) {}

    FUNC_DECL
    Intersect(bool _result)
        : result(_result), distance(helper::make_inf()), coord(), normal(), ray(), obj(NULL) {}
    
    FUNC_DECL
    Intersect(bool _result, float _distance)
        : result(_result), distance(_distance), coord(), normal(), ray(), obj(NULL) {}
    
    FUNC_DECL
    Intersect(bool _result, float _distance,
              const Coord & _coord, const Vector & _normal, const Ray & _ray, const void * _obj)
        : result(_result), distance(_distance), coord(_coord), normal(_normal), ray(_ray), obj(_obj) {}
        //      const Coord & _coord, const Vector & _normal, const Ray & _ray, const Object * _obj)
        //: result(_result), distance(_distance), coord(_coord), normal(_normal), ray(_ray), obj(_obj) {}

    FUNC_DECL
    bool operator==(const Intersect & rhs)
    {
        if (this->result == rhs.result && 
            this->distance == rhs.distance &&
            this->coord == rhs.coord &&
            this->normal == rhs.normal &&
            this->ray == rhs.ray &&
            this->obj == rhs.obj )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

struct Material
{
    RGBA color;
    float reflection;

    FUNC_DECL
    Material(void)
        : color(), reflection(0) {}
    
    FUNC_DECL
    Material(const RGBA & _color, float _reflection)
        : color(_color), reflection(_reflection) {}
};

class Object
{
protected:
    const unsigned int id_;
    Material material_;
    const RGBA invisible_color_;

public:    
    FUNC_DECL
    Object(unsigned int id)
        : id_(id), material_(), invisible_color_(RGBA(0, 0, 0)) {}

    FUNC_DECL
    Object(unsigned int id, const Material& material)
        : id_(id), material_(material), invisible_color_(RGBA(0, 0, 0)) {}

    FUNC_DECL 
    virtual Intersect intersect(const Ray& ray) const = 0;
    
    FUNC_DECL 
    virtual RGBA shading(Coord * lights, unsigned int light_num,
                          Object ** objs, unsigned int obj_num,
                          const Intersect & isect) const = 0;
};

class BackGround : public Object
{
public:
    FUNC_DECL
    BackGround()
        : Object(0) {}
    
    FUNC_DECL 
    virtual Intersect intersect(const Ray& ray) const
    {
        /* make_max 実装する*/
        return Intersect(true, helper::make_max());
    }
    
    FUNC_DECL 
    virtual RGBA shading(Coord * lights, unsigned int light_num,
                          Object ** objs, unsigned int obj_num,
                          const Intersect & isect) const
    {
        return material_.color;
    }
};

class Triangle : public Object
{
private:
    Coord v0_;
    Coord v1_;
    Coord v2_;

    Vector normal_;

    FUNC_DECL
    Vector calc_normal()
    {
        Vector v01 = v1_-v0_;
        Vector v02 = v2_-v0_;
        return v02.cross(v01).normalize();
    }

public:
    FUNC_DECL
    Triangle(unsigned int id)
        : Object(id), v0_(), v1_(), v2_() {}

    FUNC_DECL
    Triangle(unsigned int id,
             const Coord & v0,
             const Coord & v1, 
             const Coord & v2)
           : Object(id), v0_(v0), v1_(v1), v2_(v2)
    {
        normal_ = calc_normal();
    }

    FUNC_DECL
    Triangle(unsigned int id,
             const Material & material,
             const Coord & v0,
             const Coord & v1, 
             const Coord & v2)
           : Object(id, material), v0_(v0), v1_(v1), v2_(v2)
    {
        normal_ = calc_normal();
    }
    
    FUNC_DECL 
    virtual Intersect intersect(const Ray& ray) const
    {
        float nume = (v0_ - ray.origin).dot(normal_);
        float deno = ray.direction.dot(normal_);

        if (-0.0001f <= deno)
        {
            return Intersect(false);
        }
        
        float t = nume / deno;

        if (t < 0)
        {
            return Intersect(false);
        }
        
        Coord p = ray.origin + ray.direction * t;
        
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
        
        float reflet = 2.0f * (ray.direction.dot(normal_));
        Ray new_ray(p, ray.direction - (normal_ * reflet), ray.strong * material_.reflection);
        
        return Intersect(true, t, p, normal_, new_ray, this);
    }

    FUNC_DECL 
    virtual RGBA shading(Coord * lights, unsigned int light_num,
                          Object ** objs, unsigned int obj_num,
                          const Intersect & isect) const
    { 
        RGBA pixel;

        for (unsigned int i=0; i<light_num; ++i)
        {
            const Coord & light = lights[i];
            
            if (isect.normal.dot(light-isect.coord) <= 0.0f)
            {
                return Object::invisible_color_;
            }
 
            Ray ray(light, isect.coord - light, 1.0);

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
                    return Object::invisible_color_;
                }
            }
            
            float lambert = fabs(ray.direction.dot(isect.normal));
            pixel = material_.color * lambert;
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
    FUNC_DECL
    Sphere(unsigned int id)
        : Object(id), center_(), radius_(0) {}
    
    FUNC_DECL
    Sphere(unsigned int id,
           const Coord & center,
           float radius)
        : Object(id), center_(center), radius_(radius) {}
    
    FUNC_DECL
    Sphere(unsigned int id,
           const Material & material,
           const Coord & center,
           float radius)
        : Object(id, material), center_(center), radius_(radius) {}

    FUNC_DECL
    virtual Intersect intersect(const Ray& ray) const
    {
        float a = ray.direction.self_dot();
        float b = ray.direction.dot((ray.origin - center_));
        float c = (ray.origin - center_).self_dot() - (radius_*radius_);

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
        Coord p = ray.origin + ray.direction * t;
        Vector n = (p - center_).normalize();
        
        float reflet = 2.0f * (ray.direction.dot(n));
        Ray new_ray(p, ray.direction - (n * reflet), ray.strong * material_.reflection);
        
        return Intersect(true, t, p, n, new_ray, this);
    }
    
    FUNC_DECL 
    virtual RGBA shading(Coord * lights, unsigned int light_num,
                         Object ** objs, unsigned int obj_num,
                         const Intersect & isect) const
    {
        RGBA pixel = material_.color;

        for (unsigned int i=0; i<light_num; ++i)
        {
            const Coord & light = lights[i];

            if (isect.normal.dot(light-isect.coord) <= 0.0f)
            {
                return Object::invisible_color_;
            }

            Ray ray(light, isect.coord - light, 1.0);

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
                    return Object::invisible_color_;
                }
            }

            float lambert = fabs(ray.direction.dot(isect.normal));
            pixel = material_.color * lambert;
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

    Object * objs_[OBJECT_NUM];
   
public:
    
    FUNC_DECL
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
#if 0
        objs_[1] = new Sphere(1, Material(RGBA(255, 0, 0), 0.5), Coord(-0.7, 0.0, -1.5), 0.7);
        objs_[2] = new Sphere(2, Material(RGBA(0, 255, 0), 0.5), Coord(+0.7, 0.0, -1.5), 0.7);
        objs_[3] = new Sphere(3, Material(RGBA(0, 0, 255), 0.5), Coord(+0.0, 1.2, -1.5), 0.7);

        objs_[4] = new Triangle(4, Material(RGBA(255, 255, 255), 1.0),
                                Coord(-2.0, -2.0, -1.0), 
                                Coord(-2.0, -2.0, -20.0),
                                Coord(+2.0, -2.0, -1.0));

        objs_[5] = new Triangle(5, Material(RGBA(255, 255, 255), 1.0),
                                Coord(-2.0, -2.0, -20.0), 
                                Coord(+2.0, -2.0, -20.0),
                                Coord(+2.0, -2.0, -1.0));
#else
        objs_[1] = new Triangle(1, Material(RGBA(255, 255, 255), 1.0), 
                                Coord(-20.0, -20.0, -1.0),
                                Coord(+20.0, -20.0, -1.0),
                                Coord(+20.0, +20.0, -1.0));
        objs_[2] = new Triangle(2, Material(RGBA(255, 255, 255), 1.0), 
                                Coord(-20.0, -20.0, -1.0),
                                Coord(-20.0, +20.0, -1.0),
                                Coord(+20.0, +20.0, -1.0));
        objs_[3] = NULL;
        objs_[4] = NULL;
        objs_[5] = NULL;
#endif
    
    }

    FUNC_DECL
    ~Scene()
    {
        for (int i=0; i<OBJECT_NUM; ++i)
        {
            delete objs_[i];
        }
    }

    FUNC_DECL
    RGBA8U render(int x, int y)
    {

#ifndef USE_CUDA        
        deb_x = x;
        deb_y = y;
#endif

        Coord screen_coord = 
            Coord(static_cast<float>(x-(width_/2))/static_cast<float>(width_/2),
                  static_cast<float>(y-(height_/2))/static_cast<float>(height_/2),
                  0.0);

        Vector direction = screen_coord - view_point_;
        Ray ray(view_point_, direction, 1.0);
        Intersect isects[REFLECT_NUM];

        unsigned int obj_idx = 0;
        RGBA8U pixel;
       
        unsigned int reflect_count = 0;

        do
        {
            for (unsigned int i=1; i<OBJECT_NUM; ++i)
            {
                Intersect isect;

                if (NULL == objs_[i])
                {
                    continue;
                }

                isect = objs_[i]->intersect(ray);
                
                if (isect.result)
                {
                    if (isect.distance < isects[reflect_count].distance)
                    {
                        isects[reflect_count] = isect;
                        obj_idx = i;
                    }
                }
            }
            
            ray = isects[reflect_count].ray;
            
            reflect_count++;

        } while (false != isects[reflect_count-1].result && 
                 0.0f < isects[reflect_count-1].ray.strong 
                 && reflect_count < REFLECT_NUM);
        
        pixel = objs_[obj_idx]->shading(&lights_[0], LIGHT_NUM, 
                                        &objs_[0], OBJECT_NUM, 
                                        isects[0]);

        return pixel;
    }

    FUNC_DECL
    void displace_view(const Vector& displacement)
    {
        view_point_ = view_point_ + displacement;
    }
};

} /* namespace gtc */

#endif  /* RAY_TRACe_HPP */
