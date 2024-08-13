#pragma once

class EnemyComponent;

class EnemyState {
public:
    virtual ~EnemyState() = default;

    EnemyState(EnemyState&&) = delete;
    EnemyState(const EnemyState&) = delete;
    EnemyState& operator=(EnemyState&&) = delete;
    EnemyState& operator=(const EnemyState&) = delete;

    virtual void Enter(EnemyComponent* enemy) = 0;
    virtual void Update(EnemyComponent* enemy) = 0;
    virtual void Exit(EnemyComponent* enemy) = 0;

protected:
    EnemyState() = default;
};

class EnemyGhostState final: public EnemyState {
public:
    void Enter(EnemyComponent* enemy) override;
    void Update(EnemyComponent* enemy) override;
    void Exit(EnemyComponent* enemy) override;
};


class EnemyWandering final : public EnemyState {
public:
    void Enter(EnemyComponent* enemy) override;
    void Update(EnemyComponent* enemy) override;
    void Exit(EnemyComponent* enemy) override;
};


class EnemyPumpedState : public EnemyState {
public:
    void Enter(EnemyComponent* enemy) override;
    void Update(EnemyComponent* enemy) override;
    void Exit(EnemyComponent* enemy) override;

private:
    float m_DeflationTimer = 0;
    float m_DeflationTimeLimit = 5.0f;
    int m_Pumped4Duration = 0;
    float m_ElapsedTime = 0.0f;
};

class EnemyDeadState : public EnemyState {
public:
    void Enter(EnemyComponent* enemy) override;
    void Update(EnemyComponent* enemy) override;
    void Exit(EnemyComponent* enemy) override;
};

class EnemyCrushedState : public EnemyState {
public:
    void Enter(EnemyComponent* enemy) override;
    void Update(EnemyComponent* enemy) override;
    void Exit(EnemyComponent* enemy) override;
private:
    int m_CrushedDuration = 0;
    float m_ElapsedTime = 0.f;
};

class EnemyBreathingFireState : public EnemyState {
public:
    void Enter(EnemyComponent* enemy) override;
    void Update(EnemyComponent* enemy) override;
    void Exit(EnemyComponent* enemy) override;

private:
    float m_FireAnimationDuration = 0.f; // Duration for how long the fire-breathing lasts
    float m_ElapsedTime = 0.0f;
    bool m_FireBreathTriggered = false;
};