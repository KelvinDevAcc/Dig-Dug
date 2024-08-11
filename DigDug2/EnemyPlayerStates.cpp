#include "EnemyPlayerStates.h"

#include <iostream>

#include "AnimationComponent.h"


namespace game
{
    void WalkingState::Enter(EnemyPlayer* enemyPlayer)
    {
        if (enemyPlayer->m_deltaX > 0) {
            enemyPlayer->m_animationComponent->Play("Walk_Right", true, 0);
            enemyPlayer->m_animationComponent->FlipSprite(true, false);
        }
        else if (enemyPlayer->m_deltaX < 0) {
            enemyPlayer->m_animationComponent->Play("Walk_Left", true, 0);
            enemyPlayer->m_animationComponent->FlipSprite(false, false);
        }
        else if (enemyPlayer->m_deltaY > 0) {
            enemyPlayer->m_animationComponent->Play("Walk_Down", true, 0);
        }
        else if (enemyPlayer->m_deltaY < 0) {
            enemyPlayer->m_animationComponent->Play("Walk_Up", true, 0);
        }
    }

    void WalkingState::Update(EnemyPlayer* enemyPlayer)
    {
       // enemyPlayer->Move(enemyPlayer->m_deltaX, enemyPlayer->m_deltaY);

        // Example condition to transition to AttackingState
        bool shouldAttack = false;  // Replace with your condition logic
        if (shouldAttack)
        {
            enemyPlayer->SetState(std::make_shared<AttackingState>());
            return;
        }

        // Example condition to transition to GhostState
        bool shouldGhost = false;  // Replace with your condition logic
        if (shouldGhost)
        {
            enemyPlayer->SetState(std::make_shared<GhostState>());
            return;
        }

        // Example condition to transition to GettingPumpedState
        bool shouldGetPumped = false;  // Replace with your condition logic
        if (shouldGetPumped)
        {
            enemyPlayer->SetState(std::make_shared<GettingPumpedState>());
            return;
        }
    }

    void WalkingState::Exit(EnemyPlayer* /*enemyPlayer*/)
    {
        // Cleanup any state-specific settings here if necessary
        std::cout << "Enemy exiting Walking State.\n";
    }

    void GhostState::Enter(EnemyPlayer* /*enemyPlayer*/)
    {
        // Set animation to ghost, initialize any variables
    }

    void GhostState::Update(EnemyPlayer* /*enemyPlayer*/)
    {
        // Ghost logic
    }

    void GhostState::Exit(EnemyPlayer* /*enemyPlayer*/)
    {
        // Cleanup or reset any variables specific to ghost state
    }
    void AttackingState::Enter(EnemyPlayer* /*enemyPlayer*/)
    {
        // Set animation to attacking, initialize any variables
    }

    void AttackingState::Update(EnemyPlayer* /*enemyPlayer*/)
    {
        // Attack logic
    }

    void AttackingState::Exit(EnemyPlayer* /*enemyPlayer*/)
    {
        // Cleanup or reset any variables specific to attacking state
    }


    void GettingPumpedState::Enter(EnemyPlayer* /*enemyPlayer*/)
    {
        // Getting pumped logic
    }

    void GettingPumpedState::Update(EnemyPlayer* /*enemyPlayer*/)
    {
        // Getting pumped logic
    }

    void GettingPumpedState::Exit(EnemyPlayer* /*enemyPlayer*/)
    {
        // Cleanup or reset any variables specific to getting pumped state
    }


    void DeadState::Enter(EnemyPlayer* /*enemyPlayer*/)
    {

    }
    void DeadState::Update(EnemyPlayer* /*enemyPlayer*/)
    {

    }
    void DeadState::Exit(EnemyPlayer* /*enemyPlayer*/)
    {

    }

}
