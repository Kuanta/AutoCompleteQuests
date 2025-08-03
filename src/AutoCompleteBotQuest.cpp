#include "ScriptMgr.h"
#include "Player.h"
#include "Group.h"
#include "QuestDef.h"
#include "WorldSession.h"

class AutoCompletePartyQuests : public PlayerScript
{
public:
    AutoCompletePartyQuests() : PlayerScript("AutoCompletePartyQuests") {}

    void OnQuestReward(Player *player, Quest const *quest, Object * /*questGiver*/) override
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

            // Eğer başka bir hesaba aitse, dokunma
            if (member->GetSession()->GetAccountId() != masterAccountId)
                continue;

            // Eğer quest'te ödül seçimi varsa, dokunma
            if (quest->GetRewChoiceItemsCount() > 1)
                continue;

            // Eğer quest alınmamışsa, aldır
            if (!member->GetQuestStatus(quest->GetQuestId()))
                member->AddQuest(quest, nullptr);

            // Zaten bitmişse geç
            if (member->GetQuestStatus(quest->GetQuestId()) == QUEST_STATUS_COMPLETE)
                continue;

            // Bitmemişse tamamlat
            member->CompleteQuest(quest->GetQuestId());
            member->RewardQuest(quest, false);

            TC_LOG_INFO("server", "AutoComplete: %s completed quest %u (copied from %s)",
                        member->GetName().c_str(),
                        quest->GetQuestId(),
                        player->GetName().c_str());
        }
    }
};

void Addmod_autocomplete_partyquestsScripts()
{
    new AutoCompletePartyQuests();
}
