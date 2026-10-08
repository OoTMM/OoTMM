#include <combo.h>
#include <combo/mm/sequence.h>

int Sakon_ShouldEndThiefEscape(EnSuttari* sakon, PlayState* ctxt) {
    /* Check if Sakon has dropped the luggage (with speedup) or has finished the escape sequence. */
    return (sakon->animIndex == 6);
}

void Sakon_Destroy_Hook(Actor* thisx, PlayState* play)
{
    EnSuttari* this = (EnSuttari*)thisx;
    u16 seqId = AudioSeq_GetActiveSeqId(SEQ_PLAYER_BGM_MAIN);

    if (play->sceneId == SCE_MM_CLOCK_TOWN_NORTH && (seqId & 0xff) == NA_BGM_CHASE) {
        if (play->sceneSequences.ambienceId == AMBIENCE_ID_13) {
            u8 dayMinusOne = gSaveContext.save.day - 1;

            if (dayMinusOne >= 3)
                dayMinusOne = 0;

            AudioSeq_StopSequence(SEQ_PLAYER_BGM_MAIN, 0);
            Audio_StartSceneSequence(play->sceneSequences.seqId);
            AudioSeq_QueueSeqCmd(0x70040000u | dayMinusOne);
            sRequestedSceneSeqId = play->sceneSequences.seqId;
        } else {
            AudioSeq_QueueSeqCmd(0x100014ffu);
        }
    }

    Collider_DestroyCylinder(play, &this->collider);
}