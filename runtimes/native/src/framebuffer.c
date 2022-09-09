#include "framebuffer.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"

static const uint8_t font[1792] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc7, 0xc7, 0xc7, 0xcf, 0xcf, 0xff, 0xcf, 0xff,
    0x93, 0x93, 0x93, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x93, 0x01, 0x93, 0x93, 0x93, 0x01, 0x93, 0xff,
    0xef, 0x83, 0x2f, 0x83, 0xe9, 0x03, 0xef, 0xff,
    0x9d, 0x5b, 0x37, 0xef, 0xd9, 0xb5, 0x73, 0xff,
    0x8f, 0x27, 0x27, 0x8f, 0x25, 0x33, 0x81, 0xff,
    0xcf, 0xcf, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf3, 0xe7, 0xcf, 0xcf, 0xcf, 0xe7, 0xf3, 0xff,
    0x9f, 0xcf, 0xe7, 0xe7, 0xe7, 0xcf, 0x9f, 0xff,
    0xff, 0x93, 0xc7, 0x01, 0xc7, 0x93, 0xff, 0xff,
    0xff, 0xe7, 0xe7, 0x81, 0xe7, 0xe7, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xcf, 0x9f,
    0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xcf, 0xff,
    0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f, 0xff,
    0xc7, 0xb3, 0x39, 0x39, 0x39, 0x9b, 0xc7, 0xff,
    0xe7, 0xc7, 0xe7, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0x83, 0x39, 0xf1, 0xc3, 0x87, 0x1f, 0x01, 0xff,
    0x81, 0xf3, 0xe7, 0xc3, 0xf9, 0x39, 0x83, 0xff,
    0xe3, 0xc3, 0x93, 0x33, 0x01, 0xf3, 0xf3, 0xff,
    0x03, 0x3f, 0x03, 0xf9, 0xf9, 0x39, 0x83, 0xff,
    0xc3, 0x9f, 0x3f, 0x03, 0x39, 0x39, 0x83, 0xff,
    0x01, 0x39, 0xf3, 0xe7, 0xcf, 0xcf, 0xcf, 0xff,
    0x87, 0x3b, 0x1b, 0x87, 0x61, 0x79, 0x83, 0xff,
    0x83, 0x39, 0x39, 0x81, 0xf9, 0xf3, 0x87, 0xff,
    0xff, 0xcf, 0xcf, 0xff, 0xcf, 0xcf, 0xff, 0xff,
    0xff, 0xcf, 0xcf, 0xff, 0xcf, 0xcf, 0x9f, 0xff,
    0xf3, 0xe7, 0xcf, 0x9f, 0xcf, 0xe7, 0xf3, 0xff,
    0xff, 0xff, 0x01, 0xff, 0x01, 0xff, 0xff, 0xff,
    0x9f, 0xcf, 0xe7, 0xf3, 0xe7, 0xcf, 0x9f, 0xff,
    0x83, 0x01, 0x39, 0xf3, 0xc7, 0xff, 0xc7, 0xff,
    0x83, 0x7d, 0x45, 0x55, 0x41, 0x7f, 0x83, 0xff,
    0xc7, 0x93, 0x39, 0x39, 0x01, 0x39, 0x39, 0xff,
    0x03, 0x39, 0x39, 0x03, 0x39, 0x39, 0x03, 0xff,
    0xc3, 0x99, 0x3f, 0x3f, 0x3f, 0x99, 0xc3, 0xff,
    0x07, 0x33, 0x39, 0x39, 0x39, 0x33, 0x07, 0xff,
    0x01, 0x3f, 0x3f, 0x03, 0x3f, 0x3f, 0x01, 0xff,
    0x01, 0x3f, 0x3f, 0x03, 0x3f, 0x3f, 0x3f, 0xff,
    0xc1, 0x9f, 0x3f, 0x31, 0x39, 0x99, 0xc1, 0xff,
    0x39, 0x39, 0x39, 0x01, 0x39, 0x39, 0x39, 0xff,
    0x81, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0x39, 0x83, 0xff,
    0x39, 0x33, 0x27, 0x0f, 0x07, 0x23, 0x31, 0xff,
    0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x81, 0xff,
    0x39, 0x11, 0x01, 0x01, 0x29, 0x39, 0x39, 0xff,
    0x39, 0x19, 0x09, 0x01, 0x21, 0x31, 0x39, 0xff,
    0x83, 0x39, 0x39, 0x39, 0x39, 0x39, 0x83, 0xff,
    0x03, 0x39, 0x39, 0x39, 0x03, 0x3f, 0x3f, 0xff,
    0x83, 0x39, 0x39, 0x39, 0x21, 0x33, 0x85, 0xff,
    0x03, 0x39, 0x39, 0x31, 0x07, 0x23, 0x31, 0xff,
    0x87, 0x33, 0x3f, 0x83, 0xf9, 0x39, 0x83, 0xff,
    0x81, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xff,
    0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x83, 0xff,
    0x39, 0x39, 0x39, 0x11, 0x83, 0xc7, 0xef, 0xff,
    0x39, 0x39, 0x29, 0x01, 0x01, 0x11, 0x39, 0xff,
    0x39, 0x11, 0x83, 0xc7, 0x83, 0x11, 0x39, 0xff,
    0x99, 0x99, 0x99, 0xc3, 0xe7, 0xe7, 0xe7, 0xff,
    0x01, 0xf1, 0xe3, 0xc7, 0x8f, 0x1f, 0x01, 0xff,
    0xc3, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xc3, 0xff,
    0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xff,
    0x87, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x87, 0xff,
    0xc7, 0x93, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01,
    0xef, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0x3f, 0x3f, 0x03, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xff, 0xff, 0x81, 0x3f, 0x3f, 0x3f, 0x81, 0xff,
    0xf9, 0xf9, 0x81, 0x39, 0x39, 0x39, 0x81, 0xff,
    0xff, 0xff, 0x83, 0x39, 0x01, 0x3f, 0x83, 0xff,
    0xf1, 0xe7, 0x81, 0xe7, 0xe7, 0xe7, 0xe7, 0xff,
    0xff, 0xff, 0x81, 0x39, 0x39, 0x81, 0xf9, 0x83,
    0x3f, 0x3f, 0x03, 0x39, 0x39, 0x39, 0x39, 0xff,
    0xe7, 0xff, 0xc7, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0xf3, 0xff, 0xe3, 0xf3, 0xf3, 0xf3, 0xf3, 0x87,
    0x3f, 0x3f, 0x31, 0x03, 0x07, 0x23, 0x31, 0xff,
    0xc7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0xff, 0xff, 0x03, 0x49, 0x49, 0x49, 0x49, 0xff,
    0xff, 0xff, 0x03, 0x39, 0x39, 0x39, 0x39, 0xff,
    0xff, 0xff, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xff, 0xff, 0x03, 0x39, 0x39, 0x03, 0x3f, 0x3f,
    0xff, 0xff, 0x81, 0x39, 0x39, 0x81, 0xf9, 0xf9,
    0xff, 0xff, 0x91, 0x8f, 0x9f, 0x9f, 0x9f, 0xff,
    0xff, 0xff, 0x83, 0x3f, 0x83, 0xf9, 0x03, 0xff,
    0xe7, 0xe7, 0x81, 0xe7, 0xe7, 0xe7, 0xe7, 0xff,
    0xff, 0xff, 0x39, 0x39, 0x39, 0x39, 0x81, 0xff,
    0xff, 0xff, 0x99, 0x99, 0x99, 0xc3, 0xe7, 0xff,
    0xff, 0xff, 0x49, 0x49, 0x49, 0x49, 0x81, 0xff,
    0xff, 0xff, 0x39, 0x01, 0xc7, 0x01, 0x39, 0xff,
    0xff, 0xff, 0x39, 0x39, 0x39, 0x81, 0xf9, 0x83,
    0xff, 0xff, 0x01, 0xe3, 0xc7, 0x8f, 0x01, 0xff,
    0xf3, 0xe7, 0xe7, 0xcf, 0xe7, 0xe7, 0xf3, 0xff,
    0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xff,
    0x9f, 0xcf, 0xcf, 0xe7, 0xcf, 0xcf, 0x9f, 0xff,
    0xff, 0xff, 0x8f, 0x45, 0xe3, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x93, 0x93, 0xff,
    0x83, 0x29, 0x29, 0x11, 0x29, 0x29, 0x83, 0xff,
    0x83, 0x39, 0x09, 0x11, 0x21, 0x39, 0x83, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x83, 0x11, 0x21, 0x7d, 0x21, 0x11, 0x83, 0xff,
    0x83, 0x11, 0x09, 0x7d, 0x09, 0x11, 0x83, 0xff,
    0x83, 0x11, 0x39, 0x55, 0x11, 0x11, 0x83, 0xff,
    0x83, 0x11, 0x11, 0x55, 0x39, 0x11, 0x83, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xe7, 0xff, 0xe7, 0xe7, 0xc7, 0xc7, 0xc7, 0xff,
    0xef, 0x83, 0x29, 0x2f, 0x29, 0x83, 0xef, 0xff,
    0xc3, 0x99, 0x9f, 0x03, 0x9f, 0x9f, 0x01, 0xff,
    0xff, 0xa5, 0xdb, 0xdb, 0xdb, 0xa5, 0xff, 0xff,
    0x99, 0x99, 0xc3, 0x81, 0xe7, 0x81, 0xe7, 0xff,
    0xe7, 0xe7, 0xe7, 0xff, 0xe7, 0xe7, 0xe7, 0xff,
    0xc3, 0x99, 0x87, 0xdb, 0xe1, 0x99, 0xc3, 0xff,
    0x93, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc3, 0xbd, 0x66, 0x5e, 0x5e, 0x66, 0xbd, 0xc3,
    0x87, 0xc3, 0x93, 0xc3, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xc9, 0x93, 0x27, 0x93, 0xc9, 0xff, 0xff,
    0xff, 0xff, 0x81, 0xf9, 0xf9, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc3, 0xbd, 0x46, 0x5a, 0x46, 0x5a, 0xbd, 0xc3,
    0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xef, 0xd7, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xe7, 0xe7, 0x81, 0xe7, 0xe7, 0xff, 0x81, 0xff,
    0xc7, 0xf3, 0xe7, 0xc3, 0xff, 0xff, 0xff, 0xff,
    0xc3, 0xe7, 0xf3, 0xc7, 0xff, 0xff, 0xff, 0xff,
    0xf7, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x33, 0x33, 0x33, 0x33, 0x09, 0x3f,
    0xc1, 0x95, 0xb5, 0x95, 0xc1, 0xf5, 0xf5, 0xff,
    0xff, 0xff, 0xff, 0xcf, 0xcf, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xcf,
    0xe7, 0xc7, 0xe7, 0xc3, 0xff, 0xff, 0xff, 0xff,
    0xc7, 0x93, 0x93, 0xc7, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x27, 0x93, 0xc9, 0x93, 0x27, 0xff, 0xff,
    0xbd, 0x3b, 0xb7, 0xad, 0xd9, 0xb1, 0x7d, 0xff,
    0xbd, 0x3b, 0xb7, 0xa9, 0xdd, 0xbb, 0x71, 0xff,
    0x1d, 0xbb, 0xd7, 0x2d, 0xd9, 0xb1, 0x7d, 0xff,
    0xc7, 0xff, 0xc7, 0x9f, 0x39, 0x01, 0x83, 0xff,
    0xdf, 0xef, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0xf7, 0xef, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0xc7, 0x93, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0xcb, 0xa7, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0x93, 0xff, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0xef, 0xd7, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0xc1, 0x87, 0x27, 0x21, 0x07, 0x27, 0x21, 0xff,
    0xc3, 0x99, 0x3f, 0x3f, 0x99, 0xc3, 0xf7, 0xcf,
    0xdf, 0xef, 0x01, 0x3f, 0x03, 0x3f, 0x01, 0xff,
    0xf7, 0xef, 0x01, 0x3f, 0x03, 0x3f, 0x01, 0xff,
    0xc7, 0x93, 0x01, 0x3f, 0x03, 0x3f, 0x01, 0xff,
    0x93, 0xff, 0x01, 0x3f, 0x03, 0x3f, 0x01, 0xff,
    0xef, 0xf7, 0x81, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0xf7, 0xef, 0x81, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0xe7, 0xc3, 0x81, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0x99, 0xff, 0x81, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0x87, 0x93, 0x99, 0x09, 0x99, 0x93, 0x87, 0xff,
    0xcb, 0xa7, 0x19, 0x09, 0x01, 0x21, 0x31, 0xff,
    0xdf, 0xef, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xf7, 0xef, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xc7, 0x93, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xcb, 0xa7, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0x93, 0xff, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xff, 0xbb, 0xd7, 0xef, 0xd7, 0xbb, 0xff, 0xff,
    0x83, 0x39, 0x31, 0x29, 0x19, 0x39, 0x83, 0xff,
    0xdf, 0xef, 0x39, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xf7, 0xef, 0x39, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xc7, 0x93, 0xff, 0x39, 0x39, 0x39, 0x83, 0xff,
    0x93, 0xff, 0x39, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xf7, 0xef, 0x99, 0x99, 0xc3, 0xe7, 0xe7, 0xff,
    0x3f, 0x03, 0x39, 0x39, 0x39, 0x03, 0x3f, 0xff,
    0xc3, 0x99, 0x99, 0x93, 0x99, 0x89, 0x93, 0xff,
    0xdf, 0xef, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0xf7, 0xef, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0xc7, 0x93, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0xcb, 0xa7, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0x93, 0xff, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0xef, 0xd7, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0xff, 0xff, 0x83, 0xe9, 0x81, 0x2f, 0x83, 0xff,
    0xff, 0xff, 0x81, 0x3f, 0x3f, 0x81, 0xf7, 0xcf,
    0xdf, 0xef, 0x83, 0x39, 0x01, 0x3f, 0x83, 0xff,
    0xf7, 0xef, 0x83, 0x39, 0x01, 0x3f, 0x83, 0xff,
    0xc7, 0x93, 0x83, 0x39, 0x01, 0x3f, 0x83, 0xff,
    0x93, 0xff, 0x83, 0x39, 0x01, 0x3f, 0x83, 0xff,
    0xdf, 0xef, 0xff, 0xc7, 0xe7, 0xe7, 0x81, 0xff,
    0xf7, 0xef, 0xff, 0xc7, 0xe7, 0xe7, 0x81, 0xff,
    0xc7, 0x93, 0xff, 0xc7, 0xe7, 0xe7, 0x81, 0xff,
    0x93, 0xff, 0xc7, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0x9b, 0x87, 0x67, 0x83, 0x39, 0x39, 0x83, 0xff,
    0xcb, 0xa7, 0x03, 0x39, 0x39, 0x39, 0x39, 0xff,
    0xdf, 0xef, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xf7, 0xef, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xc7, 0x93, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xcb, 0xa7, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0x93, 0xff, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xff, 0xe7, 0xff, 0x81, 0xff, 0xe7, 0xff, 0xff,
    0xff, 0xff, 0x83, 0x31, 0x29, 0x19, 0x83, 0xff,
    0xdf, 0xef, 0x39, 0x39, 0x39, 0x39, 0x81, 0xff,
    0xf7, 0xef, 0x39, 0x39, 0x39, 0x39, 0x81, 0xff,
    0xc7, 0x93, 0xff, 0x39, 0x39, 0x39, 0x81, 0xff,
    0x93, 0xff, 0x39, 0x39, 0x39, 0x39, 0x81, 0xff,
    0xf7, 0xef, 0x39, 0x39, 0x39, 0x81, 0xf9, 0x83,
    0x3f, 0x3f, 0x03, 0x39, 0x39, 0x03, 0x3f, 0x3f,
    0x93, 0xff, 0x39, 0x39, 0x39, 0x81, 0xf9, 0x83
};

