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

class GamePlayState : public GameState {
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
};

class EditorState : public GameState {
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
