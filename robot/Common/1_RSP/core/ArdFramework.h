/*
 * ArdFramework.h
 *
 *  Created on: 15 janv. 2017
 *      Author: wix
 * 
 * This file contain all decoupling interfaces in the RSP layer 
 */

#ifndef ROBOT_COMMON_1_RSP_ARDFRAMEWORK_H_
#define ROBOT_COMMON_1_RSP_ARDFRAMEWORK_H_

#include "Types.pb.h"
#include "WString.h"

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

//Use this function in "enumToString" functions so that we don't miss copy the label which could fool us in debug
#define ENUM2STR(myenum) \
    case myenum: \
      return #myenum; \
      break

//To be implemented at Applicative layer
extern "C"
{
    void extern ardAssertImpl(bool condition, char const* file, unsigned int line, char const* text);
}
#define ASSERT(x)              ardAssertImpl(x, __FILE__, __LINE__, "")
#define ASSERT_TEXT(x, text)   ardAssertImpl(x, __FILE__, __LINE__, text)

namespace ard
{
    typedef uint32_t TimeMs;
    typedef uint32_t DelayMs;

    /**
     * An ILogger can generate a log and send it the a log stack
     * usage reserved to the class
     */
	class ILogger
	{
	    public:
	        virtual ~ILogger();
	        
	        //Publish a log message
	        virtual void log(eLogLevel lvl, String const& text) = 0;
	};

	/**
	 * The Base abstract class from whom every body shall inherit
	 * It manages the initialization process, knowing that you cannot
	 * do everything you'd like in constructors because for instance:
	 * - all the injected depencies may not have been initialized
	 * - we do not have any way to catch errors, as try/catch are not available on lightweight targets
	 * - the OS is not running yet and some functions are not available until it is up
	 * - the logger thread is not ready to receive logs
	 * It also provide a name for instrospection tools
	 *
	 * This class could also be used for tracing all instanciated objects if needed with a static table
	 * registering each constructor call
	 */
	class ArdObject
	{
	    public:
	        ArdObject(String const& name = String::empty);
	        virtual ~ArdObject();

	        /**
	         * Do here everything where you need to have the object instanciation and OS initialization has been done
	         * said differently NOOB advice : put less in your constructor, put more here, and your life is better
	         * PLEASE call this when your inheritate before anything else
	         */
	        virtual void init();

	        /**
	         * Check if the init has been done
	         */
	        bool isInitialized() const
	        {
	            return initialized;
	        }

	        //inline getter
	        String const& getName() const
	        {
	            return name;
	        }

	    private:
	        bool initialized;
	        String name;

	    protected:
	        /**
	         * Generates a log with file and line numbers
	         * Use ASSERT_TEXT maccro to call this
	         */
	        void _ASSERT_TEXT_impl(bool condition, String const& file, String const& line);
	};


} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_ARDFRAMEWORK_H_ */
