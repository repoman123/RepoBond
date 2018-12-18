//#define NO_RAYTRACE
//#define NO_UPDATE
//#define NO_ANIM
#define SHOW_BOUNDS
//#define GHOST_MODE
#ifdef GHOST_MODE
#define NO_RAYTRACE
#endif
#define todeg(radians) ((radians) * (180.0f / (float)D3DX_PI))
#define torad(angle) ((angle) * ((float)D3DX_PI / 180.0f))