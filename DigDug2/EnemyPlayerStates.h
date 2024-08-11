#pragma once
#include "EnemyPlayer.h"

namespace game
{
    class EnemyPlayer;

    class EnemyPlayerState
    {
    public:
        virtual ~EnemyPlayerState() = default;

        virtual void Enter(EnemyPlayer* enemyPlayer) = 0;
        virtual void Update(EnemyPlayer* enemyPlayer) = 0;
        virtual void Exit(EnemyPlayer* enemyPlayer) = 0;
    };

    class WalkingState : public EnemyPlayerState
    {
    public:
        void Enter(EnemyPlayer* enemyPlayer) override;
        void Update(EnemyPlayer* enemyPlayer) override;
        void Exit(EnemyPlayer* enemyPlayer) override;
    };

    class GhostState : public EnemyPlayerState
    {
    public:
        void Enter(EnemyPlayer* enemyPlayer) override;
        void Update(EnemyPlayer* enemyPlayer) override;
        void Exit(EnemyPlayer* enemyPlayer) override;
    };

    class AttackingState : public EnemyPlayerState
    {
    public:
        void Enter(EnemyPlayer* enemyPlayer) override;
        void Update(EnemyPlayer* enemyPlayer) override;
        void Exit(EnemyPlayer* enemyPlayer) override;
    };

    class GettingPumpedState : public EnemyPlayerState
    {
    public:
        void Enter(EnemyPlayer* enemyPlayer) override;
        void Update(EnemyPlayer* enemyPlayer) override;
        void Exit(EnemyPlayer* enemyPlayer) override;
    };

    class DeadState : public EnemyPlayerState {
    public:
        void Enter(EnemyPlayer* enemyPlayer) override;
        void Update(EnemyPlayer* enemyPlayer) override;
        void Exit(EnemyPlayer* enemyPlayer) override;
    };
}
