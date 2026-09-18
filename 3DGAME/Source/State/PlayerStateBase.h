#pragma once

class Player;

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
    ROLL
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

    default:
        return "UNKNOWN";
    }
}

class PlayerStateBase
{
public:
    PlayerStateBase() {}
    virtual ~PlayerStateBase() {}

    virtual void Enter(Player* player) = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;

    //接地判定
    virtual bool IsGroundState() const { return false; }

    //状態タイプ取得
    virtual PlayerStateType GetStateType() const = 0;

    PlayerStateBase* GetNextState()
    {
        return m_pNextState;
    }

    void ChangeState(PlayerStateBase* nextState)
    {
        m_pNextState = nextState;
    }

protected:
    Player* m_pPlayer = nullptr;
    PlayerStateBase* m_pNextState = nullptr;
};