static const uint8_t* drawColors;
static uint8_t* framebuffer;

static int w4_min (int a, int b) {
    return a < b ? a : b;
}

static int w4_max (int a, int b) {
    return a > b ? a : b;
}

static void drawPoint (uint8_t color, int x, int y) {
    int idx = (WIDTH * y + x) >> 2;
    int shift = (x & 0x3) << 1;
    int mask = 0x3 << shift;
    framebuffer[idx] = (color << shift) | (framebuffer[idx] & ~mask);
}

static void drawPointUnclipped (uint8_t color, int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        drawPoint(color, x, y);
    }
}

static void drawHLine (uint8_t color, int startX, int y, int endX) {
    int fillEnd = endX - (endX & 3);
    int fillStart = w4_min((startX + 3) & ~3, fillEnd);

    if (fillEnd - fillStart > 3) {
        for (int xx = startX; xx < fillStart; xx++) {
            drawPoint(color, xx, y);
        }

        int from = (WIDTH * y + fillStart) >> 2;
        int to = (WIDTH * y + fillEnd) >> 2;
        uint8_t fillColor = color * 0x55;

        memset(framebuffer+from, fillColor, to-from);
        startX = fillEnd;
    }

    for (int xx = startX; xx < endX; xx++) {
        drawPoint(color, xx, y);
    }
}

