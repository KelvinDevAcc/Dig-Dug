#pragma once

class PookaComponent;

class PookaState {
public:
    virtual ~PookaState() = default;

    PookaState(PookaState&&) = delete;
    PookaState(const PookaState&) = delete;
    PookaState& operator=(PookaState&&) = delete;
    PookaState& operator=(const PookaState&) = delete;

    virtual void Enter(PookaComponent* pooka) = 0;
    virtual void Update(PookaComponent* pooka) = 0;
    virtual void Exit(PookaComponent* pooka) = 0;

protected:
    PookaState() = default;
};

class PookaNormalState final: public PookaState {
public:
    void Enter(PookaComponent* pooka) override;
    void Update(PookaComponent* pooka) override;
    void Exit(PookaComponent* pooka) override;
};


class PookaGhostState final: public PookaState {
public:
    void Enter(PookaComponent* pooka) override;
    void Update(PookaComponent* pooka) override;
    void Exit(PookaComponent* pooka) override;
};


class PookaWandering final : public PookaState {
public:
    void Enter(PookaComponent* pooka) override;
    void Update(PookaComponent* pooka) override;
    void Exit(PookaComponent* pooka) override;
};

//class PookaChaseState final : public PookaState {
//public:
//    void Enter(PookaComponent* pooka) override;
//    void Update(PookaComponent* pooka) override;
//    void Exit(PookaComponent* pooka) override;
//};

class PookaPumpedState : public PookaState {
public:
    void Enter(PookaComponent* pooka) override;
    void Update(PookaComponent* pooka) override;
    void Exit(PookaComponent* pooka) override;

private:
    float m_DeflationTimer = 0;
    float m_DeflationTimeLimit = 5.0f; // Set the deflation time limit (e.g., 5 seconds)
    int m_Pumped4Duration = 0; // Duration of "Pumped4" animation
    float m_ElapsedTime = 0.0f; // Duration of "Pumped4" animation
};

class PookaDeadState : public PookaState {
public:
    void Enter(PookaComponent* pooka) override;
    void Update(PookaComponent* pooka) override;
    void Exit(PookaComponent* pooka) override;
};

class PookaCrushedState : public PookaState {
public:
    void Enter(PookaComponent* pooka) override;
    void Update(PookaComponent* pooka) override;
    void Exit(PookaComponent* pooka) override;
private:
    int m_CrushedDuration = 0;
    float m_ElapsedTime = 0.f;
};