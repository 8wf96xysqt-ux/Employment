#include "NormalEnemyTree.h"
#include "../Core/INode.h"
#include "../Composite/Selector.h"
#include "../Composite/Sequence.h"
#include "../Leaf/ReturnToBase.h"
#include "../Leaf/IsPlayerDetected.h"
#include "../Leaf/ChasePlayer.h"
#include "../Leaf/AttackPlayer.h"
#include "../Leaf/Roam.h"

INode* NormalEnemyTree::Build(BlackBoard* blackBoard)
{
    // s“®‚Ì—Dæ‡ˆÊ‚ðŠÇ—‚·‚é
    Selector* root = new Selector(blackBoard);

    // ‹’“_‚Ö–ß‚é
    root->AddNode(new ReturnToBase(blackBoard));

    // ƒvƒŒƒCƒ„[‚ðŒŸ’m‚µ‚½‚çs“®‚·‚é
    Sequence* chaseSequence = new Sequence(blackBoard);

    chaseSequence->AddNode(new IsPlayerDetected(blackBoard));

    // UŒ‚‚ð—Dæ‚µAUŒ‚‚Å‚«‚È‚¯‚ê‚Î’ÇÕ‚·‚é
    Selector* attackOrChase = new Selector(blackBoard);
    attackOrChase->AddNode(new AttackPlayer(blackBoard));
    attackOrChase->AddNode(new ChasePlayer(blackBoard));

    chaseSequence->AddNode(attackOrChase);

    root->AddNode(chaseSequence);

    // ã‹L‚ÌðŒ‚É“–‚Ä‚Í‚Ü‚ç‚È‚¢ê‡‚Íœpœj‚·‚é
    root->AddNode(new Roam(blackBoard));

    return root;
}