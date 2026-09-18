#include <combo.h>
#include <combo/item.h>
#include <combo/inventory.h>
#include <combo/interface.h>
void EnKendoJs_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_MM_SWORDSMAN_HEART_PIECE, a, b);
}

PATCH_CALL(0x80b27730, EnKendoJs_GiveItem);

#define ENKENDOJS_SKEL_ANIME(actor) \
((SkelAnime*)((u8*)(actor) + 0x190))

#define ENKENDOJS_CUR_TEXT_ID(actor) \
(*(s16*)((u8*)(actor) + 0x288))

static s32 EnKendoJs_HasSwordCustom(void)
{
    MmSwordId sword;

    MmSword_EnsureState();
    sword = MmSword_GetEquipped();
    switch (sword)
    {
        case MM_SWORD_KOKIRI:
        case MM_SWORD_RAZOR:
        case MM_SWORD_GILDED:

        case MM_SWORD_MASTER:
        case MM_SWORD_GIANTS_KNIFE:
        case MM_SWORD_BIGGORON:
            return 1;

        default:
            return 0;
    }
}

static void EnKendoJs_StartMsgCustom(Actor* actor, PlayState* play, s16 textId)
{
    Message_StartTextbox(play, textId, actor);
    ENKENDOJS_CUR_TEXT_ID(actor) = textId;
}

#define ENKENDOJS_FIRST_CHOICE_PRICE(play) \
(*(s32*)((u8*)&(play)->msgCtx + 0x1206C))

#define ENKENDOJS_SECOND_CHOICE_PRICE(play) \
(*(s32*)((u8*)&(play)->msgCtx + 0x12070))

void EnKendoJs_HandleMessageChoicesCustom(Actor* thisx, PlayState* play)
{
    if (Message_ShouldAdvance(play) &&
        ENKENDOJS_CUR_TEXT_ID(thisx) == 0x2716)
    {
        switch (play->msgCtx.choiceIndex)
        {

            case 0:
            {
                if (!EnKendoJs_HasSwordCustom())
                {
                    Audio_PlaySfx(NA_SE_SY_ERROR);

                    EnKendoJs_StartMsgCustom(thisx, play, 0x272C);
                }
                else if (
                    gSaveContext.save.info.playerData.rupees <
                    ENKENDOJS_FIRST_CHOICE_PRICE(play))
                {
                    Audio_PlaySfx(NA_SE_SY_ERROR);
                    EnKendoJs_StartMsgCustom(thisx, play, 0x2718);
                }
                else
                {
                    Audio_PlaySfx_MessageDecide();
                    Rupees_ChangeBy(-ENKENDOJS_FIRST_CHOICE_PRICE(play));
                    EnKendoJs_StartMsgCustom(thisx, play, 0x2719);
                }
                break;
            }

            case 1:
            {
                if (!EnKendoJs_HasSwordCustom())
                {
                    Audio_PlaySfx(NA_SE_SY_ERROR);
                    EnKendoJs_StartMsgCustom(thisx, play, 0x272C);
                }
                else if (
                    gSaveContext.save.info.playerData.rupees <
                    ENKENDOJS_SECOND_CHOICE_PRICE(play))
                {
                    Audio_PlaySfx(NA_SE_SY_ERROR);
                    EnKendoJs_StartMsgCustom(thisx, play, 0x2718);
                }
                else
                {
                    Audio_PlaySfx_MessageDecide();
                    Rupees_ChangeBy(-ENKENDOJS_SECOND_CHOICE_PRICE(play));
                    EnKendoJs_StartMsgCustom(thisx, play, 0x273A);
                }
                break;
            }
            case 2:
            {
                Audio_PlaySfx_MessageCancel();
                EnKendoJs_StartMsgCustom(thisx, play, 0x2717);
                break;
            }

            default:
                break;
        }
    }
}

PATCH_FUNC(0x80B26758, EnKendoJs_HandleMessageChoicesCustom);