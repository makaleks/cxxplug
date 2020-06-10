#ifndef PLUGIN_HUMAN_H
#define PLUGIN_HUMAN_H 0

#ifdef __cplusplus
extern "C" {
#endif

// Minimal DLL API
#ifndef _WIN32
	// For GCC you can also use
	// __attribute__((visibility("hidden")))
	// and -fvisibility=hidden compilation flag. It is a good
	// flag, but this example is 'minimal to work'
	#define TEST_SHARED_API
#else
	#ifdef TEST_SHARED_BUILD
		// API to build DLL
		#define TEST_SHARED_API __declspec(dllexport)
	#else
		// API to call DLL
		#define TEST_SHARED_API __declspec(dllimport)
	#endif
#endif


#include <stdint.h>

//TEST_SHARED_API int            sum      (int a1, int a2);
//TEST_SHARED_API char           to_upper (char *source);
//TEST_SHARED_API double         div2     (double source);
//TEST_SHARED_API const uint8_t* get_id   ();
//
TEST_SHARED_API const void* get_implementation_id   ();

TEST_SHARED_API const char* get_full_name ();

TEST_SHARED_API const char* get_first_name ();
TEST_SHARED_API const char* get_last_name ();
TEST_SHARED_API uint8_t     get_age ();

TEST_SHARED_API void        work_with_int (int *value);

// TODO: ISO 369-3
//TEST_SHARED_API const char*    get_implementation_name ();


#ifdef __cplusplus
}
#endif

#endif // PLUGIN_HUMAN_H
