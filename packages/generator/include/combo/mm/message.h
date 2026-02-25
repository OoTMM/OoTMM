#ifndef COMBO_MM_MESSAGE_H
#define COMBO_MM_MESSAGE_H

typedef enum TextState {
    /*  0 */ TEXT_STATE_NONE,
    /*  1 */ TEXT_STATE_NEXT,
    /*  2 */ TEXT_STATE_CLOSING,
    /*  3 */ TEXT_STATE_FADING,
    /*  4 */ TEXT_STATE_CHOICE,
    /*  5 */ TEXT_STATE_EVENT,
    /*  6 */ TEXT_STATE_DONE,
    /*  7 */ TEXT_STATE_SONG_DEMO_DONE,
    /*  8 */ TEXT_STATE_8,
    /*  9 */ TEXT_STATE_9,
    /* 10 */ TEXT_STATE_AWAITING_NEXT,
    /* 11 */ TEXT_STATE_11,
    /* 12 */ TEXT_STATE_12,
    /* 13 */ TEXT_STATE_13,
    /* 14 */ TEXT_STATE_INPUT_RUPEES,
    /* 15 */ TEXT_STATE_INPUT_BOMBER_CODE,
    /* 16 */ TEXT_STATE_PAUSE_MENU,
    /* 17 */ TEXT_STATE_INPUT_LOTTERY_CODE,
    /* 18 */ TEXT_STATE_18
} TextState;

#endif
