// PCX image loader. Public domain. See "unlicense" statement at the end of this file.
// dr_pcx - v0.3.1 - 2018-09-11
//
// David Reid - mackron@gmail.com

// USAGE
//
// dr_pcx is a single-file library. To use it, do something like the following in one .c file.
//     #define DR_PCX_IMPLEMENTATION
//     #include "dr_pcx.h"
//
// You can then #include this file in other parts of the program as you would with any other header file. Do something like
// the following to load and decode an image:
//
//     int width;
//     int height;
//     int components
//     drpcx_uint8* pImageData = drpcx_load_file("my_image.pcx", DRPCX_FALSE, &width, &height, &components, 0);
//     if (pImageData == NULL) {
//         // Failed to load image.
//     }
//
//     ...
//
//     drpcx_free(pImageData);
//
// The boolean parameter (second argument in the above example) is whether or not the image should be flipped upside down.
// 
//
//
// OPTIONS
// #define these options before including this file.
//
// #define DR_PCX_NO_STDIO
//   Disable drpcx_load_file().
//
//
//
// QUICK NOTES
// - 2-bpp/4-plane and 4-bpp/1-plane formats have not been tested.

#ifndef dr_pcx_h
#define dr_pcx_h

#include <stddef.h>

#if defined(_MSC_VER) && _MSC_VER < 1600
typedef   signed char    drpcx_int8;
typedef unsigned char    drpcx_uint8;
typedef   signed short   drpcx_int16;
typedef unsigned short   drpcx_uint16;
typedef   signed int     drpcx_int32;
typedef unsigned int     drpcx_uint32;
typedef   signed __int64 drpcx_int64;
typedef unsigned __int64 drpcx_uint64;
#else
#include <stdint.h>
typedef int8_t           drpcx_int8;
typedef uint8_t          drpcx_uint8;
typedef int16_t          drpcx_int16;
typedef uint16_t         drpcx_uint16;
typedef int32_t          drpcx_int32;
typedef uint32_t         drpcx_uint32;
typedef int64_t          drpcx_int64;
typedef uint64_t         drpcx_uint64;
#endif
typedef drpcx_uint8      drpcx_bool8;
typedef drpcx_uint32     drpcx_bool32;
#define DRPCX_TRUE       1
#define DRPCX_FALSE      0

#ifdef __cplusplus
extern "C" {
#endif

// Callback for when data is read. Return value is the number of bytes actually read.
typedef size_t (* drpcx_read_proc)(void* userData, void* bufferOut, size_t bytesToRead);


// Loads a PCX file using the given callbacks.
drpcx_uint8* drpcx_load(drpcx_read_proc onRead, void* pUserData, drpcx_bool32 flipped, int* x, int* y, int* internalComponents, int desiredComponents);

// Frees memory returned by drpcx_load() and family.
void drpcx_free(void* pReturnValueFromLoad);


#ifndef DR_PCX_NO_STDIO
// Loads an PCX file from an actual file.
drpcx_uint8* drpcx_load_file(const char* filename, drpcx_bool32 flipped, int* x, int* y, int* internalComponents, int desiredComponents);
#endif

// Helper for loading an PCX file from a block of memory.
drpcx_uint8* drpcx_load_memory(const void* data, size_t dataSize, drpcx_bool32 flipped, int* x, int* y, int* internalComponents, int desiredComponents);


#ifdef __cplusplus
}
#endif

#endif  // dr_pcx_h
