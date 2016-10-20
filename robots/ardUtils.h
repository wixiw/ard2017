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

// Declare the copy construtors with a default implementation
// Tipically used to declare copy construtors as private to prevent the user
// to do unexpected thing.
#define COPY_CONSTRUCTORS(className) \
	className& operator= (const className&){return *this;};\
	className (const className&){}\

// Utilities to initialize arrays
#define INIT_TABLE_TO_ZERO(x) memset(x, 0, sizeof(x))
#define INIT_STRUCT_TO_ZERO(x) memset(&x, 0, sizeof(x))
#define ARRAY_SIZE(a)               (sizeof(a)/sizeof(a[0]))


#endif /* ROBOTS_ARDUTILS_H_ */
