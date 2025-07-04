#include "raylib.h"
#include <vector>
class Resources 
{
  public:
    static Texture2D Koopa;
    static Texture2D Goomba; 
    static std::vector<Rectangle> Enemy_sprite_boxes;
    static Texture2D  Enemy_Textures;  
    static void Load();
    static void UnLoad();
} ;
