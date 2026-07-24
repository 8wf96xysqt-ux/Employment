#include "PlayerStateMove.h"

#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../Camera/CameraManager.h"
#include "../MyMath/MyMath.h"


void PlayerStateMove::Enter(Player* player)
{
    m_pPlayer = player;
    m_pNextState = nullptr;
}


void PlayerStateMove::Update()
{

    Player* player = m_pPlayer;


    float camYaw =
        CameraManager::GetInstance()
        ->GetCamera()
        ->GetYaw();


    VECTOR camForward =
        MyMath::VecForwardZX(camYaw);


    VECTOR camRight;

    camRight.x = camForward.z;
    camRight.y = 0.0f;
    camRight.z = -camForward.x;


    VECTOR move = VGet(0, 0, 0);


    float lx = Input::GetStickLX();
    float ly = -Input::GetStickLY();



    if (Input::IsInputKey(Input::KEY_A))
        lx -= 1.0f;

    if (Input::IsInputKey(Input::KEY_D))
        lx += 1.0f;

    if (Input::IsInputKey(Input::KEY_W))
        ly += 1.0f;

    if (Input::IsInputKey(Input::KEY_S))
        ly -= 1.0f;



    move =
        MyMath::VecAdd(
            move,
            MyMath::VecScale(camRight, lx)
        );


    move =
        MyMath::VecAdd(
            move,
            MyMath::VecScale(camForward, ly)
        );


    float length =
        MyMath::VecLong(move);



    if (length > 0.02f)
    {

        move =
            MyMath::VecNormalize(move);


        player->SetMove(
            VGet(
                move.x * 0.08f,
                player->GetMove().y,
                move.z * 0.08f
            )
        );


        player->SetTargetYaw(
            atan2f(move.x, move.z)
        );


        player->DirectionRot();

    }
    else
    {
        player->SetMove(
            VGet(
                0,
                player->GetMove().y,
                0
            )
        );


        // “ü—Í‚ª‚È‚­‚È‚Á‚½‚çIdle‚Ö
        m_pNextState = new PlayerStateIdle();

    }

}


void PlayerStateMove::Exit()
{

}