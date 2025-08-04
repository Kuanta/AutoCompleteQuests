#include "PlayerScript.h"
#include "Player.h"
#include "Group.h"
#include "QuestDef.h"
#include "WorldSession.h"
#include "Chat.h"

class AutoCompletePartyQuests : public PlayerScript
{
public:
    AutoCompletePartyQuests() : PlayerScript("AutoCompletePartyQuests") {}

    void OnPlayerCompleteQuest(Player *player, Quest const *quest) override
    {
        if (!player->GetGroup())
            return;

        // Oyuncunun hesap ID'si
        uint32 masterAccountId = player->GetSession()->GetAccountId();

        Group *group = player->GetGroup();
        for (GroupReference *itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            Player *member = itr->GetSource();

            if (!member || member == player)
                continue;

            if (!member->GetSession() || member->GetSession()->GetAccountId() != masterAccountId)
                continue;

            if (quest->GetRewChoiceItemsCount() > 1)
                continue;

            if (!member->CanTakeQuest(quest, false))
                continue;

            if (!member->GetQuestStatus(quest->GetQuestId()))
                member->AddQuest(quest, nullptr);

            if (member->GetQuestStatus(quest->GetQuestId()) == QUEST_STATUS_COMPLETE)
                continue;

            member->CompleteQuest(quest->GetQuestId());
            member->RewardQuest(quest, 0, player); // burada player object ver, en güvenlisi

            if (player && player->GetSession())
            {
                ChatHandler(player->GetSession()).PSendSysMessage("AutoComplete: %s adlı grup üyesi quest %u görevini tamamladı.", member->GetName().c_str(), quest->GetQuestId());
            }
        }
    }
};

void AddAutoCompleteQuestsScripts()
{
    new AutoCompletePartyQuests();
}
