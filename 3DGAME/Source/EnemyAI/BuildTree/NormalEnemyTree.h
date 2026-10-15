#pragma once

class INode;
class BlackBoard;

class NormalEnemyTree
{
public:
    static INode* Build(BlackBoard* blackBoard);
};