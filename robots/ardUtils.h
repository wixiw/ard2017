/*
 * ardUtils.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ARDUTILS_H_
#define ROBOTS_ARDUTILS_H_

#ifndef NULL
	#define NULL 0
#endif

// Declare the copy construtors with a default implementation
// Tipically used to declare copy construtors as private to prevent the user
// to do unexpected thing.
#define COPY_CONSTRUCTORS(className) \
	className& operator= (const className&){return *this;};\
	className (const className&){}\




#endif /* ROBOTS_ARDUTILS_H_ */