static void drawHLineUnclipped (uint8_t color, int startX, int y, int endX) {
    if (y >= 0 && y < HEIGHT) {
        if (startX < 0) {
            startX = 0;
        }
        if (endX > WIDTH) {
            endX = WIDTH;
        }
        if (startX < endX) {
            drawHLine(color, startX, y, endX);
        }
    }
}

void w4_framebufferInit (const uint8_t* drawColors_, uint8_t* framebuffer_) {
    drawColors = drawColors_;
    framebuffer = framebuffer_;
}

void w4_framebufferClear () {
    memset(framebuffer, 0, WIDTH*HEIGHT >> 2);
}

void w4_framebufferHLine (int x, int y, int len) {
    uint8_t dc0 = drawColors[0] & 0xf;
    if (dc0 == 0) {
        return;
    }

    uint8_t strokeColor = (dc0 - 1) & 0x3;
    drawHLineUnclipped(strokeColor, x, y, x + len);
}

void w4_framebufferVLine (int x, int y, int len) {
    if (y + len <= 0 || x < 0 || x >= WIDTH) {
        return;
    }

    uint8_t dc0 = drawColors[0] & 0xf;
    if (dc0 == 0) {
        return;
    }

    int startY = w4_max(0, y);
    int endY = w4_min(HEIGHT, y + len);
    uint8_t strokeColor = (dc0 - 1) & 0x3;
    for (int yy = startY; yy < endY; yy++) {
        drawPoint(strokeColor, x, yy);
    }
}

