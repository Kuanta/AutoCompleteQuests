#include "ScriptMgr.h"
#include "Player.h"
#include "Group.h"
#include "QuestDef.h"

class AutoCompletePartyQuests : public PlayerScript
{
public:
    AutoCompletePartyQuests() : PlayerScript("AutoCompletePartyQuests") {}

    void OnQuestReward(Player *player, Quest const *quest, Object * /*questGiver*/) override
    {
        if (!player->GetGroup())
            return;

        Group *group = player->GetGroup();
        for (GroupReference *itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            Player *member = itr->GetSource();
            if (!member || member == player)
                continue;

            // Eğer ödül seçimi gerekiyorsa, atla
            if (quest->GetRewChoiceItemsCount() > 1)
                continue;

            // Eğer quest alınmamışsa, aldır
            if (!member->GetQuestStatus(quest->GetQuestId()))
                member->AddQuest(quest, nullptr);

            // Eğer zaten tamamlanmışsa, atla
            if (member->GetQuestStatus(quest->GetQuestId()) == QUEST_STATUS_COMPLETE)
                continue;

            // Bitmemişse, tamamlat ve ödülü ver
            member->CompleteQuest(quest->GetQuestId());
            member->RewardQuest(quest, false);
        }
    }
};

void Addmod_autocomplete_partyquestsScripts()
{
    new AutoCompletePartyQuests();
}
