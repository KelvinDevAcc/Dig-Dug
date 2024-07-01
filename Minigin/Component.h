#pragma once
#include <typeinfo>

namespace dae
{
    class GameObject; 

    class Component
    {
    public:
        Component() : m_pGameObject(nullptr) {}
        virtual ~Component() = default;

        virtual void Initialize() {}
        virtual void Update() = 0; 
        virtual void Render() const {} 
        virtual const std::type_info& GetComponentType() const = 0; 

        void SetGameObject(GameObject* pGameObject) { m_pGameObject = pGameObject; }
        GameObject* GetGameObject() const { return m_pGameObject; }

    private:
        GameObject* m_pGameObject;
    };
}