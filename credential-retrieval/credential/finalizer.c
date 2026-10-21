#include "finalizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef uint8_t byte;
typedef uint8_t undefined;
typedef uint64_t undefined8;
typedef uint32_t uint;
typedef unsigned long ulong;
typedef struct {
    uint32_t start, end, offset;
} Span;
static bool missing_constant;
static const Span spans[] = {
    {0xc8c4f0, 0xc8c5f0, 0x0}, {0xcacae5, 0xcacaf5, 0x100}, {0xcb25c0, 0xcb29c0, 0x110}, {0xcb3000, 0xcb3400, 0x510}, {0xcf2cc0, 0xcf30c0, 0x910}, {0xd353f0, 0xd357f0, 0xd10}, {0xd36f30, 0xd37030, 0x1110}, {0xd3f070, 0xd3f170, 0x1210}, {0xd87ab0, 0xd87bb0, 0x1310}, {0xd91190, 0xd911a0, 0x1410}, {0xd963d0, 0xd964d0, 0x1420}, {0xdaeb90, 0xdaef90, 0x1520}, {0xdbbba1, 0xdbbbb1, 0x1920}, {0xdea323, 0xdea333, 0x1930}, {0xe02860, 0xe02c60, 0x1940}, {0xe04270, 0xe04370, 0x1d40}, {0xe0f130, 0xe0f530, 0x1e40}, {0xe34a30, 0xe34b30, 0x2240}, {0xe94a50, 0xe94e50, 0x2340}, {0xeb89a0, 0xeb89b0, 0x2740}, {0xee7db0, 0xee7eb0, 0x2750}, {0xf36224, 0xf36234, 0x2850},
};
static const uint8_t constants[] = "\x35\x4e\x62\x19\x0c\x77\x5b\x20\x9e\xe5\xc9\xb2\xa7\xdc\xf0\x8b\x93\xe8\xc4\xbf\xaa\xd1\xfd\x86\x38\x43\x6f\x14\x01\x7a\x56\x2d\xd2\xa9\x85\xfe\xeb\x90\xbc\xc7\x79\x02\x2e\x55\x40\x3b\x17\x6c\x74\x0f\x23\x58\x4d\x36\x1a\x61\xdf\xa4\x88\xf3\xe6\x9d\xb1\xca\x5f\x24\x08\x73\x66\x1d\x31\x4a\xf4\x8f\xa3\xd8\xcd\xb6\x9a\xe1\xf9\x82\xae\xd5\xc0\xbb\x97\xec\x52\x29\x05\x7e\x6b\x10\x3c\x47\xb8\xc3\xef\x94\x81\xfa\xd6\xad\x13\x68\x44\x3f\x2a\x51\x7d\x06\x1e\x65\x49\x32\x27\x5c\x70\x0b\xb5\xce\xe2\x99\x8c\xf7\xdb\xa0\xed\x96\xba\xc1\xd4\xaf\x83\xf8\x46\x3d\x11\x6a\x7f\x04\x28\x53\x4b\x30\x1c\x67\x72\x09\x25\x5e\xe0\x9b\xb7\xcc\xd9\xa2\x8e\xf5\x0a\x71\x5d\x26\x33\x48\x64\x1f\xa1\xda\xf6\x8d\x98\xe3\xcf\xb4\xac\xd7\xfb\x80\x95\xee\xc2\xb9\x07\x7c\x50\x2b\x3e\x45\x69\x12\x87\xfc\xd0\xab\xbe\xc5\xe9\x92\x2c\x57\x7b\x00\x15\x6e\x42\x39\x21\x5a\x76\x0d\x18\x63\x4f\x34\x8a\xf1\xdd\xa6\xb3\xc8\xe4\x9f\x60\x1b\x37\x4c\x59\x22\x0e\x75\xcb\xb0\x9c\xe7\xf2\x89\xa5\xde\xc6\xbd\x91\xea\xff\x84\xa8\xd3\x6d\x16\x3a\x41\x54\x2f\x03\x78"
                                   "\x63\xbc\xf2\x9d\xd4\x81\xca\x51\xe1\x9e\x9e\x27\x6d\xc7\xdd\xa3\x00\xd5\xce\x27\x32\xd5\xce\x27\x87\xd4\xce\x27\x8f\xd4\xce\x27\x0e\xd5\xce\x27\x75\xd4\xce\x27\x35\xd5\xce\x27\x1a\xd5\xce\x27\x93\xd4\xce\x27\x12\xd5\xce\x27\xdd\xd4\xce\x27\x6f\xd4\xce\x27\xe7\xd4\xce\x27\x64\xd4\xce\x27\xf6\xd4\xce\x27\x44\xd4\xce\x27\x17\xd5\xce\x27\xf9\xd4\xce\x27\xb1\xd4\xce\x27\x28\xd5\xce\x27\x71\xd4\xce\x27\x54\xd4\xce\x27\xf5\xd4\xce\x27\xb5\xd4\xce\x27\x21\xd5\xce\x27\xc3\xd4\xce\x27\x46\xd4\xce\x27\x2b\xd5\xce\x27\x02\xd5\xce\x27\xc6\xd4\xce\x27\x56\xd4\xce\x27\xd8\xd4\xce\x27\x33\xd5\xce\x27\x99\xd4\xce\x27\xf3\xd4\xce\x27\x19\xd5\xce\x27\x6d\xd4\xce\x27\xa7\xd4\xce\x27\xfc\xd4\xce\x27\xd6\xd4\xce\x27\x91\xd4\xce\x27\xdc\xd4\xce\x27\xb4\xd4\xce\x27\xa5\xd4\xce\x27\x11\xd5\xce\x27\x7f\xd4\xce\x27\xcc\xd4\xce\x27\x63\xd4\xce\x27\xf8\xd4\xce\x27\xd7\xd4\xce\x27\xc1\xd4\xce\x27\x97\xd4\xce\x27\x5f\xd4\xce\x27\xd2\xd4\xce\x27\x49\xd4\xce\x27\xc0\xd4\xce\x27\x39\xd5\xce\x27\x7b\xd4\xce\x27\x6c\xd4\xce\x27\xc8\xd4\xce\x27"
                                   "\x42\xd4\xce\x27\x36\xd5\xce\x27\x4f\xd4\xce\x27\x9a\xd4\xce\x27\x78\xd4\xce\x27\x1b\xd5\xce\x27\x53\xd4\xce\x27\x7c\xd4\xce\x27\x08\xd5\xce\x27\x3f\xd4\xce\x27\xbd\xd4\xce\x27\x5b\xd4\xce\x27\xe8\xd4\xce\x27\x18\xd5\xce\x27\x2f\xd5\xce\x27\x23\xd5\xce\x27\x8a\xd4\xce\x27\xf4\xd4\xce\x27\x22\xd5\xce\x27\x40\xd4\xce\x27\xf7\xd4\xce\x27\x37\xd5\xce\x27\x27\xd5\xce\x27\x5c\xd4\xce\x27\xb9\xd4\xce\x27\xa9\xd4\xce\x27\xfb\xd4\xce\x27\x8c\xd4\xce\x27\xe6\xd4\xce\x27\x51\xd4\xce\x27\x77\xd4\xce\x27\x3a\xd5\xce\x27\x15\xd5\xce\x27\x4d\xd4\xce\x27\x5a\xd4\xce\x27\x9e\xd4\xce\x27\x96\xd4\xce\x27\x60\xd4\xce\x27\x58\xd4\xce\x27\x1f\xd5\xce\x27\x81\xd4\xce\x27\x3e\xd4\xce\x27\xcf\xd4\xce\x27\x52\xd4\xce\x27\x88\xd4\xce\x27\x85\xd4\xce\x27\x65\xd4\xce\x27\x3d\xd4\xce\x27\x3c\xd4\xce\x27\xa8\xd4\xce\x27\xda\xd4\xce\x27\xf0\xd4\xce\x27\x8d\xd4\xce\x27\x9c\xd4\xce\x27\x6a\xd4\xce\x27\x03\xd5\xce\x27\xce\xd4\xce\x27\x80\xd4\xce\x27\x5d\xd4\xce\x27\x6e\xd4\xce\x27\x7a\xd4\xce\x27\xc5\xd4\xce\x27\x90\xd4\xce\x27\xc9\xd4\xce\x27"
                                   "\x41\xd4\xce\x27\xfe\xd4\xce\x27\x05\xd5\xce\x27\x29\xd5\xce\x27\xa0\xd4\xce\x27\x9f\xd4\xce\x27\x7e\xd4\xce\x27\xcd\xd4\xce\x27\xff\xd4\xce\x27\xab\xd4\xce\x27\xb2\xd4\xce\x27\xd3\xd4\xce\x27\x98\xd4\xce\x27\x84\xd4\xce\x27\x45\xd4\xce\x27\x01\xd5\xce\x27\x61\xd4\xce\x27\x13\xd5\xce\x27\x50\xd4\xce\x27\x57\xd4\xce\x27\x38\xd5\xce\x27\x48\xd4\xce\x27\xa6\xd4\xce\x27\xd9\xd4\xce\x27\x89\xd4\xce\x27\x8b\xd4\xce\x27\x66\xd4\xce\x27\xf2\xd4\xce\x27\x3b\xd4\xce\x27\xc7\xd4\xce\x27\x1d\xd5\xce\x27\xfd\xd4\xce\x27\x79\xd4\xce\x27\x43\xd4\xce\x27\x26\xd5\xce\x27\xbe\xd4\xce\x27\xe3\xd4\xce\x27\x1c\xd5\xce\x27\x30\xd5\xce\x27\xde\xd4\xce\x27\xa1\xd4\xce\x27\x59\xd4\xce\x27\xb6\xd4\xce\x27\x2d\xd5\xce\x27\x34\xd5\xce\x27\xd1\xd4\xce\x27\xe1\xd4\xce\x27\xe9\xd4\xce\x27\x0f\xd5\xce\x27\xba\xd4\xce\x27\x76\xd4\xce\x27\x0b\xd5\xce\x27\xd4\xd4\xce\x27\x82\xd4\xce\x27\xb3\xd4\xce\x27\x83\xd4\xce\x27\x55\xd4\xce\x27\xb8\xd4\xce\x27\x9b\xd4\xce\x27\x04\xd5\xce\x27\xec\xd4\xce\x27\x14\xd5\xce\x27\x68\xd4\xce\x27\xaa\xd4\xce\x27"
                                   "\xd0\xd4\xce\x27\x47\xd4\xce\x27\x2c\xd5\xce\x27\xdb\xd4\xce\x27\x09\xd5\xce\x27\x9d\xd4\xce\x27\xb0\xd4\xce\x27\xa3\xd4\xce\x27\x0d\xd5\xce\x27\xed\xd4\xce\x27\x07\xd5\xce\x27\xfa\xd4\xce\x27\x6b\xd4\xce\x27\xdf\xd4\xce\x27\xae\xd4\xce\x27\x06\xd5\xce\x27\x69\xd4\xce\x27\x31\xd5\xce\x27\x70\xd4\xce\x27\xbf\xd4\xce\x27\xb7\xd4\xce\x27\xa2\xd4\xce\x27\xaf\xd4\xce\x27\xad\xd4\xce\x27\xea\xd4\xce\x27\xbc\xd4\xce\x27\x2e\xd5\xce\x27\xc2\xd4\xce\x27\xeb\xd4\xce\x27\x73\xd4\xce\x27\x95\xd4\xce\x27\xe0\xd4\xce\x27\x4e\xd4\xce\x27\x0a\xd5\xce\x27\xf1\xd4\xce\x27\x8e\xd4\xce\x27\x7d\xd4\xce\x27\x4c\xd4\xce\x27\x4b\xd4\xce\x27\x25\xd5\xce\x27\x67\xd4\xce\x27\xef\xd4\xce\x27\x10\xd5\xce\x27\xbb\xd4\xce\x27\xd5\xd4\xce\x27\xe2\xd4\xce\x27\x20\xd5\xce\x27\x16\xd5\xce\x27\x24\xd5\xce\x27\x1e\xd5\xce\x27\xee\xd4\xce\x27\x94\xd4\xce\x27\x2a\xd5\xce\x27\xe4\xd4\xce\x27\x62\xd4\xce\x27\x5e\xd4\xce\x27\x86\xd4\xce\x27\x0c\xd5\xce\x27\xa4\xd4\xce\x27\xca\xd4\xce\x27\x92\xd4\xce\x27\xe5\xd4\xce\x27\x74\xd4\xce\x27\xc4\xd4\xce\x27"
                                   "\xcb\xd4\xce\x27\xac\xd4\xce\x27\x72\xd4\xce\x27\x4a\xd4\xce\x27\x2d\xbd\xd9\x14\x33\xbd\xd9\x7c\x31\xbd\xd9\x58\x33\xbd\xd9\xb1\x05\xbd\xd9\x86\x33\xbd\xd9\xfb\x31\xbd\xd9\x5f\x33\xbd\xd9\xc2\xfd\xbc\xd9\xd3\x03\xbd\xd9\x84\x01\xbd\xd9\x07\x03\xbd\xd9\x3e\x05\xbd\xd9\xc9\x03\xbd\xd9\xf2\x01\xbd\xd9\xa2\x03\xbd\xd9\x43\x2d\xbd\xd9\xcb\x33\xbd\xd9\x85\x31\xbd\xd9\x96\x33\xbd\xd9\x7f\x25\xbd\xd9\xf4\x33\xbd\xd9\xa5\x31\xbd\xd9\xab\x33\xbd\xd9\x76\x1d\xbd\xd9\xa3\x23\xbd\xd9\x89\x21\xbd\xd9\x5a\x23\xbd\xd9\xe1\x25\xbd\xd9\x21\x23\xbd\xd9\xf6\x21\xbd\xd9\xaa\x23\xbd\xd9\xa1\xed\xbc\xd9\x13\xf3\xbc\xd9\x3d\xf1\xbc\xd9\xcd\xf3\xbc\xd9\x08\x05\xbd\xd9\xf8\xf3\xbc\xd9\x57\xf1\xbc\xd9\xd8\xf3\xbc\xd9\x55\xfd\xbc\xd9\xc3\x03\xbd\xd9\xc7\x01\xbd\xd9\x9c\x03\xbd\xd9\x2e\x05\xbd\xd9\x24\x03\xbd\xd9\x8c\x01\xbd\xd9\xbe\x03\xbd\xd9\x99\xed\xbc\xd9\x94\xf3\xbc\xd9\x04\xf1\xbc\xd9\xe2\xf3\xbc\xd9\xed\xe5\xbc\xd9\x7a\xf3\xbc\xd9\xdf\xf1\xbc\xd9\xbc\xf3\xbc\xd9\xf3\xdd\xbc\xd9\x8a\xe3\xbc\xd9\xa7\xe1\xbc\xd9\xc0\xe3\xbc\xd9\xe6"
                                   "\xe5\xbc\xd9\x48\xe3\xbc\xd9\x5c\xe1\xbc\xd9\x28\xe3\xbc\xd9\xf5\x2d\xbd\xd9\x62\x33\xbd\xd9\xc1\x31\xbd\xd9\x38\x33\xbd\xd9\x0c\x85\xbd\xd9\xd9\x33\xbd\xd9\x97\x31\xbd\xd9\x1c\x33\xbd\xd9\xb4\x7d\xbd\xd9\x2f\x83\xbd\xd9\x6e\x81\xbd\xd9\x5b\x83\xbd\xd9\x1f\x85\xbd\xd9\x1a\x83\xbd\xd9\x91\x81\xbd\xd9\x83\x83\xbd\xd9\xd5\x2d\xbd\xd9\xba\x33\xbd\xd9\x92\x31\xbd\xd9\xb5\x33\xbd\xd9\x56\x25\xbd\xd9\x4d\x33\xbd\xd9\xea\x31\xbd\xd9\xb3\x33\xbd\xd9\xac\x1d\xbd\xd9\xe0\x23\xbd\xd9\xdc\x21\xbd\xd9\x2d\x23\xbd\xd9\x6a\x25\xbd\xd9\x16\x23\xbd\xd9\x80\x21\xbd\xd9\xd4\x23\xbd\xd9\x4e\x6d\xbd\xd9\x0e\x73\xbd\xd9\x3a\x71\xbd\xd9\xce\x73\xbd\xd9\xd0\x85\xbd\xd9\x27\x73\xbd\xd9\x54\x71\xbd\xd9\xff\x73\xbd\xd9\x75\x7d\xbd\xd9\x31\x83\xbd\xd9\x20\x81\xbd\xd9\x37\x83\xbd\xd9\x12\x85\xbd\xd9\x93\x83\xbd\xd9\x30\x81\xbd\xd9\x35\x83\xbd\xd9\xa9\x6d\xbd\xd9\x02\x73\xbd\xd9\x82\x71\xbd\xd9\xb7\x73\xbd\xd9\x52\x65\xbd\xd9\x4c\x73\xbd\xd9\x7e\x71\xbd\xd9\x70\x73\xbd\xd9\x9e\x5d\xbd\xd9\xda\x63\xbd\xd9\x44\x61\xbd\xd9\x81\x63\xbd\xd9\x5e"
                                   "\x65\xbd\xd9\xee\x63\xbd\xd9\x95\x61\xbd\xd9\xe7\x63\xbd\xd9\x29\x2d\xbd\xd9\x79\x33\xbd\xd9\x25\x31\xbd\xd9\x78\x33\xbd\xd9\x90\x05\xbd\xd9\x68\x33\xbd\xd9\x2b\x31\xbd\xd9\x1d\x33\xbd\xd9\x06\xfd\xbc\xd9\x41\x03\xbd\xd9\x6b\x01\xbd\xd9\xa0\x03\xbd\xd9\x64\x05\xbd\xd9\x5d\x03\xbd\xd9\x63\x01\xbd\xd9\x9f\x03\xbd\xd9\xf0\x2d\xbd\xd9\xe8\x33\xbd\xd9\x05\x31\xbd\xd9\xd1\x33\xbd\xd9\xd7\x25\xbd\xd9\x19\x33\xbd\xd9\xe5\x31\xbd\xd9\x9b\x33\xbd\xd9\xe3\x1d\xbd\xd9\x67\x23\xbd\xd9\xbf\x21\xbd\xd9\xc8\x23\xbd\xd9\x8e\x25\xbd\xd9\xe4\x23\xbd\xd9\x0a\x21\xbd\xd9\x33\x23\xbd\xd9\xfa\xed\xbc\xd9\x10\xf3\xbc\xd9\xb0\xf1\xbc\xd9\x01\xf3\xbc\xd9\xb8\x05\xbd\xd9\x9a\xf3\xbc\xd9\xbb\xf1\xbc\xd9\x09\xf3\xbc\xd9\x53\xfd\xbc\xd9\x7b\x03\xbd\xd9\x39\x01\xbd\xd9\xec\x03\xbd\xd9\x73\x05\xbd\xd9\x26\x03\xbd\xd9\x7d\x01\xbd\xd9\xc4\x03\xbd\xd9\xae\xed\xbc\xd9\x23\xf3\xbc\xd9\xb2\xf1\xbc\xd9\x4f\xf3\xbc\xd9\xc5\xe5\xbc\xd9\x87\xf3\xbc\xd9\x18\xf1\xbc\xd9\xad\xf3\xbc\xd9\x72\xdd\xbc\xd9\xa8\xe3\xbc\xd9\x0d\xe1\xbc\xd9\x50\xe3\xbc\xd9\xaf"
                                   "\xe5\xbc\xd9\x6c\xe3\xbc\xd9\x4a\xe1\xbc\xd9\x88\xe3\xbc\xd9\x1b\x2d\xbd\xd9\x11\x33\xbd\xd9\x15\x31\xbd\xd9\x51\x33\xbd\xd9\x6d\x85\xbc\xd9\xd2\x33\xbd\xd9\x8d\x31\xbd\xd9\x6f\x33\xbd\xd9\xc6\x7d\xbc\xd9\x2c\x83\xbc\xd9\x8f\x81\xbc\xd9\xd6\x83\xbc\xd9\x00\x85\xbc\xd9\x69\x83\xbc\xd9\x71\x81\xbc\xd9\xde\x83\xbc\xd9\x17\x2d\xbd\xd9\x1e\x33\xbd\xd9\x47\x31\xbd\xd9\x4b\x33\xbd\xd9\xf1\x25\xbd\xd9\x59\x33\xbd\xd9\x36\x31\xbd\xd9\x34\x33\xbd\xd9\x8b\x1d\xbd\xd9\x2a\x23\xbd\xd9\x60\x21\xbd\xd9\x61\x23\xbd\xd9\x3f\x25\xbd\xd9\xbd\x23\xbd\xd9\x3b\x21\xbd\xd9\x49\x23\xbd\xd9\xfc\x6d\xbc\xd9\xdb\x73\xbc\xd9\x0b\x71\xbc\xd9\xe9\x73\xbc\xd9\xa4\x85\xbc\xd9\x22\x73\xbc\xd9\x40\x71\xbc\xd9\xeb\x73\xbc\xd9\xfd\x7d\xbc\xd9\x46\x83\xbc\xd9\x45\x81\xbc\xd9\xf9\x83\xbc\xd9\x98\x85\xbc\xd9\xdd\x83\xbc\xd9\xb9\x81\xbc\xd9\x32\x83\xbc\xd9\xca\x6d\xbc\xd9\x66\x73\xbc\xd9\x77\x71\xbc\xd9\x3c\x73\xbc\xd9\x74\x65\xbc\xd9\xf7\x73\xbc\xd9\x9d\x71\xbc\xd9\x03\x73\xbc\xd9\xfe\x5d\xbc\xd9\x65\x63\xbc\xd9\xb6\x61\xbc\xd9\xa6\x63\xbc\xd9\x42"
                                   "\x65\xbc\xd9\xcc\x63\xbc\xd9\xcf\x61\xbc\xd9\x0f\x63\xbc\xd9\xef\xf2\x3c\x1c\x56\xf2\x3c\x57\x56\xf2\x3c\x66\x56\xf2\x3c\x71\x56\xf2\x3c\xeb\x56\xf2\x3c\x21\x56\xf2\x3c\xaa\x56\xf2\x3c\x01\x56\xf2\x3c\x8c\x56\xf2\x3c\x93\x56\xf2\x3c\x39\x56\xf2\x3c\xc7\x56\xf2\x3c\x34\x56\xf2\x3c\x41\x56\xf2\x3c\x74\x56\xf2\x3c\x69\x56\xf2\x3c\xd7\x56\xf2\x3c\xb9\x56\xf2\x3c\x80\x56\xf2\x3c\x30\x56\xf2\x3c\x2c\x56\xf2\x3c\xa6\x56\xf2\x3c\x19\x56\xf2\x3c\xf8\x56\xf2\x3c\xd1\x56\xf2\x3c\x9c\x56\xf2\x3c\xc2\x56\xf2\x3c\x49\x56\xf2\x3c\x5d\x56\xf2\x3c\x9f\x56\xf2\x3c\x70\x56\xf2\x3c\x2f\x56\xf2\x3c\xda\x56\xf2\x3c\xfa\x56\xf2\x3c\xb5\x56\xf2\x3c\xee\x56\xf2\x3c\x50\x56\xf2\x3c\x3b\x56\xf2\x3c\x4a\x56\xf2\x3c\x6b\x56\xf2\x3c\xde\x56\xf2\x3c\x6c\x56\xf2\x3c\xd6\x56\xf2\x3c\xf1\x56\xf2\x3c\x04\x56\xf2\x3c\xb2\x56\xf2\x3c\x9a\x56\xf2\x3c\x62\x56\xf2\x3c\x92\x56\xf2\x3c\x55\x56\xf2\x3c\xe7\x56\xf2\x3c\x09\x56\xf2\x3c\xf9\x56\xf2\x3c\x6a\x56\xf2\x3c\x1d\x56\xf2\x3c\x63\x56\xf2\x3c\xd4\x56\xf2\x3c\xa2\x56\xf2\x3c\xe2\x56\xf2\x3c\x81\x56"
                                   "\xf2\x3c\xa5\x56\xf2\x3c\xf7\x56\xf2\x3c\x43\x56\xf2\x3c\x4b\x56\xf2\x3c\x02\x56\xf2\x3c\x0c\x56\xf2\x3c\x0f\x56\xf2\x3c\x9e\x56\xf2\x3c\x77\x56\xf2\x3c\xec\x56\xf2\x3c\x8b\x56\xf2\x3c\xdf\x56\xf2\x3c\x37\x56\xf2\x3c\x4c\x56\xf2\x3c\x7a\x56\xf2\x3c\x96\x56\xf2\x3c\x45\x56\xf2\x3c\x7d\x56\xf2\x3c\xdd\x56\xf2\x3c\x20\x56\xf2\x3c\x3f\x56\xf2\x3c\x5f\x56\xf2\x3c\x18\x56\xf2\x3c\xa9\x56\xf2\x3c\x6d\x56\xf2\x3c\xc1\x56\xf2\x3c\x0b\x56\xf2\x3c\x94\x56\xf2\x3c\xa1\x56\xf2\x3c\xbb\x56\xf2\x3c\xad\x56\xf2\x3c\xfe\x56\xf2\x3c\xb0\x56\xf2\x3c\xab\x56\xf2\x3c\x88\x56\xf2\x3c\x3c\x56\xf2\x3c\xc3\x56\xf2\x3c\x31\x56\xf2\x3c\x4f\x56\xf2\x3c\x90\x56\xf2\x3c\xbe\x56\xf2\x3c\x11\x56\xf2\x3c\x13\x56\xf2\x3c\xba\x56\xf2\x3c\x10\x56\xf2\x3c\xe9\x56\xf2\x3c\xfd\x56\xf2\x3c\x1b\x56\xf2\x3c\x12\x56\xf2\x3c\xb8\x56\xf2\x3c\xb6\x56\xf2\x3c\xa4\x56\xf2\x3c\x76\x56\xf2\x3c\x44\x56\xf2\x3c\xe3\x56\xf2\x3c\xf5\x56\xf2\x3c\xa3\x56\xf2\x3c\x2a\x56\xf2\x3c\xcf\x56\xf2\x3c\x25\x56\xf2\x3c\xd8\x56\xf2\x3c\xa7\x56\xf2\x3c\xd3\x56\xf2\x3c\x89\x56"
                                   "\xf2\x3c\xc6\x56\xf2\x3c\x7b\x56\xf2\x3c\x14\x56\xf2\x3c\xe8\x56\xf2\x3c\xfb\x56\xf2\x3c\xea\x56\xf2\x3c\x53\x56\xf2\x3c\x7c\x56\xf2\x3c\x24\x56\xf2\x3c\xf6\x56\xf2\x3c\x16\x56\xf2\x3c\xf3\x56\xf2\x3c\xb3\x56\xf2\x3c\x36\x56\xf2\x3c\x52\x56\xf2\x3c\x8f\x56\xf2\x3c\x42\x56\xf2\x3c\x6e\x56\xf2\x3c\x83\x56\xf2\x3c\x1e\x56\xf2\x3c\x84\x56\xf2\x3c\xc5\x56\xf2\x3c\xef\x56\xf2\x3c\x5c\x56\xf2\x3c\x35\x56\xf2\x3c\xac\x56\xf2\x3c\x4d\x56\xf2\x3c\x6f\x56\xf2\x3c\x2b\x56\xf2\x3c\x5a\x56\xf2\x3c\xa8\x56\xf2\x3c\x5e\x56\xf2\x3c\x91\x56\xf2\x3c\xc0\x56\xf2\x3c\x00\x56\xf2\x3c\x48\x56\xf2\x3c\x58\x56\xf2\x3c\x65\x56\xf2\x3c\x0e\x56\xf2\x3c\x23\x56\xf2\x3c\x54\x56\xf2\x3c\x98\x56\xf2\x3c\x3e\x56\xf2\x3c\x87\x56\xf2\x3c\x3a\x56\xf2\x3c\xe0\x56\xf2\x3c\xcb\x56\xf2\x3c\xf2\x56\xf2\x3c\xb4\x56\xf2\x3c\x68\x56\xf2\x3c\xd9\x56\xf2\x3c\xbc\x56\xf2\x3c\xa0\x56\xf2\x3c\xd2\x56\xf2\x3c\x22\x56\xf2\x3c\xc9\x56\xf2\x3c\x2d\x56\xf2\x3c\x2e\x56\xf2\x3c\xb7\x56\xf2\x3c\x1a\x56\xf2\x3c\xae\x56\xf2\x3c\xc4\x56\xf2\x3c\x51\x56\xf2\x3c\x33\x56"
                                   "\xf2\x3c\x38\x56\xf2\x3c\x64\x56\xf2\x3c\xe5\x56\xf2\x3c\x07\x56\xf2\x3c\xe4\x56\xf2\x3c\xe1\x56\xf2\x3c\xe6\x56\xf2\x3c\x46\x56\xf2\x3c\x29\x56\xf2\x3c\x7e\x56\xf2\x3c\x61\x56\xf2\x3c\x86\x56\xf2\x3c\xc8\x56\xf2\x3c\x03\x56\xf2\x3c\xff\x56\xf2\x3c\x78\x56\xf2\x3c\xdc\x56\xf2\x3c\x85\x56\xf2\x3c\x15\x56\xf2\x3c\x26\x56\xf2\x3c\x8a\x56\xf2\x3c\x67\x56\xf2\x3c\x0d\x56\xf2\x3c\x3d\x56\xf2\x3c\x79\x56\xf2\x3c\xce\x56\xf2\x3c\x8d\x56\xf2\x3c\xf4\x56\xf2\x3c\x60\x56\xf2\x3c\xfc\x56\xf2\x3c\x27\x56\xf2\x3c\x9b\x56\xf2\x3c\xaf\x56\xf2\x3c\x40\x56\xf2\x3c\x73\x56\xf2\x3c\x82\x56\xf2\x3c\xcd\x56\xf2\x3c\xb1\x56\xf2\x3c\xcc\x56\xf2\x3c\x59\x56\xf2\x3c\x97\x56\xf2\x3c\xd5\x56\xf2\x3c\x08\x56\xf2\x3c\x95\x56\xf2\x3c\x17\x56\xf2\x3c\x47\x56\xf2\x3c\xd0\x56\xf2\x3c\x56\x56\xf2\x3c\xf0\x56\xf2\x3c\x72\x56\xf2\x3c\xbf\x56\xf2\x3c\x05\x56\xf2\x3c\x99\x56\xf2\x3c\xed\x56\xf2\x3c\x8e\x56\xf2\x3c\x06\x56\xf2\x3c\x75\x56\xf2\x3c\x4e\x56\xf2\x3c\xca\x56\xf2\x3c\x7f\x56\xf2\x3c\x28\x56\xf2\x3c\x1f\x56\xf2\x3c\xdb\x56\xf2\x3c\x5b\x56"
                                   "\xf2\x3c\x9d\x56\xf2\x3c\x0a\x56\xf2\x3c\x32\x56\xf2\x3c\xbd\x56\x85\x85\x5e\x7f\x85\xb7\x5e\x7f\x85\xec\x8b\x7f\x85\xbd\x5e\x7f\x85\xa9\x5e\x7f\x85\x9d\x5e\x7f\x85\x10\x34\x30\x85\xe2\x8b\x7f\x85\x62\x61\x7f\x85\xe1\x8b\x7f\x85\x02\x34\x30\x85\x3c\x34\x30\x85\x5c\x61\x7f\x85\xff\x8b\x7f\x85\x03\x34\x30\x85\xf8\x8b\x7f\x85\x73\x61\x7f\x85\x0d\x34\x30\x85\xa8\x5e\x7f\x85\x39\x34\x30\x85\x5b\x61\x7f\x85\xb9\x5e\x7f\x85\x68\x61\x7f\x85\x1b\x34\x30\x85\x8a\x5e\x7f\x85\x6b\x61\x7f\x85\xf9\x8b\x7f\x85\xee\x8b\x7f\x85\x27\x34\x30\x85\xdf\x8b\x7f\x85\x53\x61\x7f\x85\xd6\x8b\x7f\x85\xa1\x5e\x7f\x85\x54\x61\x7f\x85\x38\x34\x30\x85\x60\x61\x7f\x85\xda\x8b\x7f\x85\xf3\x8b\x7f\x85\xb4\x5e\x7f\x85\xb1\x5e\x7f\x85\x59\x61\x7f\x85\x26\x34\x30\x85\x83\x5e\x7f\x85\xa6\x5e\x7f\x85\x43\x61\x7f\x85\x15\x37\x30\x85\x78\x61\x7f\x85\x92\x5e\x7f\x85\x51\x61\x7f\x85\xe6\x8b\x7f\x85\xbf\x5e\x7f\x85\x7d\x61\x7f\x85\xb5\x5e\x7f\x85\xc5\x8b\x7f\x85\x9e\x5e\x7f\x85\x88\x5e\x7f\x85\x32\x34\x30\x85\x95\x61\x7f\x85\x48\x61\x7f\x85\x9f\x5e\x7f"
                                   "\x85\x4f\x61\x7f\x85\x34\x34\x30\x85\xbb\x5e\x7f\x85\x7e\x61\x7f\x85\xf5\x8b\x7f\x85\x28\x34\x30\x85\x97\x5e\x7f\x85\x50\x61\x7f\x85\x37\x34\x30\x85\x2e\x34\x30\x85\x86\x5e\x7f\x85\x2d\x34\x30\x85\x22\x34\x30\x85\x0c\x34\x30\x85\x4d\x61\x7f\x85\x90\x5e\x7f\x85\x1d\x34\x30\x85\xc9\x8b\x7f\x85\xf2\x8b\x7f\x85\x81\x5e\x7f\x85\xe7\x8b\x7f\x85\xad\x5e\x7f\x85\x0b\x34\x30\x85\xf4\x8b\x7f\x85\x08\x34\x30\x85\xd3\x8b\x7f\x85\x18\x34\x30\x85\x2b\x34\x30\x85\x77\x61\x7f\x85\xfe\x8b\x7f\x85\xc1\x8b\x7f\x85\x6f\x61\x7f\x85\x80\x5e\x7f\x85\x6e\x61\x7f\x85\x12\x34\x30\x85\x64\x61\x7f\x85\x72\x61\x7f\x85\x45\x61\x7f\x85\xbc\x5e\x7f\x85\x5e\x61\x7f\x85\xdb\x8b\x7f\x85\xd2\x8b\x7f\x85\xed\x8b\x7f\x85\x16\x34\x30\x85\x1c\x34\x30\x85\x4c\x61\x7f\x85\xde\x8b\x7f\x85\xac\x5e\x7f\x85\x04\x34\x30\x85\x69\x61\x7f\x85\xe9\x8b\x7f\x85\x2a\x34\x30\x85\x9b\x5e\x7f\x85\xd4\x8b\x7f\x85\x76\x61\x7f\x85\x5a\x61\x7f\x85\x61\x61\x7f\x85\x79\x61\x7f\x85\xf6\x8b\x7f\x85\xe8\x8b\x7f\x85\x5f\x61\x7f\x85\x74\x61\x7f\x85\xf1\x8b\x7f\x85\x4a\x61\x7f"
                                   "\x85\x6a\x61\x7f\x85\x96\x5e\x7f\x85\xe3\x8b\x7f\x85\x75\x61\x7f\x85\xe5\x8b\x7f\x85\x41\x61\x7f\x85\x3a\x34\x30\x85\xb6\x5e\x7f\x85\x6d\x61\x7f\x85\xc3\x8b\x7f\x85\xa5\x5e\x7f\x85\x89\x5e\x7f\x85\x0a\x34\x30\x85\x99\x5e\x7f\x85\x33\x34\x30\x85\xfc\x8b\x7f\x85\x7c\x61\x7f\x85\x00\x34\x30\x85\xc7\x8b\x7f\x85\xa0\x5e\x7f\x85\x0e\x34\x30\x85\x1a\x34\x30\x85\xfb\x8b\x7f\x85\x84\x5e\x7f\x85\x4b\x61\x7f\x85\x4e\x61\x7f\x85\x23\x34\x30\x85\x11\x34\x30\x85\x46\x61\x7f\x85\x19\x34\x30\x85\x93\x5e\x7f\x85\xfd\x8b\x7f\x85\xf7\x8b\x7f\x85\x67\x61\x7f\x85\x3f\x34\x30\x85\xeb\x8b\x7f\x85\xbe\x5e\x7f\x85\x9a\x5e\x7f\x85\xba\x5e\x7f\x85\xb2\x5e\x7f\x85\xd8\x8b\x7f\x85\x2c\x34\x30\x85\x35\x34\x30\x85\x7f\x61\x7f\x85\xd9\x8b\x7f\x85\x55\x34\x30\x85\xe0\x8b\x7f\x85\xf0\x8b\x7f\x85\x5d\x61\x7f\x85\xa3\x5e\x7f\x85\x1e\x34\x30\x85\x25\x34\x30\x85\xc4\x8b\x7f\x85\x56\x61\x7f\x85\x9c\x5e\x7f\x85\x7a\x61\x7f\x85\xcd\x8b\x7f\x85\xd1\x8b\x7f\x85\x98\x5e\x7f\x85\xce\x8b\x7f\x85\x57\x61\x7f\x85\x66\x61\x7f\x85\xc0\x8b\x7f\x85\x36\x34\x30"
                                   "\x85\x8e\x5e\x7f\x85\x30\x34\x30\x85\x09\x34\x30\x85\x42\x61\x7f\x85\x01\x34\x30\x85\xd5\x5e\x7f\x85\x8f\x5e\x7f\x85\xb8\x5e\x7f\x85\xae\x5e\x7f\x85\xfa\x8b\x7f\x85\x63\x61\x7f\x85\x20\x34\x30\x85\x82\x5e\x7f\x85\xea\x8b\x7f\x85\x52\x61\x7f\x85\xa7\x5e\x7f\x85\xc2\x8b\x7f\x85\xb3\x5e\x7f\x85\x7b\x61\x7f\x85\x44\x61\x7f\x85\xca\x8b\x7f\x85\x1f\x34\x30\x85\xdc\x8b\x7f\x85\x14\x34\x30\x85\x40\x61\x7f\x85\x06\x34\x30\x85\xb0\x5e\x7f\x85\x0f\x34\x30\x85\x3e\x34\x30\x85\xef\x8b\x7f\x85\x58\x61\x7f\x85\x13\x34\x30\x85\x8b\x5e\x7f\x85\x87\x5e\x7f\x85\x31\x34\x30\x85\x71\x61\x7f\x85\x91\x5e\x7f\x85\xcb\x8b\x7f\x85\x24\x34\x30\x85\x05\x34\x30\x85\xcf\x8b\x7f\x85\xab\x5e\x7f\x85\x17\x34\x30\x85\x2f\x34\x30\x85\xa2\x5e\x7f\x85\xc8\x8b\x7f\x85\xaf\x5e\x7f\x85\x70\x61\x7f\x85\x07\x34\x30\x85\x6c\x61\x7f\x85\xe4\x8b\x7f\x85\x49\x61\x7f\x85\xcc\x8b\x7f\x85\x3d\x34\x30\x85\x8c\x5e\x7f\x85\xaa\x5e\x7f\x85\x21\x34\x30\x85\x3b\x34\x30\x85\x8d\x5e\x7f\x85\x47\x61\x7f\x85\x29\x34\x30\x85\xd0\x8b\x7f\x85\xdd\x8b\x7f\x85\x65\x61\x7f"
                                   "\x85\xd7\x8b\x7f\x85\xc6\x8b\x7f\x85\x94\x5e\x7f\x85\xa4\x5e\x7f\x50\xb1\x8d\x6c\x3a\xdb\xe7\x06\x4b\xaa\x96\x77\x31\xd0\xec\x0d\xb1\x50\x6c\x8d\xbb\x5a\x66\x87\xaa\x4b\x77\x96\xd0\x31\x0d\xec\x5b\xba\x86\x67\x31\xd0\xec\x0d\x40\xa1\x9d\x7c\x3a\xdb\xe7\x06\xfa\x1b\x27\xc6\xb0\x51\x6d\x8c\xe1\x00\x3c\xdd\x9b\x7a\x46\xa7\xd6\x37\x0b\xea\xbc\x5d\x61\x80\xcd\x2c\x10\xf1\xb7\x56\x6a\x8b\xb7\x56\x6a\x8b\x3d\xdc\xe0\x01\xac\x4d\x71\x90\xd6\x37\x0b\xea\x5d\xbc\x80\x61\x37\xd6\xea\x0b\x46\xa7\x9b\x7a\x3c\xdd\xe1\x00\xfc\x1d\x21\xc0\xb6\x57\x6b\x8a\xe7\x06\x3a\xdb\x9d\x7c\x40\xa1\x59\xb8\x84\x65\x33\xd2\xee\x0f\x42\xa3\x9f\x7e\x38\xd9\xe5\x04\xb8\x59\x65\x84\xb2\x53\x6f\x8e\xa3\x42\x7e\x9f\xd9\x38\x04\xe5\x52\xb3\x8f\x6e\x38\xd9\xe5\x04\x49\xa8\x94\x75\x33\xd2\xee\x0f\xf3\x12\x2e\xcf\xb9\x58\x64\x85\xe8\x09\x35\xd4\x92\x73\x4f\xae\xdf\x3e\x02\xe3\xb5\x54\x68\x89\xc4\x25\x19\xf8\xbe\x5f\x63\x82\xbe\x5f\x63\x82\x34\xd5\xe9\x08\xa5\x44\x78\x99\xdf\x3e\x02\xe3\x54\xb5\x89\x68\x3e\xdf\xe3\x02\x4f\xae\x92\x73\x35\xd4\xe8\x09"
                                   "\xf5\x14\x28\xc9\xbf\x5e\x62\x83\xee\x0f\x33\xd2\x94\x75\x49\xa8\xc6\x7c\x47\xfd\xc3\x79\x42\xf8\x32\x88\xb3\x09\x37\x8d\xb6\x0c\x8c\x36\x0d\xb7\x89\x33\x08\xb2\x78\xc2\xf9\x43\x7d\xc7\xfc\x46\xff\x45\x7e\xc4\xfa\x40\x7b\xc1\x0b\xb1\x8a\x30\x0e\xb4\x8f\x35\xb5\x0f\x34\x8e\xb0\x0a\x31\x8b\x41\xfb\xc0\x7a\x44\xfe\xc5\x7f\xa9\x13\x28\x92\xac\x16\x2d\x97\x5d\xe7\xdc\x66\x58\xe2\xd9\x63\xe3\x59\x62\xd8\xe6\x5c\x67\xdd\x17\xad\x96\x2c\x12\xa8\x93\x29\x90\x2a\x11\xab\x95\x2f\x14\xae\x64\xde\xe5\x5f\x61\xdb\xe0\x5a\xda\x60\x5b\xe1\xdf\x65\x5e\xe4\x2e\x94\xaf\x15\x2b\x91\xaa\x10\x50\xea\xd1\x6b\x55\xef\xd4\x6e\xa4\x1e\x25\x9f\xa1\x1b\x20\x9a\x1a\xa0\x9b\x21\x1f\xa5\x9e\x24\xee\x54\x6f\xd5\xeb\x51\x6a\xd0\x69\xd3\xe8\x52\x6c\xd6\xed\x57\x9d\x27\x1c\xa6\x98\x22\x19\xa3\x23\x99\xa2\x18\x26\x9c\xa7\x1d\xd7\x6d\x56\xec\xd2\x68\x53\xe9\x3f\x85\xbe\x04\x3a\x80\xbb\x01\xcb\x71\x4a\xf0\xce\x74\x4f\xf5\x75\xcf\xf4\x4e\x70\xca\xf1\x4b\x81\x3b\x00\xba\x84\x3e\x05\xbf\x06\xbc\x87\x3d\x03\xb9\x82\x38\xf2\x48\x73\xc9\xf7\x4d\x76\xcc"
                                   "\x4c\xf6\xcd\x77\x49\xf3\xc8\x72\xb8\x02\x39\x83\xbd\x07\x3c\x86\x92\x8f\xb1\xac\x8b\x96\xa8\xb5\x5e\x43\x7d\x60\x47\x5a\x64\x79\x37\x2a\x14\x09\x2e\x33\x0d\x10\xfb\xe6\xd8\xc5\xe2\xff\xc1\xdc\xc6\xdb\xe5\xf8\xdf\xc2\xfc\xe1\x0a\x17\x29\x34\x13\x0e\x30\x2d\x63\x7e\x40\x5d\x7a\x67\x59\x44\xaf\xb2\x8c\x91\xb6\xab\x95\x88\x6d\x70\x4e\x53\x74\x69\x57\x4a\xa1\xbc\x82\x9f\xb8\xa5\x9b\x86\xc8\xd5\xeb\xf6\xd1\xcc\xf2\xef\x04\x19\x27\x3a\x1d\x00\x3e\x23\x39\x24\x1a\x07\x20\x3d\x03\x1e\xf5\xe8\xd6\xcb\xec\xf1\xcf\xd2\x9c\x81\xbf\xa2\x85\x98\xa6\xbb\x50\x4d\x73\x6e\x49\x54\x6a\x77\x8d\x90\xae\xb3\x94\x89\xb7\xaa\x41\x5c\x62\x7f\x58\x45\x7b\x66\x28\x35\x0b\x16\x31\x2c\x12\x0f\xe4\xf9\xc7\xda\xfd\xe0\xde\xc3\xd9\xc4\xfa\xe7\xc0\xdd\xe3\xfe\x15\x08\x36\x2b\x0c\x11\x2f\x32\x7c\x61\x5f\x42\x65\x78\x46\x5b\xb0\xad\x93\x8e\xa9\xb4\x8a\x97\x72\x6f\x51\x4c\x6b\x76\x48\x55\xbe\xa3\x9d\x80\xa7\xba\x84\x99\xd7\xca\xf4\xe9\xce\xd3\xed\xf0\x1b\x06\x38\x25\x02\x1f\x21\x3c\x26\x3b\x05\x18\x3f\x22\x1c\x01\xea\xf7\xc9\xd4\xf3\xee\xd0\xcd"
                                   "\x83\x9e\xa0\xbd\x9a\x87\xb9\xa4\x4f\x52\x6c\x71\x56\x4b\x75\x68\xda\x52\xf1\x36\xdf\x88\xfa\xb8\xb7\x1a\x5a\xec\xcd\xf3\xdd\x56\x63\xb1\x05\xd7\xf3\x21\x95\x47\x92\x40\xf4\x26\x02\xd0\x64\xb6\x8c\x5e\xea\x38\x1c\xce\x7a\xa8\x7d\xaf\x1b\xc9\xed\x3f\x8b\x59\xa6\x74\xc0\x12\x36\xe4\x50\x82\x57\x85\x31\xe3\xc7\x15\xa1\x73\x49\x9b\x2f\xfd\xd9\x0b\xbf\x6d\xb8\x6a\xde\x0c\x28\xfa\x4e\x9c\x98\x4a\xfe\x2c\x08\xda\x6e\xbc\x69\xbb\x0f\xdd\xf9\x2b\x9f\x4d\x77\xa5\x11\xc3\xe7\x35\x81\x53\x86\x54\xe0\x32\x16\xc4\x70\xa2\x5d\x8f\x3b\xe9\xcd\x1f\xab\x79\xac\x7e\xca\x18\x3c\xee\x5a\x88\xb2\x60\xd4\x06\x22\xf0\x44\x96\x43\x91\x25\xf7\xd3\x01\xb5\x67\xe2\x30\x84\x56\x72\xa0\x14\xc6\x13\xc1\x75\xa7\x83\x51\xe5\x37\x0d\xdf\x6b\xb9\x9d\x4f\xfb\x29\xfc\x2e\x9a\x48\x6c\xbe\x0a\xd8\x27\xf5\x41\x93\xb7\x65\xd1\x03\xd6\x04\xb0\x62\x46\x94\x20\xf2\xc8\x1a\xae\x7c\x58\x8a\x3e\xec\x39\xeb\x5f\x8d\xa9\x7b\xcf\x1d\x19\xcb\x7f\xad\x89\x5b\xef\x3d\xe8\x3a\x8e\x5c\x78\xaa\x1e\xcc\xf6\x24\x90\x42\x66\xb4\x00\xd2\x07\xd5\x61\xb3\x97\x45\xf1\x23"
                                   "\xdc\x0e\xba\x68\x4c\x9e\x2a\xf8\x2d\xff\x4b\x99\xbd\x6f\xdb\x09\x33\xe1\x55\x87\xa3\x71\xc5\x17\xc2\x10\xa4\x76\x52\x80\x34\xe6\xbd\xf5\xe9\xaf\x87\xbf\x79\x2f\x7b\x12\x7d\xe7\x71\xbb\xf6\x1c\xfa\x17\xcc\x49\x3a\x65\x5f\x96\xce\x4c\x2e\x8f\xf7\xd4\x09\xa4\x63\xfd\x98\x8b\xcb\x65\x32\xfd\x80\xd5\xb9\x86\xf8\x54\x87\x1a\xe1\x44\x12\x49\x28\x4e\x13\xfa\xac\x69\xe0\xb5\x25\x1b\x4e\x54\x78\xae\x41\x8b\x6c\xf1\x82\x72\x0e\xc5\x94\xd1\xd7\xd0\x2f\x45\x75\x6d\x83\x60\xc7\xcd\x95\x80\xea\x86\x75\xed\x08\x5a\xed\xd6\x6d\x16\xfc\x64\x04\x02\x8b\x62\xaf\xdd\x9d\x50\x02\x6d\x33\x4e\xf2\xfd\x0d\xea\x73\x00\x3b\x48\x2a\x0f\xd0\x2a\x4b\x9f\x7c\x6c\x3e\xb3\xda\xe2\x57\x32\x7a\xb4\x17\x49\x95\x03\x16\xc0\x2c\xd6\xd2\xe9\x33\xc7\xf3\x02\x75\xd7\x21\xcd\xb0\x07\x5d\x13\x5a\xa0\xbe\x51\x94\x52\xe6\x2e\xd3\x71\x7e\x3b\x79\xa5\x85\xfc\xbd\x04\x37\x5d\x6b\xe4\x38\x26\xa4\xcd\x9d\x9b\xfb\xdf\x56\xcd\x00\x23\x33\x8b\xce\x98\xe4\x6d\x0e\xc3\xfe\xc9\x2e\x16\xd8\xc8\x13\x7b\x0c\x88\x50\xa2\x32\x76\x67\x26\x51\xa7\x40\xbc\x5c\xec\xe0\x2e"
                                   "\x60\x49\xe5\xe6\x13\x97\x28\x38\x22\x79\x0d\x6b\xf9\xab\xee\xac\xc1\x60\xec\x67\x52\x1b\x7e\x56\xed\xec\xb5\xd4\x9b\x0e\xc3\xc8\xc0\xa7\x55\x59\x6b\x83\xc2\x4b\xd6\x37\xc7\x92\xba\x9f\x29\x07\xd9\x3f\xab\x4c\x96\x45\x62\x65\xae\x22\x25\x7f\xaa\xf4\x99\xba\xa4\x1f\xfb\xf5\x3d\x0f\xa0\x3f\xa5\xe8\x83\xe3\x42\x1e\xd8\x9a\xa1\x36\xf7\x97\x03\x94\xca\x7b\x8b\x17\xd7\xab\x30\x37\xac\xfd\x43\xe9\x5e\x2c\x6f\x55\x37\x10\xb7\xa6\x25\x55\x5f\x58\x9d\xd3\x9c\x64\x82\xf1\x5a\x81\x99\x39\xd0\xa2\xef\x9d\x2b\xfa\x66\x9c\x93\x6c\x5e\xa7\x5b\x86\x20\x88\x86\x48\x00\xa2\xdb\x7c\x66\x17\x50\x60\xb9\x0d\xbc\x0a\x51\x21\xc5\x8e\x51\xb3\x1d\x6a\xb5\x4f\x14\xfd\xe8\x9e\x91\x2f\xa3\x7c\x31\xd2\x12\xcc\xc2\xe4\x91\x0a\xab\x0b\x21\x0d\xb9\x23\x55\x6b\x0d\x81\xe9\x34\x4a\x68\x05\x5b\xfc\x82\xf2\x5e\x54\x8e\x44\x52\xb0\xcc\xe5\x6d\x29\xb3\xaa\xc7\x2e\xe1\x6a\xee\x11\xd6\xec\xec\x49\xdc\xb8\xb7\xbb\x68\x90\xb8\x98\xb2\xf7\x2d\x65\x70\xe1\xa4\xc8\xe1\x87\x99\x2d\x65\x0f\x94\x72\x07\xc4\x79\x8d\x82\x9f\xc4\x18\x51\xb1\xbd\xad\x9e\x59\x23"
                                   "\x2c\x9c\xa6\x45\x7f\xc4\x01\x34\xb6\x61\x9c\x82\x68\x3f\xf8\xa6\xe9\x3a\x49\x89\x1a\x0c\x76\x66\x05\x07\xf4\x74\xb3\x78\x98\x08\x55\x79\xbd\x7f\x94\x06\x9f\x0e\xa7\xa3\xbf\x90\xca\x9a\xcb\xca\x4e\xb6\x78\x1e\x92\x93\xd7\x19\xa9\x50\xe4\x57\xb5\xe5\xe1\x1f\x81\x2a\x41\x39\x4c\xf5\xbc\x55\xee\x58\x08\x32\x88\xab\xa3\x46\x1b\xf5\x0c\x59\x7d\x87\x45\x01\x34\xe9\x0e\x42\x10\x2b\x56\xda\x27\x62\x09\x21\x20\x34\x4a\xba\x46\xd0\x62\xde\x39\xe1\x1a\xfc\x8a\xf0\x5f\x1d\xd5\x8b\xf3\xef\xdf\xaa\xd3\x63\x45\x74\x17\x44\x0b\xee\x90\x3b\xd3\x1e\xcb\xf8\x84\x03\x45\xd3\xff\x3e\xc8\xc3\x23\x90\x74\x5c\xe7\xd9\x6b\x5f\x90\xd8\x1b\x4b\x24\xe6\xb4\x85\x69\xc8\x7e\x18\x59\xc5\xe4\xdc\x15\x04\x52\x30\x64\x97\x58\x32\xe3\x07\xcf\x1b\x40\x5d\x1b\xca\x8e\x3e\xdb\x61\xa6\x5c\x36\x3f\x4d\x0a\x46\x84\xda\x83\xcf\xa9\xe8\xc5\xb1\x36\x1c\x9f\x4c\x82\xfd\x7d\x8b\x70\x7a\xed\x05\xda\xb4\x1a\x59\xb1\xcd\xf8\x8b\xf3\x8f\xc9\x63\x6f\x9a\xf9\x3b\xf7\x12\x72\x91\x89\xdd\xf1\x0f\x11\xf4\x70\xb4\xfe\x97\xba\xdb\x73\xc6\x32\x2d\x4f\x99\x4d\x7e\x9b"
                                   "\x70\x44\x7f\xaa\x9e\x27\xbf\x42\x8c\x7d\x16\x13\xec\x13\x4d\x03\x3b\x9a\xd6\x28\xb8\xdc\xed\x5d\x6e\xaa\xbe\xc1\x82\x96\x7d\xdd\x0a\x13\x2a\x8e\x61\xbe\x5c\xd8\x1e\xde\x08\xb3\x0f\x3c\x2e\x00\xf0\xbe\x49\x3b\x00\x30\xf0\x16\x53\xe4\x04\x69\xb2\x8f\x21\xb7\xc4\x79\xe8\x25\x07\x46\x2f\xc0\xf5\x97\x4d\xd1\xe2\x00\x68\xad\x95\x01\x18\xc1\x76\xd9\x36\xc6\xa3\x75\x3a\xcd\xb1\x33\x5d\x5b\xc3\x1b\x29\xbd\x62\x02\x22\x3e\x47\x37\xdc\xf0\xc9\x26\xef\x27\xcc\x0f\xf3\xe5\x89\x54\x1a\x79\xa8\xb7\x5d\x69\x35\x20\xa8\x8f\xcf\xb3\xc7\x40\x9f\xe0\x37\x95\x77\x2e\xc0\x13\x79\x59\xb8\x3c\x44\x8b\x9e\x95\xa0\xd1\x7f\x29\xde\x55\x6a\x75\xdc\x16\xa7\xbe\xbf\xb6\x2d\x85\x36\xa4\xd2\xf2\xd4\x74\x8a\xe1\x01\xd9\x86\xa8\x19\xb8\x48\x0c\x7c\x80\xbc\xce\x06\xbb\xc6\x11\x58\x3a\x5e\xea\xfb\xf0\x32\x78\x4f\x51\x01\xb1\x83\x71\x04\xec\x2f\x28\xd4\xe0\x1f\x23\x72\x24\x6a\x7c\x3b\x7e\xef\xaf\x71\x9f\xf6\x2b\x71\x35\xa2\x8a\xc3\xfa\x26\xa7\x70\xae\xd1\x5d\x86\x2b\xeb\x81\x0c\xdc\x48\x23\x42\x0a\x74\x92\xfa\x6e\x3c\xf8\x16\x92\x3f\x4c\x63\xf4"
                                   "\xe5\x92\xa7\x14\xe0\xbb\xab\x76\x41\xa2\xa2\xf7\x66\xd4\x9c\x01\x09\xaf\x54\xe8\x5e\xaf\x24\x06\xf1\x49\xb0\x8c\x67\x2b\x26\xd0\xde\x3e\xbe\x3a\x83\x12\xfa\x19\x65\xfb\x25\xc7\x7b\xa0\x5b\xb8\xed\x9c\x70\x1f\x5f\x35\x81\x7a\x1f\xb9\x49\x2d\xaf\xbb\x96\x82\xa1\x95\x10\x6a\xe2\xe5\x87\x79\x05\x5b\xd3\x21\xf9\xee\x99\x58\xfc\x4e\x1d\xaf\x5a\xa6\xab\x97\xad\x52\xf4\xbb\xe5\x63\x76\xfd\x30\xd5\xbf\x63\xc8\x8d\xce\x61\x5d\x20\x5a\x13\x6e\x65\x78\x59\x22\xb9\xc1\x64\x4e\x01\x9b\x98\xc0\xa7\x11\xcb\x06\x30\x19\xde\x28\xb5\x6b\xbd\x29\xf8\xd4\x31\x2b\xde\xa1\x42\xe0\xc3\xf2\x0a\x77\x48\x13\x0b\x20\x9f\xb4\x17\x64\x69\xd2\x80\x52\x8c\x74\x3d\xa6\x13\xe1\xee\x7e\x2f\x73\x2d\xe8\xe9\x2d\xa0\x19\xd6\x3c\x84\x02\xdd\x22\xa5\xbd\x18\xff\xcf\xfd\x03\xa2\x23\xe6\x08\xbc\x02\x58\x80\xde\xe4\xa3\xb3\x65\x19\x63\xef\x23\x04\xa9\xbf\xcf\xc0\x68\xae\x36\x51\x4f\x4c\x24\x14\xb9\xf5\xc4\xb4\x9c\x31\xa3\x82\x6f\x28\xc7\xd5\xfe\x68\x68\xdc\x5e\x4b\x90\xec\xb2\xb4\xd1\xe1\xec\x04\x16\xdb\xc5\x07\x95\x3c\x0b\xba\x42\x83\x17\x37\xad\x26"
                                   "\x2c\x58\x50\xc6\x27\x19\x45\x93\x1b\xf0\x49\xf7\x92\xd0\x32\x20\xca\xab\xbb\x12\x88\x96\x93\x8d\x0a\xf7\xfd\x0f\x33\xbe\x75\x9c\xb4\x7f\x9f\xe9\xfb\xc8\xec\x2b\xd1\xa0\xa5\x33\xef\x6f\xdc\x24\xd2\xcb\x6f\xcc\x34\x38\x84\x18\x87\x3a\xbd\xa3\xb3\xf9\x6e\x6d\xa0\xd8\xaf\xe6\xc1\xea\xae\x47\xdd\x67\x41\xe2\x37\x53\x4e\xe7\xa8\xf2\x70\x4c\xac\x1f\x4b\x37\xce\x46\x80\x69\xd7\x6b\xeb\x3b\xc3\xcc\xdb\x34\x16\x7a\x12\xaa\xd9\x8a\x7a\x99\x8f\x10\x62\x09\xe7\x45\x03\x8e\xb6\x59\xea\x9a\xe1\x8e\x4d\x86\x45\x40\x8e\xcd\xd3\x86\xd0\x40\x1e\x50\xcd\x00\xba\x9e\x0e\x4b\x96\x3d\x09\x5b\x39\xb2\xdf\x45\x54\x47\x3a\x35\x90\xf6\x47\x53\x72\xe8\x97\xfc\xeb\x82\xe7\x5f\x6d\x0e\xb2\xa6\xb7\x14\x55\x16\x9a\xfa\xed\x8a\x57\x2c\xf0\xca\xf2\xaf\x8c\x0d\xf5\x29\x7d\x89\xed\x49\xa9\x57\x12\x97\x29\xd1\x86\x77\x02\x2f\x3f\x79\x91\x4d\xb5\x32\x20\x65\xd0\xed\x1a\xbf\x80\xbc\x4c\x27\x56\x61\x4f\x46\x78\xe4\x3d\x25\xe4\x2e\xc9\x71\x50\xaa\x01\x4e\x85\x1c\xc8\xd0\x51\xe7\xbe\xc2\xd8\xc7\xc5\x15\x75\x6e\x66\x78\x32\xf3\xca\x10\xbe\x73\x35\x30"
                                   "\xbb\xd3\xb1\xc7\x11\xfc\xe3\x2e\x5f\x06\x2f\x60\x89\xdb\x2c\x01\x8a\xb0\xe6\xfe\x8d\x36\x17\x7a\x46\x2b\x44\x32\x49\x87\x6a\x1c\x1f\x1d\x72\x8c\x13\xda\x96\x5d\x82\x9a\x39\x54\x3c\x12\x5b\xb2\x14\x5c\x67\xd9\x4a\xec\xa0\xe3\xcf\x0b\x3f\xe5\x21\xd2\x0b\x9b\x08\xd1\x88\x7c\x26\x54\xfa\x1f\x3d\x5f\xe4\x3e\xbf\x3e\x8a\xbc\x0d\x71\x0c\x8b\x83\xd7\x86\xd8\x79\xa9\x82\xa9\x01\xb6\xe8\x5a\x09\x9c\x37\xf0\x93\x9d\x8d\xac\x44\x0d\x31\x41\xe3\xa8\x38\xf5\x8e\x5d\xdd\x85\xff\x25\xd7\x50\xd4\x00\x21\xc4\xe9\xa4\x92\x2c\x9b\xb7\x52\x06\x95\x56\xc3\xa4\xee\x22\x63\xa8\x07\x7d\xa6\x52\x1a\xbd\xf6\x7b\x74\x23\xd9\xf4\x43\x8b\xc0\xc5\xc2\x81\x64\xb8\xaa\xd4\x05\x3f\xbc\x55\x40\x43\x10\xb1\x5c\xa2\x60\x84\xe9\xfb\x5b\xeb\x14\x1b\x4b\xa1\x1f\x6f\x6b\xc5\xfc\xae\x0f\x57\x79\xf8\x65\x24\x6d\x0c\xf6\x42\xb7\x76\x2a\x93\x1e\xce\x42\xc6\x7f\x49\xc9\xc0\x71\xed\x18\x9b\x83\x08\xf7\x0f\x08\xfa\xcc\x60\xf5\x1a\xab\x99\xba\xb3\xb1\xdf\x1b\x1e\x61\xc9\x56\x77\x67\x02\x18\x7f\x84\x51\x77\x5c\x35\x75\x3b\x94\x7f\x62\xcc\xa1\x9f\x5a\x69\x7d"
                                   "\x04\x16\x6c\xad\x62\xa2\x9c\x88\x7d\x44\xb9\xd2\x00\xfb\x57\xd6\xb0\x92\xa4\x92\xa5\x78\x2b\x11\x94\x1b\x7c\x28\xdf\x41\x34\x91\xde\x0c\x8b\x1d\xf4\x64\xc2\x05\x8c\x7b\xa8\xf6\xa7\x5e\x5e\x62\x9e\x17\xd6\xf1\xaf\x74\x81\xc8\x97\x70\xb6\xd7\x70\xce\xe2\x8f\x53\xc1\xcb\xb1\x7b\x8f\xf7\xda\x7a\xc2\x48\x56\x25\x3f\x30\xe0\xdb\xac\x0f\xea\x38\xff\x60\xc9\xc7\x21\xe0\x4f\xd6\x26\x54\xb7\x1d\x3b\x07\xff\x47\x66\xfb\xbe\x24\x72\x8f\x6c\x69\xe3\x89\xdd\x98\xdc\x98\xf9\x36\x1e\xf1\x6b\xda\xe1\xb0\x66\x66\x4f\xa7\xf3\xc6\x6c\x5f\xc3\xf3\xe2\x33\x81\x59\xcd\x61\x68\x2d\x15\xef\x4a\xf0\x89\xf9\x7e\x48\xca\xd5\x90\x15\x11\xd8\x55\x6a\x88\x43\x22\x31\x98\x00\xef\x91\xbb\xf8\xdf\x6c\x43\x0d\x2a\x4c\x27\xee\xeb\x41\xad\xb5\xb6\xae\x39\x3e\x44\xf8\xa3\x26\xd4\xfa\x85\x53\xa7\x2e\x7e\x25\xb5\x55\x0a\x85\xb9\xcd\x2d\x4a\x96\x3b\x94\xaa\x36\x5c\x6d\xe5\x9f\x1c\x76\xb8\x73\x4d\x6a\x51\x67\x9d\x7c\x1c\x0e\x0e\x1a\xc6\x74\x03\x90\x9d\x29\x0c\x3c\xb3\x07\x3e\x34\x2e\xc1\x99\x91\x27\x75\xd5\x4d\x9e\x48\x76\x05\xac\x87\xa4\x35\x94\x9d"
                                   "\x7c\x09\x06\x5e\x3a\xd9\x15\xba\x40\xe0\x0a\x3a\xc4\x4a\x2a\xb0\x71\x83\x5d\x03\xa2\xfe\xda\x95\x8b\xfd\x59\x72\x81\xf1\xf3\xab\x73\xa5\x28\x70\xea\xcf\x58\xd3\xdc\x2a\xfe\x6e\x2f\x33\x9a\x39\x23\xf4\x7e\xe8\xcb\xe6\x04\x9e\xb8\xb8\x7b\x38\xf1\xc4\x46\xf2\x55\x67\xf1\xc3\x81\xb3\x25\x17\xef\xdd\x4b\x79\x3b\x09\x9f\xad\xe1\xd3\x45\x77\x35\x07\x91\xa3\x5b\x69\xff\xcd\x8f\xbd\x2b\x19\xd7\xe5\x73\x41\x03\x31\xa7\x95\x6d\x5f\xc9\xfb\xb9\x8b\x1d\x2f\x63\x51\xc7\xf5\xb7\x85\x13\x21\xd9\xeb\x7d\x4f\x0d\x3f\xa9\x9b\x7b\x49\xdf\xed\xaf\x9d\x0b\x39\xc1\xf3\x65\x57\x15\x27\xb1\x83\xcf\xfd\x6b\x59\x1b\x29\xbf\x8d\x75\x47\xd1\xe3\xa1\x93\x05\x37\xf9\xcb\x5d\x6f\x2d\x1f\x89\xbb\x43\x71\xe7\xd5\x97\xa5\x33\x01\x4d\x7f\xe9\xdb\x99\xab\x3d\x0f\xf7\xc5\x53\x61\x23\x11\x87\xb5\x3c\x0e\x98\xaa\xe8\xda\x4c\x7e\x86\xb4\x22\x10\x52\x60\xf6\xc4\x88\xba\x2c\x1e\x5c\x6e\xf8\xca\x32\x00\x96\xa4\xe6\xd4\x42\x70\xbe\x8c\x1a\x28\x6a\x58\xce\xfc\x04\x36\xa0\x92\xd0\xe2\x74\x46\x0a\x38\xae\x9c\xde\xec\x7a\x48\xb0\x82\x14\x26\x64\x56\xc0\xf2"
                                   "\x12\x20\xb6\x84\xc6\xf4\x62\x50\xa8\x9a\x0c\x3e\x7c\x4e\xd8\xea\xa6\x94\x02\x30\x72\x40\xd6\xe4\x1c\x2e\xb8\x8a\xc8\xfa\x6c\x5e\x90\xa2\x34\x06\x44\x76\xe0\xd2\x2a\x18\x8e\xbc\xfe\xcc\x5a\x68\x24\x16\x80\xb2\xf0\xc2\x54\x66\x9e\xac\x3a\x08\x4a\x78\xee\xdc\xca\x2b\x56\xbc\x51\x8e\x41\x4f\xb8\x73\xaf\xf0\x8c\xa7\x37\xd4\x3a\xb6\x24\x6f\xd2\x92\x49\xdb\x8a\x84\x74\x87\xa9\xde\x23\xde\x33\xa2\xb7\x26\x93\xe4\xe8\xeb\x1b\xd7\x0c\x7d\x50\x57\xc2\x44\x1e\x16\xb3\x6e\xde\x47\x65\x81\x3c\x95\x67\x3c\x25\xf5\xfb\x50\xac\x1f\x9c\xcd\xd8\x64\x26\xd2\x69\x8f\xf5\x31\x10\xf8\xe0\x7f\x71\x36\xea\x56\x37\xba\x8b\x3e\x41\xfa\x4e\x6a\xb6\x9d\xfc\xe1\x63\x79\x9a\x38\x5f\x60\x12\x5e\xae\x24\xe3\x86\x8b\x5d\xf7\x8c\xa8\x07\xa0\xd5\x26\x17\x07\x10\x0f\xbb\x3f\x40\x06\xaf\xac\x09\xef\x52\x42\xb6\x1d\xf4\x4f\x2e\xb0\xbe\xbf\xb2\xbd\xb2\x10\xe3\x14\xe0\xdc\x67\x44\x3b\xf1\x79\x00\x8c\xef\x5a\x7f\xd8\xb9\x47\xe0\x65\x92\xac\x7c\x3a\x45\x07\xe7\x9f\x52\xf4\xe5\xa4\x2d\xbf\x07\x76\x2f\x02\x40\x23\xcd\x61\xa4\xd2\x8c\x8f\x74\xf7\x55\x45"
                                   "\x04\x94\xd3\x42\x96\x25\x57\xf8\xe2\x5e\xed\xe7\x2c\xe1\x68\x19\x15\x39\x5f\x6c\x4a\xd5\xa2\x68\xad\xc6\x1f\xc6\x58\x9a\xd2\x06\xba\x48\xd0\xbb\xb5\x7f\x00\xa1\x85\xb3\xa4\x9d\xed\x69\x3d\xfd\x2d\x38\xeb\x12\x64\x83\x5a\x60\x65\x5a\xd9\x6b\xc6\xfe\x7a\xe6\x83\x90\xe7\xce\xd4\xb1\x0a\x88\x4d\x2f\x62\x30\xcd\xd1\x96\xe4\xe3\x87\x6e\xec\x6b\xb4\x8a\x7a\xfb\x3e\x71\x8b\x02\xb7\x90\x11\xc9\xc9\xaa\xfc\xa6\xe9\xf3\xc4\x6c\x4e\x4a\x22\xdb\x86\xda\x92\xdf\x9e\xe6\x8a\xd9\xbd\xa5\xd9\xd0\xa9\x36\x90\x31\x99\xc8\x6d\x7d\xe3\xc6\x0c\x2f\x03\x94\x59\x24\x2c\x78\x5b\x9e\xe8\x47\xba\x59\x43\x51\x0d\xe8\xa8\x82\xee\xf1\xc8\x1e\x82\x4e\xcd\x9e\x70\x9d\x0a\xbb\xfa\x3f\x77\x9b\x7c\xc3\x3f\xc5\xf5\xfd\x1d\x32\xd8\x5e\xb9\x91\x55\x97\xfc\xd4\xf3\xb1\x67\x3c\xb9\x61\x42\xe5\x73\xec\xb0\xbe\xf6\xc7\x27\xf9\xed\x87\x88\xdb\xd6\x68\x56\x76\x3a\x73\x0d\x95\x1d\x3b\x6f\xa7\x64\x5d\x5b\x6d\x15\x38\x8d\x5b\x24\x6d\x97\xc9\x29\x34\x58\x77\x7e\x76\xcc\x2a\x0e\x03\x6e\x13\x1a\x16\xdb\xa3\x2c\x6a\x6d\x09\x71\x54\x4f\xfe\x5c\xb2\x85\xc0\xf9"
                                   "\xa3\x28\x4c\xd7\xf0\x11\x9d\x89\x27\xce\x84\x1b\x29\x20\xd7\x0a\xd5\x68\x89\x83\x39\x54\xd8\x2f\x8e\x9c\x48\x9f\x18\x35\xf0\x3d\x5b\x78\x2e\x46\xdc\x7c\x1a\xca\x0d\x80\x40\x0b\x90\x06\x14\xab\xcf\xea\xe9\xaf\x9b\x29\xf8\xa9\x2b\x1b\xa8\x41\xea\x93\xfd\xa5\x84\x6a\x27\x96\x1c\x2d\xcc\x25\xc2\xe6\x46\xfe\x80\x72\x1b\x8e\xf8\xdc\x8d\xcb\xda\x5f\x59\x99\x0e\x62\xbc\x4b\x6e\x75\x35\x69\xf9\x05\x0e\xc0\x92\x3d\x6b\xe0\x47\xd9\x0d\x39\xc8\x10\x29\xf7\x77\x15\xa9\x05\x01\x55\x6c\x51\x7e\x01\x3a\x4c\x0c\x59\xc3\x00\x08\x41\xff\x18\x30\x40\x4b\x66\xe9\x71\x01\xe5\x3e\xae\x18\x77\x52\x6c\xbd\x0f\x5a\xa1\xad\x4d\xaf\xfd\x60\x8d\xe1\xbc\x11\xa7\x05\x4d\x50\x49\xee\x8b\xc1\xbd\xd7\x53\xf6\xc8\xdd\xa5\x99\xc1\x17\x02\x20\x27\xf5\xd0\x22\x9a\x9f\x31\xc4\xb1\x72\xd4\x16\x16\xf2\x2a\xe2\xc2\xa7\x30\x70\xcf\xf6\x32\xde\xda\x70\xef\x69\x5d\x46\x00\x8e\x32\x53\xb5\x3e\x04\xff\x26\x4d\x93\x88\xbf\x0b\xcc\xc4\xc5\x05\xad\x48\xee\xdd\x23\x6f\xac\xb6\x62\x55\x96\x7d\x57\x9c\xd3\x38\xf1\x66\xb8\x25\x2b\x89\x66\x88\xc7\x8d\x7e\xb4\xdf"
                                   "\x4b\x0c\x21\x63\xab\xe5\x5c\x95\x91\xdf\x97\xa0\x4c\xf6\xe1\x3b\x2e\xda\x17\x52\x81\xab\x98\x85\x8f\x45\xcb\x94\xa5\x0b\x0f\x84\x82\x49\x64\xc5\x79\xfb\xfa\x14\xc1\x04\xba\xbe\xe6\x46\xd1\xff\x75\x2e\xd6\x4e\xb3\x5c\x43\xf2\xa0\xca\xb0\x97\x78\x22\x79\x1f\x67\x61\xa6\x20\x28\xf9\x54\x01\xeb\x4a\x7e\xae\x20\x34\x44\x43\x09\x98\x7c\x13\xd6\x8a\x75\xc3\x19\xec\x73\x36\xd3\x4b\xca\xd0\xa2\xf1\xcf\xdc\x45\xe2\x72\x72\x11\x21\x63\x74\x23\xd6\xb8\x03\xc5\x1c\x86\xa6\x56\x74\x81\x17\x43\xc1\x31\x21\xbb\x91\x53\xb0\x94\x1e\x28\xb3\x98\xcb\x04\xe9\xe4\x7d\xae\xb4\x60\x9b\x66\x78\x36\x63\x08\x35\xf3\xf3\x61\xc9\x5c\x82\xee\x1e\x57\xad\x02\x1c\xfe\xff\xce\x98\x1a\x0e\x8f\x76\x0a\x7a\x80\x53\xb7\x44\x7f\xea\x62\xa0\x19\x33\xcc\x08\x15\xef\xb4\xa6\x83\xaa\x13\x1a\x1c\x3f\xc0\xdd\x39\xb5\x49\x37\x5e\x28\xb9\xaa\x2c\xfb\x99\x12\x87\xe2\x7a\x19\x06\x54\x0b\xa3\x03\x58\x86\x51\x58\xdd\x9a\xf0\x7b\xa2\xfa\xe7\xf2\x80\x32\x7b\x34\x2d\x3d\x4c\xe4\x37\xbf\x89\x6f\xa8\xf7\xeb\x5d\xd1\x42\x18\xb2\x2a\xcb\xf2\xd5\xb7\x12\xc3\x9f\x34"
                                   "\xd1\x70\xb5\x9b\x22\x0f\x3b\x08\xbe\x50\xec\xa3\xce\x33\x6a\xa4\x95\xc7\xab\xb8\x4f\x14\x1d\x7b\xbc\x6b\x93\xe8\xaa\x3c\xdf\x9e\xfc\xc4\xb1\xd3\x2a\xc2\x2b\x4a\x1f\xcf\x30\x65\x7b\xc0\x85\x5f\xa1\x13\x33\x9c\x35\x81\xf4\x75\x21\xed\xc7\x48\xf4\x09\xa1\x91\x27\xb9\x40\xde\x5a\xc4\x3d\xa3\x81\x1f\xe6\x78\xfc\x62\x9b\x05\x30\xae\x57\xc9\x4d\xd3\x2a\xb4\x96\x08\xf1\x6f\xeb\x75\x8c\x12\x6c\xf2\x0b\x95\x11\x8f\x76\xe8\xca\x54\xad\x33\xb7\x29\xd0\x4e\x7b\xe5\x1c\x82\x06\x98\x61\xff\xdd\x43\xba\x24\xa0\x3e\xc7\x59\x91\x0f\xf6\x68\xec\x72\x8b\x15\x37\xa9\x50\xce\x4a\xd4\x2d\xb3\x86\x18\xe1\x7f\xfb\x65\x9c\x02\x20\xbe\x47\xd9\x5d\xc3\x3a\xa4\xda\x44\xbd\x23\xa7\x39\xc0\x5e\x7c\xe2\x1b\x85\x01\x9f\x66\xf8\xcd\x53\xaa\x34\xb0\x2e\xd7\x49\x6b\xf5\x0c\x92\x16\x88\x71\xef\xec\x72\x8b\x15\x91\x0f\xf6\x68\x4a\xd4\x2d\xb3\x37\xa9\x50\xce\xfb\x65\x9c\x02\x86\x18\xe1\x7f\x5d\xc3\x3a\xa4\x20\xbe\x47\xd9\xa7\x39\xc0\x5e\xda\x44\xbd\x23\x01\x9f\x66\xf8\x7c\xe2\x1b\x85\xb0\x2e\xd7\x49\xcd\x53\xaa\x34\x16\x88\x71\xef\x6b\xf5\x0c\x92"
                                   "\x5a\xc4\x3d\xa3\x27\xb9\x40\xde\xfc\x62\x9b\x05\x81\x1f\xe6\x78\x4d\xd3\x2a\xb4\x30\xae\x57\xc9\xeb\x75\x8c\x12\x96\x08\xf1\x6f\x11\x8f\x76\xe8\x6c\xf2\x0b\x95\xb7\x29\xd0\x4e\xca\x54\xad\x33\x06\x98\x61\xff\x7b\xe5\x1c\x82\xa0\x3e\xc7\x59\xdd\x43\xba\x24\xb0\x2e\x21\x25\x10\x2d\x80\x75\x53\x3c\x3f\x91\xd5\xe4\x3c\x8e\x99\xf3\x0f\xbc\xee\x03\x94\x49\xdc\xfa\x48\x33\xd4\x6c\x08\x77\x4e\x00\x35\x19\x98\x7b\x3b\x45\x33\xae\xc8\xab\xf8\xe9\xcc\x7f\x32\x26\xfc\x52\x71\x37\x43\xb6\xd3\xfc\xc4\xe5\x27\x8c\xb6\x9e\x1e\xa3\x38\x5a\x65\x15\x3d\x83\x8a\x41\xbd\x1b\x16\x35\x78\x1e\xcb\x98\x24\xfc\xfe\xf1\x34\x14\x49\x90\xf9\x8b\xe3\xcd\x3e\x9c\xef\x8b\xa0\xb0\xe1\x05\x18\x9f\x6c\x0b\x49\x3e\x30\xee\xda\x51\xaa\x82\xe7\x3f\xda\xe2\xb0\x58\x9d\x23\xd1\xd4\x81\x97\xef\xa5\x02\x17\x06\x2b\x97\x7d\xb7\x93\x39\xf0\xae\xec\x6d\x83\x7d\xc7\x15\x75\x6a\xe4\x9b\x3b\x29\xbf\x89\x01\xaf\xe1\x26\x04\x82\x67\x5d\xb2\x87\xbe\x77\x2f\xbb\xdd\xe5\xd5\xc6\xf7\x5b\xaa\x7f\xd5\x00\xdf\x20\x28\xbc\x68\xbf\x09\xa3\x9c\x93\x82\x8f\x19\x57\x8a"
                                   "\x37\x7e\x16\xc3\x56\x64\xd5\x00\x8b\xc9\x89\xe2\xc4\x9e\xa8\x2a\x38\x78\x9a\x15\x61\xc5\xe3\xeb\xc8\xd8\x36\x06\x63\x0d\xc5\xe8\x07\x4f\xec\xba\xcd\x80\xdc\x97\x08\x49\x60\x6c\xdd\x72\x7c\xca\xd0\xbc\xd6\x1f\x0f\xd9\xac\xfe\xe4\x5d\xf2\x0e\x74\x6f\xa9\x27\x03\x9f\x32\xd2\x4a\xd0\xeb\x71\x3d\x20\x70\x84\x95\xb5\x91\x90\x52\xb4\x0b\x68\x3b\x38\x88\xef\xa8\x4a\xc1\x3c\x1f\x2b\x0c\xa3\xe6\x95\xd4\x0d\x47\x1e\x41\xa4\xd7\x2c\x1a\x8d\xbb\xf8\x73\x9b\x54\xac\xf3\x03\x19\x33\xf4\xc8\xf4\xaf\x52\x53\xd1\x34\xe2\xe6\x2b\xca\x28\xb2\x9f\xeb\xf7\xd7\x04\x0f\xfe\x40\xdf\xba\x5a\xc9\x9e\x63\xc3\x2e\x9c\xab\xe5\x2d\xb9\x30\x55\x98\x1a\x73\xe6\x32\xfb\xa9\xde\x85\x12\xe5\xa6\x76\x6b\x9b\x85\xac\xa2\x14\xa7\x7b\x36\xf6\x22\x3a\xbd\xe0\x8b\xf0\xc9\x50\x02\xff\x0a\x81\x46\x6f\xb7\xbe\xed\xb7\xbe\xa0\x99\x0a\xc6\x56\x8e\x29\xaf\xda\x0d\xae\x1d\xe3\x2a\xa0\x67\xdd\x1b\x80\x22\xd4\x5c\x0f\x87\x8f\x17\xce\xfd\xb1\x26\xee\xf2\xb7\xaa\x38\x84\xcf\x05\x34\x7e\x31\xcf\x60\x6a\x13\xb1\x55\x0d\x11\x8a\xfd\xeb\x5b\x7e\xd8\x3f\xe8\x56\x87"
                                   "\x94\x3d\xa5\x69\x7c\xf9\xe9\x63\x2e\x92\xc2\x23\xca\x10\x10\x05\xe2\x45\x0a\x65\x96\xf5\x83\x6a\x66\x55\x2f\x79\x73\xff\x65\xb5\xab\x0a\xd3\xc6\x57\xec\xe1\xf9\x3c\xa8\x44\x7d\x13\x6d\x92\x8f\x64\x9d\x09\x7a\x23\x5c\x68\xf6\xed\x43\x86\xb3\x68\xdb\x97\x56\x31\x66\xee\xa8\xc0\x4e\x76\x42\x24\xcc\xa4\x64\x75\xe7\x9d\xde\xf0\x7f\x8c\x3b\x4b\x58\xdf\x88\x41\x06\xb9\xcf\xbf\x28\xad\xf3\x2f\x1a\xf6\xda\xc1\xc6\x42\xbb\x60\x4d\xd7\x12\xa7\x4c\x4d\xea\x2a\x42\x1c\x4b\x90\xed\x7b\x01\x70\xbf\x77\x4f\xfc\x39\x12\x17\x5a\x22\x4b\x2c\xc7\xde\xba\xd0\x78\x29\x37\x0b\xa6\xc4\x79\x13\x62\x85\xf1\x11\x09\xc1\x54\x95\xc3\x0e\x64\xb8\xc5\x16\x9c\xd3\x5e\xf2\x95\x44\x82\xd7\xfd\x5f\x44\x5e\x53\x5e\xf6\x67\x74\x50\xce\xc0\xce\x6d\x0b\x09\x72\x96\x8d\xd1\x71\x89\x0e\x51\x98\x07\xa4\x0c\x5f\x10\x45\xd6\x67\xa7\x55\x24\xc7\xfa\x48\x18\xcd\x72\xa9\xc2\xf5\xc5\xb1\xa6\x15\xdc\x6f\x4b\x5b\xc4\xb2\xe6\x07\x26\xd8\x2a\x96\x5b\x58\xea\x6d\x2f\x5f\x7a\xa1\xbd\xb5\x76\xcb\xb4\x42\x46\xab\x35\x2d\xd2\xd0\xd9\xad\x12\x2b\xad\xff\x79\x00\xed"
                                   "\xba\x70\x47\x62\xf1\xf7\xb8\xc2\x91\x65\x4f\xf8\xae\x52\x39\x57\x85\x47\x31\xcd\xcf\x48\xfa\x94\x8e\x91\x63\x73\x59\x62\x59\xd6\x4f\x88\x01\xe0\xe9\x93\x58\xdb\xe8\x1b\x6c\x22\xb8\xb8\x61\x61\x28\x8a\x3a\x48\xfa\x21\xea\x7c\x3e\x60\x62\x7e\x35\xb6\x30\xc0\x76\xa7\x8f\x24\x46\x96\x75\x5d\x69\x53\xa3\xaf\x3a\xb0\xbc\x16\xf5\x27\x66\xaa\x72\x77\x51\x4c\x7f\xb9\xfb\x99\x20\x1c\x7a\x0c\x4c\xc8\x13\x1a\xdb\x6a\x84\xa1\x92\x25\x5d\x02\x83\x5f\xc9\xa6\x05\x87\xca\xb9\xe7\x1d\xe0\xf4\x1c\x6b\x1e\x59\xa5\x84\x6b\xe9\xf3\x3f\x9e\xc1\x86\x07\x23\x37\xf7\xef\x40\xa9\x1b\xfb\xd2\xcb\xde\x32\x6e\x30\xd2\x74\xf0\x1c\xb3\x6e\x33\xdf\xac\x9a\x1f\x54\xe0\x8d\x2c\x66\x80\x1f\xdb\x5c\x18\xbb\xc0\x31\x8c\x59\x45\x70\xd9\xa2\xa2\xa2\x7d\x71\xdd\x9a\xb6\x36\xd9\x4e\x50\x7c\x2d\x6b\x34\x3e\x04\x39\x0c\x99\xbe\x04\xa1\x54\xb5\x81\xa0\xdc\x81\x78\xd6\xa4\x2e\x74\x93\xad\x69\xfb\x21\x94\x4e\xf5\x7a\xe1\x11\x08\x14\xfd\x5e\x1d\x79\xa1\x03\xf2\x6e\xc3\x6f\x3d\x2c\x5a\xe4\x20\x88\x89\x9b\x18\x9a\xb3\x1d\x46\x7b\x69\x25\xf1\xea\xd3\x4a\x21"
                                   "\xc2\x86\x50\x41\x5c\x3a\xb3\x47\x29\x02\x0e\xb1\xf9\x61\xf8\x86\x51\xf4\x19\x92\xb4\xfe\xff\x4d\x4d\x40\x27\xe3\xcc\x08\xe8\x6e\x40\x8e\x8d\x36\x43\xce\x9f\xcc\x01\x57\x14\xd1\x17\xbd\x4c\xe7\xec\xcb\xb2\x4a\x11\xa5\xb4\x8c\x25\x44\x90\x9d\x06\xc7\xd8\x43\xe3\x8f\x49\x21\x63\xfb\xf2\x6d\x2a\xc5\x71\xde\x0e\x03\x2b\x97\x13\xb8\xc9\x62\x6c\xc7\xb6\x1d\x81\x2a\x5b\xf0\xfe\x55\x24\x8f\x97\x3c\x4d\xe6\xe8\x43\x32\x99\x05\xae\xdf\x74\x7a\xd1\xa0\x0b\x6b\xc0\xb1\x1a\x14\xbf\xce\x65\xf9\x52\x23\x88\x86\x2d\x5c\xf7\xef\x44\x35\x9e\x90\x3b\x4a\xe1\x7d\xd6\xa7\x0c\x02\xa9\xd8\x73\xb3\x18\x69\xc2\xcc\x67\x16\xbd\x21\x8a\xfb\x50\x5e\xf5\x84\x2f\x37\x9c\xed\x46\x48\xe3\x92\x39\xa5\x0e\x7f\xd4\xda\x71\x00\xab\xcb\x60\x11\xba\xb4\x1f\x6e\xc5\x59\xf2\x83\x28\x26\x8d\xfc\x57\x4f\xe4\x95\x3e\x30\x9b\xea\x41\xdd\x76\x07\xac\xa2\x09\x78\xd3\xaa\x01\x70\xdb\xd5\x7e\x0f\xa4\x38\x93\xe2\x49\x47\xec\x9d\x36\x2e\x85\xf4\x5f\x51\xfa\x8b\x20\xbc\x17\x66\xcd\xc3\x68\x19\xb2\xd2\x79\x08\xa3\xad\x06\x77\xdc\x40\xeb\x9a\x31\x3f\x94\xe5\x4e"
                                   "\x56\xfd\x8c\x27\x29\x82\xf3\x58\xc4\x6f\x1e\xb5\xbb\x10\x61\xca\x0a\xa1\xd0\x7b\x75\xde\xaf\x04\x98\x33\x42\xe9\xe7\x4c\x3d\x96\x8e\x25\x54\xff\xf1\x5a\x2b\x80\x1c\xb7\xc6\x6d\x63\xc8\xb9\x12\x72\xd9\xa8\x03\x0d\xa6\xd7\x7c\xe0\x4b\x3a\x91\x9f\x34\x45\xee\xf6\x5d\x2c\x87\x89\x22\x53\xf8\x64\xcf\xbe\x15\x1b\xb0\xc1\x6a\xed\x5f\x59\xff\x4f\xa2\xc3\x04\xc7\xd4\x59\x77\xf4\x92\x51\x0d";
