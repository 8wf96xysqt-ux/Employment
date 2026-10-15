#pragma once

class INode;
class BlackBoard;

class BehaviorTree
{
public:
    BehaviorTree();
    ~BehaviorTree();

    void Init();
    void Tick();
    void Finalize();

    void SetRootNode(INode* rootNode);

    BlackBoard* GetBlackBoard();
    const char* GetCurrentNodeName() const;

private:
    INode* m_RootNode;
    BlackBoard* m_BlackBoard;
};