void w4_framebufferRect (int x, int y, int width, int height) {
    int startX = w4_max(0, x);
    int startY = w4_max(0, y);
    int endXUnclamped = x + width;
    int endYUnclamped = y + height;
    int endX = w4_min(endXUnclamped, WIDTH);
    int endY = w4_min(endYUnclamped, HEIGHT);

    uint8_t dc01 = drawColors[0];
    uint8_t dc0 = dc01 & 0xf;
    uint8_t dc1 = (dc01 >> 4) & 0xf;

    if (dc0 != 0) {
        uint8_t fillColor = (dc0 - 1) & 0x3;
        for (int yy = startY; yy < endY; ++yy) {
            drawHLine(fillColor, startX, yy, endX);
        }
    }

    if (dc1 != 0) {
        uint8_t strokeColor = (dc1 - 1) & 0x3;

        // Left edge
        if (x >= 0 && x < WIDTH) {
            for (int yy = startY; yy < endY; ++yy) {
                drawPoint(strokeColor, x, yy);
            }
        }

        // Right edge
        if (endXUnclamped >= 0 && endXUnclamped <= WIDTH) {
            for (int yy = startY; yy < endY; ++yy) {
                drawPoint(strokeColor, endXUnclamped - 1, yy);
            }
        }

        // Top edge
        if (y >= 0 && y < HEIGHT) {
            drawHLine(strokeColor, startX, y, endX);
        }

        // Bottom edge
        if (endYUnclamped >= 0 && endYUnclamped <= HEIGHT) {
            drawHLine(strokeColor, startX, endYUnclamped - 1, endX);
        }
    }
}

