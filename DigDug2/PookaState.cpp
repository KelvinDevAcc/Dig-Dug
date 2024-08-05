#include "PookaState.h"
#include "PookaComponent.h"
#include <iostream>

void PookaNormalState::Enter(PookaComponent* /*pooka*/) {
    std::cout << "Pooka entered Normal State" << std::endl;
}

void PookaNormalState::Update(PookaComponent* pooka) {
    pooka->MoveToNextWaypoint();
    std::cout << "Pooka is patrolling" << std::endl;

    if (pooka->DetectsPlayer()) {
        pooka->SetState(std::make_unique<PookaChaseState>());
    }
}

void PookaNormalState::Exit(PookaComponent* /*pooka*/) {
    std::cout << "Pooka exiting Normal State" << std::endl;
}

void PookaGhostState::Enter(PookaComponent* pooka) {
    std::cout << "Pooka entered Ghost State" << std::endl;
    pooka->EnableGhostMode();
}

void PookaGhostState::Update(PookaComponent* pooka) {
    std::cout << "Pooka is in ghost mode" << std::endl;

    if (pooka->ReachedDestination() || !pooka->CanRemainGhost()) {
        pooka->SetState(std::make_unique<PookaNormalState>());
    }
}

void PookaGhostState::Exit(PookaComponent* pooka) {
    std::cout << "Pooka exiting Ghost State" << std::endl;
    pooka->DisableGhostMode();
}

void PookaWandering::Enter(PookaComponent* pooka) {
    pooka->ChooseRandomDirection();
}

void PookaWandering::Update(PookaComponent* pooka) {
    if (pooka->ReachedDestination())
    {
        pooka->ChooseRandomDirection();
    }
    else
    {
        pooka->MoveToNextWaypoint();
        /*  if (pooka->SeesPlayer()) {
              pooka->SetState(std::make_unique<PookaChaseState>());
          }*/
    }
}

void PookaWandering::Exit(PookaComponent* /*pooka*/) {
}

void PookaChaseState::Enter(PookaComponent* /*pooka*/) {
    std::cout << "Pooka entered Chase State" << std::endl;
}

void PookaChaseState::Update(PookaComponent* pooka) {
    std::cout << "Pooka is chasing the player" << std::endl;
    pooka->ChasePlayer();

    if (!pooka->DetectsPlayer()) {
        pooka->SetState(std::make_unique<PookaWandering>());
    }
}

void PookaChaseState::Exit(PookaComponent* /*pooka*/) {
    std::cout << "Pooka exiting Chase State" << std::endl;
}
