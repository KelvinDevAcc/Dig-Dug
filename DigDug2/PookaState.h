#pragma once
#include "PookaComponent.h"

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

