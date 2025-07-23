#pragma once
#include "raylib.h"
#include <iostream>
#include "../System/Interface.h"

class Anima {
private:
    int first;
    int last;
    int cur;

    float speed;
    float durationLeft;

	int numFramesPerRow = 4; // Assuming 4 frames per row

    //Rectangle sourceArea= { 0, 0, 400, 500 };
	const int framesWidth = 100;
	const int framesHeight = 100;

    ItemType Type;


public:
    Anima(int firstFrame, int lastFrame, float speed)
        : first(firstFrame), last(lastFrame), cur(firstFrame),
        speed(speed), durationLeft(speed)
        
    {

    }

    void SetType(ItemType type) {
        Type = type;
    }

    void Update() {
         float dt = GetFrameTime();
         durationLeft -= dt;

         while (durationLeft <= 0) {
             durationLeft = speed;
             cur++;
             if (cur > last) cur = first;
         }
		 //std::cout << cur << std::endl;
     }


   Rectangle GetFrame() const {
	   float x = (cur % numFramesPerRow) * framesWidth;
	   float y = + (static_cast<int>(Type) + cur / numFramesPerRow) * framesHeight;
       //std::cout << static_cast<int>(Type) << std::endl;
	   return { x, y,(float) framesWidth, (float) framesHeight };
   }
};
