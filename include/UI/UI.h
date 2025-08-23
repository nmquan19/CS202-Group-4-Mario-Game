#pragma once
#include <raylib.h>
#include <string>
using namespace std;
class UIManager;

class ScoreDisplay {
public:
    Vector2 position;
    int score;
    Font font;

    ScoreDisplay(Vector2 pos) : position(pos), score(0) {
        font = GetFontDefault();
    }
    int getScore() {
        return score;
    }
    void Draw(Font f, Color color) {
        font = f;
        DrawTextEx(font, "MARIO", position, 40, 2, color);
        const char* scoreStr = TextFormat("%06d", score);
        Vector2 size = MeasureTextEx(font, "MARIO", 40, 2);
        Vector2 scorePos = { position.x, position.y + size.y };
        DrawTextEx(font, scoreStr, scorePos, 40, 2, color);
    }
};

class CoinDisplay {
private:
    Vector2 position;

    Texture2D coinSpriteSheet;
    Rectangle currentFrame;

    int frameCount;
    float frameWidth;
    float frameHeight;

    float animationTime;
    float frameDuration;
    int currentFrameIndex;
public:
    int coinCount;
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
    void Draw(Font f, Color color) {
        font = f;
        Rectangle dest = { position.x, position.y, 30, 30 };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(coinSpriteSheet, currentFrame, dest, origin, 0.0f, WHITE);

        const char* coinText = TextFormat(" x %d", coinCount);
        DrawTextEx(font, coinText, { position.x + 30, position.y }, 40, 2, color);
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

    void Draw(Font f, Color color) {
        font = f;
        DrawTextEx(font, "WORLD", position, 40, 2, color);

        Vector2 size1 = MeasureTextEx(font, "WORLD", 40, 2);
        Vector2 size2 = MeasureTextEx(font, worldStr, 40, 2);
        Vector2 pos2 = { position.x + (size1.x - size2.x) / 2.0f, position.y + size1.y };
        DrawTextEx(font, worldStr, pos2, 40, 2, color);
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

    void Draw(Font f, Color color) {
        font = f;
        DrawTextEx(font, "TIME", position, 40, 2, color);
        const char* timeStr = TextFormat("%03d", (int)timeLeft);
        Vector2 size1 = MeasureTextEx(font, "TIME", 40, 2);
        Vector2 size2 = MeasureTextEx(font, timeStr, 40, 2);
        Vector2 pos2 = { position.x + (size1.x - size2.x) / 2.0f, position.y + size1.y };
        DrawTextEx(font, timeStr, pos2, 40, 2, color);
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

    void drawInformationBoard(Color color) {
        score.Draw(menuFont, color);
        coin.Draw(menuFont, color);
        world.Draw(menuFont, color);
        timer.Draw(menuFont, color);
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
/*
class BackgroundManager {
private:
    Texture2D Airship_night_1;
    Texture2D Airship_night_2;
    Texture2D Airship_night_3;
    Texture2D Airship_night_4;
    Texture2D Airship_night_5;
    Texture2D Airship_night_6;
    Texture2D Forest_1;
    Texture2D Forest_2;
    Texture2D Ghost_house_1;
    Texture2D Ghost_house_2;
    Texture2D Ghost_house_3;
    Texture2D Snow_night_1;
    Texture2D Snow_night_2;

    Vector2 position;
public:
    BackgroundManager() {
        Airship_night_1 = LoadTexture("../../assets/Airship_night_1.png");
        Airship_night_2 = LoadTexture("../../assets/Airship_night_2.png");
        Airship_night_3 = LoadTexture("../../assets/Airship_night_3.png");
        Airship_night_4 = LoadTexture("../../assets/Airship_night_4.png");
        Airship_night_5 = LoadTexture("../../assets/Airship_night_5.png");
        Airship_night_6 = LoadTexture("../../assets/Airship_night_6.png");
        Forest_1 = LoadTexture("../../assets/Forest_1.png");
        Forest_2 = LoadTexture("../../assets/Forest_2.png");
        Ghost_house_1 = LoadTexture("../../assets/Ghost_house_1.png");
        Ghost_house_2 = LoadTexture("../../assets/Ghost_house_2.png");
        Ghost_house_3 = LoadTexture("../../assets/Ghost_house_3.png");
        Snow_night_1 = LoadTexture("../../assets/Snow_night_1.png");
        Snow_night_2 = LoadTexture("../../assets/Snow_night_2.png");
    }
    ~BackgroundManager() {
        UnloadTexture(Airship_night_1);
        UnloadTexture(Airship_night_2);
        UnloadTexture(Airship_night_3);
        UnloadTexture(Airship_night_4);
        UnloadTexture(Airship_night_5);
        UnloadTexture(Airship_night_6);
        UnloadTexture(Forest_1);
        UnloadTexture(Forest_2);
        UnloadTexture(Ghost_house_1);
        UnloadTexture(Ghost_house_2);
        UnloadTexture(Ghost_house_3);
        UnloadTexture(Snow_night_1);
        UnloadTexture(Snow_night_2);
    }
    Texture2D findBackground(string s) {
        if (s == "Airship_night_1") return Airship_night_1;
        if (s == "Airship_night_2") return Airship_night_2;
        if (s == "Airship_night_3") return Airship_night_3;
        if (s == "Airship_night_4") return Airship_night_4;
        if (s == "Airship_night_5") return Airship_night_5;
        if (s == "Airship_night_6") return Airship_night_6;
        if (s == "Forest_1") return Forest_1;
        if (s == "Forest_2") return Forest_2;
        if (s == "Ghost_house_1") return Ghost_house_1;
        if (s == "Ghost_house_2") return Ghost_house_2;
        if (s == "Ghost_house_3") return Ghost_house_3;
        if (s == "Snow_night_1") return Snow_night_1;
        return Snow_night_2;
    }
    void draw(Vector2 Position, string s, float scale) {
        position = Position;
        for (int i = 0; i < 100; i++) {
            DrawTextureEx(findBackground(s), { position.x + findBackground(s).width * i, position.y }, 0, scale, WHITE);
        }
        
    }
    void update(float deltaTime) {
        position.x -= deltaTime;
    }
};
*/