#pragma once

class AttackCombo
{
public:
    AttackCombo();

    // ƒRƒ“ƒ{ŠJn
    void Start(int maxCount);

    // UŒ‚“ü—Í‚ğ—\–ñ
    void BufferInput();

    // Ÿ‚ÌUŒ‚‚Öi‚ß‚é‚©”»’è
    bool CanNextAttack(float animationRate, float chainRate) const;

    // Ÿ‚ÌUŒ‚‚Öi‚Ş
    void Next();

    // ƒRƒ“ƒ{“ü—Í‚Ì—\–ñ‚ğ‰ğœ
    void ClearBuffer();

    // “ü—Í—\–ñ‚ğæ“¾
    bool HasBufferedInput() const;

    // Œ»İ‚ÌUŒ‚‰ñ”‚ğæ“¾
    int GetCount() const;

private:
    int m_AttackCount;
    int m_MaxCount;
    bool m_AttackBuffered;
};