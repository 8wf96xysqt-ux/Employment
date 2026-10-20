#include "AttackCombo.h"

AttackCombo::AttackCombo()
    : m_AttackCount(0)
    , m_MaxCount(0)
    , m_AttackBuffered(false)
{
}

// ƒRƒ“ƒ{ŠJŽn
void AttackCombo::Start(int maxCount)
{
    m_AttackCount = 1;
    m_MaxCount = maxCount;
    m_AttackBuffered = false;
}

// UŒ‚“ü—Í‚ð—\–ñ
void AttackCombo::BufferInput()
{
    m_AttackBuffered = true;
}

// ŽŸ‚ÌUŒ‚‚Öi‚ß‚é‚©”»’è
bool AttackCombo::CanNextAttack(float animationRate, float chainRate) const
{
    return animationRate >= chainRate
        && m_AttackBuffered
        && m_AttackCount < m_MaxCount;
}

// ŽŸ‚ÌUŒ‚‚Öi‚Þ
void AttackCombo::Next()
{
    if (m_AttackCount < m_MaxCount)
    {
        m_AttackCount++;
    }

    m_AttackBuffered = false;
}

// ƒRƒ“ƒ{“ü—Í‚Ì—\–ñ‚ð‰ðœ
void AttackCombo::ClearBuffer()
{
    m_AttackBuffered = false;
}

// “ü—Í—\–ñ‚ðŽæ“¾
bool AttackCombo::HasBufferedInput() const
{
    return m_AttackBuffered;
}

// Œ»Ý‚ÌUŒ‚‰ñ”‚ðŽæ“¾
int AttackCombo::GetCount() const
{
    return m_AttackCount;
}