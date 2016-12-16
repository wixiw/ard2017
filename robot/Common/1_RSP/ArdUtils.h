/*
 * ardUtils.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ARDUTILS_H_
#define ROBOTS_ARDUTILS_H_

#include <Arduino.h>

#ifndef NULL
#define NULL 0
#endif

// Tell the compilator to NOT declare the copy construtors with a default implementation
// Tipically used to prevent the user to do unexpected thing (ex : in a singleton)
#define COPY_CONSTRUCTORS(className) \
	className& operator= (const className&) = default;\
	className (const className&) = default;

// Utilities to initialize arrays
#define INIT_TABLE_TO_ZERO(x) memset(x, 0, sizeof(x))
#define INIT_STRUCT_TO_ZERO(x) memset(&x, 0, sizeof(x))
#define ARRAY_SIZE(a)               (sizeof(a)/sizeof(a[0]))

//Uses to disable "unsused param" warnings
#define UNUSED(x) ((void)(x))

typedef enum class eColor
{
    UNKOWN, PREF, SYM
} eColor;

//Use this function in "enumToString" functions so that we don't miss copy the label which could fool us in debug
#define ENUM2STR(myenum) \
    case myenum: \
      return #myenum; \
      break

#endif /* ROBOTS_ARDUTILS_H_ */
