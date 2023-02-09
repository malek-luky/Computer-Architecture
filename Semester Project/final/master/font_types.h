/* File:     font_types.h
 * Author:   Pavel Pisa
 * Date:     9.6.2020
 * Course:   APO 2020
 */

///////////////////////////////////////////////////////////////////////////////
//  SIMPLE BITMAP FONTS TYPE DEFINITION
///////////////////////////////////////////////////////////////////////////////

#ifndef FONT_TYPES_H
#define FONT_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef uint16_t font_bits_t;

  /* Builtin C-based proportional/fixed font structure */
  typedef struct
  {
    char *name;                 // font name
    int maxwidth;               // max width in pixels
    unsigned int height;        // height in pixels
    int ascent;                 // ascent (baseline) height
    int firstchar;              // first character in bitmap
    int size;                   // font size in characters
    const font_bits_t *bits;    // 16-bit right-padded bitmap data
    const uint32_t *offset;     // offsets into bitmap data
    const unsigned char *width; // character widths or 0 if fixed
    int defaultchar;            // default char (not glyph index
    int32_t bits_size;          // words of MWIMAGEBITS bits
  } font_descriptor_t;

  extern font_descriptor_t font_winFreeSystem14x16;

  extern font_descriptor_t font_rom8x16;

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif
