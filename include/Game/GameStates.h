#pragma once

class GameContext;
class GameState {
public:
    virtual ~GameState() = default;
    virtual void handleInput(GameContext& context) = 0;
    virtual void update(GameContext& context, float deltaTime) = 0;
    virtual void draw(GameContext& context) = 0;
    virtual void setLevel(GameContext& context) = 0;
};

class MenuState : public GameState {
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
    void setLevel(GameContext& context) override {
        return;
    }
};

class RedirectState : public GameState {
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
    void setLevel(GameContext& context) override {
        return;
    }
};

class PlayerSelectingState : public GameState {
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
    void setLevel(GameContext& context) override {
        return;
    }
};

class LevelRedirectState : public GameState {
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
    void setLevel(GameContext& context) override {
        return;
    }
};

class GamePlayState : public GameState {
private:
    int level = 1;
public:
    void setLevel(GameContext& context) { level = context.level; }
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
    void DrawBackGround(Texture2D&);
};

class InformationState : public GameState {
private:
    int level = 1;
    float animationTime = 0;
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
    void setLevel(GameContext& context) override {
        level = context.level;
	}
};

class CharacterSelectingState : public GameState {
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
    void setLevel(GameContext& context) override {
		return;
	}
};
 

class LevelSelectingState : public GameState {
private:
    int stateSelect = 1;
public:
    void handleInput(GameContext& context) override;
	void update(GameContext& context, float deltaTime) override;
	void draw(GameContext& context) override;
    void setLevel(GameContext& context) override {
		return;
	}

};

class EditorState : public GameState {
private:
    int stateSelect = 1;
    int mapSelect = 1;
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
    void setLevel(GameContext& context) override {
		return;
	}
};

class EditorSelectingState : public GameState {
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
    void setLevel(GameContext& context) override {
        return;
    }
};


class GameOverState : public GameState {
private:
    int level = 1;
public:
    void handleInput(GameContext& context) override;
    void update(GameContext& context, float deltaTime) override;
    void draw(GameContext& context) override;
    void setLevel(GameContext& context) override {
		return;
	}
};