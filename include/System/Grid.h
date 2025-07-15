#pragma once

#include "raylib.h"
#include <cmath>

class GridSystem {
public:
    static const int GRID_SIZE = 128;
    static Vector2 snapToGrid(Vector2 position) {
        return {
            (float)floor(position.x / GRID_SIZE) * GRID_SIZE,
            (float)floor(position.y / GRID_SIZE) * GRID_SIZE
        };
    }
    static Vector2 getWorldPosition(Vector2 gridPosition) {
        return {
            gridPosition.x * GRID_SIZE,
            gridPosition.y * GRID_SIZE
        };
	}
    static Vector2 getGridCoord(Vector2 worldPosition) {
        return {
            (float)floor(worldPosition.x / GRID_SIZE),
            (float)floor(worldPosition.y / GRID_SIZE)
        };
    }

    static void drawGrid(int screenWidth, int screenHeight) {
        for (int x = 0; x < screenWidth; x += GRID_SIZE) {
            DrawLine(x, 0, x, screenHeight, BLACK);
        }
        for (int y = 0; y < screenHeight; y += GRID_SIZE) {
            DrawLine(0, y, screenWidth, y, BLACK);
        }
    }

    static Rectangle getGridRect(Vector2 gridPosition) {
        return {
            gridPosition.x * GRID_SIZE,
            gridPosition.y * GRID_SIZE,
            GRID_SIZE,
            GRID_SIZE
        };
    }
};