// Oval drawing function using a variation on the midpoint algorithm.
// TIC-80's ellipse drawing function used as reference.
// https://github.com/nesbox/TIC-80/blob/main/src/core/draw.c
//
// Javatpoint has a in depth academic explanation that mostly went over my head:
// https://www.javatpoint.com/computer-graphics-midpoint-ellipse-algorithm
//
// Draws the eliipse by "scanning" along the edge in one quadrant, and mirroring
// the movement for the other four quadrants.
//
// There are a lot of details to get correct while implementing this algorithm,
// so ensure the edge cases are covered when changing it. Long, thin ellipses
// are particularly susceptible to being drawn incorrectly.
void w4_framebufferOval (int x, int y, int width, int height) {
    uint8_t dc01 = drawColors[0];
    uint8_t dc0 = dc01 & 0xf;
    uint8_t dc1 = (dc01 >> 4) & 0xf;

    if (dc1 == 0xf) {
        return;
    }

    uint8_t strokeColor = (dc1 - 1) & 0x3;
    uint8_t fillColor = (dc0 - 1) & 0x3;

    int a = width;
    int b = height;
    int b1 = (height + 1) % 2; // Compensates for precision loss when dividing

    int north = y;
    north += b / 2; // Precision loss here
    int west = x;
    int east = x + width - 1;
    int south = north - b1; // Compensation here. Moves the bottom line up by
                            // one (overlapping the top line) for even heights

    // Error increments. Also known as the decision parameters
    int dx = 4 * (1 - a) * b * b;
    int dy = 4 * (b1 + 1) * a * a;

    // Error of 1 step
    int err = dx + dy + b1 * a * a;

    a *= 8 * a;
    b1 = 8 * b * b;

    do {
        drawPointUnclipped(strokeColor, east, north); /*   I. Quadrant     */
        drawPointUnclipped(strokeColor, west, north); /*   II. Quadrant    */
        drawPointUnclipped(strokeColor, west, south); /*   III. Quadrant   */
        drawPointUnclipped(strokeColor, east, south); /*   IV. Quadrant    */
        const start = west + 1;
        const len = east - start;
        if (dc0 != 0 && len > 0) { // Only draw fill if the length from west to east is not 0
            drawHLineUnclipped(fillColor, start, north, east); /*   I and III. Quadrant */
            drawHLineUnclipped(fillColor, start, south, east); /*  II and IV. Quadrant */
        }
        const err2 = 2 * err;
        if (err2 <= dy) {
            // Move vertical scan
            north += 1;
            south -= 1;
            dy += a;
            err += dy;
        }
        if (err2 >= dx || 2 * err > dy) {
            // Move horizontal scan
            west += 1;
            east -= 1;
            dx += b1;
            err += dx;
        }
    } while (west <= east);

    // Make sure north and south have moved the entire way so top/bottom aren't missing
    while (north - south < height) {
        drawPointUnclipped(strokeColor, west - 1, north); /*   II. Quadrant    */
        drawPointUnclipped(strokeColor, east + 1, north); /*   I. Quadrant     */
        north += 1;
        drawPointUnclipped(strokeColor, west - 1, south); /*   III. Quadrant   */
        drawPointUnclipped(strokeColor, east + 1, south); /*   IV. Quadrant    */
        south -= 1;
    }
}

