#pragma once
#include "Component.h"

namespace dae
{
    class EnemyAIComponent : public dae::Component
    {
    public:
        EnemyAIComponent() {}
        virtual void Update() override = 0;
        virtual std::type_info const& GetComponentType() const override { return typeid(EnemyAIComponent); }
    };
}