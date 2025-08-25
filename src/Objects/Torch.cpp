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

json Torch::toJson() const {
    json data;
    data["saveType"] = getSaveType();
    data["position"] = {position.x, position.y};
    data["curFrame"] = curFrame;
    
    // Save light source data
    if (source) {
        data["lightSource"] = {
            {"radius", source->radius},
            {"brightness", source->brightness},
            {"innerColor", { source->innerColor.r, source->innerColor.g, source->innerColor.b, source->innerColor.a }},
            {"outerColor", { source->outerColor.r, source->outerColor.g, source->outerColor.b, source->outerColor.a }},
            {"isActive", source->isActive}
        };
    }
    
    return data;
}

void Torch::fromJson(const json& data) {  
    curFrame = data["curFrame"];
    
    // Restore light source
    if (data.contains("lightSource")) {
        const auto& lightData = data["lightSource"];
        Color innerColor = { 
            lightData["innerColor"][0], 
            lightData["innerColor"][1], 
            lightData["innerColor"][2], 
            lightData["innerColor"][3] 
        };
        Color outerColor = { 
            lightData["outerColor"][0], 
            lightData["outerColor"][1], 
            lightData["outerColor"][2], 
            lightData["outerColor"][3] 
        };
        
        LightSource s = { 
            position, 
            lightData["radius"], 
            lightData["brightness"], 
            innerColor, 
            outerColor, 
            lightData["isActive"] 
        };
        source = std::make_shared<LightSource>(s);
        LightingManager::getInstance().addLight(source);
    }
}

std::string Torch::getSaveType() const {
    return "Torch";
}