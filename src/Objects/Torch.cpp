#include "../../include/Objects/Torch.h"
#include "../../include/System/Constant.h"
#include "../../include/System/TextureManager.h"
Torch::Torch(Vector2 position, Vector2 size, const LightSource& light) :texture(TextureManager::torch_texture), source(std::make_shared<LightSource>(light))
{
   this->size = size; 
   this->position = position;
   source->position = this->position;
   //LightingManager::getInstance().addLight(source);
   curFrame = 0; 
   spriteBox = { 0,0,0,0 };
   animController.set(0,5,1,Easing::linear, false, true);
  
}
Torch::Torch(Vector2 position, Vector2 size, float radius, float brightness, Color innerColor, Color outerColor):texture(TextureManager::torch_texture)
{
    this->size = size;
    this->position = position;
    LightSource s = { position, radius, brightness,innerColor,outerColor};
    source = std::make_shared<LightSource>(s);
    LightingManager::getInstance().addLight(source);
    curFrame = 0;
    spriteBox = { 0,0,0,0 };
    animController.set(0, 5, 1, Easing::linear, false, true);
}
Vector2 Torch::getSize() const {
	return size;
}
void Torch::setActive(bool flag) {
    active = flag;
}

bool Torch::isActive() const {
    return active;
}

void Torch::setCollided(bool flag) {
    collided = flag;
}

bool Torch::isCollided() const {
    return collided;
}

Vector2 Torch::getPosition() const {
    return position;
}

void Torch::setPosition(Vector2 newPos) {
    position = newPos;
}
void Torch::draw()
{
    Rectangle sourceRec = this->spriteBox;
    Vector2 origin = { 0, 0 };
   DrawTexturePro(this->texture, sourceRec, { this->position.x,this->position.y, size.x * Constants::TILE_SIZE,size.y * Constants::TILE_SIZE }, origin, 0.0f, WHITE);
}
void Torch::update(float deltaTime)
{
    curFrame = animController.getCurrentFrame();
    animController.update(deltaTime);
    spriteBox = TextureManager::torch_sprite_boxes[curFrame];
}