#pragma once

class GameContext;
class GameState {
public:
    virtual ~GameState() = default;
    virtual void handleInput(GameContext& context) = 0;
    virtual void update(GameContext& context, float deltaTime) = 0;
    virtual void draw(GameContext& context) = 0;
};

class MenuState : public GameState {
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
};

class RedirectState : public GameState {
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
};

class GamePlayState : public GameState {
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
    void DrawBackGround(Texture2D&);
};

class InformationState : public GameState {
private:
    float animationTime = 0;
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
};

class CharacterSelectingState : public GameState {
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
};



class EditorState : public GameState {
private:
    int stateSelect = 1;
    int mapSelect = 1;
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
};

class EditorSelectingState : public GameState {
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
};


class GameOverState : public GameState {
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
};