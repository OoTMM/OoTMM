export type MmAgeReqSide = 'child' | 'adult';

export type MmAgeReqItem = {
    id: string;
    label: string;
    cItem?: string;
    childSetting: string;
    adultSetting: string;
    childConfvar: string;
    adultConfvar: string;
};

export const MM_AGE_REQ_ITEMS = [
    {
        id: 'hookshot',
        label: 'Hookshot',
        cItem: 'ITEM_MM_HOOKSHOT',
        childSetting: 'mmAgeReqChildHookshot',
        adultSetting: 'mmAgeReqAdultHookshot',
        childConfvar: 'MM_AGE_REQ_CHILD_HOOKSHOT',
        adultConfvar: 'MM_AGE_REQ_ADULT_HOOKSHOT',
        
    },
    {
        id: 'hookshotShort',
        label: 'Short Hookshot',
        cItem: 'ITEM_MM_HOOKSHOT_SHORT',
        childSetting: 'mmAgeReqChildHookshotShort',
        adultSetting: 'mmAgeReqAdultHookshotShort',
        childConfvar: 'MM_AGE_REQ_CHILD_HOOKSHOT_SHORT',
        adultConfvar: 'MM_AGE_REQ_ADULT_HOOKSHOT_SHORT',
        
    },
    {
        id: 'bow',
        label: 'Bow',
        cItem: 'ITEM_MM_BOW',
        childSetting: 'mmAgeReqChildBow',
        adultSetting: 'mmAgeReqAdultBow',
        childConfvar: 'MM_AGE_REQ_CHILD_BOW',
        adultConfvar: 'MM_AGE_REQ_ADULT_BOW',
        
    },
    {
        id: 'ocarinaOfTime',
        label: 'Ocarina of Time',
        cItem: 'ITEM_MM_OCARINA_OF_TIME',
        childSetting: 'mmAgeReqChildOcarinaOfTime',
        adultSetting: 'mmAgeReqAdultOcarinaOfTime',
        childConfvar: 'MM_AGE_REQ_CHILD_OCARINA_OF_TIME',
        adultConfvar: 'MM_AGE_REQ_ADULT_OCARINA_OF_TIME',
        
    },
    {
        id: 'ocarinaFairy',
        label: 'Fairy Ocarina',
        cItem: 'ITEM_MM_OCARINA_FAIRY',
        childSetting: 'mmAgeReqChildOcarinaFairy',
        adultSetting: 'mmAgeReqAdultOcarinaFairy',
        childConfvar: 'MM_AGE_REQ_CHILD_OCARINA_FAIRY',
        adultConfvar: 'MM_AGE_REQ_ADULT_OCARINA_FAIRY',
        
    },
    {
        id: 'bomb',
        label: 'Bombs',
        cItem: 'ITEM_MM_BOMB',
        childSetting: 'mmAgeReqChildBomb',
        adultSetting: 'mmAgeReqAdultBomb',
        childConfvar: 'MM_AGE_REQ_CHILD_BOMB',
        adultConfvar: 'MM_AGE_REQ_ADULT_BOMB',
        
    },
    {
        id: 'bombchu',
        label: 'Bombchu',
        cItem: 'ITEM_MM_BOMBCHU',
        childSetting: 'mmAgeReqChildBombchu',
        adultSetting: 'mmAgeReqAdultBombchu',
        childConfvar: 'MM_AGE_REQ_CHILD_BOMBCHU',
        adultConfvar: 'MM_AGE_REQ_ADULT_BOMBCHU',
        
    },
    {
        id: 'arrowFire',
        label: 'Fire Arrow',
        cItem: 'ITEM_MM_ARROW_FIRE',
        childSetting: 'mmAgeReqChildArrowFire',
        adultSetting: 'mmAgeReqAdultArrowFire',
        childConfvar: 'MM_AGE_REQ_CHILD_ARROW_FIRE',
        adultConfvar: 'MM_AGE_REQ_ADULT_ARROW_FIRE',
        
    },
    {
        id: 'arrowIce',
        label: 'Ice Arrow',
        cItem: 'ITEM_MM_ARROW_ICE',
        childSetting: 'mmAgeReqChildArrowIce',
        adultSetting: 'mmAgeReqAdultArrowIce',
        childConfvar: 'MM_AGE_REQ_CHILD_ARROW_ICE',
        adultConfvar: 'MM_AGE_REQ_ADULT_ARROW_ICE',
        
    },
    {
        id: 'arrowLight',
        label: 'Light Arrow',
        cItem: 'ITEM_MM_ARROW_LIGHT',
        childSetting: 'mmAgeReqChildArrowLight',
        adultSetting: 'mmAgeReqAdultArrowLight',
        childConfvar: 'MM_AGE_REQ_CHILD_ARROW_LIGHT',
        adultConfvar: 'MM_AGE_REQ_ADULT_ARROW_LIGHT',
        
    },
    {
        id: 'hammer',
        label: 'Megaton Hammer',
        cItem: 'ITEM_MM_HAMMER',
        childSetting: 'mmAgeReqChildHammer',
        adultSetting: 'mmAgeReqAdultHammer',
        childConfvar: 'MM_AGE_REQ_CHILD_HAMMER',
        adultConfvar: 'MM_AGE_REQ_ADULT_HAMMER',
        
    },
    {
        id: 'spellWind',
        label: "Farore's Wind",
        cItem: 'ITEM_MM_SPELL_WIND',
        childSetting: 'mmAgeReqChildSpellWind',
        adultSetting: 'mmAgeReqAdultSpellWind',
        childConfvar: 'MM_AGE_REQ_CHILD_SPELL_WIND',
        adultConfvar: 'MM_AGE_REQ_ADULT_SPELL_WIND',
        
    },
    {
        id: 'spellLove',
        label: "Nayru's Love",
        cItem: 'ITEM_MM_SPELL_LOVE',
        childSetting: 'mmAgeReqChildSpellLove',
        adultSetting: 'mmAgeReqAdultSpellLove',
        childConfvar: 'MM_AGE_REQ_CHILD_SPELL_LOVE',
        adultConfvar: 'MM_AGE_REQ_ADULT_SPELL_LOVE',
        
    },
    {
        id: 'spellFire',
        label: "Din's Fire",
        cItem: 'ITEM_MM_SPELL_FIRE',
        childSetting: 'mmAgeReqChildSpellFire',
        adultSetting: 'mmAgeReqAdultSpellFire',
        childConfvar: 'MM_AGE_REQ_CHILD_SPELL_FIRE',
        adultConfvar: 'MM_AGE_REQ_ADULT_SPELL_FIRE',
        
    },
    {
        id: 'bootsIron',
        label: 'Iron Boots',
        cItem: 'ITEM_MM_BOOTS_IRON',
        childSetting: 'mmAgeReqChildBootsIron',
        adultSetting: 'mmAgeReqAdultBootsIron',
        childConfvar: 'MM_AGE_REQ_CHILD_BOOTS_IRON',
        adultConfvar: 'MM_AGE_REQ_ADULT_BOOTS_IRON',
        
    },
    {
        id: 'bootsHover',
        label: 'Hover Boots',
        cItem: 'ITEM_MM_BOOTS_HOVER',
        childSetting: 'mmAgeReqChildBootsHover',
        adultSetting: 'mmAgeReqAdultBootsHover',
        childConfvar: 'MM_AGE_REQ_CHILD_BOOTS_HOVER',
        adultConfvar: 'MM_AGE_REQ_ADULT_BOOTS_HOVER',
        
    },
    {
        id: 'tunicGoron',
        label: 'Goron Tunic',
        cItem: 'ITEM_MM_TUNIC_GORON',
        childSetting: 'mmAgeReqChildTunicGoron',
        adultSetting: 'mmAgeReqAdultTunicGoron',
        childConfvar: 'MM_AGE_REQ_CHILD_TUNIC_GORON',
        adultConfvar: 'MM_AGE_REQ_ADULT_TUNIC_GORON',
        
    },
    {
        id: 'tunicZora',
        label: 'Zora Tunic',
        cItem: 'ITEM_MM_TUNIC_ZORA',
        childSetting: 'mmAgeReqChildTunicZora',
        adultSetting: 'mmAgeReqAdultTunicZora',
        childConfvar: 'MM_AGE_REQ_CHILD_TUNIC_ZORA',
        adultConfvar: 'MM_AGE_REQ_ADULT_TUNIC_ZORA',
        
    },

    {
        id: 'greatFairySword',
        label: 'Great Fairy Sword',
        cItem: 'ITEM_MM_GREAT_FAIRY_SWORD',
        childSetting: 'mmAgeReqChildGreatFairySword',
        adultSetting: 'mmAgeReqAdultGreatFairySword',
        childConfvar: 'MM_AGE_REQ_CHILD_GREAT_FAIRY_SWORD',
        adultConfvar: 'MM_AGE_REQ_ADULT_GREAT_FAIRY_SWORD',
        
    },
    {
        id: 'boomerang',
        label: 'Boomerang',
        cItem: 'ITEM_MM_BOOMERANG',
        childSetting: 'mmAgeReqChildBoomerang',
        adultSetting: 'mmAgeReqAdultBoomerang',
        childConfvar: 'MM_AGE_REQ_CHILD_BOOMERANG',
        adultConfvar: 'MM_AGE_REQ_ADULT_BOOMERANG',
        
    },
    {
        id: 'slingshot',
        label: 'Slingshot',
        cItem: 'ITEM_MM_SLINGSHOT',
        childSetting: 'mmAgeReqChildSlingshot',
        adultSetting: 'mmAgeReqAdultSlingshot',
        childConfvar: 'MM_AGE_REQ_CHILD_SLINGSHOT',
        adultConfvar: 'MM_AGE_REQ_ADULT_SLINGSHOT',
        
    },
    {
        id: 'stick',
        label: 'Deku Stick',
        cItem: 'ITEM_MM_STICK',
        childSetting: 'mmAgeReqChildStick',
        adultSetting: 'mmAgeReqAdultStick',
        childConfvar: 'MM_AGE_REQ_CHILD_STICK',
        adultConfvar: 'MM_AGE_REQ_ADULT_STICK',
        
    },
    {
        id: 'maskDeku',
        label: 'Deku Mask',
        cItem: 'ITEM_MM_MASK_DEKU',
        childSetting: 'mmAgeReqChildMaskDeku',
        adultSetting: 'mmAgeReqAdultMaskDeku',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_DEKU',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_DEKU',
        
    },
    {
        id: 'nut',
        label: 'Deku Nuts',
        cItem: 'ITEM_MM_NUT',
        childSetting: 'mmAgeReqChildDekuNuts',
        adultSetting: 'mmAgeReqAdultDekuNuts',
        childConfvar: 'MM_AGE_REQ_CHILD_DEKU_NUTS',
        adultConfvar: 'MM_AGE_REQ_ADULT_DEKU_NUTS',
        
    },
    {
        id: 'magicBean',
        label: 'Magic Bean',
        cItem: 'ITEM_MM_MAGIC_BEAN',
        childSetting: 'mmAgeReqChildMagicBean',
        adultSetting: 'mmAgeReqAdultMagicBean',
        childConfvar: 'MM_AGE_REQ_CHILD_MAGIC_BEAN',
        adultConfvar: 'MM_AGE_REQ_ADULT_MAGIC_BEAN',
        
    },
    {
        id: 'powderKeg',
        label: 'Powder Keg',
        cItem: 'ITEM_MM_POWDER_KEG',
        childSetting: 'mmAgeReqChildPowderKeg',
        adultSetting: 'mmAgeReqAdultPowderKeg',
        childConfvar: 'MM_AGE_REQ_CHILD_POWDER_KEG',
        adultConfvar: 'MM_AGE_REQ_ADULT_POWDER_KEG',
        
    },
    {
        id: 'pictographBox',
        label: 'Pictograph Box',
        cItem: 'ITEM_MM_PICTOGRAPH_BOX',
        childSetting: 'mmAgeReqChildPictographBox',
        adultSetting: 'mmAgeReqAdultPictographBox',
        childConfvar: 'MM_AGE_REQ_CHILD_PICTOGRAPH_BOX',
        adultConfvar: 'MM_AGE_REQ_ADULT_PICTOGRAPH_BOX',
        
    },
    {
        id: 'lensOfTruth',
        label: 'Lens of Truth',
        cItem: 'ITEM_MM_LENS_OF_TRUTH',
        childSetting: 'mmAgeReqChildLensOfTruth',
        adultSetting: 'mmAgeReqAdultLensOfTruth',
        childConfvar: 'MM_AGE_REQ_CHILD_LENS_OF_TRUTH',
        adultConfvar: 'MM_AGE_REQ_ADULT_LENS_OF_TRUTH',
        
    },
    {
        id: 'maskGoron',
        label: 'Goron Mask',
        cItem: 'ITEM_MM_MASK_GORON',
        childSetting: 'mmAgeReqChildMaskGoron',
        adultSetting: 'mmAgeReqAdultMaskGoron',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_GORON',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_GORON',
        
    },
    {
        id: 'maskZora',
        label: 'Zora Mask',
        cItem: 'ITEM_MM_MASK_ZORA',
        childSetting: 'mmAgeReqChildMaskZora',
        adultSetting: 'mmAgeReqAdultMaskZora',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_ZORA',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_ZORA',
        
    },
    {
        id: 'maskFierceDeity',
        label: 'Fierce Deity Mask',
        cItem: 'ITEM_MM_MASK_FIERCE_DEITY',
        childSetting: 'mmAgeReqChildMaskFierceDeity',
        adultSetting: 'mmAgeReqAdultMaskFierceDeity',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_FIERCE_DEITY',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_FIERCE_DEITY',
        
    },
    {
        id: 'maskPostman',
        label: 'Postman Hat',
        cItem: 'ITEM_MM_MASK_POSTMAN',
        childSetting: 'mmAgeReqChildMaskPostman',
        adultSetting: 'mmAgeReqAdultMaskPostman',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_POSTMAN',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_POSTMAN',
        
    },
    {
        id: 'maskAllNight',
        label: 'All-Night Mask',
        cItem: 'ITEM_MM_MASK_ALL_NIGHT',
        childSetting: 'mmAgeReqChildMaskAllNight',
        adultSetting: 'mmAgeReqAdultMaskAllNight',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_ALL_NIGHT',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_ALL_NIGHT',
        
    },
    {
        id: 'maskBlast',
        label: 'Blast Mask',
        cItem: 'ITEM_MM_MASK_BLAST',
        childSetting: 'mmAgeReqChildMaskBlast',
        adultSetting: 'mmAgeReqAdultMaskBlast',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_BLAST',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_BLAST',
        
    },
    {
        id: 'maskStone',
        label: 'Stone Mask',
        cItem: 'ITEM_MM_MASK_STONE',
        childSetting: 'mmAgeReqChildMaskStone',
        adultSetting: 'mmAgeReqAdultMaskStone',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_STONE',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_STONE',
        
    },
    {
        id: 'maskGreatFairy',
        label: 'Great Fairy Mask',
        cItem: 'ITEM_MM_MASK_GREAT_FAIRY',
        childSetting: 'mmAgeReqChildMaskGreatFairy',
        adultSetting: 'mmAgeReqAdultMaskGreatFairy',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_GREAT_FAIRY',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_GREAT_FAIRY',
        
    },
    {
        id: 'maskKeaton',
        label: 'Keaton Mask',
        cItem: 'ITEM_MM_MASK_KEATON',
        childSetting: 'mmAgeReqChildMaskKeaton',
        adultSetting: 'mmAgeReqAdultMaskKeaton',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_KEATON',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_KEATON',
        
    },
    {
        id: 'maskBremen',
        label: 'Bremen Mask',
        cItem: 'ITEM_MM_MASK_BREMEN',
        childSetting: 'mmAgeReqChildMaskBremen',
        adultSetting: 'mmAgeReqAdultMaskBremen',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_BREMEN',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_BREMEN',
        
    },
    {
        id: 'maskBunny',
        label: 'Bunny Hood',
        cItem: 'ITEM_MM_MASK_BUNNY',
        childSetting: 'mmAgeReqChildMaskBunny',
        adultSetting: 'mmAgeReqAdultMaskBunny',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_BUNNY',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_BUNNY',
        
    },
    {
        id: 'maskDonGero',
        label: 'Don Gero Mask',
        cItem: 'ITEM_MM_MASK_DON_GERO',
        childSetting: 'mmAgeReqChildMaskDonGero',
        adultSetting: 'mmAgeReqAdultMaskDonGero',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_DON_GERO',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_DON_GERO',
        
    },
    {
        id: 'maskScents',
        label: 'Mask of Scents',
        cItem: 'ITEM_MM_MASK_SCENTS',
        childSetting: 'mmAgeReqChildMaskScents',
        adultSetting: 'mmAgeReqAdultMaskScents',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_SCENTS',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_SCENTS',
        
    },
    {
        id: 'maskRomani',
        label: 'Romani Mask',
        cItem: 'ITEM_MM_MASK_ROMANI',
        childSetting: 'mmAgeReqChildMaskRomani',
        adultSetting: 'mmAgeReqAdultMaskRomani',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_ROMANI',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_ROMANI',
        
    },
    {
        id: 'maskTroupeLeader',
        label: 'Troupe Leader Mask',
        cItem: 'ITEM_MM_MASK_TROUPE_LEADER',
        childSetting: 'mmAgeReqChildMaskTroupeLeader',
        adultSetting: 'mmAgeReqAdultMaskTroupeLeader',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_TROUPE_LEADER',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_TROUPE_LEADER',
        
    },
    {
        id: 'maskKafei',
        label: 'Kafei Mask',
        cItem: 'ITEM_MM_MASK_KAFEI',
        childSetting: 'mmAgeReqChildMaskKafei',
        adultSetting: 'mmAgeReqAdultMaskKafei',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_KAFEI',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_KAFEI',
        
    },
    {
        id: 'maskCouple',
        label: "Couple's Mask",
        cItem: 'ITEM_MM_MASK_COUPLE',
        childSetting: 'mmAgeReqChildMaskCouple',
        adultSetting: 'mmAgeReqAdultMaskCouple',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_COUPLE',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_COUPLE',
        
    },
    {
        id: 'maskTruth',
        label: 'Mask of Truth',
        cItem: 'ITEM_MM_MASK_TRUTH',
        childSetting: 'mmAgeReqChildMaskTruth',
        adultSetting: 'mmAgeReqAdultMaskTruth',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_TRUTH',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_TRUTH',
        
    },
    {
        id: 'maskKamaro',
        label: 'Kamaro Mask',
        cItem: 'ITEM_MM_MASK_KAMARO',
        childSetting: 'mmAgeReqChildMaskKamaro',
        adultSetting: 'mmAgeReqAdultMaskKamaro',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_KAMARO',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_KAMARO',
        
    },
    {
        id: 'maskGibdo',
        label: 'Gibdo Mask',
        cItem: 'ITEM_MM_MASK_GIBDO',
        childSetting: 'mmAgeReqChildMaskGibdo',
        adultSetting: 'mmAgeReqAdultMaskGibdo',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_GIBDO',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_GIBDO',
        
    },
    {
        id: 'maskGaro',
        label: 'Garo Mask',
        cItem: 'ITEM_MM_MASK_GARO',
        childSetting: 'mmAgeReqChildMaskGaro',
        adultSetting: 'mmAgeReqAdultMaskGaro',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_GARO',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_GARO',
        
    },
    {
        id: 'maskCaptain',
        label: "Captain's Hat",
        cItem: 'ITEM_MM_MASK_CAPTAIN',
        childSetting: 'mmAgeReqChildMaskCaptain',
        adultSetting: 'mmAgeReqAdultMaskCaptain',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_CAPTAIN',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_CAPTAIN',
        
    },
    {
        id: 'maskGiant',
        label: "Giant's Mask",
        cItem: 'ITEM_MM_MASK_GIANT',
        childSetting: 'mmAgeReqChildMaskGiant',
        adultSetting: 'mmAgeReqAdultMaskGiant',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_GIANT',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_GIANT',
        
    },
    {
        id: 'maskGerudo',
        label: 'Gerudo Mask',
        cItem: 'ITEM_MM_MASK_GERUDO',
        childSetting: 'mmAgeReqChildMaskGerudo',
        adultSetting: 'mmAgeReqAdultMaskGerudo',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_GERUDO',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_GERUDO',
        
    },
    {
        id: 'maskSkull',
        label: 'Skull Mask',
        cItem: 'ITEM_MM_MASK_SKULL',
        childSetting: 'mmAgeReqChildMaskSkull',
        adultSetting: 'mmAgeReqAdultMaskSkull',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_SKULL',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_SKULL',
        
    },
    {
        id: 'maskSpooky',
        label: 'Spooky Mask',
        cItem: 'ITEM_MM_MASK_SPOOKY',
        childSetting: 'mmAgeReqChildMaskSpooky',
        adultSetting: 'mmAgeReqAdultMaskSpooky',
        childConfvar: 'MM_AGE_REQ_CHILD_MASK_SPOOKY',
        adultConfvar: 'MM_AGE_REQ_ADULT_MASK_SPOOKY',
        
    },
    {
        id: 'moonTear',
        label: "Moon's Tear",
        cItem: 'ITEM_MM_MOON_TEAR',
        childSetting: 'mmAgeReqChildMoonTear',
        adultSetting: 'mmAgeReqAdultMoonTear',
        childConfvar: 'MM_AGE_REQ_CHILD_MOON_TEAR',
        adultConfvar: 'MM_AGE_REQ_ADULT_MOON_TEAR',
    },
    {
        id: 'deedLand',
        label: 'Land Title Deed',
        cItem: 'ITEM_MM_DEED_LAND',
        childSetting: 'mmAgeReqChildDeedLand',
        adultSetting: 'mmAgeReqAdultDeedLand',
        childConfvar: 'MM_AGE_REQ_CHILD_DEED_LAND',
        adultConfvar: 'MM_AGE_REQ_ADULT_DEED_LAND',
    },
    {
        id: 'deedSwamp',
        label: 'Swamp Title Deed',
        cItem: 'ITEM_MM_DEED_SWAMP',
        childSetting: 'mmAgeReqChildDeedSwamp',
        adultSetting: 'mmAgeReqAdultDeedSwamp',
        childConfvar: 'MM_AGE_REQ_CHILD_DEED_SWAMP',
        adultConfvar: 'MM_AGE_REQ_ADULT_DEED_SWAMP',
    },
    {
        id: 'deedMountain',
        label: 'Mountain Title Deed',
        cItem: 'ITEM_MM_DEED_MOUNTAIN',
        childSetting: 'mmAgeReqChildDeedMountain',
        adultSetting: 'mmAgeReqAdultDeedMountain',
        childConfvar: 'MM_AGE_REQ_CHILD_DEED_MOUNTAIN',
        adultConfvar: 'MM_AGE_REQ_ADULT_DEED_MOUNTAIN',
    },
    {
        id: 'deedOcean',
        label: 'Ocean Title Deed',
        cItem: 'ITEM_MM_DEED_OCEAN',
        childSetting: 'mmAgeReqChildDeedOcean',
        adultSetting: 'mmAgeReqAdultDeedOcean',
        childConfvar: 'MM_AGE_REQ_CHILD_DEED_OCEAN',
        adultConfvar: 'MM_AGE_REQ_ADULT_DEED_OCEAN',
    },
    {
        id: 'letterToMama',
        label: 'Priority Mail to Mama',
        cItem: 'ITEM_MM_LETTER_TO_MAMA',
        childSetting: 'mmAgeReqChildLetterToMama',
        adultSetting: 'mmAgeReqAdultLetterToMama',
        childConfvar: 'MM_AGE_REQ_CHILD_LETTER_TO_MAMA',
        adultConfvar: 'MM_AGE_REQ_ADULT_LETTER_TO_MAMA',
    },
    {
        id: 'letterToKafei',
        label: 'Letter to Kafei',
        cItem: 'ITEM_MM_LETTER_TO_KAFEI',
        childSetting: 'mmAgeReqChildLetterToKafei',
        adultSetting: 'mmAgeReqAdultLetterToKafei',
        childConfvar: 'MM_AGE_REQ_CHILD_LETTER_TO_KAFEI',
        adultConfvar: 'MM_AGE_REQ_ADULT_LETTER_TO_KAFEI',
    },
    {
        id: 'pendantOfMemories',
        label: 'Pendant of Memories',
        cItem: 'ITEM_MM_PENDANT_OF_MEMORIES',
        childSetting: 'mmAgeReqChildPendantOfMemories',
        adultSetting: 'mmAgeReqAdultPendantOfMemories',
        childConfvar: 'MM_AGE_REQ_CHILD_PENDANT_OF_MEMORIES',
        adultConfvar: 'MM_AGE_REQ_ADULT_PENDANT_OF_MEMORIES',
    },
    {
        id: 'strength',
        label: 'Silver/Golden Gauntlets',
        childSetting: 'mmAgeReqChildStrength',
        adultSetting: 'mmAgeReqAdultStrength',
        childConfvar: 'MM_AGE_REQ_CHILD_STRENGTH',
        adultConfvar: 'MM_AGE_REQ_ADULT_STRENGTH',
    },
] as const;