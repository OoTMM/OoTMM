#ifndef LINE_NUMBERS_H
#define LINE_NUMBERS_H

// The macros in this file serve to reduce clutter from version differences that are only due to line numbers being different.

// The "LN" macros defined here are not meant to be used directly. See the wrapper macros below.
#if OOT_VERSION == NTSC_1_0
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (ntsc_1_0)
#elif OOT_VERSION == NTSC_1_1
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (ntsc_1_1)
#elif OOT_VERSION == PAL_1_0
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (pal_1_0)
#elif OOT_VERSION == NTSC_1_2
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (ntsc_1_2)
#elif OOT_VERSION == PAL_1_1
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (pal_1_1)
#elif OOT_VERSION == GC_JP
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (gc_jp)
#elif OOT_VERSION == GC_JP_MQ
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (gc_jp_mq)
#elif OOT_VERSION == GC_US
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (gc_us)
#elif OOT_VERSION == GC_US_MQ
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (gc_us_mq)
#elif OOT_VERSION == GC_EU_MQ_DBG
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (gc_eu_mq_dbg)
#elif OOT_VERSION == GC_EU
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (gc_eu)
#elif OOT_VERSION == GC_EU_MQ
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (gc_eu_mq)
#elif OOT_VERSION == GC_JP_CE
#define LN(ntsc_1_0, ntsc_1_1, pal_1_0, ntsc_1_2, pal_1_1, gc_jp, gc_jp_mq, gc_us, gc_us_mq, gc_eu_mq_dbg, gc_eu, gc_eu_mq, gc_jp_ce) (gc_jp_ce)
#endif

/**
 * These wrappers exist to remove duplication of line numbers being passed to LN.
 * The wrappers have ambiguous names purposefully so they are short and don't add clutter.
 *
 * The groupings of different line numbers are difficult to name, as the exact grouping
 * depends on which versions happen to have changes in a given file.
 */

// NTSC 1.0 and all the other versions
#define LN1(ntsc_1_0, other) \
    LN(ntsc_1_0, other, other, other, other, other, other, other, other, other, other, other, other)

// NTSC 1.0, NTSC 1.1, and all the other versions
#define LN2(ntsc_1_0, ntsc_1_1, other) \
    LN(ntsc_1_0, ntsc_1_1, other, other, other, other, other, other, other, other, other, other, other)

// NTSC 1.0, Gamecube, and all the other versions
#define LN3(ntsc_1_0, other, gc) \
    LN(ntsc_1_0, other, other, other, other, gc, gc, gc, gc, gc, gc, gc, gc)

// NTSC 1.0, NTSC 1.1, Gamecube, and all the other versions
#define LN4(ntsc_1_0, ntsc_1_1, other, gc) \
    LN(ntsc_1_0, ntsc_1_1, other, other, other, gc, gc, gc, gc, gc, gc, gc, gc)

// NTSC 1.0, NTSC 1.1, NTSC 1.2/PAL 1.0, Gamecube US/JP, Gamecube EU, and Gamecube CE
#define LN5(ntsc_1_0, ntsc_1_1, other, pal_1_1, gc_ntsc, gc_eu, gc_jp_ce) \
    LN(ntsc_1_0, ntsc_1_1, other, other, pal_1_1, gc_ntsc, gc_ntsc, gc_ntsc, gc_ntsc, gc_eu, gc_eu, gc_eu, gc_jp_ce)

#endif
