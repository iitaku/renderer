#ifndef PPB_RAYTRACE_H
#define PPB_RAYTRACE_H

#define SPHERE_RAD    50.0f
#define SPHERE_CENTER 80.0f,80.0f,80.0f
#define VIEW_POINT    500.0f,500.0f,500.0f
#define LIGHT         600.0f,-600.0f,600.0f

#define BLACK         0,0,0
#define RED           255,0,0
#define SPHERE_COLOR  RED
#define BG_COLOR      BLACK

//#define INFINITY      (1.0e8)

#define OUTPUT        "out.ppm"
#define IMG_WIDTH     (400)
#define IMG_HEIGHT    (400)
#define THREAD_NUM    (2)

#ifdef WIN32
#define sqrtf(f) (float)sqrt(f)
#endif

#define Sq(x) ((x)*(x))
#define Pow(y,x,n) {int i; for (i=0, y=1; i<n; ++i) y*=x; }
#define Max(a,b) ((a)>(b)) ? (a) : (b)
#define Min(a,b) ((a)<(b)) ? (a) : (b)

typedef struct _vector
{
    float x;
    float y;
    float z;
} vector_t;

typedef struct _sphere
{
    rgb_t color;
    vector_t center;
    float radius;
} sphere_t;

typedef struct _scene_data
{
    sphere_t ball;
    rgb_t bgcolor;
    vector_t view;
    vector_t viewpoint;
    vector_t light;
} scene_data_t;

typedef struct _workarea
{
    int startx;
    int endx;
    int starty;
    int endy;
} workarea_t;

typedef struct _thread_arg
{
    int cpuid;
    image_t* img;
    scene_data_t* scene;
    workarea_t warea;
} thread_arg_t; 

#endif  /* PPB_RAYTRACE_H */
