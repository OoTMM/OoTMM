/**
 * Enums
 *
 * This file contains the enums used by the
 * music group smart enum, as well as music files
 * to avoid using more volatile data types.
 */

/**
 * Represents the possible in-game song types.
 */
export enum SongType {
  Bgm,
  Fanfare,
  Cutscene, // Unused
}

/**
 * Represents one of the possible audio cache load types
 * in Ocarina of Time and Majora's Mask.
 *
 * - PERMANENT: reserved for sound effects.
 * - PERSISTENT: generally used for fanfares.
 * - TEMPORARY: generally used for bgm.
 * - EITHER: generally used for audio samples.
 * - EITHER_NOSYNC: generally used for audio samples.
 */
export enum CacheLoad {
  PERMANENT,
  PERSISTENT,
  TEMPORARY,
  EITHER,
  EITHER_NOSYNC,
}