static const uint64_t pointer_table[0x1380] = {
    [0x80] = 0x17a43507bULL, [0xd4] = 0x12839a397ULL, [0x102] = 0x1109ddec7ULL, [0x1e7] = 0x1662d3562ULL, [0x281] = 0x16515b8bdULL, [0x3d0] = 0x16858bb5bULL, [0x4b4] = 0x1099e1977ULL, [0x4bf] = 0x14998adcaULL, [0x599] = 0x15f4080b2ULL, [0x5c8] = 0x17bf184a3ULL, [0x6d7] = 0x1689348d2ULL, [0x856] = 0x13ce7c923ULL, [0x91f] = 0x1589164caULL, [0x93d] = 0x17b4422ffULL, [0x9c2] = 0x12e957ff7ULL, [0xc26] = 0x158c0d903ULL, [0xd3c] = 0x133bcf1efULL, [0xe4a] = 0x16c00c6cfULL, [0x10e3] = 0x16cd74b56ULL, [0x1173] = 0x1600f1b46ULL, [0x1244] = 0x11ea53317ULL, [0x1377] = 0x167f3c972ULL,
};

static uint8_t read8(uint64_t address) {
    if (address < 0x100000000ULL) {
        missing_constant = true;
        return 0;
    }
    uint64_t offset = address - 0x100000000ULL;
    for (unsigned index = 0; index < sizeof(spans) / sizeof(spans[0]); index++) {
        if (offset >= spans[index].start && offset < spans[index].end) {
            return constants[spans[index].offset + offset - spans[index].start];
        }
    }
    missing_constant = true;
    return 0;
}
static uint32_t read32(uint64_t address) {
    uint32_t value = 0;
    for (int index = 0; index < 4; index++) {
        value |= (uint32_t)read8(address + index) << (8 * index);
    }
    return value;
}
static void finalizer_native(uint8_t *tail)

