#include "BlackBoard.h"

BlackBoard::BlackBoard()
{
}

BlackBoard::~BlackBoard()
{
}

bool BlackBoard::HasKey(const std::string& key) const
{
    // 指定したキーが登録されているか確認
    return m_Data.find(key) != m_Data.end();
}