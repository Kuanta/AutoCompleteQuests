#include "ScriptMgr.h"
#include "Player.h"
#include "Group.h"
#include "Quest.h"
#include "PlayerBotMgr.h"

class AutoCompleteBotQuest : public PlayerScript
{
public:
    AutoCompleteBotQuest() : PlayerScript("AutoCompleteBotQuest") {}

    void OnQuestReward(Player* player, Quest const* quest, Object* /*questGiver*/) override
    {
        if (!player->GetGroup())
            return;

        Group* group = player->GetGroup();
        for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            Player* member = itr->GetSource();
            if (!member || member == player)
                continue;

            if (!member->GetPlayerbotMgr() || !member->GetPlayerbotMgr()->IsBot())
                continue;

            if (!member->GetQuestStatus(quest->GetQuestId()))
                member->AddQuest(quest, nullptr);

            member->CompleteQuest(quest->GetQuestId());
            member->RewardQuest(quest, false);
        }
    }
};

void Addmod_autocomplete_bots_questScripts()
{
    new AutoCompleteBotQuest();
}