void w4_framebufferLine (int x1, int y1, int x2, int y2) {
    uint8_t dc0 = drawColors[0] & 0xf;
    if (dc0 == 0) {
        return;
    }
    uint8_t strokeColor = (dc0 - 1) & 0x3;

    if (y1 > y2) {
        int swap = x1;
        x1 = x2;
        x2 = swap;

        swap = y1;
        y1 = y2;
        y2 = swap;
    }

    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = y2 - y1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;) {
        drawPointUnclipped(strokeColor, x1, y1);
        if (x1 == x2 && y1 == y2) {
            break;
        }
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y1++;
        }
    }
}

void w4_framebufferText (const uint8_t* str, int x, int y) {
    for (int currentX = x; *str != '\0'; ++str) {
        if (*str == 10) {
            y += 8;
            currentX = x;
        } else {
            w4_framebufferBlit(font, currentX, y, 8, 8, 0, (*str - 32) << 3, 8,
                false, false, false, false);
            currentX += 8;
        }
    }
}

void w4_framebufferTextUtf8 (const uint8_t* str, int byteLength, int x, int y) {
    for (int currentX = x; byteLength > 0; ++str, --byteLength) {
        if (*str == 10) {
            y += 8;
            currentX = x;
        } else {
            w4_framebufferBlit(font, currentX, y, 8, 8, 0, (*str - 32) << 3, 8,
                false, false, false, false);
            currentX += 8;
        }
    }
}

