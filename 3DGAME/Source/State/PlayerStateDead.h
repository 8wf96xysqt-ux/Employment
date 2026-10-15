    #pragma once
    #include "PlayerStateBase.h"

    class PlayerStateDead : public PlayerStateBase
    {
    public:
        PlayerStateDead();
        ~PlayerStateDead() {}

        void Enter(Player* player) override;
        void Update() override;
        void Exit() override;

        PlayerStateType GetStateType() const override
        {
            return PlayerStateType::DEAD;
        }

    private:
        bool m_IsRespawned;
    };