#pragma once

class FygarComponent;

class FygarState {
public:
    virtual ~FygarState() = default;

    FygarState(FygarState&&) = delete;
    FygarState(const FygarState&) = delete;
    FygarState& operator=(FygarState&&) = delete;
    FygarState& operator=(const FygarState&) = delete;

    virtual void Enter(FygarComponent* Fygar) = 0;
    virtual void Update(FygarComponent* Fygar) = 0;
    virtual void Exit(FygarComponent* Fygar) = 0;

protected:
    FygarState() = default;
};

class FygarNormalState final: public FygarState {
public:
    void Enter(FygarComponent* Fygar) override;
    void Update(FygarComponent* Fygar) override;
    void Exit(FygarComponent* Fygar) override;
};


class FygarGhostState final: public FygarState {
public:
    void Enter(FygarComponent* Fygar) override;
    void Update(FygarComponent* Fygar) override;
    void Exit(FygarComponent* Fygar) override;
};


class FygarWandering final : public FygarState {
public:
    void Enter(FygarComponent* Fygar) override;
    void Update(FygarComponent* Fygar) override;
    void Exit(FygarComponent* Fygar) override;
};

class FygarPumpedState : public FygarState {
public:
    void Enter(FygarComponent* Fygar) override;
    void Update(FygarComponent* Fygar) override;
    void Exit(FygarComponent* Fygar) override;

private:
    float m_DeflationTimer = 0;
    float m_DeflationTimeLimit = 5.0f; // Set the deflation time limit (e.g., 5 seconds)
    int m_Pumped4Duration = 0; // Duration of "Pumped4" animation
    float m_ElapsedTime = 0.0f; // Duration of "Pumped4" animation
};

class FygarDeadState : public FygarState {
public:
    void Enter(FygarComponent* Fygar) override;
    void Update(FygarComponent* Fygar) override;
    void Exit(FygarComponent* Fygar) override;
};

class FygarCrushedState : public FygarState {
public:
    void Enter(FygarComponent* Fygar) override;
    void Update(FygarComponent* Fygar) override;
    void Exit(FygarComponent* Fygar) override;
private:
    int m_CrushedDuration = 0;
    float m_ElapsedTime = 0.f;
};