{
    uint8_t descriptor[32] = {0};
    long param_1 = (long)descriptor;
    *(uint8_t **)(descriptor + 8) = tail;
    *(uint8_t **)(descriptor + 16) = tail;
    *(uint32_t *)(descriptor + 24) = 5 - (((uint32_t)param_1 ^ 0x7c01fd9c) * 0x458b076f);
    uint8_t pre[16];
    uint uVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    uint uVar5;
    uint uVar6;
    uint uVar7;
    uint uVar8;
    uint uVar9;
    uint uVar10;
    uint uVar11;
    uint uVar12;
    uint uVar13;
    uint uVar14;
    byte bVar15;
    byte bVar16;
    uint uVar17;
    byte bVar18;
    uint uVar19;
    uint uVar20;
    char cVar21;
    bool bVar22;
    uint64_t puVar23;
    uint64_t puVar24;
    uint64_t puVar25;
    int iVar26;
    uint64_t puVar27;
    uint64_t puVar28;
    uint64_t puVar29;
    uint64_t puVar30;
    long lVar31;
    uint64_t puVar32;
    uint uVar33;
    byte *uStack_80;

    iVar26 = *(int *)(param_1 + 0x18) + ((uint)param_1 ^ 0x7c01fd9c) * 0x458b076f + -3;
    lVar31 = 0x14;
    do {
        cVar21 = (char)lVar31 + -5;
        pre[lVar31 - 5] = cVar21 * 'y' ^ *(byte *)(*(long *)(param_1 + 8) + lVar31 + -5) ^ cVar21 * '5' ^ read8(pointer_table[iVar26 + 0x27f] + (lVar31 + -0x6437159f)) ^ read8(pointer_table[iVar26 + 0xe48] + (lVar31 + -0x6b0d64b0)) ^ read8(pointer_table[iVar26 + 0x91d] + (lVar31 + -0x57b8533f)) ^ read8(pointer_table[iVar26 + 0x93b] + (lVar31 + -0x7a686763)) ^ read8(pointer_table[iVar26 + 0x1242] + (lVar31 + -0x1db9a97c)) ^ read8(pointer_table[iVar26 + 0x1171] + (lVar31 + -0x5f445066));
        bVar22 = lVar31 == 5;
        iVar26 = iVar26 + (uint)bVar22 * -2;
        lVar31 = lVar31 + -1;
    } while (!bVar22);
    uStack_80 = *(byte **)(param_1 + 0x10);
    uVar33 = (uint)(uStack_80 == (byte *)0x0);
    iVar26 = iVar26 + (uint)(uStack_80 != (byte *)0x0) * 4 + (uVar33 | uVar33 << 2);
    puVar27 = pointer_table[iVar26 + 0xd0];
    puVar28 = pointer_table[iVar26 + 0x4bb];
    puVar29 = pointer_table[iVar26 + 0x3cc];
    bVar15 = read8(puVar29 + (((ulong)pre[14] ^ 0xd) - 0x678ff66b));
    puVar32 = pointer_table[iVar26 + 0xc22];
    uVar33 = (uint)bVar15 * 0x10 + (bVar15 & 0x7fffffd) * -0x20 + 0x5a ^ 0x5a | bVar15 >> 4 ^ 5;
    uVar33 = uVar33 + (uVar33 & 0x7ffffff5) * -2 + 0x75;
    uVar17 = uVar33 * 4;
    uVar5 = uVar17 ^ 0xffffffd4;
    bVar15 = read8(puVar29 + (((ulong)pre[2] ^ 0xb7) - 0x678ff66b));
    uVar19 = (uint)bVar15 * 0x10 + (bVar15 & 0x7fffffd) * -0x20 + 0x53 ^ 0x53 | bVar15 >> 4 ^ 5;
    uVar19 = uVar19 + (uVar19 & 0x7fffffe1) * -2 + 0x61;
    uVar6 = uVar19 * 4 ^ 0xffffff84;
    puVar30 = pointer_table[iVar26 + 0x1373];
    puVar23 = pointer_table[iVar26 + 0x595];
    puVar24 = pointer_table[iVar26 + 0x10df];
    puVar25 = pointer_table[iVar26 + 0x6d3];
    uVar1 = ((int32_t)read32(puVar30 + ((ulong)(byte)read8(puVar27 + (((ulong)pre[9] ^ 0xb0) - 0x276128e7)) ^ 99) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + ((ulong)(byte)read8(puVar27 + (((ulong)pre[9] ^ 0xb0) - 0x276128e7)) ^ 99) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xc6e0d22c)) + 0xfd2226de;
    uVar33 = read32(puVar23 + (ulong)(((byte)read8(puVar32 + (((ulong)pre[4] ^ 0x93) - 0x57d25b53)) ^ 0x6b) + (((byte)read8(puVar32 + (((ulong)pre[4] ^ 0x93) - 0x57d25b53)) ^ 0x6b) & 0x7fffffe7) * -2 + 0x689900e7 ^ 0x6899004f) * 4 + -0x5e5f8f82) ^ read32(puVar24 + ((ulong)(uVar33 ^ uVar5 + (uVar17 ^ (uVar5 | 0x3f) ^ 0x6b) * -2 + 0x3d ^ 0x3c) & 0xff) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(read8(puVar28 + (((ulong)pre[3] ^ 0x44) - 0x48b5639a)) ^ pre[3]) * 4 + -0x67a9fe82) ^ uVar1 ^ uVar1 >> 3 ^ uVar1 >> 4;
    uVar1 = uVar33 ^ 0x681b93;
    uVar5 = ((uVar33 >> 0x18 ^ 0x3ffafba1) + (uVar33 >> 0x17 & 0x7c ^ 0x3c) + 0xc00504c2) % 0xe0;
    uVar33 = ((int32_t)read32(puVar30 + ((ulong)(byte)read8(puVar27 + (((ulong)pre[13] ^ 0x21) - 0x276128e7)) ^ 0x13) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + ((ulong)(byte)read8(puVar27 + (((ulong)pre[13] ^ 0x21) - 0x276128e7)) ^ 0x13) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x20c23bee)) + 0xaa12dbbf;
    uVar33 = read32(puVar23 + ((ulong)(byte)read8(puVar32 + (((ulong)pre[8] ^ 0xd0) - 0x57d25b53)) ^ 0xae) * 4 + -0x5e5f8f82) ^ read32(puVar24 + ((ulong)(uVar19 ^ uVar6 + (uVar6 & 0x1c) * -2 + 0x1f ^ 0xc4) & 0xff) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)((pre[7] ^ read8(puVar28 + (((ulong)pre[7] ^ 0x36) - 0x48b5639a))) ^ 0xa1) * 4 + -0x67a9fe82) ^ uVar33 ^ uVar33 >> 3 ^ uVar33 >> 4 ^ 0xe9716923;
    iVar4 = 0x4084096d;
    if ((int)uVar1 < 0x60000000) {
        iVar4 = 0x4084088d;
    }
    uVar6 = (int32_t)read32(puVar30 + (ulong)(uVar33 >> 0x10 & 0xff ^ 0x6e) * 4 + -0x6718dde2) + 0x99b1bdc8;
    uVar7 = ((uVar6 >> 4) - (uVar6 >> 3 & 0x1a3ae678)) + 0x7d1d733c ^ ((uVar6 >> 3) - (uVar6 >> 2 & 0x1b1d1b40)) + 0x6d8e8da0;
    uVar19 = uVar7 & 0x8000000;
    iVar2 = (uVar19 ^ 0x7bdfeeff) + uVar19 * 2;
    uVar8 = (((byte)read8(puVar27 + (((ulong)pre[5] ^ 0x9c) - 0x276128e7)) ^ 0x7b) + (((byte)read8(puVar27 + (((ulong)pre[5] ^ 0x9c) - 0x276128e7)) ^ 0x7b) & 0x74) * -2) * 0x10000 + 0x13740000 ^ 0x13740000;
    bVar15 = read8(puVar29 + (((ulong)pre[6] ^ 0xee) - 0x678ff66b));
    uVar19 = (((uint)bVar15 << 4 ^ 0xffffff80) + (uint)bVar15 * -0x20) - 4 ^ 0x7c;
    uVar17 = (uVar19 | bVar15 >> 4) ^ 5;
    uVar19 = uVar17 + (uVar17 ^ uVar19 & 0x60) * -2 + 0x1f;
    uVar17 = uVar19 * 4 ^ 0x7c;
    uVar19 = uVar19 ^ uVar17 + (uVar17 & 0x24) * -2 + 0x24 ^ 0xaaaaaaaa;
    uVar19 = (uVar19 & 0xff) * 0x100 + (uVar19 & 10) * -0x200 + 0x4be30a6e ^ 0x4be30a6e | uVar8 + (uVar8 & 0x27d30000) * -2 + 0x27d324c7 ^ 0x27d324c7;
    uVar17 = uVar19 + (uVar19 & 0x1888055) * -2 + 0x1888055 ^ 0x888055;
    bVar15 = read8(puVar29 + (((ulong)pre[10] ^ 0xf0) - 0x678ff66b));
    uVar19 = (bVar15 & 0xfc) * 0x20 + (uint)bVar15 * 0x10 + 0x4c;
    uVar19 = (((uVar19 ^ 0x4c | (uint)(bVar15 >> 4)) ^ 5) - (uVar19 * 2 & 0x20)) + 0x10;
    uVar8 = (pre[7] ^ read8(puVar28 + (((ulong)pre[7] ^ 0x36) - 0x48b5639a))) ^ 0xb2;
    uVar17 = uVar17 + (uVar17 & 0x72e919f3) * -2 + 0x72e919f3 ^ 0x72e919f3 | uVar8 + (uVar8 & 0xe2) * -2 + 0x30bae0e2 ^ 0x30bae0e2;
    uVar17 = uVar17 + (uVar17 & 0x14005071) * -2 + 0x14005071;
    uVar8 = ((int32_t)read32(puVar30 + (ulong)(uVar17 >> 0x10 & 0xff ^ 0xf5) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar17 >> 0x10 & 0xff ^ 0xf5) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xb5242168)) + 0xf443ce7c;
    uVar9 = read32(puVar23 + ((ulong)(byte)read8(puVar32 + (((ulong)pre[0] ^ 0xe4) - 0x57d25b53)) ^ 0x79) * 4 + -0x5e5f8f82) ^ read32(puVar24 + ((ulong)(uVar19 ^ ((uVar19 * 4 ^ 0x40) - (uVar19 * 8 & 0x48)) + 0x26 ^ 0x7d) & 0xff) * 4 + -0x6bf722f6) ^ uVar8 ^ uVar8 >> 3 ^ uVar8 >> 4 ^ read32(puVar25 + (ulong)((pre[15] ^ read8(puVar28 + (((ulong)pre[15] ^ 0xe2) - 0x48b5639a))) ^ 0xc5) * 4 + -0x67a9fe82) ^ 0xed8a8e51;
    uVar8 = ((int32_t)read32(puVar30 + ((ulong)(byte)read8(puVar27 + (((ulong)pre[1] ^ 0x75) - 0x276128e7)) ^ 0xb5) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + ((ulong)(byte)read8(puVar27 + (((ulong)pre[1] ^ 0x75) - 0x276128e7)) ^ 0xb5) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x284f5888)) + 0xadd96a0c;
    uVar19 = (read32(puVar25 + (ulong)((pre[11] ^ read8(puVar28 + (((ulong)pre[11] ^ 0xfe) - 0x48b5639a))) ^ 0x51) * 4 + -0x67a9fe82) ^ 0x3d8ea520) + 1;
    uVar10 = read32(puVar25 + (ulong)((pre[11] ^ read8(puVar28 + (((ulong)pre[11] ^ 0xfe) - 0x48b5639a))) ^ 0x51) * 4 + -0x67a9fe82) ^ 0x22b7de90;
    iVar3 = uVar10 + (uVar19 ^ 0x5fbf3fea) + (uVar19 * 2 & 0xbf7e7fd4) + -0x5fbf3fea;
    uVar8 = read32(puVar23 + ((ulong)(byte)read8(puVar32 + (((ulong)pre[12] ^ 0x83) - 0x57d25b53)) ^ 0xac) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar17 >> 8 & 0xff ^ 200) * 4 + -0x6bf722f6) ^ uVar8 ^ uVar8 >> 3 ^ uVar8 >> 4;
    iVar3 = ((iVar3 - (iVar3 * 2 & 0x95736728U)) + 0x4ab9b394 ^ 0xb5464c6a) + uVar10;
    uVar17 = uVar8 ^ 0xa8cbed2;
    uVar8 = uVar8 ^ 0x957d80d9;
    uVar19 = (iVar3 - (iVar3 * 2 & 0x7305de7eU)) + 0x3982ef3f;
    uVar10 = uVar19 ^ 0x3982ef3f;
    iVar3 = (uVar17 ^ uVar19 ^ 0xa673d134) + uVar10 + 1;
    iVar3 = (uVar17 ^ (uVar8 - (uVar8 * 2 + 2 & 0xf283df64)) + 0x7941efb3 ^ (iVar3 - (iVar3 * 2 & 0x10b72a12U)) + 0x85b9509 ^ 0x1114bb4f) + uVar10;
    uVar19 = (iVar3 - (iVar3 * 2 & 0xb8aab15aU)) + 0x5c5558ad;
    iVar3 = iVar2 + -0x40d04811;
    if ((iVar2 + 0x84201101U & uVar6) != 0) {
        iVar3 = -0x49106a13 - iVar2;
    }
    uVar6 = uVar19 >> 0x10 & 0xff ^ 0x31;
    uVar6 = uVar6 + (uVar6 & 0x5d) * -2 + 0x28c4a5d ^ 0x48c6e1d;
    uVar6 = ((int32_t)read32(puVar30 + (ulong)(uVar6 + (uVar6 & 0x21fd) * -2 + 0xff39fd ^ 0x6ff1d72) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar6 + (uVar6 & 0x21fd) * -2 + 0xff39fd ^ 0x6ff1d72) * 4 + -0x6718dde2) * 2 + 0x3637b90U & 0x44869aa)) + 0x9bd5f29d;
    uVar17 = uVar9 >> 8 & 0xff ^ 6;
    uVar8 = uVar17 + (uVar17 & 0x16) * -2 + 0xb1033216 ^ 0x112b1807;
    uVar17 = uVar1 & 0xff ^ 0x7e;
    uVar17 = uVar17 + (uVar17 & 0x2c) * -2 + 0xe73502c ^ 0x143150ad;
    uVar11 = read32(puVar23 + (ulong)(uVar33 >> 0x18 ^ 0xe5) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar8 + (uVar8 & 0x1c5) * -2 + 0x5c9781c5 ^ 0xfcbfab51) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar17 + (uVar17 & 0x1a001283) * -2 + 0x7fb9d283 ^ 0x65fbd261) * 4 + -0x67a9fe82) ^ uVar6 ^ uVar6 >> 3 ^ uVar6 >> 4 ^ 0x8aadde75;
    uVar6 = ((int32_t)read32(puVar30 + (ulong)(uVar11 >> 0x10 & 0xff ^ 0x84) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar11 >> 0x10 & 0xff ^ 0x84) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xb1c4c2aa)) + 0xf2941f1d;
    uVar17 = uVar6 >> 1 & 0x10;
    uVar12 = (uVar17 | (uVar17 >> 4) << 5) ^ 0xdebf7bd7;
    uVar10 = uVar12 + 0x21408429 & (uVar6 ^ 0x58e26150);
    uVar17 = ((uVar1 >> 0x10 & 0xff ^ 0x30) - (uVar1 >> 0xf & 0xc)) + 0x6d849406 ^ 0x38849211;
    uVar17 = ((int32_t)read32(puVar30 + (ulong)(uVar17 + (uVar17 & 0x100005d) * -2 + 0x365d85d ^ 0x5665ded2) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar17 + (uVar17 & 0x100005d) * -2 + 0x365d85d ^ 0x5665ded2) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xc1b716b0)) + 0xfa8d4920;
    uVar8 = uVar19 & 0xff ^ 0x77;
    uVar13 = uVar8 + (uVar8 & 0x8d) * -2 + 0x1d7d1b8d ^ 0xd7f5999;
    uVar17 = uVar17 ^ uVar17 >> 3 ^ uVar17 >> 4 ^ 0x6acd4286;
    uVar8 = uVar17 & (read32(puVar23 + (ulong)(uVar9 >> 0x18 ^ 0xde) * 4 + -0x5e5f8f82) ^ 0xb824d0f6);
    uVar17 = uVar17 + (read32(puVar23 + (ulong)(uVar9 >> 0x18 ^ 0xde) * 4 + -0x5e5f8f82) ^ 0xb824d0f6);
    uVar8 = (uVar8 + (uVar8 & 0x3319d91e) * -2) * 2 + 0xe633b23c;
    iVar2 = (uVar17 ^ 0xff79c33f) + (uVar17 * 2 & 0xfef3867e) + (uVar8 ^ 0xa5324281) + (uVar8 * 2 & 0x79fc1e84 ^ 0xb79bfbff) + 1;
    iVar4 = (uVar5 ^ 0xbf7bf773) + iVar4 + (uVar5 * 2 & 0xe6);
    uVar17 = read32(puVar24 + (ulong)(uVar33 >> 8 & 0xff ^ 0xcd) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar13 + (uVar13 & 0x2427f) * -2 + 0x5f2f27f ^ 0x15f0b057) * 4 + -0x67a9fe82) ^ (iVar2 - (iVar2 * 2 + 0x7b089808U & 0x992e23e2)) + 0xa1b5df5 ^ 0xc970a5af;
    uVar5 = uVar7 & 0x17ffffff ^ 0x14ebb549;
    uVar8 = uVar5 + (uVar5 & 0x1004a18a) * -2 + 0x3804a18a ^ 0x1c24a21b;
    iVar3 = iVar3 + (int32_t)read32(puVar30 + (ulong)(uVar33 >> 0x10 & 0xff ^ 0x6e) * 4 + -0x6718dde2);
    uVar5 = uVar9 & 0xff ^ 0x9f;
    uVar5 = uVar5 + (uVar5 & 0x71) * -2 + 0xd6433971 ^ 0x3a6b953;
    uVar5 = uVar5 + (uVar5 & 0x10101c6) * -2 + 0x890171c6;
    uVar7 = uVar9 & 0xff ^ 0xd2;
    uVar7 = uVar7 + (uVar7 & 0xa0) * -2 + 0x68fbda0 ^ 0x287bda0;
    iVar2 = ((uVar5 * 2 & 0x67bb9fae ^ 0x21898382) + (uVar5 ^ 0x2f393e3e)) * 2;
    uVar5 = iVar2 + 0x56b757b4;
    iVar2 = ((iVar2 + (uVar5 ^ 0x50803801)) - (uVar5 * 2 & 0x5eff8ffc)) + -0x19025bfe;
    uVar5 = uVar9 >> 0x10 & 0xff ^ 0x17;
    uVar5 = uVar5 + (uVar5 & 0xc2) * -2 + 0x8010c2 ^ 0x11c1;
    uVar8 = read32(puVar23 + (ulong)((iVar4 - (iVar4 * 2 & 0xb8dc4c30U)) + 0x5c6e2618 ^ 0x5c6e262b) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar19 >> 8 & 0xff ^ 0x6e) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar7 + (uVar7 & 0x801af) * -2 + 0x7bf9af ^ 0x473f9c7) * 4 + -0x67a9fe82) ^ (iVar2 - (iVar2 * 2 & 0xdffe9022U)) + 0x6fff4811 ^ uVar8 + (uVar8 & 0x4b2269c) * -2 + 0x4b2269c ^ (iVar3 - (iVar3 * 2 + 0xbd442db4U & 0xd66b2afe)) + 0xc9d7ac59;
    uVar5 = ((int32_t)read32(puVar30 + (ulong)(uVar5 + (uVar5 & 0x8011b8) * -2 + 0xee7f1b8 ^ 0xe67f027) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar5 + (uVar5 & 0x8011b8) * -2 + 0xee7f1b8 ^ 0xe67f027) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x64026f68)) + 0xcbb2f57c;
    uVar7 = uVar5 >> 4 ^ uVar5 >> 3;
    uVar19 = read32(puVar23 + (ulong)(uVar19 >> 0x18 ^ 0x73) * 4 + -0x5e5f8f82) ^ uVar5 ^ 0x81bc7f23;
    uVar5 = uVar19 & 0x79fb3e3e ^ 0x1032043a;
    uVar5 = uVar5 + (uVar5 & 0x11f80c38) * -2 + 0x17f88cf9 ^ 0xfdbd4b9;
    uVar19 = uVar19 & 0x8604c1c1 ^ 0x84044181;
    uVar19 = uVar19 + (uVar19 & 0x2008041) * -2 + 0xa638c5f ^ 0xe423e951;
    uVar9 = uVar7 & 0x19fb3e3e ^ 0x1188160c;
    uVar9 = uVar9 + (uVar9 & 0x933a24) * -2 + 0x20977b65 ^ 0x10ff19e4;
    uVar9 = uVar5 + (uVar5 & 0x4ee9fad4) * -2 + 0x4eedfad4 ^ uVar9 + (uVar9 & 0xd7ed392) * -2 + 0xd7ed392 ^ 0x629b2d9d | uVar19 + (uVar19 & 0x6e6d6590) * -2 + 0x6e6d7d90 ^ 0x629d95e;
    uVar19 = uVar7 & 0x604c1c1 ^ 0x204c180;
    uVar5 = uVar19 + (uVar19 & 0x44000) * -2 + 0x1156c22 ^ 0x18c803;
    uVar1 = ((uVar1 >> 8 & 0xff ^ 0x18) - (uVar1 >> 7 & 10)) + 0x1e23505 ^ 0x46c39ab;
    uVar33 = uVar33 & 0xff ^ 0xed;
    uVar7 = uVar33 + (uVar33 & 0x54) * -2 + 0x11e66c54 ^ 0x1e4a884;
    uVar33 = (uVar8 ^ 0x1cf09ae1) << 1 ^ 0xc60957d2;
    uVar33 = uVar33 + (uVar33 & 0x43b83e40) * -2 + 0x43b83e40;
    uVar19 = (uVar8 ^ 0x7ff43108) + 0x7cf8b09d;
    iVar4 = (uVar19 * 2 & 0x250df6ee) + (uVar19 ^ 0x9286fb77) + (uVar33 & 0xf9f1613a ^ 0x49316207) + (uVar33 * 2 & 0xe2e04270 ^ 0x7d9fbfff);
    uVar33 = (iVar4 - (iVar4 * 2 + 0x9ef8506U & 0x3c9a7a64)) + 0x8344ffb5;
    uVar19 = uVar33 >> 0x10 & 0xff ^ 0x2c;
    uVar19 = uVar19 + (uVar19 & 0x32) * -2 + 0x4cdd232 ^ 0x8ddb53;
    uVar5 = read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x5020518) * -2 + 0x6732c518 ^ 0x62bcc94c) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar7 + (uVar7 & 0x845e) * -2 + 0x29dc8c5e ^ 0x39de48f9) * 4 + -0x67a9fe82) ^ uVar5 + (uVar5 & 0x14c5e1) * -2 + 0x50f4cde1 ^ uVar9 + (uVar9 & 0x6924861e) * -2 + 0x6924861e ^ 0xa3751a09;
    uVar19 = ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x400089b) * -2 + 0x6c02189b ^ 0x68421159) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x400089b) * -2 + 0x6c02189b ^ 0x68421159) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x435a8920)) + 0xbb5f0258;
    uVar1 = uVar5 & 0xff ^ 0x4f;
    uVar8 = uVar1 + (uVar1 & 0x5f) * -2 + 0xd7cd855f ^ 0x168dd54e;
    uVar1 = read32(puVar25 + (ulong)(uVar8 + (uVar8 & 0x140048a) * -2 + 0xa55e0c8a ^ 0x641e5c88) * 4 + -0x67a9fe82) & 1;
    iVar4 = (uVar1 ^ 0xe605fdff) + uVar1 * 2;
    uVar1 = read32(puVar25 + (ulong)(uVar8 + (uVar8 & 0x140048a) * -2 + 0xa55e0c8a ^ 0x641e5c88) * 4 + -0x67a9fe82) & 0xfffffffe ^ 0xe3441aee;
    uVar1 = uVar1 + (uVar1 & 0x56dcca1a) * -2 + 0x56dcca1a ^ 0x16dfce78;
    uVar19 = read32(puVar23 + (ulong)(uVar17 >> 0x18 ^ 0xe) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar11 >> 8 & 0xff ^ 0x85) * 4 + -0x6bf722f6) ^ uVar19 ^ uVar19 >> 3 ^ uVar19 >> 4 ^ 0x3d5792cf - ((iVar4 * -2 + 0x9cU & 0x102 | 0xc1040000) + iVar4) ^ uVar1 + (uVar1 & 0x11448072) * -2 + 0x91448072 ^ 0xfc8372cf;
    uVar1 = uVar6 >> 1 & 0x7fffffe8 ^ 0x3824f900;
    uVar1 = uVar1 + (uVar1 & 0x4a96a8) * -2 + 0x4a96a9 ^ 0x1f1fbe;
    iVar4 = uVar12 + 0x6de7bf3b;
    if (uVar10 != (uVar10 & 0x3d5e5428) << 1) {
        iVar4 = 0x2b66b6e9 - uVar12;
    }
    iVar4 = (uVar6 ^ 0x58e26155) + iVar4 + -0x4ca73b12;
    iVar3 = (((uVar17 & 0xfe) << 1 ^ 0x2c) + (uVar17 & 0xff ^ 0x64ec7be8)) * 2;
    uVar8 = iVar3 + 0x7499ff66;
    uVar7 = ((uVar5 >> 8 & 0xff ^ 0x22) - (uVar5 >> 7 & 0x98)) + 0xac0d4c;
    iVar3 = ((iVar3 + (uVar8 ^ 0x26140207)) - (uVar8 * 2 & 0x30c5fbf0)) + 0x2f4c81ae;
    uVar8 = uVar17 & 0xff ^ 0xeb;
    uVar9 = uVar8 + (uVar8 & 0xd2) * -2 + 0x3a15f0d2 ^ 0x1b15f5f9;
    uVar8 = uVar5 >> 0x10 & 0xff ^ 0x78;
    uVar8 = uVar8 + (uVar8 & 0xd5) * -2 + 0x2c06d5 ^ 0x1cb7d5;
    uVar7 = read32(puVar23 + (ulong)(uVar33 >> 0x18 ^ 0x2d) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(((uVar7 ^ 0x12d88c8) - (uVar7 * 2 & 0xe0e)) + 0x24105f07 ^ 0x2591da22) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar9 + (uVar9 & 0x164a) * -2 + 0x477564a ^ 0x257753b4) * 4 + -0x67a9fe82) ^ uVar6 ^ (iVar3 - (iVar3 * 2 & 0xba823bbaU)) + 0x5d411ddd ^ (uVar1 + (uVar1 & 0xcc9a043) * -2 + 0xcc9a043 ^ (iVar4 - (iVar4 * 2 & 0x300abbc4U)) + 0x18055de2 ^ 0x77621fe8) >> 3 ^ 0x3fab738c;
    uVar1 = ((int32_t)read32(puVar30 + (ulong)(uVar8 + (uVar8 & 0x101308) * -2 + 0x585e1f08 ^ 0x586eae1d) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar8 + (uVar8 & 0x101308) * -2 + 0x585e1f08 ^ 0x586eae1d) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x8bcf8f76)) + 0xdf998583;
    uVar6 = uVar33 & 0xff ^ 0x39;
    uVar8 = uVar6 + (uVar6 & 0xf6) * -2 + 0xaeda33f6 ^ 0x2ca80efc;
    uVar6 = uVar17 >> 8 & 0xff ^ 0x1e;
    uVar6 = uVar6 + (uVar6 & 0x72) * -2 + 0x6fe0c72 ^ 0x31e20c52;
    uVar6 = read32(puVar23 + (ulong)(uVar11 >> 0x18 ^ 0x61) * 4 + -0x5e5f8f82) ^ read32(puVar25 + (ulong)(uVar8 + (uVar8 & 0x230137d) * -2 + 0xce38937d ^ 0x4c4aae54) * 4 + -0x67a9fe82) ^ read32(puVar24 + (ulong)(uVar6 + (uVar6 & 0x160c0518) * -2 + 0x162fd518 ^ 0x2133d5f8) * 4 + -0x6bf722f6) ^ uVar1 ^ uVar1 >> 3 ^ 0xfc53f30d;
    uVar17 = uVar17 >> 0x10 & 0xff ^ 0x79;
    uVar17 = uVar17 + (uVar17 & 0xd9) * -2 + 0x626d99d9 ^ 0x207db8df;
    uVar8 = read32(puVar23 + (ulong)(uVar5 >> 0x18 ^ 0x5a) * 4 + -0x5e5f8f82);
    uVar5 = ((int32_t)read32(puVar30 + (ulong)(uVar17 + (uVar17 & 0x100399) * -2 + 0x35339399 ^ 0x7723b2d3) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar17 + (uVar17 & 0x100399) * -2 + 0x35339399 ^ 0x7723b2d3) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xb1c84dce)) + 0xf295e4af;
    uVar33 = uVar33 >> 8 & 0xff ^ 0x93;
    uVar33 = uVar33 + (uVar33 & 0x4d) * -2 + 0xe8e6054d ^ 0x6d470bed;
    uVar17 = uVar8 ^ ((uVar8 ^ 0x498eb8b9) - ((uVar8 ^ 0x498eb8b9) * 2 + 2 & 0xaee92dfe)) + 0x57749700 ^ ((uVar8 ^ 0x829093fa) - ((uVar8 ^ 0x829093fa) * 2 + 0x163c5688 & 0x7e95d0b6)) + 0xa69139f ^ 0xde4f39e2;
    uVar33 = read32(puVar24 + (ulong)(uVar33 + (uVar33 & 0x5a00780) * -2 + 0xedbc4780 ^ 0x681d49e5) * 4 + -0x6bf722f6) ^ uVar5 ^ uVar5 >> 3 ^ uVar5 >> 4 ^ (uVar17 - (uVar17 * 2 + 0x163c5686 & 0x735f7f96)) + 0x4cdeb0e ^ 0x46831b0c;
    uVar5 = (uVar11 & 0xff ^ 10) + (uVar11 & 100) * -2 + 0x52a57364 ^ 0x98e56730;
    uVar9 = uVar33 << 6;
    uVar17 = uVar9 ^ 0x559d2e40;
    uVar8 = read32(puVar25 + (ulong)(uVar5 + (uVar5 & 0x4000015a) * -2 + 0xf09f615a ^ 0x3adf75e1) * 4 + -0x67a9fe82) << 6 ^ 0x9c56b7c0;
    uVar17 = uVar17 + (uVar17 & 0x7f2b6fc0 ^ uVar9 & 0x8020100) * -2 + 0x77296ed3 ^ 0x77296ed3 | uVar33 >> 0x1a ^ 10;
    uVar33 = ((uVar8 + (uVar8 & 0x29cbf240) * -2 + 0x29cbf27f ^ 0x29cbf27f) & 0xffffffc0 | read32(puVar25 + (ulong)(uVar5 + (uVar5 & 0x4000015a) * -2 + 0xf09f615a ^ 0x3adf75e1) * 4 + -0x67a9fe82) >> 0x1a) ^ 0x30;
    uVar17 = uVar17 + (uVar17 & 0x6e170b84) * -2 + 0x6e170b84 ^ uVar33 + (uVar33 & 0x58add6d0) * -2 + 0x58add6d0;
    uVar8 = uVar6 ^ uVar1 >> 4;
    uVar33 = uVar8 >> 0x10 & 0xff ^ 0x3a;
    uVar33 = uVar33 + (uVar33 & 0xbc) * -2 + 0x78327bbc ^ 0x3870f3bc;
    uVar33 = ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x404089ac) * -2 + 0x6578d9ac ^ 0x253a5146) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x404089ac) * -2 + 0x6578d9ac ^ 0x253a5146) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xd6c4e514)) + 0x5143052;
    uVar1 = uVar19 & 0xff ^ 0x22;
    uVar9 = uVar1 + (uVar1 & 0xae) * -2 + 0xb8cda0ae ^ 0x39f3b0a4;
    uVar1 = uVar17 >> 0xe & 0xff ^ 0x34;
    uVar1 = uVar1 + (uVar1 & 0xe5) * -2 + 0x868ecce5 ^ 0x1a874ab6;
    uVar10 = read32(puVar23 + (ulong)(uVar7 >> 0x18 ^ 0x67) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x4080271) * -2 + 0xc47c3a71 ^ 0x5875bceb) * 4 + -0x6bf722f6) ^ uVar33 ^ uVar33 >> 3 ^ uVar33 >> 4 ^ 0x78430785;
    uVar33 = uVar10 & 0x200;
    uVar11 = (uVar33 | (uVar33 >> 9) << 10) ^ 0x3d77ea7d;
    uVar1 = uVar11 + 0xc2881583 & (read32(puVar25 + (ulong)(uVar9 + (uVar9 & 0x12e1d4c) * -2 + 0xf12e1d4c ^ 0x70100d5d) * 4 + -0x67a9fe82) ^ 0x22b7de90);
    uVar12 = uVar8 >> 0x18;
    uVar33 = (uVar12 << 0x1c ^ 0xa0000000) + (uVar8 & 0x4000000) * 0x20 + 0x4d2fdbb5;
    uVar5 = ((uVar33 ^ 0x4d2fdbb5) & 0xfffffff0 | uVar6 >> 0x1c) ^ 0xd;
    uVar33 = uVar5 + (uVar5 & 0x5fdefeea ^ uVar33 & 0x10000000) * -2 + 0x435ea86a;
    uVar5 = uVar33 * 0x10 ^ 0x4e4e6460;
    uVar33 = (uVar5 + (uVar5 & 0x7dd14a20) * -2 + 0x7dd14a2d ^ 0x7dd14a2d) & 0xfffffff0 | uVar33 >> 0x1c;
    uVar5 = uVar33 ^ 1;
    iVar4 = uVar11 + 0xfeea95e8;
    if (uVar1 != (uVar1 & 0x74bf0c) << 1) {
        iVar4 = 0x79da6ae2 - uVar11;
    }
    uVar1 = uVar12 ^ uVar5 + (uVar5 & 0x7fef9f5f ^ uVar33 & 0x2e080a04) * -2 + 0x51e7955b;
    uVar33 = read32(puVar23 + (ulong)(uVar12 ^ 0xe9) * 4 + -0x5e5f8f82) & 0x20;
    uVar5 = (uVar33 | (uVar33 >> 5) << 6) ^ 0x6e77fd3f;
    uVar33 = (uVar1 ^ 0xa137ae64) & uVar5 + 0x918802c1;
    iVar3 = uVar5 + 0xf79ef6ff;
    if (uVar33 != (uVar33 & 0x5eaaf3a2) << 1) {
        iVar3 = -0x2b710e83 - uVar5;
    }
    uVar11 = ((uVar17 >> 0x16 & 0xff ^ 0x28) - (uVar17 >> 0x15 & 0xaa)) + 0xe65cdb55 ^ 0x764c7057;
    uVar33 = (int32_t)read32(puVar30 + (ulong)(uVar11 + (uVar11 & 0x10108821) * -2 + 0xf0368821 ^ 0x60262397) * 4 + -0x6718dde2) + 0x99b1bdc8;
    uVar13 = ((uVar33 >> 4) - (uVar33 >> 3 & 0x1feae2b4)) + 0x4ff5715a ^ ((uVar33 >> 3) - (uVar33 >> 2 & 0x3287ae0c)) + 0x3943d706;
    uVar5 = uVar13 & 0x200000;
    uVar14 = (uVar5 | (uVar5 >> 0x15) << 0x16) ^ 0x7dcfebaf;
    uVar33 = uVar14 + 0x82501451 & uVar33;
    uVar20 = (uVar17 << 0x1a ^ 0x1c000000) + (uVar17 & 0x10) * 0x8000000 + 0x416fd383 ^ 0x416fd383 | uVar17 >> 6 ^ 0x3ec2d3a;
    uVar5 = ((uVar7 >> 0x10 & 0xff ^ 0xa0) - (uVar7 >> 0xf & 0x80)) + 0x61e95840 ^ 0x20a57840;
    uVar5 = ((int32_t)read32(puVar30 + (ulong)(uVar5 + (uVar5 & 0x1402dae) * -2 + 0x1c22dae ^ 0x408e0def) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar5 + (uVar5 & 0x1402dae) * -2 + 0x1c22dae ^ 0x408e0def) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x81ec56ae)) + 0xdaa7e91f;
    uVar6 = uVar17 >> 6 & 0xff ^ 0x1f;
    uVar6 = uVar6 + (uVar6 & 0x7fffffdf) * -2 + 0x8d9bddf ^ 0x10189adf;
    uVar5 = read32(puVar23 + (ulong)(uVar19 >> 0x18 ^ 0x3a) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar8 >> 8 & 0xff ^ 0x3a) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar6 + (uVar6 & 0x18000799) * -2 + 0x3f2ac799 ^ 0x27ebe0dc) * 4 + -0x67a9fe82) ^ uVar5 ^ uVar5 >> 3 ^ uVar5 >> 4 ^ 0x2eb8ba38;
    uVar6 = uVar10 & 0xfffffdff ^ 0xc31c009a;
    uVar6 = uVar6 + (uVar6 & 0xe17653b) * -2 + 0xe17653b ^ 0x8417a9aa;
    iVar4 = (read32(puVar25 + (ulong)(uVar9 + (uVar9 & 0x12e1d4c) * -2 + 0xf12e1d4c ^ 0x70100d5d) * 4 + -0x67a9fe82) ^ 0xc2715adf) + iVar4 + -0x3c628065;
    uVar6 = uVar6 + (uVar6 & 0x5891a5e4) * -2 + 0x5891a5e4 ^ (iVar4 - (iVar4 * 2 & 0xa5ad7a82U)) + 0x52d6bd41 ^ 0x4ba2597e;
    uVar17 = read32(puVar23 + (ulong)(uVar12 ^ 0xe9) * 4 + -0x5e5f8f82) & 0xffffffdf ^ 0x9d5874cb;
    uVar17 = uVar17 + (uVar17 & 0x65efd69d) * -2 + 0xe5efd69d ^ 0x69ef9fb4;
    iVar4 = (uVar1 ^ 0xa137ae70) + iVar3 + -0x6616f43e;
    uVar1 = uVar13 & 0x1fdfffff ^ 0x1c50a906;
    uVar1 = uVar1 + (uVar1 & 0x280efd1) * -2 + 0x2a0efd1 ^ 0xe6e0c1;
    iVar3 = uVar14 + 0xfef23f60;
    if (uVar33 != (uVar33 & 0x9f00400) << 1) {
        iVar3 = -0x5ade942 - uVar14;
    }
    iVar3 = iVar3 + (int32_t)read32(puVar30 + (ulong)(uVar11 + (uVar11 & 0x10108821) * -2 + 0xf0368821 ^ 0x60262397) * 4 + -0x6718dde2);
    uVar33 = uVar19 >> 8 & 0xff ^ 0x2b;
    uVar33 = uVar33 + (uVar33 & 0xc) * -2 + 0xab3070c ^ 0x8213e10c;
    uVar9 = uVar7 & 0xff ^ 0xec;
    uVar9 = uVar9 + (uVar9 & 0x54) * -2 + 0x1909f54 ^ 0x841d24;
    uVar17 = read32(puVar24 + (ulong)(uVar33 + (uVar33 & 0x8804568) * -2 + 0x8a8e4568 ^ 0x22ea3fe) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar9 + (uVar9 & 0x667d) * -2 + 0x8c2667d ^ 0x9d6e4c4) * 4 + -0x67a9fe82) ^ uVar17 + (uVar17 & 0x1c866105) * -2 + 0x1c866105 ^ uVar1 + (uVar1 & 0x27e2a433) * -2 + 0x27e2a433 ^ (iVar3 - (iVar3 * 2 + 0x3a1f2572U & 0x2f4756aa)) + 0x34b33e0e ^ (iVar4 - (iVar4 * 2 & 0x7c1c3eccU)) + 0x3e0e1f66 ^ 0x8648a47c;
    uVar33 = uVar19 >> 0x10 & 0xff ^ 0x55;
    uVar33 = uVar33 + (uVar33 & 100) * -2 + 0x650a9464 ^ 0x2c0d3404;
    uVar33 = ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x80185cf) * -2 + 0xc0185cf ^ 0x45062570) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x80185cf) * -2 + 0xc0185cf ^ 0x45062570) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x6186ae82)) + 0xca751509;
    uVar19 = uVar7 >> 8 & 0xff ^ 0x4d;
    uVar1 = uVar19 + (uVar19 & 0xa4) * -2 + 0x246fba4 ^ 0x1107dbb4;
    uVar19 = uVar8 & 0xff ^ 0x3f;
    uVar8 = uVar19 + (uVar19 & 0x30) * -2 + 0xbce1ee30 ^ 0x3cb1ee31;
    uVar19 = read32(puVar23 + (ulong)(uVar20 + (uVar20 & 0x2f170462) * -2 + 0x2f170462 >> 0x18 ^ 0xfa) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x10474) * -2 + 0xa9d474 ^ 0x13e8f4aa) * 4 + -0x6bf722f6) ^ uVar33 ^ uVar33 >> 3 ^ uVar33 >> 4 ^ 0x48b454f3;
    uVar33 = uVar19 & (read32(puVar25 + (ulong)(uVar8 + (uVar8 & 0x100047) * -2 + 0x10b33847 ^ 0x90e33818) * 4 + -0x67a9fe82) ^ 0x22b7de90);
    uVar33 = (uVar33 + (uVar33 & 0x287f93bd) * -2) * 2 + 0x50ff277a;
    uVar19 = uVar19 + (read32(puVar25 + (ulong)(uVar8 + (uVar8 & 0x100047) * -2 + 0x10b33847 ^ 0x90e33818) * 4 + -0x67a9fe82) ^ 0x22b7de90);
    iVar4 = (uVar19 ^ 0x5f37effe) + (uVar19 * 2 & 0xbe6fdffc) + (uVar33 ^ 0x14f321e9) + (uVar33 * 2 & 0x77e7f2d8 ^ 0xde19bd2f) + 1;
    uVar33 = (iVar4 - (iVar4 * 2 + 0xb97812deU & 0xc9002394)) + 0xc13c1b39;
    uVar19 = uVar6 >> 0x10 & 0xff ^ 0x4e;
    uVar19 = uVar19 + (uVar19 & 0x5c) * -2 + 0x9c5e325c ^ 0x444e1652;
    uVar19 = ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x4810200b) * -2 + 0xca3bb00b ^ 0x122b94d2) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x4810200b) * -2 + 0xca3bb00b ^ 0x122b94d2) * 4 + -0x6718dde2) * 2 + 0x3637b90U & 0x51a419e)) + 0x9c3ede97;
    uVar1 = uVar17 >> 8 & 0xff ^ 0x1b;
    uVar1 = uVar1 + (uVar1 & 0x7fffffef) * -2 + 0x2f9ef3ef ^ 0x57ffbee;
    uVar8 = uVar33 & 0xff ^ 0xc6;
    uVar8 = uVar8 + (uVar8 & 0xc9) * -2 + 0x1e0999c9 ^ 0x6801dcd;
    uVar8 = read32(puVar23 + (ulong)(uVar5 >> 0x18 ^ 0xa4) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x28600322) * -2 + 0x2878c322 ^ 0x299cbb6) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar8 + (uVar8 & 0x10018cbb) * -2 + 0x7607bcbb ^ 0x6e8e38e5) * 4 + -0x67a9fe82) ^ uVar19 ^ uVar19 >> 3 ^ uVar19 >> 4 ^ 0x806a18bc;
    uVar19 = uVar17 >> 0x10 & 0xff ^ 0xb3;
    uVar19 = uVar19 + (uVar19 & 0xd0) * -2 + 0x4efd3bd0 ^ 0xfaf3b60;
    uVar1 = uVar33 >> 8 & 0xff ^ 0x4f;
    uVar7 = uVar1 + (uVar1 & 0x1f) * -2 + 0x39b32a1f ^ 0x12873e17;
    uVar19 = ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x404205a4) * -2 + 0x54c7c5a4 ^ 0x1595c5d1) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x404205a4) * -2 + 0x54c7c5a4 ^ 0x1595c5d1) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x38226c72)) + 0xb5c2f401;
    uVar1 = uVar5 & 0xff ^ 0xa8;
    uVar9 = uVar1 + (uVar1 & 0x3d) * -2 + 0x155df83d ^ 0x375cc19;
    uVar1 = ((uVar33 >> 0x10 & 0xff ^ 3) - (uVar33 >> 0xf & 0x120)) + 0xb2bf4490 ^ 0x22b45490;
    uVar1 = ((int32_t)read32(puVar30 + (ulong)(uVar1 + (uVar1 & 0x10091278) * -2 + 0x973d5a78 ^ 0x7364a17) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar1 + (uVar1 & 0x10091278) * -2 + 0x973d5a78 ^ 0x7364a17) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xef45a2ba)) + 0x11548f25;
    uVar7 = read32(puVar23 + (ulong)(uVar6 >> 0x18 ^ 0xa1) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar7 + (uVar7 & 0x810042e) * -2 + 0x1cd04c2e ^ 0x37e458a9) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar9 + (uVar9 & 0x10081851) * -2 + 0x191c9851 ^ 0xf34acc9) * 4 + -0x67a9fe82) ^ uVar19 ^ uVar19 >> 3 ^ uVar19 >> 4 ^ 0x9b00e69f;
    uVar19 = uVar5 >> 8 & 0xff ^ 0x8b;
    uVar19 = uVar19 + (uVar19 & 0xb7) * -2 + 0x11ff6b7 ^ 0x95c477;
    uVar9 = uVar6 & 0xff ^ 0x71;
    uVar9 = uVar9 + (uVar9 & 0x72) * -2 + 0x9b1cc72 ^ 0x1a96daa;
    uVar5 = uVar5 >> 0x10 & 0xff ^ 0xae;
    uVar5 = uVar5 + (uVar5 & 3) * -2 + 0x93b8c803 ^ 0x1110c903;
    uVar9 = read32(puVar23 + (ulong)(uVar17 >> 0x18 ^ 0x2f) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar19 + (uVar19 & 0x10a012b) * -2 + 0x331a852b ^ 0x3290b7bf) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar9 + (uVar9 & 0x810a3dd) * -2 + 0x3ef7a3dd ^ 0x36ef0260) * 4 + -0x67a9fe82) ^ uVar1 ^ uVar1 >> 3 ^ uVar1 >> 4 ^ 0xcf6e74bf;
    uVar19 = ((int32_t)read32(puVar30 + (ulong)(uVar5 + (uVar5 & 0x280722) * -2 + 0xf13ba722 ^ 0x7393a6d4) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar5 + (uVar5 & 0x280722) * -2 + 0xf13ba722 ^ 0x7393a6d4) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x54df4b28)) + 0xc421635c;
    uVar1 = uVar6 >> 8 & 0xff ^ 0xd6;
    uVar1 = uVar1 + (uVar1 & 0xa2) * -2 + 0x2b4d5da2 ^ 0x2cdf4a0;
    uVar5 = read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x29802f1e) * -2 + 0x3dce6f1e ^ 0x144ec6c8) * 4 + -0x6bf722f6) << 0x16 ^ 0x65400000;
    uVar1 = uVar5 + (uVar5 & 0x16c00000) * -2 + 0x16fcb410 ^ 0x16fcb410 | read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x29802f1e) * -2 + 0x3dce6f1e ^ 0x144ec6c8) * 4 + -0x6bf722f6) >> 10 ^ 0x10ccb6;
    uVar1 = uVar1 + (uVar1 & 0x863bf98) * -2 + 0x863bf98;
    uVar5 = uVar1 * 0x400 ^ 0xb90aac00;
    uVar1 = ((uVar5 + (uVar5 & 0x50ba7400) * -2 + 0x50ba77ad ^ 0x50ba77ad) & 0xfffffc00 | uVar1 >> 0x16) ^ 0xf3;
    uVar5 = uVar17 & 0xff ^ 0xc9;
    uVar5 = uVar5 + (uVar5 & 0x80) * -2 + 0xbbf98a80 ^ 0x39fba210;
    uVar1 = read32(puVar23 + (ulong)(uVar33 >> 0x18 ^ 0x31) * 4 + -0x5e5f8f82) ^ read32(puVar25 + (ulong)(uVar5 + (uVar5 & 0x21f3) * -2 + 0xc05161f3 ^ 0x42534980) * 4 + -0x67a9fe82) ^ uVar19 ^ uVar19 >> 3 ^ uVar19 >> 4 ^ uVar1 + (uVar1 & 0x70d60361) * -2 + 0x70d60361 ^ 0x2d27305d;
    uVar33 = uVar7 >> 0x10 & 0xff ^ 0x6f;
    uVar33 = uVar33 + (uVar33 & 0x56) * -2 + 0xe182f256 ^ 0x68cbaa70;
    uVar33 = ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x1010a61) * -2 + 0x67272a61 ^ 0xee6e72de) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x1010a61) * -2 + 0x67272a61 ^ 0xee6e72de) * 4 + -0x6718dde2) * 2 + 0x13637b90U & 0x15483cd8)) + 0xa455dc34;
    uVar5 = uVar9 >> 0x10 & 0xff;
    uVar19 = uVar5 ^ 0x8c;
    uVar19 = uVar19 + (uVar19 & 0xe6) * -2 + 0x305fe1e6 ^ 0x114ee122;
    uVar19 = uVar19 + (uVar19 & 0x11124bf) * -2 + 0x5115a4bf;
    uVar5 = ((uVar5 ^ 4) - (uVar9 >> 0xf & 0xe4)) + 0x278972 ^ 0x60f36;
    uVar5 = uVar5 + (uVar5 & 0x18a8c) * -2 + 0xcd8a8c ^ 0xff13f3b9;
    uVar6 = uVar1 & 0xff ^ 5;
    uVar6 = uVar6 + (uVar6 & 0xb7) * -2 + 0x21b177b7 ^ 0x5b0f7be;
    uVar17 = uVar19 ^ 0x8ffb5b40;
    uVar19 = ((int32_t)read32(puVar30 + (ulong)((uVar19 ^ (uVar5 - (uVar5 * 2 + 0x186 & 0x65d75718)) + 0x32ebac4f ^ (uVar17 - (uVar17 * 2 + 2 & 0x3d6b6dd4)) + 0x1eb5b6eb ^ 0x5c5ab9d9) + 0xc2) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)((uVar19 ^ (uVar5 - (uVar5 * 2 + 0x186 & 0x65d75718)) + 0x32ebac4f ^ (uVar17 - (uVar17 * 2 + 2 & 0x3d6b6dd4)) + 0x1eb5b6eb ^ 0x5c5ab9d9) + 0xc2) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x40e2dc3a)) + 0xba232be5;
    uVar5 = read32(puVar23 + (ulong)(uVar8 >> 0x18 ^ 0xe3) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar9 >> 8 & 0xff ^ 0xc2) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar6 + (uVar6 & 0x8cd3) * -2 + 0x4100ecd3 ^ 0x65016c72) * 4 + -0x67a9fe82) ^ uVar33 ^ uVar33 >> 3 ^ uVar33 >> 4 ^ 0x933e3fbb;
    uVar33 = uVar8 & 0xff ^ 0x8b;
    uVar6 = uVar33 + (uVar33 & 0x36) * -2 + 0x4f89a36 ^ 0x20ea1357;
    uVar33 = uVar1 >> 8 & 0xff ^ 0x9b;
    uVar17 = uVar33 + (uVar33 & 0xe6) * -2 + 0x44f6d1e6 ^ 0x6e651ee;
    uVar33 = uVar1 >> 0x10 & 0xff ^ 0x1c;
    uVar33 = uVar33 + (uVar33 & 0x3b) * -2 + 0x2c83f33b ^ 0x4c07b33f;
    uVar33 = ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x6004192a) * -2 + 0x620f192a ^ 0x28b5946) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x6004192a) * -2 + 0x620f192a ^ 0x28b5946) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xb561fef4)) + 0xf462bd42;
    uVar6 = read32(puVar23 + (ulong)(uVar7 >> 0x18 ^ 0x91) * 4 + -0x5e5f8f82) ^ read32(puVar25 + (ulong)(uVar6 + (uVar6 & 0x4008067) * -2 + 0x4c88d067 ^ 0x689a59f1) * 4 + -0x67a9fe82) ^ read32(puVar24 + (ulong)(uVar17 + (uVar17 & 0x100a54) * -2 + 0x19f52a54 ^ 0x5be5aaf5) * 4 + -0x6bf722f6) ^ uVar19 ^ uVar19 >> 3 ^ uVar19 >> 4 ^ 0xe3b846eb;
    uVar19 = uVar8 >> 8 & 0xff ^ 0x3d;
    uVar19 = uVar19 + (uVar19 & 0x20) * -2 + 0x20 ^ 0x211;
    uVar17 = uVar7 & 0xff ^ 0xd0;
    uVar17 = uVar17 + (uVar17 & 0x30) * -2 + 0x109e30 ^ 0x29a38;
    uVar17 = read32(puVar23 + (ulong)(uVar9 >> 0x18 ^ 0x8a) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar19 + (uVar19 & 0x5d1c487) * -2 + 0x5d1c487 ^ 0x5d1c6a3) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar17 + (uVar17 & 0x100174) * -2 + 0x28fcd974 ^ 0x28eedd20) * 4 + -0x67a9fe82) ^ uVar33 ^ uVar33 >> 3 ^ uVar33 >> 4 ^ 0x7ee14644;
    uVar11 = uVar1 >> 0x18;
    uVar10 = (uVar11 | 0x7ef6044c) ^ 0x2777054d;
    uVar19 = uVar1 >> 0x17 & 0x80;
    uVar1 = (uVar11 ^ 0x3500078) & (uVar11 ^ 0x8b74d934);
    uVar33 = uVar8 >> 0x10 & 0xff ^ 0xe7;
    uVar33 = uVar33 + (uVar33 & 0x40) * -2 + 0xe3b5df40 ^ 0x53af566a;
    uVar33 = ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x3002a926) * -2 + 0xf326a926 ^ 0x433c200c) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x3002a926) * -2 + 0xf326a926 ^ 0x433c200c) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xf51abb2c)) + 0x143f1b5e;
    uVar8 = ((uVar11 & 0x4c ^ 0xc) - uVar19) + 0x5e167563 ^ 0x9f161044;
    uVar7 = uVar7 >> 8 & 0xff ^ 0xb0;
    uVar7 = uVar7 + (uVar7 & 0xd4) * -2 + 0x1f389ad4 ^ 0xe3b99d6;
    iVar3 = ((((uVar11 & 0x4c ^ 0x20810c) - uVar19) + 0x41db7063 ^ 0x59f547b) + 0xabe1a802 ^ uVar8 + (uVar8 & 0x400020c0) * -2 + 0xfae7a8c0 ^ 0x438559f9) + (uVar10 + (uVar10 & 0x182) * -2 + 0x4562f8a ^ (uVar1 - ((uVar1 & 0xe9) << 1 | 0x4200000)) + 0x2e399ae9 ^ 0x73eeb462);
    uVar19 = read32(puVar24 + (ulong)(uVar7 + (uVar7 & 0x20065) * -2 + 0x6921065 ^ 0x17911361) * 4 + -0x6bf722f6) << 1 ^ 0x8665b32a;
    uVar19 = uVar19 + (uVar19 & 0x36ad6ec) * -2 + 0x1b6ad6ec;
    uVar1 = (read32(puVar24 + (ulong)(uVar7 + (uVar7 & 0x20065) * -2 + 0x6921065 ^ 0x17911361) * 4 + -0x6bf722f6) ^ 0x4332d995) + 0x8675554d;
    iVar4 = (uVar1 ^ 0x7fbdb75d) + (uVar1 * 2 & 0xff7b6eba) + (uVar19 & 0xceaaa9a ^ 0xea60a289) + (uVar19 * 2 & 0x19c11534 ^ 0xef3efaef) + 1;
    uVar19 = uVar9 & 0xff ^ 0x9d;
    uVar19 = uVar19 + (uVar19 & 0x6b) * -2 + 0x7d6aa6b ^ 0x34213aa;
    uVar8 = read32(puVar23 + (ulong)(uVar11 ^ 0x33) * 4 + -0x5e5f8f82) ^ read32(puVar25 + (ulong)(uVar19 + (uVar19 & 0x142b3f) * -2 + 0x19162f3f ^ 0x1d829607) * 4 + -0x67a9fe82) ^ uVar33 ^ uVar33 >> 3 ^ uVar33 >> 4 ^ (iVar4 - (iVar4 * 2 + 0x3c705144U & 0x896176d0)) + 0xe2e8e40a ^ (iVar3 - (iVar3 * 2 & 0x6b345ca4U)) + 0x359a2e52 ^ 0xfdc2c631;
    uVar1 = uVar6 & 0xa0 ^ 0x18408061;
    uVar33 = ((int32_t)read32(puVar30 + (ulong)(uVar6 >> 0x10 & 0xff ^ 0xfe) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar6 >> 0x10 & 0xff ^ 0xfe) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x66f26368)) + 0xcd2aef7c;
    uVar19 = uVar8 & 0xff ^ 0x1e;
    uVar7 = uVar19 + (uVar19 & 0xce) * -2 + 0x1525cce ^ 0x45bfceb;
    uVar33 = read32(puVar23 + (ulong)(uVar5 >> 0x18 ^ 0xea) * 4 + -0x5e5f8f82) ^ uVar33 ^ uVar33 >> 3 ^ uVar33 >> 4 ^ 0xc7ed5af;
    uVar19 = uVar33 << 0x16 ^ 0xdd400000;
    uVar33 = (uVar19 + (uVar19 & 0x67ffffff) * -2 + 0x67f0fc91 ^ 0x67f0fc91 | uVar33 >> 10) ^ 0x3d7525;
    uVar33 = uVar33 + (uVar33 & 0x11928c5c) * -2 + 0x11928c5c;
    uVar19 = uVar33 * 0x400 ^ 0x3d9e6400;
    uVar9 = uVar17 >> 8 & 0xff ^ 0xba;
    uVar9 = uVar9 + (uVar9 & 200) * -2 + 0x889cd3c8 ^ 0x98cd1c8;
    uVar19 = uVar19 + (uVar19 & 0x2d3ca400) * -2 + 0x2d3ca652 ^ 0x2d3ca652;
    uVar33 = uVar33 >> 0x16;
    uVar19 = ((uVar19 & 0xfffffc00 | uVar33) ^ 0x177cfd0c) + ((uVar19 & 0x177cfc00 | uVar33) << 1 ^ 0x5e6) + 0xe8830001;
    uVar33 = uVar19 & (read32(puVar24 + (ulong)(uVar9 + (uVar9 & 0x1004fb) * -2 + 0xc23e44fb ^ 0x432e46b6) * 4 + -0x6bf722f6) ^ 0xc5478cd8);
    uVar33 = (uVar33 + (uVar33 & 0x3dbccffa) * -2) * 2 + 0xfb799ff4;
    uVar19 = uVar19 + (read32(puVar24 + (ulong)(uVar9 + (uVar9 & 0x1004fb) * -2 + 0xc23e44fb ^ 0x432e46b6) * 4 + -0x6bf722f6) ^ 0xc5478cd8);
    iVar4 = (uVar19 ^ 0x2ff73ef1) + (uVar19 * 2 & 0x5fee7de2) + (uVar33 ^ 0x6bf1db23) + (uVar33 * 2 & 0xdeef7650 ^ 0x291cc9bf) + 1;
    uVar7 = read32(puVar25 + (ulong)(uVar7 + (uVar7 & 0x108060d) * -2 + 0x9ac1e0d ^ 0xca5be9a) * 4 + -0x67a9fe82) ^ (iVar4 - (iVar4 * 2 + 0x1f00f870U & 0x190cf782)) + 0x4c06f7f9;
    uVar33 = uVar17 >> 0x10 & 0xff ^ 0x4b;
    uVar33 = uVar33 + (uVar33 & 0xba) * -2 + 0x3dfbd7ba ^ 0x15f307ba;
    uVar33 = ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x2000c0fe) * -2 + 0x60f2c0fe ^ 0x48fa10d8) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x2000c0fe) * -2 + 0x60f2c0fe ^ 0x48fa10d8) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xadf907ca)) + 0xf0ae41ad;
    uVar19 = uVar8 >> 8 & 0xff ^ 0x72;
    uVar9 = uVar19 + (uVar19 & 0xc5) * -2 + 0xe00433c5 ^ 0x2485e6d5;
    uVar19 = uVar5 & 0xff ^ 0xe4;
    uVar19 = uVar19 + (uVar19 & 0x7d) * -2 + 0xf67af57d ^ 0x7269f53d;
    uVar19 = uVar19 + (uVar19 & 0x4020054) * -2 + 0xdc6e9854;
    uVar10 = uVar5 & 0xff ^ 0x9a;
    uVar10 = uVar10 + (uVar10 & 0x73) * -2 + 0xc1877c73 ^ 0x50cb3013;
    uVar10 = uVar10 + (uVar10 & 0x1000927) * -2 + 0x8d90a927 ^ 0xe3231a0e;
    uVar11 = uVar19 ^ 0xa782670b;
    uVar9 = read32(puVar23 + (ulong)(uVar6 >> 0x18 ^ 0xa5) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar9 + (uVar9 & 0x48114ad) * -2 + 0x9cd514ad ^ 0x5854c1f7) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)((uVar19 ^ (uVar10 - (uVar10 * 2 + 0x52 & 0xbafdc088)) + 0x5d7ee06d ^ (uVar11 - (uVar11 * 2 + 2 & 0x4ccafe5c)) + 0x26657f2f ^ 0x2366079e) + 0x28) * 4 + -0x67a9fe82) ^ uVar33 ^ uVar33 >> 3 ^ uVar33 >> 4 ^ 0xe96b771f;
    uVar33 = uVar8 >> 0x10 & 0xff ^ 0x1c;
    uVar33 = uVar33 + (uVar33 & 0x13) * -2 + 0x3b464f13 ^ 0xdb6aceb2;
    uVar33 = ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x20281353) * -2 + 0xa6291353 ^ 0x46059270) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar33 + (uVar33 & 0x20281353) * -2 + 0xa6291353 ^ 0x46059270) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xfb5457ba)) + 0x175be9a5;
    uVar19 = uVar6 & 0xff ^ 0xb9;
    uVar19 = uVar19 + (uVar19 & 0x73) * -2 + 0x695c0573 ^ 0x297c0091;
    uVar10 = uVar19 + (uVar19 & 0x20006a) * -2 + 0x187f786a ^ 0x585f7d6a | uVar1 + (uVar1 & 0x3fffc0) * -2 + 0x228fdbd6 ^ 0x3acf5bb7;
    uVar19 = uVar5 >> 8 & 0xff ^ 0x56;
    uVar1 = uVar19 + (uVar19 & 0xb3) * -2 + 0x567c6b3 ^ 0x4347d684;
    uVar19 = ((int32_t)read32(puVar30 + (ulong)(uVar5 >> 0x10 & 0xff ^ 0xc9) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar5 >> 0x10 & 0xff ^ 0xc9) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xea84f540)) + 0xef43868;
    uVar10 = read32(puVar23 + (ulong)(uVar17 >> 0x18 ^ 0x77) * 4 + -0x5e5f8f82) ^ read32(puVar25 + (ulong)(uVar10 + (uVar10 & 0xe9cf18b) * -2 + 0xe9cf18b ^ 0xe9cf1a3) * 4 + -0x67a9fe82) ^ read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x4001231) * -2 + 0x1dc2f231 ^ 0x5be2e2ae) * 4 + -0x6bf722f6) ^ uVar33 ^ uVar33 >> 3 ^ uVar33 >> 4 ^ 0xbf6c8b39;
    uVar5 = ((uVar6 >> 8 & 0xff ^ 0x34) - (uVar6 >> 7 & 0x112)) + 0x73f21489 ^ 0x3cc28d45;
    uVar33 = (uVar7 ^ 0xcef7211e) + 0xe62a9000;
    uVar1 = (uVar7 ^ 0x55f71ddc) << 1 ^ 0x36007984;
    uVar1 = uVar1 + (uVar1 & 0x3b5ceee4) * -2 + 0x3b5ceee5;
    uVar6 = uVar17 & 0xff ^ 0xf3;
    uVar6 = uVar6 + (uVar6 & 0x7fffffc7) * -2 + 0xadb8bc7 ^ 0x6c907e7;
    uVar6 = read32(puVar23 + (ulong)(uVar8 >> 0x18 ^ 0xdc) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar5 + (uVar5 & 0x4510029c) * -2 + 0x659a229c ^ 0x2aaabbc5) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar6 + (uVar6 & 0x1204a0) * -2 + 0x131614a0 ^ 0x1f0498b3) * 4 + -0x67a9fe82) ^ uVar19 ^ uVar19 >> 3 ^ uVar19 >> 4 ^ 0xf7f713db;
    iVar4 = (uVar33 ^ 0x3ffef9f4) + (uVar33 * 2 & 0x7ffdf3e8) + (uVar1 & 0xcc552000 ^ 0x9146013) + (uVar1 * 2 & 0x982a4000 ^ 0xefd7bfff) + 1;
    uVar19 = (iVar4 - (iVar4 * 2 + 0x7d818bf2U & 0xb985fe88)) + 0x1b83c53d;
    uVar33 = uVar10 >> 8 & 0xff ^ 0xb2;
    uVar1 = uVar33 + (uVar33 & 0x2f) * -2 + 0x28b65e2f ^ 0x617ce47;
    uVar33 = ((int32_t)read32(puVar30 + (ulong)(uVar9 >> 0x10 & 0xff ^ 0xd4) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar9 >> 0x10 & 0xff ^ 0xd4) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x70edc8ea)) + 0xd228a23d;
    uVar1 = read32(puVar23 + (ulong)(uVar19 >> 0x18 ^ 0x6f) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x2c000028) * -2 + 0x7d420028 ^ 0x53e390a4) * 4 + -0x6bf722f6) ^ uVar33 ^ uVar33 >> 3 ^ uVar33 >> 4;
    uVar33 = uVar6 & 0xff ^ 0x41;
    uVar33 = uVar33 + (uVar33 & 0x68) * -2 + 0xdf3ea368 ^ 0xebf296c;
    uVar17 = read32(puVar25 + (ulong)(uVar33 + (uVar33 & 0x5000af6a) * -2 + 0x566aef6a ^ 0x87eb657d) * 4 + -0x67a9fe82);
    uVar5 = uVar1 ^ 0x2ba5617;
    uVar33 = (uVar17 ^ uVar5 ^ 0x7ea8f9a5) + 1;
    iVar4 = ((uVar5 & 0x6e7f9bdf) << 1 ^ 0x844c310a) + (uVar1 ^ 0xafe3914d);
    iVar3 = (uVar33 * 2 & 0xfbd7ffee) + (uVar33 ^ 0xfdebfff7) + iVar4;
    iVar4 = iVar4 + (uVar17 ^ ((uVar17 ^ 0x3d8ea520) - ((uVar17 ^ 0x3d8ea520) * 2 + 2 & 0xc15bf342)) + 0x60adf9a2 ^ (iVar3 - (iVar3 * 2 + 0x2728c854U & 0x8d8fde80)) + 0x5a5c536a ^ 0xe41b4c3e) + 0x11806421;
    uVar33 = (iVar4 - (iVar4 * 2 & 0xa4999b8U)) + 0x524ccdc;
    uVar1 = uVar10 >> 0x10 & 0xff ^ 0x72;
    uVar1 = uVar1 + (uVar1 & 0x92) * -2 + 0xfbb19192 ^ 0x72a15112;
    uVar1 = ((int32_t)read32(puVar30 + (ulong)(uVar1 + (uVar1 & 0x5dcd) * -2 + 0xe6205dcd ^ 0x6f309d93) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar1 + (uVar1 & 0x5dcd) * -2 + 0xe6205dcd ^ 0x6f309d93) * 4 + -0x6718dde2) * 2 + 0x13637b90U & 0x1c435818)) + 0xa7d369d4;
    uVar5 = uVar6 >> 8 & 0xff ^ 0x77;
    uVar17 = uVar5 + (uVar5 & 0x3b) * -2 + 0x28badd3b ^ 0x9a5017;
    uVar8 = uVar19 & 0xff ^ 0xc9;
    uVar5 = uVar6 >> 0x10 & 0xff ^ 0x37;
    uVar5 = uVar5 + (uVar5 & 7) * -2 + 0xb36c6f07 ^ 0x36626bc7;
    uVar5 = ((int32_t)read32(puVar30 + (ulong)(uVar5 + (uVar5 & 0x40e29d4) * -2 + 0xf44f69d4 ^ 0x71416d69) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar5 + (uVar5 & 0x40e29d4) * -2 + 0xf44f69d4 ^ 0x71416d69) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xaf203c22)) + 0xf141dbd9;
    uVar17 = read32(puVar23 + (ulong)(uVar9 >> 0x18 ^ 0xbc) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar17 + (uVar17 & 0x2007bc) * -2 + 0x4e777bc ^ 0x2cc7fa16) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(0xfa4e97ff - (uVar8 + (uVar8 & 0x78) * -2 + 0xd6417678 ^ 0x5e49f47c) ^ 0x7246155e) * 4 + -0x67a9fe82) ^ uVar1 ^ uVar1 >> 3 ^ uVar1 >> 4 ^ 0xaf3db330;
    uVar1 = uVar9 & 0xff ^ 0xad;
    uVar1 = uVar1 + (uVar1 & 0xa5) * -2 + 0x59e2ffa5 ^ 0x114a3f2c;
    uVar5 = read32(puVar23 + (ulong)(uVar10 >> 0x18 ^ 0x9d) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar19 >> 8 & 0xff ^ 0x94) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar1 + (uVar1 & 0x48897da1) * -2 + 0x5add7da1 ^ 0x1275bdba) * 4 + -0x67a9fe82) ^ uVar5 ^ uVar5 >> 3 ^ uVar5 >> 4 ^ 0x7b9d4a45;
    uVar19 = ((int32_t)read32(puVar30 + (ulong)(uVar19 >> 0x10 & 0xff) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar19 >> 0x10 & 0xff) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xee0ba576)) + 0x10b79083;
    uVar1 = uVar10 & 0xff ^ 0x9b;
    uVar1 = uVar1 + (uVar1 & 0x48) * -2 + 0x1815bc48 ^ 0x8418c69;
    uVar6 = read32(puVar23 + (ulong)(uVar6 >> 0x18 ^ 0xb6) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar9 >> 8 & 0xff ^ 0x1f) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar1 + (uVar1 & 0x10441645) * -2 + 0x5eedd645 ^ 0x4eb9e645) * 4 + -0x67a9fe82) ^ uVar19 ^ uVar19 >> 3 ^ uVar19 >> 4 ^ 0x207f70d0;
    uVar19 = uVar17 >> 0x10 & 0xff ^ 0x72;
    uVar19 = uVar19 + (uVar19 & 0xdc) * -2 + 0x18cec2dc ^ 0xa96c284;
    uVar19 = ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x120003e7) * -2 + 0x7786f3e7 ^ 0x65def3fe) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x120003e7) * -2 + 0x7786f3e7 ^ 0x65def3fe) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xa3cfd7f4)) + 0xeb99a9c2;
    uVar1 = uVar5 >> 8 & 0xff ^ 0x3f;
    uVar1 = uVar1 + (uVar1 & 0x84) * -2 + 0xe614a084 ^ 0x6e148000;
    uVar8 = (uVar6 & 0xff ^ 0x67) + (uVar6 & 0x88) * -2 + 0x2396a988 ^ 0x9abe8cc8;
    uVar8 = read32(puVar23 + (ulong)(uVar33 >> 0x18 ^ 7) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x8000f52) * -2 + 0x78664f52 ^ 0xf0666fe8) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar8 + (uVar8 & 0xf9a) * -2 + 0x82d38f9a ^ 0x3bfbaaf5) * 4 + -0x67a9fe82) ^ uVar19 ^ uVar19 >> 3 ^ uVar19 >> 4 ^ 0x614a08e6;
    uVar19 = uVar5 >> 0x10 & 0xff ^ 0xf3;
    uVar19 = uVar19 + (uVar19 & 0x8a) * -2 + 0x7b979e8a ^ 0x39949e98;
    uVar19 = ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x20192) * -2 + 0x198aed92 ^ 0x5b89edce) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x20192) * -2 + 0x198aed92 ^ 0x5b89edce) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x922098fc)) + 0xe2c20a46;
    uVar1 = uVar6 >> 8 & 0xff ^ 0x6f;
    uVar1 = uVar1 + (uVar1 & 0x8d) * -2 + 0x7eb5238d ^ 0x1d3363bc;
    uVar7 = uVar33 & 0xff ^ 0x81;
    uVar7 = uVar7 + (uVar7 & 0xa6) * -2 + 0x752f1aa6 ^ 0x376a1af2;
    uVar7 = read32(puVar23 + (ulong)(uVar17 >> 0x18 ^ 0x17) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x43827240) * -2 + 0x4383f240 ^ 0x2005b200) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar7 + (uVar7 & 0x40026e) * -2 + 0x19529a6e ^ 0x5b179af8) * 4 + -0x67a9fe82) ^ uVar19 ^ uVar19 >> 3 ^ uVar19 >> 4 ^ 0x90c85d84;
    uVar19 = uVar6 >> 0x10 & 0xff ^ 0x1f;
    uVar19 = uVar19 + (uVar19 & 0x7b) * -2 + 0xc836d7b ^ 0x4296cfb;
    uVar19 = ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x828069f) * -2 + 0x6d79269f ^ 0x65d327e5) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar19 + (uVar19 & 0x828069f) * -2 + 0x6d79269f ^ 0x65d327e5) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0x3fc1ee44)) + 0xb992b4ea;
    uVar1 = uVar33 >> 8 & 0xff ^ 0x5e;
    uVar1 = uVar1 + (uVar1 & 0x2b) * -2 + 0x2bd4422b ^ 0xb90c229;
    uVar9 = read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x440136) * -2 + 0xc6f7d36 ^ 0x2c2bfd55) * 4 + -0x6bf722f6) & 0xd64fe9ea ^ 0x1002084a;
    uVar9 = uVar9 + (uVar9 & 0x4608a828) * -2 + 0x66a8ba3c ^ 0x3e283bf6;
    uVar1 = read32(puVar24 + (ulong)(uVar1 + (uVar1 & 0x440136) * -2 + 0xc6f7d36 ^ 0x2c2bfd55) * 4 + -0x6bf722f6) & 0x29b01615 ^ 0x8900414;
    uVar10 = uVar1 + (uVar1 & 0x29a00014) * -2 + 0x7fed2956 ^ 0x26e5bd7d;
    uVar1 = uVar17 & 0xff ^ 0xe;
    uVar1 = uVar1 + (uVar1 & 0xa6) * -2 + 0x16dd8a6 ^ 0xbdaa1e;
    uVar9 = read32(puVar23 + (ulong)(uVar5 >> 0x18 ^ 0xc5) * 4 + -0x5e5f8f82) ^ uVar19 ^ read32(puVar25 + (ulong)(uVar1 + (uVar1 & 0x14039d4) * -2 + 0x3960b9d4 ^ 0x38b0cb0f) * 4 + -0x67a9fe82) ^ uVar19 >> 3 ^ uVar19 >> 4 ^ uVar9 + (uVar9 & 0x7826afb0) * -2 + 0x7926afb5 ^ uVar10 + (uVar10 & 0x6dd88604) * -2 + 0x6dd9af04 ^ 0x4ab22eea;
    uVar19 = uVar17 >> 8 & 0xff ^ 0x9e;
    uVar17 = uVar19 + (uVar19 & 0xed) * -2 + 0x3b46aed ^ 0x1baeacd;
    uVar19 = ((int32_t)read32(puVar30 + (ulong)(uVar33 >> 0x10 & 0xff ^ 0xfb) * 4 + -0x6718dde2) - ((int32_t)read32(puVar30 + (ulong)(uVar33 >> 0x10 & 0xff ^ 0xfb) * 4 + -0x6718dde2) * 2 + 0x33637b90U & 0xfebc02a8)) + 0x190fbf1c;
    uVar33 = uVar5 & 0xff ^ 0xe2;
    uVar33 = uVar33 + (uVar33 & 0xd9) * -2 + 0x5aa19dd9 ^ 0x32219d51;
    uVar1 = uVar33 + (uVar33 & 0x800059a) * -2 + 0xd3da59a;
    uVar33 = uVar5 << 1 ^ 0x8afed45c;
    uVar33 = uVar33 + (uVar33 & 0x7e) * -2 + 0x27f;
    uVar33 = (uVar33 & 0x162 ^ 0x69f79bbe) + (uVar33 * 2 & 0x280 ^ 0x80) + (uVar1 ^ 0x7a84de6f) + 0x96086424;
    iVar4 = (((uVar1 ^ 0x65bda5de) + (uVar33 ^ 0x94384000)) - (uVar33 * 2 & 0xd78f7ffe)) + 0x6bc7c000;
    uVar33 = ((uVar5 & 0xff ^ 0xa0) - (uVar5 << 1 & 0x8a)) + 0x4d19d245 ^ 0x8b58e845;
    uVar33 = read32(puVar23 + (ulong)(uVar6 >> 0x18 ^ 0x83) * 4 + -0x5e5f8f82) ^ read32(puVar24 + (ulong)(uVar17 + (uVar17 & 0xe0121) * -2 + 0x1bf5921 ^ 0x3b1d915) * 4 + -0x6bf722f6) ^ read32(puVar25 + (ulong)(uVar33 + (uVar33 & 0x20028a6) * -2 + 0x32b8a8a6 ^ 0xf4f99200) * 4 + -0x67a9fe82) ^ uVar19 ^ (iVar4 - (iVar4 * 2 & 0x1dd1d852U)) + 0xee8ec29 ^ uVar19 >> 3 ^ uVar19 >> 4 ^ 0xdc90db95;
    uVar1 = uVar8 >> 0x18;
    uVar19 = uVar7 >> 0x10 & 0xff ^ 199;
    uVar19 = uVar19 + (uVar19 & 0x84) * -2 + 0x22767484 ^ 0x2553109;
    puVar23 = pointer_table[iVar26 + 0x7c];
    puVar24 = pointer_table[iVar26 + 0x852];
    iVar4 = (uVar1 ^ ((uVar1 ^ 0x78) - ((uVar1 ^ 0x78) * 2 + 0x2e & 0x4c | 0x81089c00)) + 0x3d79acfd ^ 0x48b54e5e) + (int32_t)read32(puVar24 + (ulong)(uVar1 ^ 0x14) * 4 + -0x3c1c9923) + -0x7ae940df;
    uVar1 = uVar9 >> 8 & 0xff ^ 0xb7;
    uVar1 = uVar1 + (uVar1 & 0x7fffffc3) * -2 + 0xe2acaac3 ^ 0x379eaee3;
    puVar27 = pointer_table[iVar26 + 0x1e3];
    uVar5 = read32(puVar27 + (ulong)(uVar1 + (uVar1 & 0x51300291) * -2 + 0xd3307a91 ^ 0x6027eee) * 4 + -0x6559e172);
    iVar3 = ((uVar5 & 0xd6b9fab) << 3 ^ 0x808ad00) + (uVar5 << 2 ^ 0x71ba287f);
    uVar6 = uVar7 >> 0x18;
    puVar25 = pointer_table[iVar26 + 0x9be];
    uVar1 = uVar9 >> 0x10 & 0xff ^ 0xf2;
    uVar1 = uVar1 + (uVar1 & 0x94) * -2 + 0x18729f94 ^ 0x8021f14;
    uVar5 = read32(puVar23 + (ulong)(uVar19 + (uVar19 & 0x2307e6) * -2 + 0x8e71fe6 ^ 0x28c45a1f) * 4 + -0x797423bb) ^ uVar5 ^ (iVar4 - (iVar4 * 2 & 0xfaa30feaU)) + 0x7d5187f5 ^ ((int32_t)read32(puVar25 + (ulong)(uVar33 & 0xff ^ 0xef) * 4 + -0x2dca5a37) - ((int32_t)read32(puVar25 + (ulong)(uVar33 & 0xff ^ 0xef) * 4 + -0x2dca5a37) * 2 + 0xc3affb8aU & 0x98deb7a2)) + 0x2e475996 ^ (iVar3 - (iVar3 * 2 + 0x5e300d2U & 0x7094bff0)) + 0x3b3be064 ^ 0xa576c69b;
    iVar4 = (uVar6 ^ ((uVar6 ^ 0x22) - ((uVar6 ^ 0x22) * 2 + 0x1ae & 0x1a4 | 0x89881c00)) + 0x43a38da9 ^ 0x4edf2ef0) + (int32_t)read32(puVar24 + (ulong)(uVar6 ^ 0x4e) * 4 + -0x3c1c9923) + -0x7ae940df;
    uVar19 = uVar33 >> 8 & 0xff ^ 0x9a;
    uVar19 = uVar19 + (uVar19 & 0xb0) * -2 + 0xaf09ffb0 ^ 0x2f80fff0;
    uVar17 = read32(puVar27 + (ulong)(uVar19 + (uVar19 & 0x5da4c) * -2 + 0xc475da4c ^ 0x44fcda9a) * 4 + -0x6559e172);
    iVar3 = ((uVar17 & 0x17ddefd9) << 3 ^ 0x88282c80) + (uVar17 << 2 ^ 0x1b63e9b7);
    uVar10 = uVar9 >> 0x18;
    uVar19 = uVar8 & 0xff ^ 0xdb;
    uVar19 = uVar19 + (uVar19 & 0x3d) * -2 + 0xd437ae3d ^ 0x4423aefd;
    uVar6 = uVar8 >> 8 & 0xff ^ 0xd1;
    uVar6 = uVar6 + (uVar6 & 0xd5) * -2 + 0xb7b10ed5 ^ 0x3ce32a4c;
    iVar2 = (int32_t)read32(puVar24 + (ulong)(uVar10 ^ 0x20) * 4 + -0x3c1c9923) + (uVar10 ^ ((uVar10 ^ 0x4c) - ((uVar10 ^ 0x4c) * 2 + 0x2e & 0x76 | 0x21808c00)) + 0xf94c512 ^ 0x1ad06677) + -0x7ae940df;
    uVar17 = read32(puVar23 + (ulong)(uVar1 + (uVar1 & 0x1000fcc9) * -2 + 0x3e0ffcc9 ^ 0x2e7f7cda) * 4 + -0x797423bb) ^ uVar17 ^ (iVar4 - (iVar4 * 2 & 0xc6cebd9eU)) + 0x63675ecf ^ ((int32_t)read32(puVar25 + (ulong)(uVar19 + (uVar19 & 0x4013c) * -2 + 0x25cc3d3c ^ 0xb5d83d4b) * 4 + -0x2dca5a37) - ((int32_t)read32(puVar25 + (ulong)(uVar19 + (uVar19 & 0x4013c) * -2 + 0x25cc3d3c ^ 0xb5d83d4b) * 4 + -0x2dca5a37) * 2 + 0x3affb8aU & 0x39061f0)) + 0xe3a02ebd ^ (iVar3 - (iVar3 * 2 + 0xb2507f62U & 0xa051e530)) + 0x29513249 ^ 0xd037ac7d;
    uVar1 = read32(puVar27 + (ulong)(uVar6 + (uVar6 & 0x24227dd) * -2 + 0xc64ba7dd ^ 0x4d198321) * 4 + -0x6559e172);
    iVar4 = ((uVar1 & 0x1fa3dbf7) << 3 ^ 0x88088da0) + (uVar1 << 2 ^ 0x3a9b390f);
    uVar19 = uVar7 & 0xff ^ 0xe1;
    uVar19 = uVar19 + (uVar19 & 0xa2) * -2 + 0x8fda2 ^ 0x1dd68;
    uVar6 = uVar33 >> 0x18;
    iVar3 = (uVar6 ^ ((uVar6 ^ 0x88) - ((uVar6 ^ 0x88) * 2 + 0x1ae & 0xfffffffb | 0xc000b400)) + 0x56dd5ad4 ^ 0x6218fb75) + (int32_t)read32(puVar24 + (ulong)(uVar6 ^ 0xe4) * 4 + -0x3c1c9923) + -0x7ae940df;
    uVar6 = uVar1 ^ read32(puVar23 + (ulong)(uVar33 >> 0x10 & 0xff ^ 0x12) * 4 + -0x797423bb) ^ (iVar2 - (iVar2 * 2 & 0x49ff59b2U)) + 0x24ffacd9 ^ ((int32_t)read32(puVar25 + (ulong)(uVar19 + (uVar19 & 0x807ae) * -2 + 0x6b4ad7ae ^ 0x6b43f76e) * 4 + -0x2dca5a37) - ((int32_t)read32(puVar25 + (ulong)(uVar19 + (uVar19 & 0x807ae) * -2 + 0x6b4ad7ae ^ 0x6b43f76e) * 4 + -0x2dca5a37) * 2 + 0xc3affb8aU & 0xc526849a)) + 0x446b4012 ^ (iVar4 - (iVar4 * 2 + 0x74211e72U & 0xc7beb160)) + 0x1defe7ea ^ 0xc9c6698e;
    uVar33 = uVar7 >> 8 & 0xff ^ 0x23;
    uVar33 = uVar33 + (uVar33 & 0x7fffff83) * -2 + 0xc85383 ^ 0x8d392;
    uVar19 = read32(puVar27 + (ulong)(uVar33 + (uVar33 & 0xc01b2d) * -2 + 0x79c57b2d ^ 0x7905fb91) * 4 + -0x6559e172);
    uVar33 = uVar9 & 0xff ^ 99;
    uVar33 = uVar33 + (uVar33 & 0x13) * -2 + 0x5ae68e13 ^ 0x84a6a5e7;
    iVar4 = ((uVar19 & 0xff9f6bb) << 3 ^ 0x808a580) + (uVar19 << 2 ^ 0x7bf38c3f);
    uVar8 = uVar19 ^ read32(puVar23 + (ulong)(uVar8 >> 0x10 & 0xff ^ 0x69) * 4 + -0x797423bb) ^ (iVar3 - (iVar3 * 2 & 0xa612af26U)) + 0x53095793 ^ ((int32_t)read32(puVar25 + (ulong)(uVar33 + (uVar33 & 0x5e00222e) * -2 + 0xdfb7e62e ^ 0x1f7cd38) * 4 + -0x2dca5a37) - ((int32_t)read32(puVar25 + (ulong)(uVar33 + (uVar33 & 0x5e00222e) * -2 + 0xdfb7e62e ^ 0x1f7cd38) * 4 + -0x2dca5a37) * 2 + 0xc3affb8aU & 0xcc969602)) + 0x482348c6 ^ (iVar4 - (iVar4 * 2 + 0xf1704852U & 0xb26ace00)) + 0x51ed8b2c ^ 0xf4fedd5a;
    uVar33 = uVar5 >> 8 & 0xff ^ 0x6e;
    uVar33 = uVar33 + (uVar33 & 0x39) * -2 + 0x2a3ad739 ^ 0xa695067;
    puVar24 = pointer_table[iVar26 + 0x5c4];
    bVar15 = (byte)(uVar5 >> 8) ^ 0x22;
    uStack_80[2] = read8(puVar24 + ((ulong)(uVar33 + (uVar33 & 0x201202af) * -2 + 0x273202af ^ 0x7618585) - 0x7b1e1573)) ^ (bVar15 - (bVar15 * '\x02' + 0x28 & 0x2c)) + 0x2a ^ 0xda;
    uVar33 = uVar6 >> 8 & 0xff ^ 0xcf;
    uVar33 = uVar33 + (uVar33 & 0x77777777) * -2 + 0x2dc45d77 ^ 0x70a5d12;
    uVar1 = uVar33 + (uVar33 & 0x2480674) * -2 + 0x348a674;
    uVar33 = uVar1 * 2 ^ 0x530d4c68;
    uVar33 = uVar33 + (uVar33 & 0x22) * -2 + 99;
    uVar19 = (uVar1 ^ 0x2986a634) + 0x38;
    cVar21 = ((byte)(uVar1 * 2) & 0x98 ^ 8) + ((byte)uVar1 ^ 0x78);
    uStack_80[10] = read8(puVar24 + ((ulong)((uVar19 ^ 0x76f8ffaf) + (uVar19 * 2 & 0xedf1ff5e) + ((uVar33 & 0x70 | (uVar33 >> 6 & 1) << 7) ^ 0x3c4255) + 0x88cabd9c) - 0x7b1e1573)) ^ (cVar21 - (cVar21 * '\x02' + 0x10U & 0x28)) - 0x24 ^ 0xc4;
    uVar33 = uVar17 & 0xff ^ 0xd3;
    uVar33 = uVar33 + (uVar33 & 0x6b) * -2 + 0x87da16b ^ 0x10d9b16f;
    puVar25 = pointer_table[iVar26 + 0xd38];
    bVar15 = read8(puVar25 + ((ulong)(uVar33 + (uVar33 & 0x843821) * -2 + 0x2dc7821 ^ 0x1a786883) - 0x32dcaf7f));
    uStack_80[7] = (bVar15 >> 2) * '\b' + (bVar15 & 8) * -4 + 0x10 ^ (((bVar15 >> 2 | bVar15 << 6) ^ 0x40) - (bVar15 >> 1 & 100)) + 0x32 ^ 0x7e;
    puVar23 = pointer_table[iVar26 + 0xfe] + -0xfc47af7;
    bVar15 = (byte)read8(puVar23 + ((ulong)(uVar5 >> 0x18) ^ 0xf7)) >> 4 ^ 0xff;
    bVar15 = (bVar15 & 0xd) + (bVar15 & 5) * -2 + 7 ^ 3;
    *uStack_80 = read8(puVar23 + ((ulong)(uVar5 >> 0x18) ^ 0xf7)) ^ bVar15 + (bVar15 & 0x4d) * -2 + 0x4d ^ 0xcd;
    bVar15 = (byte)read8(puVar23 + ((ulong)(uVar6 >> 0x18) ^ 0x5e)) >> 4 & 0xd ^ 9;
    uStack_80[8] = read8(puVar23 + ((ulong)(uVar6 >> 0x18) ^ 0x5e)) ^ bVar15 + (bVar15 & 1) * -2 + 1 ^ 0xdf;
    uVar33 = uVar6 & 0xff ^ 0x93;
    uVar33 = uVar33 + (uVar33 & 0xb0) * -2 + 0x3cb0 ^ 0x1cb5;
    bVar16 = read8(puVar25 + ((ulong)(uVar33 + (uVar33 & 0x272e) * -2 + 0x27f1b72e ^ 0x27f197f5) - 0x32dcaf7f));
    bVar15 = (bVar16 << 6 ^ 0x40) * '\x03' + 0x47 ^ 0x47;
    bVar18 = bVar15 | bVar16 >> 2;
    bVar15 = bVar18 + (bVar18 & 0x5d ^ bVar15 & 0x40) * -2 + 0x1d;
    bVar15 = bVar15 ^ ((bVar15 ^ 0xc) - ((bVar15 ^ 0xc) * '\x02' + 0x1e & 0x10)) - 9 ^ ((bVar15 ^ 0xe2) - ((bVar15 ^ 0xe2) * '\x02' + 2 & 0x6e)) + 0x38 ^ 0x22;
    uStack_80[0xb] = (bVar16 >> 2) * '\b' + (bVar16 & 0x28) * -4 + 0x52 ^ bVar15 + (bVar15 * -2 + 0x22 | 0xa1) + 0x1e ^ 0x7d;
    uVar33 = uVar8 & 0xff ^ 0x9f;
    uVar33 = uVar33 + (uVar33 & 9) * -2 + 0x1d84bd09 ^ 0xc979561;
    bVar16 = read8(puVar25 + ((ulong)(uVar33 + (uVar33 & 0x1012411) * -2 + 0x432db411 ^ 0x523e9c91) - 0x32dcaf7f));
    bVar15 = bVar16 << 6 ^ 0x40;
    bVar15 = (bVar15 | 3) + (bVar16 << 6 ^ (bVar15 | 0x36) ^ 0x76) * '\x02' + 0x13 ^ 0x16 | bVar16 >> 2;
    uVar1 = uVar5 >> 0x10 & 0xff;
    uVar33 = ((uVar1 ^ 0xa5) - (uVar5 >> 0xf & 4)) + 0x1ae02 ^ 0xee12;
    uVar19 = uVar33 + (uVar33 & 0x42e7) * -2 + 0x15ccae7 ^ 0xfea2754e;
    uVar1 = uVar1 ^ 0x61;
    uVar33 = uVar1 + (uVar1 & 0x7ffffff1) * -2 + 0x518df1 ^ 0x385f1;
    uVar33 = uVar33 + (uVar33 & 0x1201c9) * -2 + 0x4a1f51c9;
    uVar1 = uVar33 ^ 0xb5b2a627;
    uStack_80[0xf] = (bVar16 >> 2) * '\b' + (bVar16 & 0x34) * -4 + 0x68 ^ bVar15 + (bVar15 << 1 ^ 0xff | 0x89) + 0x3c ^ 0x88;
    puVar27 = pointer_table[iVar26 + 0x4b0];
    uStack_80[1] = read8(puVar27 + ((ulong)((uVar33 ^ (uVar19 - (uVar19 * 2 + 0x128 & 0xac2e84d4)) + 0x561742fe ^ (uVar1 - (uVar1 * 2 + 2 & 0xb12eb38e)) + 0x589759c8 ^ 0x44cd4275) + 0x93) - 0x8ca2907)) ^ 0x54;
    uVar33 = uVar8 >> 8 & 0xff ^ 0xc2;
    uVar33 = uVar33 + (uVar33 & 0x48) * -2 + 0xb6d9e548 ^ 0x27d8e659;
    bVar15 = (byte)(uVar8 >> 8) ^ 0x90;
    uStack_80[0xe] = (bVar15 - (bVar15 * '\x02' + 0x28 & 200)) + 0x78 ^ read8(puVar24 + ((ulong)(uVar33 + (uVar33 & 0x10010051) * -2 + 0x96c19051 ^ 0x7c0932a) - 0x7b1e1573)) ^ 0x70;
    uVar33 = uVar8 >> 0x15 & 0xf8 ^ 8;
    uVar33 = uVar33 + (uVar33 & 0x78) * -2 + 0xaae1a7a ^ 0x12a74f60;
    uVar33 = uVar33 + (uVar33 & 0x11720) * -2 + 0x5e71724 ^ 0x1dee4226 | uVar8 >> 0x1d ^ 5;
    uVar33 = uVar33 + (uVar33 & 0x65b4c892) * -2 + 0x65b4c892;
    uVar19 = uVar33 * 0x20 ^ 0xba0;
    uVar19 = ~(uVar19 + (uVar19 & 0x20) * -2 + 0xbb);
    uVar19 = (uVar19 & 0xe0) + (uVar19 & 0x80) * -2 + 0xe94829f ^ 0x6d247d1;
    uVar33 = uVar19 + (uVar19 & 0x4640c4) * -2 + 0x125f50d5 ^ 0x1a1995db | uVar33 >> 3 ^ 0xcb6990b;
    bVar15 = read8(puVar23 + (uVar33 + (uVar33 & 0x14c6ea72) * -2 + 0x14c6ea72 ^ 0x14c6ea96));
    uStack_80[0xc] = bVar15 ^ ((bVar15 >> 4 & 0xfd ^ 1) - (bVar15 >> 3 & 0x10)) + 0x18 ^ 0x86;
    uVar33 = uVar5 & 0xff ^ 0xab;
    uVar33 = uVar33 + (uVar33 & 0x7b) * -2 + 0x5a55d97b ^ 0x1a54c83b;
    bVar15 = read8(puVar25 + ((ulong)(uVar33 + (uVar33 & 0x1060d) * -2 + 0x2713a60d ^ 0x6712b784) - 0x32dcaf7f));
    uStack_80[3] = (bVar15 >> 2) * '\b' + (bVar15 & 0x34) * -4 + 0x69 ^ (((bVar15 >> 2 | bVar15 << 6) ^ 0x40) - (bVar15 >> 1 & 0x5c)) + 0x2e ^ 0x9c;
    uVar33 = uVar8 >> 0x10 & 0xff ^ 0x9a;
    uVar33 = uVar33 + (uVar33 & 0x4d) * -2 + 0xa69db4d ^ 0x12e8d94c;
    uStack_80[0xd] = read8(puVar27 + ((ulong)(uVar33 + (uVar33 & 0x8010e5d) * -2 + 0x6f39ce5d ^ 0x77b8cc4b) - 0x8ca2907)) ^ 0x25;
    uVar33 = ((uVar17 >> 0x10 & 0xff ^ 0x10) - (uVar17 >> 0xf & 0x140)) + 0x2d3da0 ^ 0x71708;
    uStack_80[5] = read8(puVar27 + ((ulong)(uVar33 + (uVar33 & 0x20071) * -2 + 0x5f965071 ^ 0x5fbc7a4a) - 0x8ca2907)) ^ 0x52;
    uVar33 = uVar17 >> 8 & 0xff ^ 0xf5;
    uVar33 = uVar33 + (uVar33 & 0xdb) * -2 + 0x3e4635db ^ 0x1e46f1d3;
    bVar15 = (byte)(uVar17 >> 8) ^ 0x9a;
    uStack_80[6] = (bVar15 - (bVar15 * '\x02' + 0x28 & 0x48)) + 0x38 ^ read8(puVar24 + ((ulong)(uVar33 + (uVar33 & 0x4291) * -2 + 0x17607a91 ^ 0x3760bece) - 0x7b1e1573)) ^ 0xc;
    bVar15 = (byte)read8(puVar23 + ((ulong)(uVar17 >> 0x18) ^ 0x5a)) >> 4 & 0xd ^ 0xe;
    uStack_80[4] = read8(puVar23 + ((ulong)(uVar17 >> 0x18) ^ 0x5a)) ^ bVar15 + (bVar15 & 8) * -2 + 0x58 ^ 0xb7;
    uVar33 = uVar6 >> 0x10 & 0xff ^ 0xb5;
    uVar33 = uVar33 + (uVar33 & 9) * -2 + 0x4e09 ^ 0xf49;
    uStack_80[9] = read8(puVar27 + ((ulong)(uVar33 + (uVar33 & 0x2de) * -2 + 0x27248ade ^ 0x2724cb75) - 0x8ca2907)) ^ 0xf7;
}

int finalizer_transform(uint8_t tail[16]) {
    missing_constant = false;
    finalizer_native(tail);
    return missing_constant ? -1 : 0;
}
