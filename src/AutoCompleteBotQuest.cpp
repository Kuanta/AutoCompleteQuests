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
            member->RewardQuest(quest, 0, nullptr);

            if (player && player->GetSession())
            {
                ChatHandler(player->GetSession()).PSendSysMessage("AutoComplete: %s adlı grup üyesi quest %u görevini tamamladı.", member->GetName().c_str(), quest->GetQuestId());
            }
        }
    }
};

void Addmod_AutoCompleteQuestsScripts()
{
    new AutoCompletePartyQuests();
}
