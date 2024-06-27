#include <combo.h>
#include <combo/item.h>

/* TODO: Double check this for repeats */
void EnKitan_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_MM_KEATON_HEART_PIECE, a, b);
    
    // Stop Keaton Quiz Music after giving the item, with a fade out of 128 frames  
    AudioSeq_QueueSeqCmd((1 << 28) | 0xFF | ((u8)(1) << 24) | ((u8)(128) << 16));
    // These casts are unnecessary besides specifying the type in case we reuse or expand this into a macro
}

PATCH_CALL(0x80c096ec, EnKitan_GiveItem);
