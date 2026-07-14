#include "CollisionOBB.h"
#include "../MyMath/MyMath.h"
#include <math.h>

// コンストラクタ
CollisionOBB::CollisionOBB()
{
    m_TargetPos = nullptr;
    m_LocalPos = VGet(0, 0, 0);
    m_Size = VGet(0, 0, 0);
    m_RotY = 0.0f;
}

// デストラクタ
CollisionOBB::~CollisionOBB()
{
}


// デバッグ描画
void CollisionOBB::Draw()
{
#ifdef _DEBUG

    VECTOR center = MyMath::VecAdd(*m_TargetPos, m_LocalPos);

    float x = m_Size.x * 0.5f;
    float y = m_Size.y * 0.5f;
    float z = m_Size.z * 0.5f;


    VECTOR vertex[8];

    // ローカル頂点
    vertex[0] = VGet(-x, -y, -z);
    vertex[1] = VGet(x, -y, -z);
    vertex[2] = VGet(x, y, -z);
    vertex[3] = VGet(-x, y, -z);

    vertex[4] = VGet(-x, -y, z);
    vertex[5] = VGet(x, -y, z);
    vertex[6] = VGet(x, y, z);
    vertex[7] = VGet(-x, y, z);


    // 回転
    float s = sinf(m_RotY);
    float c = cosf(m_RotY);


    for (int i = 0; i < 8; i++)
    {
        float nx = vertex[i].x * c - vertex[i].z * s;
        float nz = vertex[i].x * s + vertex[i].z * c;

        vertex[i].x = nx + center.x;
        vertex[i].y += center.y;
        vertex[i].z = nz + center.z;
    }


    int line[][2] =
    {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };


    for (int i = 0; i < 12; i++)
    {
        DrawLine3D(
            vertex[line[i][0]],
            vertex[line[i][1]],
            GetColor(255, 255, 0)
        );
    }

#endif
}

// OBB同士の判定
bool CollisionOBB::CheckOBB(const CollisionOBB* other) const
{

    VECTOR posA = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
    VECTOR posB = MyMath::VecAdd(other->GetTargetPos(),
        other->GetLocalPos());


    // X方向ベクトル
    VECTOR axisA[2];

    axisA[0] = VGet(cosf(m_RotY), 0, sinf(m_RotY));
    axisA[1] = VGet(-sinf(m_RotY), 0, cosf(m_RotY));


    VECTOR axisB[2];

    axisB[0] = VGet(cosf(other->GetRotation()),
        0,
        sinf(other->GetRotation()));

    axisB[1] = VGet(-sinf(other->GetRotation()),
        0,
        cosf(other->GetRotation()));


    // 中心距離
    VECTOR distance =
        MyMath::VecSub(posB, posA);



    // チェックする軸
    VECTOR axes[4];

    axes[0] = axisA[0];
    axes[1] = axisA[1];
    axes[2] = axisB[0];
    axes[3] = axisB[1];


    for (int i = 0; i < 4; i++)
    {
        VECTOR axis = axes[i];


        float len =
            sqrtf(axis.x * axis.x +
                axis.z * axis.z);


        axis.x /= len;
        axis.z /= len;


        // 中心間距離を軸に投影
        float distanceProjection =
            fabsf(distance.x * axis.x +
                distance.z * axis.z);



        // 自分側の幅
        float a =
            fabsf(m_Size.x * 0.5f *
                (axisA[0].x * axis.x +
                    axisA[0].z * axis.z))
            +
            fabsf(m_Size.z * 0.5f *
                (axisA[1].x * axis.x +
                    axisA[1].z * axis.z));


        // 相手側の幅
        float b =
            fabsf(other->GetSize().x * 0.5f *
                (axisB[0].x * axis.x +
                    axisB[0].z * axis.z))
            +
            fabsf(other->GetSize().z * 0.5f *
                (axisB[1].x * axis.x +
                    axisB[1].z * axis.z));


        // 軸上で離れている
        if (distanceProjection > a + b)
        {
            return false;
        }
    }


    // Y方向はAABBと同じ
    float topA =
        posA.y + m_Size.y * 0.5f;

    float bottomA =
        posA.y - m_Size.y * 0.5f;


    float topB =
        posB.y + other->GetSize().y * 0.5f;

    float bottomB =
        posB.y - other->GetSize().y * 0.5f;


    if (bottomA > topB ||
        topA < bottomB)
    {
        return false;
    }


    return true;
}