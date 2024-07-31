#include "PookaState.h"
#include "PookaComponent.h"

void PookaNormalState::Enter(PookaComponent* /*pooka*/) {
   /* pooka->SetAnimation("Normal");*/
}

void PookaNormalState::Update(PookaComponent* /*pooka*/) {
   /* pooka->MoveTowardsPlayer();

    if (pooka->ShouldEnterGhostForm()) {
        pooka->ChangeState(std::make_unique<PookaGhostState>());
    }*/
}

void PookaNormalState::Exit(PookaComponent* /*pooka*/) {
    // Cleanup if necessary
}

void PookaGhostState::Enter(PookaComponent* /*pooka*/) {
   /* pooka->SetAnimation("Ghost");*/
}

void PookaGhostState::Update(PookaComponent* /*pooka*/) {
    //pooka->MoveThroughObstacles();

    //if (pooka->ShouldExitGhostForm()) {
    //    pooka->ChangeState(std::make_unique<PookaNormalState>());
    //}
}

void PookaGhostState::Exit(PookaComponent* /*pooka*/) {
    // Cleanup if necessary
}
