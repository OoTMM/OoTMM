# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added

- Small preview for the next item when multiple items are in the same slot.
- Silver Scale/Golden Scale in Majora's Mask.
- Freestanding hearts shuffle for Master Quest.
- Cosmetic setting for OoT Mirror Shield.
- Hide the rupee counter when not having Child Wallet.
- Add a beginner preset to the generator.
- Add deku stick rendering as adult.
- Add masks rendering as adult.
- Add Blast Mask to Ocarina of Time.
- Add Iron Boots/Hover Boots to Majora's Mask.
- Add Goron Tunic/Zora Tunic to Majora's Mask.
- Add dedicated CSMC textures and colors for souls.
- Add shared settings for every new item extension.

### Fixed

- Fix a bug where you could unequip shields and swords or toggle items while hovering the wrong slot.
- Fix stray fairies not restroring health.
- Fix tunic color being off while having Razor Sword.
- Fix nayru's effect being visually off from link in MM in some cases.
- Fix items that don't exist in a given seed being hinted as being in Link's Pocket.

### Changed

- OoT headers to make them closer to decomp.
- Improve MM boots logic.
- Rebalance weighted prices to make it more fair.
- Make the rupee counter color more consistent between games, and consistent with wallet colors.
- Improve the behavior of open dungeons with Stone Tower Temple.
- Update the blitz presets.
- Make tunics cosmetics affect their item models.

## [18.3] - 2024-02-05

### Fixed

- Fix fish sharing flags with Deku Tree chests/collectible in multiplayer.
- Fix flying pots not dropping their vanilla content when they should.
- Fix renewables not working correctly in multiplayer.
- Fix Evan logic.
- Fix Dodongo's Cavern MQ silver rupees not working correctly when shuffled.
- Fix King Dodongo logic sometimes expecting bombchu.
- Fix Farore Wind in MM sometimes causing Song of Soaring to warp to the wrong destination.
- Fix various typos.

## [18.2] - 2024-01-23

### Changed

- Add MM spells to the blitz presets.

### Fixed

- Fix various typos.

## [18.1] - 2024-01-23

### Added

- Add OoT spells in Majora's Mask.
- Add shared settings for spells.
- Add a setting to make MM dungeons open.

### Fixed

- Fix diving game not working correctly in multiplayer.
- Fix Woodfall/Woodfall Temple logic.
- Fix Stone Tower logic.

## [18.0] - 2024-01-20

### Added

- Add multiplayer co-op, where players can see each other as flames and share items.
- Add zora diving game rupees shuffle.
- Add shared shields.
- Add bombchu bag, with bombchu drops.
- Add a setting to make the Happy Mask Shop door open at night.
- Add wallmaster ER, where wallmasters can warp you to a variety of areas.
- Add fairy fountain fairies shuffle.
- Add fairy spots big fairires shuffle.
- Add freestanding rupees shuffle.
- Add freestanding hearts shuffle.
- Add a setting to randomize the initial content of bottles.
- Add a setting to make the item pool extremely barren.
- Add the ability to unequip the kokiri sword and the various shields in OoT.

### Changed

- Make sometimes and always hints respect the no plando hint setting.

### Fixed

- Fix longstanding crashes and other issues on hardware.
- Fix the owl statue sometimes giving duplicate items in owl shuffle.
- Various pathfinder fixes.
- Fix OoT arrow cycling draining magic when OoT chateau is active.
- Fix OoT Zora Mask causing ice trap damage when obtained in a chest in Majora's Mask.
- Fix a vanilla bug where Hover Boots would be displayed over the quiver slot in some cases.
- Fix custom models sometimes causing the web generator to fail.

### Changed

- Make multiworld support the co-op improvements.

## [1.4.0] - 2023-02-28

### Added

- Add custom models for Milk and Chateau.
- Add the option to generate a seed without a spoiler log.
- Add shared items settings for bows, bomb bags, magic, magical arrows, songs, sticks and nuts, hookshot, lens, ocarinas, masks, wallets, and health.

### Changed

- Make dungeon ER honor beatable only logic.
- Make the secret shrine hint a double hint.
- Make game transitions a bit smoother on hardware.

### Fixed

- Fix grottos causing wrong warps in ER.
- Fix the ISTT boss warp pad being enabled when it shoudn't.
- Fix hints being sometimes wrong in ER.
- Fix minor issues with stray fairies and hints.

## [1.3.0] - 2023-02-23

### Added

- Add patchfiles for easy seed sharing.
- Add magical wisps to access Majora's Mask region in their cleared states, for ER.
- Add beatable only setting.
- Add a separate setting for hideout small keys.
- Add Dungeon ER.
- Add item counts when collecting skulls/fairies/keys.
- Add various tricks.
- Add the ability to junk locations.

### Changed

- Make logic slightly closer to OoTR.
- Make LACS and ravaged village obtainable from all kakariko entrances.
- Make web and local seeds more similar internally.
- Make Woodfall temple not raise if completed while not raised thanks to ER.
- Prevent time from flowing for Boss ER, when coming from OoT.
- Improve spoiler log readability.
- Add more internal automated tests.
- Major internal changes to the pathfinding.

### Fixed

- Fix bunny hood making the player fall off the cliff after the Keeta cutscene.
- Fix logic issues related to Fire Temple and Goron Tunic.
- Fix a rare issues with spheres not computed properly in some cases.
- Fix logic issues related to boss ER.
- Fix boss ER causing the 0th day in some cases.
- Fix logic for Swamp.
- Fix logic for ISTT.
- Fix some seeds failing to generate properly.
- Fix various issues with swordless link.

