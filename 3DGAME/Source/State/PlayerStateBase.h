#pragma once
#include <memory>

class Player;
class CollisionAABB;

enum class PlayerStateType
{
    IDLE,
    MOVE,
    JUMP,
    HOVER,
    FALL,
    GROUND_ATTACK,
    DASH_ATTACK,
    AIR_ATTACK,
    ROLL,
    DEAD
};

inline const char* GetPlayerStateTypeName(PlayerStateType type)
{
    switch (type)
    {
    case PlayerStateType::IDLE:
        return "IDLE";

    case PlayerStateType::MOVE:
        return "MOVE";

    case PlayerStateType::JUMP:
        return "JUMP";

    case PlayerStateType::HOVER:
        return "HOVER";

    case PlayerStateType::FALL:
        return "FALL";

    case PlayerStateType::GROUND_ATTACK:
        return "GROUND_ATTACK";

    case PlayerStateType::DASH_ATTACK:
        return "DASH_ATTACK";

    case PlayerStateType::AIR_ATTACK:
        return "AIR_ATTACK";

    case PlayerStateType::ROLL:
        return "ROLL";

    case PlayerStateType::DEAD:
        return "DEAD";

    default:
        return "UNKNOWN";
    }
}

class PlayerStateBase
{
public:
    virtual ~PlayerStateBase() = default;

    virtual void Enter(Player* player) = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;

    // 攻撃判定
    virtual bool CheckAttackHit(CollisionAABB* target)
    {
        return false;
    }

    // 状態タイプ取得
    virtual PlayerStateType GetStateType() const = 0;

    // 遷移先を取得して所有権を移す
    std::unique_ptr<PlayerStateBase> TakeNextState()
    {
        return std::move(m_pNextState);
    }

    // 次のステートを設定
    void ChangeState(std::unique_ptr<PlayerStateBase> nextState)
    {
        if (nextState.get() == this)
        {
            return;
        }

        m_pNextState = std::move(nextState);
    }

protected:
    Player* m_pPlayer = nullptr;
    std::unique_ptr<PlayerStateBase> m_pNextState;
};