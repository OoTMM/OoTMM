/**
 * Enums
 *
 * This file contains the enums used by the
 * music group smart enum, as well as music files
 * to avoid using more volatile data types.
 */

export enum SongType {
  Bgm,
  Fanfare,
  Cutscene,
}

/**
 * Everything loaded into the audio pool is given a cache
 * load type that determines which cache it goes to when
 * it is called. For audio sequences and instrument banks,
 * most entries will use persistent and temporary.
 *
 * PERMANENT is reserved for sound effects.
 * PERSISTENT is generally used for fanfares.
 * TEMPORARY is generally used for bgm.
 * EITHER is generally used for audio samples.
 * EITHER_NOSYNC is generally used for audio samples.
 */
export enum CacheLoad {
  PERMANENT,
  PERSISTENT,
  TEMPORARY,
  EITHER,
  EITHER_NOSYNC,
}