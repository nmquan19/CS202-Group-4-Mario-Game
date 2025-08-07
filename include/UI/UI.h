#pragma once
#include <raylib.h>
#include <string>

class UIManager;

class ScoreDisplay {
public:
    Vector2 position;
    int score;
    Font font;

    ScoreDisplay(Vector2 pos) : position(pos), score(0) {
        font = GetFontDefault();
    }

    void Draw(Font f) {
        font = f;
        DrawTextEx(font, "MARIO", position, 40, 2, BLACK);
        const char* scoreStr = TextFormat("%06d", score);
        Vector2 size = MeasureTextEx(font, "MARIO", 40, 2);
        Vector2 scorePos = { position.x, position.y + size.y };
        DrawTextEx(font, scoreStr, scorePos, 40, 2, BLACK);
    }
};

class CoinDisplay {
public:
    Vector2 position;
    int coinCount;
    Texture2D coinSpriteSheet;
    Rectangle currentFrame;

    int frameCount;
    float frameWidth;
    float frameHeight;

    float animationTime;
    float frameDuration;
    int currentFrameIndex;

    Font font;

    CoinDisplay(Vector2 pos, const char* c) : position(pos), coinCount(0), frameCount(5), animationTime(0), frameDuration(2.0f / 5.0f), currentFrameIndex(0) {
        coinSpriteSheet = LoadTexture(c);

        frameWidth = (float)coinSpriteSheet.width / frameCount;
        frameHeight = (float)coinSpriteSheet.height;

        currentFrame = { 0, 0, frameWidth, frameHeight };

        font = GetFontDefault();
    }
    ~CoinDisplay() {
        UnloadTexture(coinSpriteSheet);
    }
    void Update(float deltaTime) {
        animationTime += deltaTime;

        if (animationTime >= frameDuration) {
            animationTime = 0.0f;

            currentFrameIndex = (currentFrameIndex + 1) % frameCount;
            currentFrame.x = currentFrameIndex * frameWidth;
        }
    }
    void Draw(Font f) {
        font = f;
        Rectangle dest = { position.x, position.y, 30, 30 };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(coinSpriteSheet, currentFrame, dest, origin, 0.0f, WHITE);

        const char* coinText = TextFormat(" x %d", coinCount);
        DrawTextEx(font, coinText, { position.x + 30, position.y }, 40, 2, BLACK);
    }
};

class WorldDisplay {
public:
    Vector2 position;
    const char* worldStr;
    Font font;

    WorldDisplay(Vector2 pos, const char* level) : position(pos), worldStr(level) {
        font = GetFontDefault();
    }

    void Draw(Font f) {
        font = f;
        DrawTextEx(font, "WORLD", position, 40, 2, BLACK);

        Vector2 size1 = MeasureTextEx(font, "WORLD", 40, 2);
        Vector2 size2 = MeasureTextEx(font, worldStr, 40, 2);
        Vector2 pos2 = { position.x + (size1.x - size2.x) / 2.0f, position.y + size1.y };
        DrawTextEx(font, worldStr, pos2, 40, 2, BLACK);
    }
};

class TimerDisplay {
public:
    Vector2 position;
    float timeLeft;
    Font font;

    TimerDisplay(Vector2 pos, float startTime) : position(pos), timeLeft(startTime) {
        font = GetFontDefault();
    }

    void Update(float deltaTime) {
        if (timeLeft > 0.0f) {
            timeLeft -= deltaTime;
            if (timeLeft < 0.0f) timeLeft = 0.0f;
        }
    }

    void Draw(Font f) {
        font = f;
        DrawTextEx(font, "TIME", position, 40, 2, BLACK);
        const char* timeStr = TextFormat("%03d", (int)timeLeft);
        Vector2 size1 = MeasureTextEx(font, "TIME", 40, 2);
        Vector2 size2 = MeasureTextEx(font, timeStr, 40, 2);
        Vector2 pos2 = { position.x + (size1.x - size2.x) / 2.0f, position.y + size1.y };
        DrawTextEx(font, timeStr, pos2, 40, 2, BLACK);
    }
};

class UIManager {
private:
    
    UIManager();
    
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    ScoreDisplay score;
    CoinDisplay coin;
    WorldDisplay world;
    TimerDisplay timer;

public:
    
    static UIManager& getInstance();

    bool isGameOver;
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    Font menuFont;

    ~UIManager();

    void updateInformationBoard(float deltaTime) {
        coin.Update(deltaTime);
        timer.Update(deltaTime);
    }

    void drawInformationBoard() {
        score.Draw(menuFont);
        coin.Draw(menuFont);
        world.Draw(menuFont);
        timer.Draw(menuFont);
    }

    void ShowGameOver();
    bool IsGameOver() const;
    void DrawGameOver();

    void setScore(int s) { score.score = s; }
    void addCoin() { coin.coinCount++; }
    void setWorld(const char* w) { world.worldStr = w; }
    void resetTimer(float t) { timer.timeLeft = t; }

    void DrawTypewriterText(const char* text, Vector2 position, float fontSize, float spacing, Color color, float delay, float& timer, int& visibleChars) {
        timer += GetFrameTime();

        if (text[visibleChars] != '\0' && timer >= delay) {
            visibleChars++;
            timer = 0.0f;
        }

        DrawTextEx(menuFont, TextSubtext(text, 0, visibleChars), position, fontSize, spacing, color);
    }
};