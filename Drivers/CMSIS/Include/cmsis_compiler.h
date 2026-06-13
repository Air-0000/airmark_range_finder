/**
 * @file cmsis_compiler.h
 * @brief CMSIS compiler abstraction layer
 */
#ifndef __CMSIS_COMPILER_H
#define __CMSIS_COMPILER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Compiler generic macros */
#if defined(__CC_ARM)
#elif defined(__GNUC__)
  /* __GNUC__ already defined by compiler - no need to redefine */
#elif defined(__ICCARM__)
  #define __ICCARM__
#endif

/* Include compiler-specific headers */
#if defined(__CC_ARM)
  #include "cmsis_armcc.h"
#elif defined(__GNUC__)
  #include "cmsis_gcc.h"
#elif defined(__ICCARM__)
  #include "cmsis_iar.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* __CMSIS_COMPILER_H */