void w4_framebufferTextUtf16 (const uint16_t* str, int byteLength, int x, int y) {
    for (int currentX = x; byteLength > 0; ++str, byteLength -= 2) {
        uint16_t c = w4_read16LE(str);
        if (c == 10) {
            y += 8;
            currentX = x;
        } else {
            w4_framebufferBlit(font, currentX, y, 8, 8, 0, (c - 32) << 3, 8,
                false, false, false, false);
            currentX += 8;
        }
    }
}

void w4_framebufferBlit (const uint8_t* sprite, int dstX, int dstY, int width, int height,
    int srcX, int srcY, int srcStride, bool bpp2, bool flipX, bool flipY, bool rotate) {

    uint16_t colors = drawColors[0] | (drawColors[1] << 8);

    // Clip rectangle to screen
    int clipXMin, clipYMin, clipXMax, clipYMax;
    if (rotate) {
        flipX = !flipX;
        clipXMin = w4_max(0, dstY) - dstY;
        clipYMin = w4_max(0, dstX) - dstX;
        clipXMax = w4_min(width, HEIGHT - dstY);
        clipYMax = w4_min(height, WIDTH - dstX);
    } else {
        clipXMin = w4_max(0, dstX) - dstX;
        clipYMin = w4_max(0, dstY) - dstY;
        clipXMax = w4_min(width, WIDTH - dstX);
        clipYMax = w4_min(height, HEIGHT - dstY);
    }

    // Iterate pixels in rectangle
    for (int y = clipYMin; y < clipYMax; y++) {
        for (int x = clipXMin; x < clipXMax; x++) {
            // Calculate sprite target coords
            const int tx = dstX + (rotate ? y : x);
            const int ty = dstY + (rotate ? x : y);

            // Calculate sprite source coords
            const int sx = srcX + (flipX ? width - x - 1 : x);
            const int sy = srcY + (flipY ? height - y - 1 : y);

            // Sample the sprite to get a color index
            int colorIdx;
            int bitIndex = sy * srcStride + sx;
            if (bpp2) {
                uint8_t byte = sprite[bitIndex >> 2];
                int shift = 6 - ((bitIndex & 0x03) << 1);
                colorIdx = (byte >> shift) & 0x3;

            } else {
                uint8_t byte = sprite[bitIndex >> 3];
                int shift = 7 - (bitIndex & 0x07);
                colorIdx = (byte >> shift) & 0x1;
            }

            // Get the final color using the drawColors indirection
            uint8_t dc = (colors >> (colorIdx << 2)) & 0x0f;
            if (dc != 0) {
                drawPoint((dc - 1) & 0x03, tx, ty);
            }
        }
    }
}