## [1.2.0] - 2023-02-16

### Added

- Add an in-game tracker for various items, such as small keys.
- Add nice item names in the web generator.
- Add Boss ER.

### Changed

- Improve logic regarding Goron Bomb Jump.
- Skip MM boss cutscenes.
- Reworked the rules regarding playthrough hints.
- Skip the deku princess palace cutscene.
- Skip the deku princess jail cutscene.
- Disable the owl auto-talk in MM.
- Skip most of the cremia cutscene.
- Skip the aliens cutscene.
- Skip the epona ranch cutscene.
- Allow using boots on the dpad while rolling.
- Revert to human form when taking a blue warp.

### Fixed

- Fix stray fairies/gold tokens not pausing the game properly when collected in MM.
- Fix the grotto in Zora Domain not opening properly in some cases.
- Fix adult archery giving the reward even when not having a bow.
- Fix heap corruption in OoT.
- Fix some items not having nice names.
- Fix some items being impossible to select in menus in some cases.
- Fix the sheik ice cavern song check rarely giving a corrupted item.
- Fix various minor issues with hints.
- Fix a scarecrow softlock in OoT.
- Fix dpad being very permissive.
- Fix dpad being useable on epona.
- Improved hardware compatibility.
- Fix flags being corrupted in some rare cases.

## [1.1.3] - 2023-02-11

### Added

- Add nice names to items in the spoiler log.

## [1.1.2] - 2023-02-07

### Fixed

- Fix elder goron logic.

## [1.1.1] - 2023-02-07

### Changed

- Make the ikana valley blue potion logical.

## [1.1.0] - 2023-02-01

### Added

- Add custom text to OoT scrubs telling what item they sell.
- Add door of time settings.
- Add progressive goron lullaby setting.
- Add progressive shields settings.
- Add a setting to disable master sword shuffle.
- Add OoT progressive swords setting.
- Add more settings for maps/compass shuffle.

### Changed

- Rename sound check.
- Prevent Sacred Realm and Giant's Dream from being hinted.
- Make the OoT bombchu bowling reward order fixed.
- Various logic improvements.
- Internal changes to the way item IDs work.

### Fixed

- Fix crashes on hardware when coming back to OoT from MM.

## [1.0.1] - 2023-01-30

### Added

- Add hashs to seeds.

### Fixed

- Fix a logic issue with the DMC bean.
- Fix the open chests in the Moon's Goron Trial being affected by CSMC.
- Fix the Mikau check sometimes not working.
- Fix a duplication bug with the Fire Arrow check in OoT.
- Fix MM trade items being permanently lost when duping bottles over them.
- Fix Farore's Wind letting you warp across ages.
- Fix a softlock when closing the scarecrow textbox too fast in OoT.
- Fix drops not having icons when picked up.
- Fix the town guards having inconsistent behavior in MM.
- Fix the vanilla OoT 1.0 empty bomb glitch.
- Fix adult link having initial equips that the player might not be supposed to have.
- Fix a softlock in MM when voiding without having the Deku Mask.
- Fix buying bomb in OoT sometimes not working.
- Fix the ocarina making trumpet noises in MM after doing the monkey check.
- Fix owls always grabbing you after being talked to in OoT.
- Fix swordless link in OoT having weird behavior with the sword slot.
- Worked around a Project64 bug that causes crashes when entering OoT adult Bazaar.
- Fix collecting the ocarina check in MM while having the hookshot in hand causing major glitches and corruption.
- Fix severe issues with the pathfinder causing it to be overly restrictive.
- Fix a flag issue with the pictobox check.

### Changed

- Make tingle airborne even without magic.
- Make time flow normally in MM irrespective of having an ocarina.
- Make the monkey check kick you out of Deku palace.

## [1.0.0] - 2023-01-22

### Added

- Initial stable release.

[Unreleased]: https://github.com/OoTMM/OoTMM/compare/v18.3...HEAD
[18.3]: https://github.com/OoTMM/OoTMM/compare/v18.2...v18.3
[18.2]: https://github.com/OoTMM/OoTMM/compare/v18.1...v18.2
[18.1]: https://github.com/OoTMM/OoTMM/compare/v18.0...v18.1
[18.0]: https://github.com/OoTMM/OoTMM/compare/v17.3...v18.0
[1.4.0]: https://github.com/OoTMM/OoTMM/compare/v1.3.0...v1.4.0
[1.3.0]: https://github.com/OoTMM/OoTMM/compare/v1.2.0...v1.3.0
[1.2.0]: https://github.com/OoTMM/OoTMM/compare/v1.1.3...v1.2.0
[1.1.3]: https://github.com/OoTMM/OoTMM/compare/v1.1.2...v1.1.3
[1.1.2]: https://github.com/OoTMM/OoTMM/compare/v1.1.1...v1.1.2
[1.1.1]: https://github.com/OoTMM/OoTMM/compare/v1.1.0...v1.1.1
[1.1.0]: https://github.com/OoTMM/OoTMM/compare/v1.0.1...v1.1.0
[1.0.1]: https://github.com/OoTMM/OoTMM/compare/v1.0.0...v1.0.1
[1.0.0]: https://github.com/OoTMM/OoTMM/releases/tag/v1.0.0
