
struct Geom {
    sampler2D depth;
    sampler2D diff;
    sampler2D pos;
    sampler2D norm;
};

struct Camera {
    float near;
    float far;
    vec3 pos;
    vec3 dir;
};

struct Lighting {
    sampler2D depth;
    sampler2D color;
};

struct PointLight {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float radius;
    float intensity;
};

struct DirectionalLight {
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Shadow {
    sampler2D depth;
    mat4 view;
    mat4 proj;
};