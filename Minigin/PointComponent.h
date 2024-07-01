#pragma once
#include <list>

#include "Component.h"
#include "Subject.h"

namespace dae
{
    class PointComponent final : public Component, public Subject 
    {
    public:
        PointComponent(int initialScore);
        ~PointComponent() override = default;

        PointComponent(const PointComponent& other) = delete;
        PointComponent(PointComponent&& other) noexcept = delete;
        PointComponent& operator=(const PointComponent& other) = delete;
        PointComponent& operator=(PointComponent&& other) noexcept = delete;

        int GetScore() const { return m_Score; }
        void SetScore(int score);

        void Update() override;
        std::type_info const& GetComponentType() const override { return typeid(PointComponent); }


        // Implementations for Subject interface
        void Attach(Observer* observer) override { m_observers.push_back(observer); }
        void Detach(Observer* observer) override { m_observers.remove(observer); }
        void Notify() override
        {
            for (auto* observer : m_observers)
            {
                observer->Update();
            }
        }

    private:
        int m_Score;
        std::list<Observer*> m_observers;
    };
}