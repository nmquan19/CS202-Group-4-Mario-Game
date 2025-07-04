#include "../../include/System/Resources.h"
#include <fstream> 
Texture2D Resources::Koopa;
Texture2D Resources::Goomba;
std::vector<Rectangle> Resources::Enemy_sprite_boxes;
Texture2D Resources::Enemy_Textures;
void Resources::Load()
{  
 std::ifstream  enemy_in ;   
 enemy_in.open("enemy_output.txt"); 
 int id, x, y, w, h;
 while(enemy_in>>id>>x>>y>>w>>h)    
 {
     Enemy_sprite_boxes.push_back({(float)x,(float)y,(float)w, (float)h});
 }
 Enemy_Textures = Texture2D(LoadTexture("assets/enemy_spritesheet.png"));
 enemy_in.close();
}
void Resources::UnLoad()
{
 UnloadTexture(Koopa);
}
