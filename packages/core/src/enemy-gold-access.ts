export const ENEMY_GOLD_ACCESS: Readonly<Record<string, readonly { expectedRule: string; access: string }[]>> = {
  'mm/Ocean Skulltula Entrance Right Wall': [
    { 'expectedRule': 'gs && can_hookshot_short', 'access': 'gs && can_hookshot_short' },
  ],
  'mm/Ocean Skulltula Entrance Left Wall': [
    { 'expectedRule': 'gs && can_hookshot_short', 'access': 'gs && can_hookshot_short' },
  ],
  'mm/Ocean Skulltula Entrance Web': [
    { 'expectedRule': 'gs && (can_hookshot_short || (can_use_fire_arrows && (has_boomerang || has_mask_zora)))', 'access': 'gs && (can_hookshot_short || (can_use_fire_arrows && (has_boomerang || has_mask_zora)))' },
  ],
  'mm/Ocean Skulltula 2nd Room Ceiling Edge': [
    { 'expectedRule': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora)', 'access': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora)' },
  ],
  'mm/Ocean Skulltula 2nd Room Ceiling Plank': [
    { 'expectedRule': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora)', 'access': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora)' },
  ],
  'mm/Ocean Skulltula 2nd Room Jar': [
    { 'expectedRule': 'gs', 'access': '$RULE' },
  ],
  'mm/Ocean Skulltula 2nd Room Webbed Hole': [
    { 'expectedRule': 'gs && has_fire_sticks && can_hookshot_short', 'access': 'gs && has_fire_sticks && can_hookshot_short' },
  ],
  'mm/Ocean Skulltula 2nd Room Behind Skull 1': [
    { 'expectedRule': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora)', 'access': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora)' },
  ],
  'mm/Ocean Skulltula 2nd Room Behind Skull 2': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Ocean Skulltula 2nd Room Webbed Pot': [
    { 'expectedRule': 'gs && has_fire_sticks', 'access': 'gs && has_fire_sticks' },
  ],
  'mm/Ocean Skulltula 2nd Room Upper Pot': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Ocean Skulltula 2nd Room Lower Pot': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Ocean Skulltula Library Hole Behind Picture': [
    { 'expectedRule': 'gs && (has_arrows || has_sticks) && (can_hookshot || short_hook_anywhere)', 'access': 'gs && (has_arrows || has_sticks) && (can_hookshot || short_hook_anywhere)' },
  ],
  'mm/Ocean Skulltula Library Hole Behind Cabinet': [
    { 'expectedRule': 'gs && (has_arrows || has_sticks) && can_hookshot_short', 'access': 'gs && (has_arrows || has_sticks) && can_hookshot_short' },
  ],
  'mm/Ocean Skulltula Library On Corner Bookshelf': [
    { 'expectedRule': 'gs && (has_arrows || has_sticks)', 'access': 'gs && (has_arrows || has_sticks)' },
  ],
  'mm/Ocean Skulltula Library Behind Picture': [
    { 'expectedRule': 'gs && (has_arrows || has_sticks) && (can_hookshot_short || has_arrows || has_boomerang || has_mask_zora || can_use_deku_bubble)', 'access': 'gs && (has_arrows || has_sticks)' },
  ],
  'mm/Ocean Skulltula Library Behind Bookcase 1': [
    { 'expectedRule': 'gs && (has_arrows || has_sticks)', 'access': 'gs && (has_arrows || has_sticks)' },
  ],
  'mm/Ocean Skulltula Library Behind Bookcase 2': [
    { 'expectedRule': 'gs && (has_arrows || has_sticks)', 'access': 'gs && (has_arrows || has_sticks)' },
  ],
  'mm/Ocean Skulltula Library Ceiling Edge': [
    { 'expectedRule': 'gs && (has_arrows || has_sticks) && (can_hookshot_short || has_boomerang || has_mask_zora)', 'access': 'gs && (has_arrows || has_sticks) && (can_hookshot_short || has_boomerang || has_mask_zora)' },
  ],
  'mm/Ocean Skulltula Colored Skulls Chandelier 1': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Ocean Skulltula Colored Skulls Chandelier 2': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Ocean Skulltula Colored Skulls Chandelier 3': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Ocean Skulltula Colored Skulls Behind Picture': [
    { 'expectedRule': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora || (has_mask_goron && (has_arrows || can_use_deku_bubble)))', 'access': '$RULE' },
  ],
  'mm/Ocean Skulltula Colored Skulls Pot': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Ocean Skulltula Colored Skulls Ceiling Edge': [
    { 'expectedRule': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora)', 'access': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora)' },
  ],
  'mm/Ocean Skulltula Storage Room Behind Boat': [
    { 'expectedRule': 'gs && has_fire_sticks', 'access': 'gs && has_fire_sticks' },
  ],
  'mm/Ocean Skulltula Storage Room Ceiling Web': [
    { 'expectedRule': 'gs && can_use_fire_short_range && (can_hookshot_short || has_boomerang || has_mask_zora)', 'access': 'gs && can_use_fire_short_range && (can_hookshot_short || has_boomerang || has_mask_zora)' },
  ],
  'mm/Ocean Skulltula Storage Room Behind Crate': [
    {
      'expectedRule': 'gs && has_fire_sticks && is_short && (can_hookshot_short || has_boomerang || has_mask_zora || (has_mask_goron && (has_arrows || can_use_deku_bubble || has_explosives || trick_keg_explosives)))',
      'access': 'gs && has_fire_sticks && is_short && (can_hookshot_short || has_boomerang || has_mask_zora || (has_mask_goron && (has_arrows || can_use_deku_bubble || has_explosives || trick_keg_explosives)))',
    },
  ],
  'mm/Ocean Skulltula Storage Room Crate': [
    { 'expectedRule': 'gs && has_fire_sticks', 'access': '$RULE' },
  ],
  'mm/Ocean Skulltula Storage Room Jar': [
    { 'expectedRule': 'gs && has_fire_sticks && can_hookshot_short', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Main Room Near Ceiling': [
    { 'expectedRule': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora || (has_mask_deku && (has_arrows || has_magic || (has_bombs || has_bombchu || trick_keg_explosives))))', 'access': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora || has_mask_deku)' },
  ],
  'mm/Swamp Skulltula Main Room Lower Right Soft Soil': [
    { 'expectedRule': 'gs && has_bugs', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Main Room Lower Left Soft Soil': [
    { 'expectedRule': 'gs && has_bugs', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Main Room Upper Soft Soil': [
    { 'expectedRule': 'gs && has_bugs', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Main Room Upper Pillar': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Main Room Pillar': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Main Room Water': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Main Room Jar': [
    { 'expectedRule': 'gs', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Gold Room Hive': [
    { 'expectedRule': 'gs && (has_weapon_range || has_bombchu)', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Gold Room Near Ceiling': [
    { 'expectedRule': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora || can_use_beans)', 'access': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora || can_use_beans)' },
  ],
  'mm/Swamp Skulltula Gold Room Pillar': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Gold Room Wall': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Tree Room Hive': [
    { 'expectedRule': 'gs && (has_weapon_range || has_bombchu)', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Tree Room Grass 1': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Tree Room Grass 2': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Tree Room Tree 1': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Tree Room Tree 2': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Tree Room Tree 3': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Monument Room Lower Wall': [
    {
      'expectedRule': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora || (can_use_beans && can_break_boulders) || (has_hover_boots && (has_weapon_range || has_explosives || has_weapon)))',
      'access': 'gs && (can_hookshot_short || has_boomerang || has_mask_zora || (can_use_beans && can_break_boulders) || has_hover_boots)',
    },
  ],
  'mm/Swamp Skulltula Monument Room On Monument': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Monument Room Crate 1': [
    { 'expectedRule': 'gs', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Monument Room Crate 2': [
    { 'expectedRule': 'gs', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Monument Room Torch': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Pot Room Hive 1': [
    { 'expectedRule': 'gs && (has_weapon_range || has_bombchu)', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Pot Room Hive 2': [
    { 'expectedRule': 'gs && (has_weapon_range || has_bombchu)', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Pot Room Behind Vines': [
    { 'expectedRule': 'gs && has_sword', 'access': 'gs && has_sword' },
  ],
  'mm/Swamp Skulltula Pot Room Pot 1': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Pot Room Pot 2': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'mm/Swamp Skulltula Pot Room Jar': [
    { 'expectedRule': 'gs', 'access': '$RULE' },
  ],
  'mm/Swamp Skulltula Pot Room Wall': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'oot/Bottom of the Well GS East Cage': [
    {
      'expectedRule': 'gs && (small_keys_botw(3) || climb_anywhere) && has_lens && (can_collect_distance || (climb_anywhere && (can_use_sword_master || has_ranged_weapon || has_explosives || can_use_sticks || can_use_din || (age_sword_adult && has(SWORD_BIGGORON)))))',
      'access': '(gs && (small_keys_botw(3) || climb_anywhere) && has_lens) && (can_collect_distance || climb_anywhere)',
    },
  ],
  'oot/Bottom of the Well GS Inner West': [
    {
      'expectedRule': 'gs && small_keys_botw(3) && has_lens && (can_collect_distance || (climb_anywhere && (can_use_sword_master || has_ranged_weapon || has_explosives || can_use_sticks || can_use_din || (age_sword_adult && has(SWORD_BIGGORON)))))',
      'access': '(gs && small_keys_botw(3) && has_lens) && (can_collect_distance || climb_anywhere)',
    },
  ],
  'oot/Bottom of the Well GS Inner East': [
    {
      'expectedRule': 'gs && (small_keys_botw(3) || climb_anywhere) && has_lens && (can_collect_distance || (climb_anywhere && (can_use_sword_master || has_ranged_weapon || has_explosives || can_use_sticks || can_use_din || (age_sword_adult && has(SWORD_BIGGORON)))))',
      'access': '(gs && (small_keys_botw(3) || climb_anywhere) && has_lens) && (can_collect_distance || climb_anywhere)',
    },
  ],
  'oot/Deku Tree GS Compass': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/Deku Tree GS Basement Gate': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/Deku Tree GS Basement Vines': [
    { 'expectedRule': 'gs && (has_ranged_weapon || can_use_din || has_explosives)', 'access': 'gs' },
  ],
  'oot/Deku Tree GS Basement Back Room': [
    {
      'expectedRule': 'gs && event(DEKU_MUD_WALL) && event(DEKU_BURN_WEB) && (can_collect_distance || (climb_anywhere && (has_explosives || has_ranged_weapon || can_use_din || (age_sword_adult && has(SWORD_BIGGORON)))))',
      'access': '(gs && event(DEKU_MUD_WALL) && event(DEKU_BURN_WEB)) && (can_collect_distance || climb_anywhere)',
    },
  ],
  'oot/Dodongo Cavern GS Scarecrow': [
    {
      'expectedRule': 'gs && ((is_adult && scarecrow_hookshot) || (climb_anywhere && can_damage_skull) || hookshot_anywhere)',
      'access': 'gs && ((is_adult && scarecrow_hookshot) || climb_anywhere || hookshot_anywhere)',
    },
  ],
  'oot/Dodongo Cavern GS Side Room': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/Dodongo Cavern GS Stairs Vines': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'oot/Dodongo Cavern GS Stairs Top': [
    {
      'expectedRule': 'gs && ((can_collect_distance && event(DC_SHORTCUT)) || hookshot_anywhere || (climb_anywhere && (has_explosives || has_ranged_weapon || can_use_din)))',
      'access': 'gs && ((can_collect_distance && event(DC_SHORTCUT)) || hookshot_anywhere || climb_anywhere)',
    },
  ],
  'oot/Dodongo Cavern GS Near Boss': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/Fire Temple GS Hammer Statues': [
    { 'expectedRule': 'gs && can_clear_flying_floor_tiles', 'access': '$RULE' },
  ],
  'oot/Fire Temple GS Lava Side Room': [
    {
      'expectedRule': 'gs && has_tunic_goron && (climb_anywhere || hookshot_anywhere || (is_adult && can_play_time)) && can_damage_skull',
      'access': 'gs && has_tunic_goron && (climb_anywhere || hookshot_anywhere || (is_adult && can_play_time))',
    },
  ],
  'oot/Fire Temple GS Maze': [
    { 'expectedRule': 'gs && has_explosives && (climb_anywhere || can_collect_distance)', 'access': '(gs && has_explosives) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Fire Temple GS Scarecrow Wall': [
    { 'expectedRule': 'gs && (can_use_sword || has_ranged_weapon || has_explosives || can_use_sticks || can_use_din)', 'access': 'gs' },
  ],
  'oot/Fire Temple GS Scarecrow Top': [
    { 'expectedRule': 'gs && (can_hookshot || (climb_anywhere && (has_ranged_weapon || has_explosives || can_use_din)))', 'access': '(gs) && (can_hookshot || climb_anywhere)' },
  ],
  'oot/Forest Temple GS Entrance': [
    { 'expectedRule': 'gs && (has_ranged_weapon || has_explosives || can_use_din)', 'access': 'gs' },
  ],
  'oot/Forest Temple GS Main': [
    { 'expectedRule': 'gs && (can_collect_distance || (climb_anywhere && (has_ranged_weapon || has_explosives || can_use_din)))', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Forest Temple GS Garden West': [
    { 'expectedRule': 'gs && ((can_longshot || (climb_anywhere && has_bombchu)) || (event(FOREST_LEDGE_REACHED) && (can_collect_distance || (climb_anywhere && has_ranged_weapon))))', 'access': 'gs && (can_longshot || climb_anywhere || (event(FOREST_LEDGE_REACHED) && can_collect_distance))' },
  ],
  'oot/Forest Temple GS Garden East': [
    {
      'expectedRule': 'gs && (can_hookshot || (climb_anywhere && (has_ranged_weapon || can_use_sword_master || has_explosives || can_use_din || (age_sword_adult && has(SWORD_BIGGORON)) || can_use_sticks || (can_use_sword_kokiri && has_hover_boots))))',
      'access': 'gs && (can_hookshot || climb_anywhere)',
    },
    {
      'expectedRule': 'gs && (can_collect_distance || (climb_anywhere && (has_ranged_weapon || can_use_sword_master || has_explosives || can_use_din || (age_sword_adult && has(SWORD_BIGGORON)) || can_use_sticks || (can_use_sword_kokiri && has_hover_boots))))',
      'access': 'gs && (can_collect_distance || climb_anywhere)',
    },
  ],
  'oot/Forest Temple GS Antichamber': [
    { 'expectedRule': 'gs && (can_collect_distance || climb_anywhere)', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Ice Cavern GS Scythe Room': [
    { 'expectedRule': 'gs && (can_collect_distance || (climb_anywhere && (can_use_sword || can_use_sticks || has_ranged_weapon || has_explosives || can_use_din)))', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Ice Cavern GS HP Room': [
    { 'expectedRule': 'gs && (can_collect_distance || (climb_anywhere && (can_use_sword || can_use_sticks || has_ranged_weapon || has_explosives || can_use_din)))', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Ice Cavern GS Block Room': [
    { 'expectedRule': 'gs && (can_collect_distance || (climb_anywhere && (has_ranged_weapon || has_explosives || can_use_din)))', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Jabu-Jabu GS Bottom Lower': [
    { 'expectedRule': 'gs && (can_collect_distance || climb_anywhere)', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Jabu-Jabu GS Bottom Upper': [
    { 'expectedRule': 'gs && (can_collect_distance || climb_anywhere)', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Jabu-Jabu GS Water Switch': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'oot/Jabu-Jabu GS Near Boss': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'oot/Shadow Temple GS Invisible Scythe': [
    { 'expectedRule': 'gs && event(SHADOW_INVISIBLE_SCYTHE_GATE) && (can_collect_distance || climb_anywhere)', 'access': '(gs && event(SHADOW_INVISIBLE_SCYTHE_GATE)) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Shadow Temple GS Falling Spikes': [
    { 'expectedRule': 'gs && (can_collect_distance || (climb_anywhere && can_damage_skull))', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Shadow Temple GS Skull Pot': [
    { 'expectedRule': 'gs && (can_damage_skull || has_goron_bracelet)', 'access': 'gs' },
  ],
  'oot/Shadow Temple GS Near Boat': [
    { 'expectedRule': 'gs && (can_longshot || hookshot_anywhere || (climb_anywhere && (can_use_sword || has_ranged_weapon || has_explosives || can_use_din)))', 'access': '(gs) && (can_longshot || hookshot_anywhere || climb_anywhere)' },
  ],
  'oot/Shadow Temple GS Triple Skull Pot': [
    { 'expectedRule': 'gs && (can_collect_distance || (climb_anywhere && (has_ranged_weapon || can_use_sword || can_use_sticks || can_use_din || has_bombflowers)))', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Spirit Temple GS Child Fence': [
    { 'expectedRule': 'gs && (can_collect_distance || (climb_anywhere && (can_use_din || has_ranged_weapon || has_explosives)))', 'access': 'gs && (can_collect_distance || climb_anywhere)' },
    {
      'expectedRule': 'gs && ((has_ranged_weapon || has_explosives || can_use_din || can_use_sticks || can_use_sword_master || (age_sword_adult && has(SWORD_BIGGORON))) && silver_rupees_spirit_child)',
      'access': 'gs && silver_rupees_spirit_child',
    },
  ],
  'oot/Spirit Temple GS Child Climb': [
    { 'expectedRule': 'gs && (can_use_sword || has_ranged_weapon || has_explosives || can_use_sticks || can_use_din || (has_hover_boots && can_hammer))', 'access': 'gs' },
  ],
  'oot/Spirit Temple GS Iron Knuckle': [
    {
      'expectedRule': 'gs && ((event(SPIRIT_CHILD_DOOR) && has_explosives && can_collect_distance) || (event(SPIRIT_ADULT_DOOR) && can_collect_distance) || cond(!setting(climbMostSurfacesOot, off), ((has_explosives && can_reflect_light && small_keys_spirit(1)) || small_keys_spirit(3)) && (can_collect_distance || has_ranged_weapon || has_explosives || can_use_din), can_collect_ageless && (has_explosives || small_keys_spirit(2))))',
      'access': 'gs && ((event(SPIRIT_CHILD_DOOR) && has_explosives && can_collect_distance) || (event(SPIRIT_ADULT_DOOR) && can_collect_distance) || cond(!setting(climbMostSurfacesOot, off), ((has_explosives && can_reflect_light && small_keys_spirit(1)) || small_keys_spirit(3)), can_collect_ageless && (has_explosives || small_keys_spirit(2))))',
    },
  ],
  'oot/Spirit Temple GS Boulders': [
    { 'expectedRule': 'gs && can_play_time', 'access': 'gs && can_play_time' },
  ],
  'oot/Spirit Temple GS Statue': [
    {
      'expectedRule': 'gs && ((event(SPIRIT_ADULT_DOOR) && ((is_adult && scarecrow_hookshot) || has_hover_boots || can_longshot || climb_anywhere)) || (event(SPIRIT_CHILD_DOOR) && has_explosives && (has_hover_boots || can_hookshot || climb_anywhere)) || (climb_anywhere && can_damage_skull))',
      'access': 'gs && ((event(SPIRIT_ADULT_DOOR) && ((is_adult && scarecrow_hookshot) || has_hover_boots || can_longshot || climb_anywhere)) || (event(SPIRIT_CHILD_DOOR) && has_explosives && (has_hover_boots || can_hookshot || climb_anywhere)) || climb_anywhere)',
    },
  ],
  'oot/Water Temple GS Center': [
    { 'expectedRule': 'gs && (can_longshot || hookshot_anywhere || (climb_anywhere && (has_ranged_weapon || has_bombchu)))', 'access': '(gs) && (can_longshot || hookshot_anywhere || climb_anywhere)' },
  ],
  'oot/Water Temple GS Waterfalls': [
    {
      'expectedRule': 'gs && (can_collect_distance || ((can_use_sword_gilded || can_use_sword_master || (age_sword_adult && has(SWORD_BIGGORON)) || can_use_sticks || has_ranged_weapon || can_use_din || (has_hover_boots && (can_use_sword_kokiri || can_use_sword_goron || has_explosives))) && (has_hover_boots || climb_anywhere)))',
      'access': 'gs && (can_collect_distance || has_hover_boots || climb_anywhere)',
    },
  ],
  'oot/Water Temple GS Large Pit': [
    {
      'expectedRule': 'gs && (can_longshot || (can_hit_triggers_distance && climb_anywhere) || (climb_anywhere && has_bombchu && trick(OOT_WATER_PIT_GS_CHU)))',
      'access': 'gs && (can_longshot || (can_hit_triggers_distance && climb_anywhere) || (climb_anywhere && has_bombchu && trick(OOT_WATER_PIT_GS_CHU)))',
    },
  ],
  'oot/Water Temple GS River': [
    { 'expectedRule': 'gs && has_iron_boots', 'access': 'gs && has_iron_boots' },
  ],
  'oot/Water Temple GS Cage': [
    { 'expectedRule': 'gs && can_use_sword_or_sticks', 'access': 'gs && can_use_sword_or_sticks' },
  ],
  'oot/MQ Bottom of the Well GS Basement': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/MQ Bottom of the Well GS West Middle Room': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/MQ Bottom of the Well GS Coffin Room': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/MQ Deku Tree GS Lobby Crate': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/MQ Deku Tree GS Compass Room': [
    { 'expectedRule': 'gs && can_collect_distance', 'access': 'gs && can_collect_distance' },
  ],
  'oot/MQ Deku Tree GS Song of Time Blocks': [
    { 'expectedRule': 'gs && ((can_play_time && can_collect_distance) || can_longshot)', 'access': 'gs && ((can_play_time && can_collect_distance) || can_longshot)' },
  ],
  'oot/MQ Deku Tree GS Back Room': [
    { 'expectedRule': 'gs && can_collect_distance', 'access': 'gs && can_collect_distance' },
  ],
  'oot/MQ Dodongo Cavern GS Time Blocks': [
    { 'expectedRule': 'gs && can_play_time && can_damage_skull', 'access': 'gs && can_play_time' },
  ],
  'oot/MQ Dodongo Cavern GS Larve Room': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/MQ Dodongo Cavern GS Upper Lizalfos': [
    { 'expectedRule': 'gs && (has_explosives_or_hammer || (climb_anywhere && can_damage_skull) || longshot_anywhere)', 'access': 'gs && (has_explosives_or_hammer || climb_anywhere || longshot_anywhere)' },
    { 'expectedRule': 'gs && (longshot_anywhere || (climb_anywhere && can_damage_skull))', 'access': 'gs && (climb_anywhere || longshot_anywhere)' },
  ],
  'oot/MQ Dodongo Cavern GS Poe Room Side': [
    { 'expectedRule': 'gs && can_collect_distance', 'access': 'gs && can_collect_distance' },
  ],
  'oot/MQ Dodongo Cavern GS Near Boss': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/MQ Fire Temple GS 1f Lava Room': [
    { 'expectedRule': 'gs && can_hammer', 'access': 'gs && can_hammer' },
  ],
  'oot/MQ Fire Temple GS Burning Block': [
    {
      'expectedRule': 'gs && ((event(FIRE_MQ_EAST_TOWER_TOP_HOOKSHOT_TARGETS) && can_hookshot) || hookshot_anywhere || climb_anywhere)',
      'access': 'gs && ((event(FIRE_MQ_EAST_TOWER_TOP_HOOKSHOT_TARGETS) && can_hookshot) || hookshot_anywhere || climb_anywhere)',
    },
  ],
  'oot/MQ Fire Temple GS Fire Walls Side Room': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/MQ Fire Temple GS Fire Walls Middle': [
    { 'expectedRule': 'gs && has_explosives', 'access': 'gs && has_explosives' },
  ],
  'oot/MQ Fire Temple GS Topmost': [
    { 'expectedRule': 'gs && (can_collect_distance || ((has_explosives || can_use_mask_blast || has_weapon || has_ranged_weapon || has_sticks) && climb_anywhere))', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/MQ Forest Temple GS Entryway': [
    { 'expectedRule': 'gs && (can_collect_distance || ((has_ranged_weapon || has_explosives) && climb_anywhere))', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/MQ Forest Temple GS Climb Room': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/MQ Forest Temple GS West Garden': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/MQ Forest Temple GS Well': [
    { 'expectedRule': 'gs && (can_hookshot || (event(FOREST_MQ_DRAIN_WELL) && can_damage_skull))', 'access': 'gs && (can_hookshot || event(FOREST_MQ_DRAIN_WELL))' },
  ],
  'oot/MQ Forest Temple GS East Garden': [
    {
      'expectedRule': 'gs && (has_explosives || has_ranged_weapon || can_use_din || ((can_use_sword_kokiri || can_use_sword_master || can_use_sword_biggoron || can_use_sticks) && has_one_hand_shield) || ((is_child || has_shield || climb_anywhere) && has_mask_blast))',
      'access': 'gs',
    },
    { 'expectedRule': 'gs && can_collect_distance', 'access': 'gs && can_collect_distance' },
  ],
  'oot/MQ Ice Cavern GS Compass Room': [
    {
      'expectedRule': 'gs && ((can_play_time && has_blue_fire && can_damage_skull) || has_blue_fire_arrows || can_boomerang)',
      'access': 'gs && ((can_play_time && has_blue_fire) || has_blue_fire_arrows || can_boomerang)',
    },
  ],
  'oot/MQ Ice Cavern GS Clear Blocks': [
    { 'expectedRule': 'gs && event(ICE_MQ_SECOND_CRYSTAL) && can_damage_skull', 'access': 'gs && event(ICE_MQ_SECOND_CRYSTAL)' },
  ],
  'oot/MQ Ice Cavern GS Scarecrow': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/MQ Jabu-Jabu GS SoT Block': [
    { 'expectedRule': 'gs && can_play_time && can_damage_skull', 'access': 'gs && can_play_time' },
  ],
  'oot/MQ Jabu-Jabu GS Back': [
    {
      'expectedRule': 'gs && ((climb_anywhere && (can_hammer || can_use_mask_blast)) || (has_explosives && can_collect_distance))',
      'access': 'gs && ((climb_anywhere && (can_hammer || can_use_mask_blast)) || (has_explosives && can_collect_distance))',
    },
  ],
  'oot/MQ Jabu-Jabu GS Basement Side Room': [
    { 'expectedRule': 'gs && has_fire_arrows && can_longshot', 'access': 'gs && has_fire_arrows && can_longshot' },
    { 'expectedRule': 'gs && (can_collect_distance || (climb_anywhere && can_damage_skull))', 'access': 'gs && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/MQ Jabu-Jabu GS Pre-Boss': [
    { 'expectedRule': 'gs && can_collect_distance', 'access': 'gs && can_collect_distance' },
  ],
  'oot/MQ Shadow Temple GS Spike Curtain': [
    { 'expectedRule': 'gs && collect_gs_on_walls', 'access': 'gs && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/MQ Shadow Temple GS Wind Hint': [
    { 'expectedRule': 'gs && collect_gs_on_walls', 'access': 'gs && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/MQ Shadow Temple GS After Wind Bomb': [
    { 'expectedRule': 'gs && has_explosives', 'access': 'gs && has_explosives' },
  ],
  'oot/MQ Shadow Temple GS After Boat': [
    { 'expectedRule': 'gs && (can_hookshot || can_use_bow || can_use_slingshot || can_use_din || has_explosives || (can_boomerang && event(SHADOW_MQ_AFTER_BOAT_BRIDGE_FALL)))', 'access': 'gs && ((event(SHADOW_MQ_AFTER_BOAT_BRIDGE_FALL) && (can_boomerang || can_hookshot)) || can_longshot)' },
    { 'expectedRule': 'gs && ((event(SHADOW_MQ_AFTER_BOAT_BRIDGE_FALL) && (can_boomerang || can_hookshot)) || can_longshot)', 'access': 'gs && ((event(SHADOW_MQ_AFTER_BOAT_BRIDGE_FALL) && (can_boomerang || can_hookshot)) || can_longshot)' },
  ],
  'oot/MQ Shadow Temple GS Pre-Boss': [
    {
      'expectedRule': 'gs && has_lens && (hookshot_anywhere || glitch_megaflip || ((climb_anywhere || has_hover_boots) && (has_ranged_weapon || has_explosives || can_use_din)))',
      'access': 'gs && has_lens && (hookshot_anywhere || glitch_megaflip || climb_anywhere || has_hover_boots)',
    },
  ],
  'oot/MQ Spirit Temple GS Sun Block Room': [
    { 'expectedRule': 'gs && is_adult', 'access': 'gs && is_adult' },
  ],
  'oot/MQ Spirit Temple GS Leever Room': [
    { 'expectedRule': 'gs && can_collect_distance', 'access': 'gs && can_collect_distance' },
  ],
  'oot/MQ Spirit Temple GS Symphony Room': [
    {
      'expectedRule': 'gs && can_collect_distance && small_keys_spirit(7) && can_hammer && can_play_time && can_play_epona && can_play_sun && can_play_storms && can_play_zelda',
      'access': '(gs && small_keys_spirit(7) && can_hammer && can_play_time && can_play_epona && can_play_sun && can_play_storms && can_play_zelda) && (can_collect_distance)',
    },
  ],
  'oot/MQ Spirit Temple GS Top Floor Left Wall': [
    { 'expectedRule': 'gs && small_keys_spirit(7) && soul_iron_knuckle', 'access': '$RULE' },
  ],
  'oot/MQ Spirit Temple GS Top Floor Back Wall': [
    { 'expectedRule': 'gs && small_keys_spirit(7) && soul_iron_knuckle', 'access': '$RULE' },
  ],
  'oot/MQ Water Temple GS River': [
    { 'expectedRule': 'gs && can_collect_distance', 'access': 'gs && can_collect_distance' },
  ],
  'oot/MQ Water Temple GS Three Torch': [
    {
      'expectedRule': 'gs && event(WATER_MQ_THREE_TORCH_ROOM_GATE) && (scarecrow_hookshot || has_hover_boots || hookshot_anywhere || climb_anywhere)',
      'access': 'gs && event(WATER_MQ_THREE_TORCH_ROOM_GATE) && (scarecrow_hookshot || has_hover_boots || hookshot_anywhere || climb_anywhere)',
    },
  ],
  'oot/MQ Water Temple GS Side Loop': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/MQ Water Temple GS Lizalfos Hallway': [
    { 'expectedRule': 'gs && (event(WATER_LEVEL_LOW) || event(WATER_LEVEL_MID)) && can_use_din', 'access': 'gs && (event(WATER_LEVEL_LOW) || event(WATER_LEVEL_MID)) && can_use_din' },
  ],
  'oot/MQ Water Temple GS High Water Changer': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/Death Mountain Crater GS Crate': [
    { 'expectedRule': 'gs && is_child && can_damage_skull', 'access': 'gs && is_child' },
  ],
  'oot/Death Mountain Crater GS Soil': [
    { 'expectedRule': 'gs_soil && can_damage_skull', 'access': 'gs_soil' },
  ],
  'oot/Death Mountain Trail GS Entrance': [
    {
      'expectedRule': 'gs && (has_explosives || (can_hammer_or_keg && (can_use_sticks || can_use_sword_master || can_use_sword_gilded || (age_sword_adult && has(SWORD_BIGGORON)) || has_ranged_weapon || can_use_din)))',
      'access': 'gs && (has_explosives || can_hammer_or_keg)',
    },
  ],
  'oot/Death Mountain Trail GS Soil': [
    { 'expectedRule': 'gs_soil && has_bombflowers && can_damage_skull', 'access': 'gs_soil && has_bombflowers' },
  ],
  'oot/Death Mountain Trail GS Above Dodongo': [
    {
      'expectedRule': 'gs_night && is_adult && (can_hammer_or_keg || (trick(OOT_DMT_RED_ROCK_GS) && can_use_sword))',
      'access': 'is_adult && gs_night && (can_hammer_or_keg || (trick(OOT_DMT_RED_ROCK_GS) && can_use_sword))',
    },
  ],
  'oot/Death Mountain Trail GS Before Climb': [
    {
      'expectedRule': 'is_adult && gs_night && (can_hammer_or_keg || (trick(OOT_DMT_RED_ROCK_GS) && can_use_sword))',
      'access': 'is_adult && gs_night && (can_hammer_or_keg || (trick(OOT_DMT_RED_ROCK_GS) && can_use_sword))',
    },
  ],
  'oot/Desert Colossus GS Soil': [
    { 'expectedRule': 'gs_soil && can_damage_skull', 'access': 'gs_soil' },
  ],
  'oot/Desert Colossus GS Tree': [
    { 'expectedRule': 'is_adult && gs_night && (can_collect_distance || (climb_anywhere && (has_explosives || has_ranged_weapon || can_use_din)))', 'access': 'is_adult && gs_night' },
  ],
  'oot/Desert Colossus GS Plateau': [
    { 'expectedRule': 'is_adult && gs_night && ((can_collect_distance && trick(OOT_COLOSSUS_GS_NO_BEAN)) || (can_ride_bean(BEAN_DESERT_COLOSSUS) && can_damage_skull))', 'access': 'is_adult && gs_night && ((can_collect_distance && trick(OOT_COLOSSUS_GS_NO_BEAN)) || can_ride_bean(BEAN_DESERT_COLOSSUS))' },
  ],
  'oot/Gerudo Fortress GS Target': [
    { 'expectedRule': 'is_adult && gs_night && (can_collect_distance || (climb_anywhere && can_damage))', 'access': '(is_adult && gs_night) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Gerudo Fortress GS Wall': [
    { 'expectedRule': 'is_adult && gs_night && (can_collect_distance || has_bombs || can_use_bow || can_use_slingshot || (climb_anywhere && can_damage))', 'access': '(is_adult && gs_night) && (can_collect_distance || climb_anywhere)' },
    { 'expectedRule': 'is_adult && gs_night && (can_collect_distance || (climb_anywhere && can_damage))', 'access': '(is_adult && gs_night) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Gerudo Valley GS Soil': [
    { 'expectedRule': 'gs_soil && can_damage_skull', 'access': 'gs_soil' },
  ],
  'oot/Gerudo Valley GS Wall': [
    {
      'expectedRule': 'is_child && gs_night && (can_collect_distance || (climb_anywhere && (can_use_sword_master || (age_sword_adult && has(SWORD_BIGGORON)) || has_explosives || can_use_din || can_use_sticks || can_use_slingshot)))',
      'access': '(is_child && gs_night) && (can_collect_distance || climb_anywhere)',
    },
  ],
  'oot/Gerudo Valley GS Tent': [
    {
      'expectedRule': 'is_adult && gs_night && (can_collect_distance || (climb_anywhere && (has_explosives || has(SWORD_BIGGORON) || has_ranged_weapon || can_use_din)))',
      'access': '(is_adult && gs_night) && (can_collect_distance || climb_anywhere)',
    },
  ],
  'oot/Gerudo Valley GS Pillar': [
    { 'expectedRule': 'is_adult && can_collect_distance && gs_night', 'access': '(is_adult && gs_night) && (can_collect_distance)' },
  ],
  'oot/Goron City GS Platform': [
    { 'expectedRule': 'gs && is_adult && can_damage_skull', 'access': 'gs && is_adult' },
  ],
  'oot/Goron City GS Maze': [
    {
      'expectedRule': 'gs && is_child && (has_explosives_or_hammer || (climb_anywhere && can_damage_skull) || hookshot_anywhere)',
      'access': 'gs && is_child && (has_explosives_or_hammer || climb_anywhere || hookshot_anywhere)',
    },
  ],
  'oot/Graveyard GS Soil': [
    { 'expectedRule': 'gs_soil && can_damage_skull', 'access': 'gs_soil' },
  ],
  'oot/Graveyard GS Wall': [
    { 'expectedRule': 'is_child && gs_night && (can_collect_distance || (climb_anywhere && (has_ranged_weapon || has_explosives || can_use_din)))', 'access': '(is_child && gs_night) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Haunted Wasteland GS': [
    { 'expectedRule': 'gs && (can_collect_distance || (climb_anywhere && (has_ranged_weapon || has_explosives || can_use_sword || can_use_sticks || can_use_din)))', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Hyrule Field Grotto Near Gerudo GS': [
    { 'expectedRule': 'gs && (can_collect_distance || climb_anywhere) && has_fire', 'access': '(gs && has_fire) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Hyrule Field Grotto Near Kakariko GS': [
    { 'expectedRule': 'gs && (can_collect_distance || (climb_anywhere && (has_ranged_weapon || has_explosives || can_use_din)))', 'access': '(gs) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Hyrule Castle GS Tree': [
    { 'expectedRule': 'gs && can_damage_skull', 'access': 'gs' },
  ],
  'oot/Hyrule Castle GS Grotto': [
    { 'expectedRule': 'gs && has_explosives_or_hammer && can_collect_distance', 'access': '(gs && has_explosives_or_hammer) && (can_collect_distance)' },
  ],
  'oot/Ganon Castle Exterior GS': [
    { 'expectedRule': 'gs && (has_ranged_weapon || can_use_sword)', 'access': 'gs' },
  ],
  'oot/Kakariko GS Shooting Gallery': [
    { 'expectedRule': 'gs_night && is_child', 'access': 'gs_night && is_child' },
  ],
  'oot/Kakariko GS Tree': [
    { 'expectedRule': 'gs_night && is_child', 'access': 'gs_night && is_child' },
  ],
  'oot/Kakariko GS House of Skulltula': [
    { 'expectedRule': 'gs_night && is_child', 'access': 'gs_night && is_child' },
  ],
  'oot/Kakariko GS Bazaar': [
    { 'expectedRule': 'gs_night && is_child', 'access': 'gs_night && is_child' },
  ],
  'oot/Kakariko GS Ladder': [
    {
      'expectedRule': 'gs_night && is_child && (can_use_slingshot || can_use_bow || has_bombchu || can_longshot || (climb_anywhere && (can_use_sword || can_use_sticks)))',
      'access': 'gs_night && is_child',
    },
    { 'expectedRule': 'gs_night && is_child && can_use_din', 'access': 'gs_night && is_child' },
  ],
  'oot/Kakariko GS Roof': [
    { 'expectedRule': 'is_adult && gs_night && (can_hookshot || (climb_anywhere && can_damage))', 'access': '(is_adult && gs_night) && (can_hookshot || climb_anywhere)' },
  ],
  'oot/Kokiri Forest GS Soil': [
    { 'expectedRule': 'gs_soil && can_damage_skull', 'access': 'gs_soil' },
  ],
  'oot/Kokiri Forest GS Night Adult': [
    { 'expectedRule': 'is_adult && (can_collect_distance || (climb_anywhere && can_damage_skull)) && gs_night', 'access': '(is_adult && gs_night) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Kokiri Forest GS Night Child': [
    { 'expectedRule': 'gs_night && can_damage_skull', 'access': 'gs_night' },
  ],
  'oot/Lake Hylia GS Lab Wall': [
    {
      'expectedRule': 'is_child && gs_night && (can_collect_distance || (trick(OOT_LAB_WALL_GS) && (can_use_sword || can_use_sticks)) || (time_travel_at_will && (scarecrow_hookshot || hookshot_anywhere || can_ride_bean(BEAN_LAKE_HYLIA)) && (has_explosives_or_hammer || can_use_din || can_use_slingshot)))',
      'access': 'is_child && gs_night && (can_collect_distance || trick(OOT_LAB_WALL_GS) || (time_travel_at_will && (scarecrow_hookshot || hookshot_anywhere || can_ride_bean(BEAN_LAKE_HYLIA))))',
    },
  ],
  'oot/Lake Hylia GS Island': [
    {
      'expectedRule': 'is_child && gs_night && can_damage_skull && (can_swim || (has_iron_boots && can_longshot) || (can_use_ice_arrow_platforms) || (time_travel_at_will && (scarecrow_longshot || hookshot_anywhere || climb_anywhere)))',
      'access': 'gs_night && is_child && (can_swim || (has_iron_boots && can_longshot) || can_use_ice_arrow_platforms || (time_travel_at_will && (scarecrow_longshot || hookshot_anywhere || climb_anywhere)))',
    },
  ],
  'oot/Lake Hylia GS Soil': [
    { 'expectedRule': 'gs_soil && can_damage_skull', 'access': 'gs_soil' },
  ],
  'oot/Lake Hylia GS Big Tree': [
    { 'expectedRule': 'is_adult && gs_night && (can_longshot || (climb_anywhere && can_damage_skull))', 'access': 'is_adult && gs_night' },
  ],
  'oot/Laboratory GS Crate': [
    { 'expectedRule': 'gs && has_iron_boots && can_hookshot', 'access': '$RULE' },
  ],
  'oot/Lon Lon Ranch GS Tree': [
    { 'expectedRule': 'gs && is_child', 'access': 'gs && is_child' },
  ],
  'oot/Lon Lon Ranch GS House': [
    {
      'expectedRule': 'is_child && gs_night && (can_collect_distance || (climb_anywhere && (has_ranged_weapon || has_explosives || can_use_sword || can_use_sticks || can_use_din)))',
      'access': '(is_child && gs_night) && (can_collect_distance || climb_anywhere)',
    },
  ],
  'oot/Lon Lon Ranch GS Rain Shed': [
    { 'expectedRule': 'is_child && gs_night', 'access': 'is_child && gs_night' },
  ],
  'oot/Lon Lon Ranch GS Back Wall': [
    { 'expectedRule': 'is_child && gs_night && (can_collect_distance || (climb_anywhere && (has_ranged_weapon || has_explosives || can_use_din)))', 'access': '(is_child && gs_night) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Lost Woods GS Soil Bridge': [
    { 'expectedRule': 'gs_soil && can_damage_skull', 'access': 'gs_soil' },
  ],
  'oot/Lost Woods GS Soil Theater': [
    { 'expectedRule': 'gs_soil && can_damage_skull', 'access': 'gs_soil' },
  ],
  'oot/Lost Woods GS Bean Ride': [
    {
      'expectedRule': 'is_adult && gs_night && (((can_ride_bean(BEAN_LOST_WOODS_LATE) || climb_anywhere) && can_damage_skull) || hookshot_anywhere || (trick(OOT_LOST_WOODS_ADULT_GS) && can_collect_distance && (can_longshot || can_use_bow || has_bombchu || can_use_din)))',
      'access': 'is_adult && gs_night && (can_ride_bean(BEAN_LOST_WOODS_LATE) || climb_anywhere || hookshot_anywhere || (trick(OOT_LOST_WOODS_ADULT_GS) && can_collect_distance))',
    },
  ],
  'oot/Market Pot House GS': [
    { 'expectedRule': 'gs', 'access': 'gs' },
  ],
  'oot/Sacred Meadow GS Night Adult': [
    { 'expectedRule': 'is_adult && gs_night && (can_collect_distance || (climb_anywhere && (has_ranged_weapon || has_explosives || can_use_din)))', 'access': '(is_adult && gs_night) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Zora Domain GS Waterfall': [
    { 'expectedRule': 'is_adult && gs_night && (has_ranged_weapon || (has_magic && can_use_sword) || has_explosives)', 'access': 'is_adult && gs_night' },
  ],
  'oot/Zora Fountain GS Wall': [
    {
      'expectedRule': 'is_child && gs_night && (can_collect_distance || (climb_anywhere && (can_use_sword || can_use_sticks || has_ranged_weapon || has_explosives || can_use_din)))',
      'access': '(is_child && gs_night) && (can_collect_distance || climb_anywhere)',
    },
  ],
  'oot/Zora Fountain GS Tree': [
    { 'expectedRule': 'gs && is_child && can_damage_skull', 'access': 'gs && is_child' },
  ],
  'oot/Zora Fountain GS Upper': [
    {
      'expectedRule': 'is_adult && gs_night && ((has_explosives_or_hammer && can_collect_distance && can_lift_silver) || (climb_anywhere && (has_explosives || has(SWORD_BIGGORON) || has_ranged_weapon || can_use_din)))',
      'access': 'is_adult && gs_night && ((has_explosives_or_hammer && can_collect_distance && can_lift_silver) || climb_anywhere)',
    },
  ],
  'oot/Zora River GS Tree': [
    { 'expectedRule': 'gs && is_child && can_damage_skull', 'access': 'gs && is_child' },
  ],
  'oot/Zora River GS Ladder': [
    { 'expectedRule': 'is_child && gs_night && can_damage', 'access': 'is_child && gs_night' },
  ],
  'oot/Zora River GS Near Grotto': [
    { 'expectedRule': 'is_adult && gs_night && (can_collect_distance || (climb_anywhere && can_damage))', 'access': '(is_adult && gs_night) && (can_collect_distance || climb_anywhere)' },
  ],
  'oot/Zora River GS Near Bridge': [
    { 'expectedRule': 'is_adult && gs_night && (can_hookshot || (climb_anywhere && (has_ranged_weapon || has_bombchu)))', 'access': '(is_adult && gs_night) && (can_hookshot || climb_anywhere)' },
  ],
};
