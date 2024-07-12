#include "PointComponent.h"

namespace dae
{
	PointComponent::PointComponent(const int initialScore): m_Score(initialScore)
	{
	}

    void PointComponent::SetScore(int score)
    {
        m_Score = score;
        Notify(); 
    }

    void PointComponent::Update()
    {
    }

   
}