/* File:     mzapo_phys.h
 * Author:   Pavel Pisa
 * Date:     9.6.2020
 * Course:   APO 2020
 */

///////////////////////////////////////////////////////////////////////////////
//  MAPPING OF THE PHISICAL ADDRESS TO PROCESS
///////////////////////////////////////////////////////////////////////////////
#ifndef MZAPO_PHYS_H
#define MZAPO_PHYS_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

  void *map_phys_address(off_t region_base, size_t region_size, int opt_cached);

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif
