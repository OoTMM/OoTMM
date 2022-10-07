#include <combo.h>

/* Skip all cutscenes and use GI */
PATCH_START 0x80a3a398
  j EnElfgrp_GiveReward
   nop
PATCH_END
