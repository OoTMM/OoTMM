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

- Initial public release.

[Unreleased]: https://github.com/OoTMM/OoTMM/compare/v18.3...HEAD
[18.3]: https://github.com/OoTMM/OoTMM/compare/v18.2...v18.3
[18.2]: https://github.com/OoTMM/OoTMM/compare/v18.1...v18.2
[18.1]: https://github.com/OoTMM/OoTMM/compare/v18.0...v18.1
[18.0]: https://github.com/OoTMM/OoTMM/compare/v17.3...v18.0
[1.0.1]: https://github.com/OoTMM/OoTMM/compare/v1.0.0...v1.0.1
[1.0.0]: https://github.com/OoTMM/OoTMM/releases/tag/v1.0.0
