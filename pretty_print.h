#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define OUTPUT(format, args...)\
	__delta = ((xprintf_t)__xprintf)(__output, format, ## args);\
	__result += __delta;\
	if (__xprintf == (voidfunc_t)sprintf) __output = ((char *)__output) + __delta;

#define OUTPUT_CUSTOM(name, value)\
	__delta = pretty_print_##name##_with(__xprintf, __output, value);\
	__result += __delta;\
	if (__xprintf == (voidfunc_t)sprintf) __output = ((char *)__output) + __delta;

#define PRETTY_PRINT_HEADERS(type, name)\
	PRETTY_PRINT_HEADERS_SCALAR(type, name)\
	PRETTY_PRINT_HEADERS_ARRAY(type, name)\
	PRETTY_PRINT_HEADERS_NULL_ARRAY(type, name)

#define PRETTY_PRINT_HEADERS_SCALAR(type, name)\
	extern int pretty_print_##name##_size(type);\
	extern int pretty_print_##name##_with(voidfunc_t, void *, type);\
	extern int pretty_print_##name##_to_buffer(char *, type);\
	extern int pretty_print_##name##_to_file(FILE *, type);\
	extern int pretty_print_##name(type);\
	extern char *pretty_print_##name##_as_string(type);

#define PRETTY_PRINT_HEADERS_ARRAY(type, name)\
	extern int pretty_print_##name##_array_size(size_t, type *);\
	extern int pretty_print_##name##_array_with(voidfunc_t, void *, size_t, type *);\
	extern int pretty_print_##name##_array_to_buffer(char *, size_t, type *);\
	extern int pretty_print_##name##_array_to_file(FILE *, size_t, type *);\
	extern int pretty_print_##name##_array(size_t, type *);\
	extern char *pretty_print_##name##_array_as_string(size_t, type *);

#define PRETTY_PRINT_HEADERS_NULL_ARRAY(type, name)\
	extern int pretty_print_##name##_null_array_size(type *);\
	extern int pretty_print_##name##_null_array_with(voidfunc_t, void *, type *);\
	extern int pretty_print_##name##_null_array_to_buffer(char *, type *);\
	extern int pretty_print_##name##_null_array_to_file(FILE *, type *);\
	extern int pretty_print_##name##_null_array(type *);\
	extern char *pretty_print_##name##_null_array_as_string(type *);

#define PRETTY_PRINT_INT_HEADERS(type, name)\
	PRETTY_PRINT_HEADERS(type, name)\
	PRETTY_PRINT_HEADERS(unsigned type, unsigned_##name)\
	PRETTY_PRINT_HEADERS(unsigned type, name##_hex)

#define PRETTY_PRINT(type, name, code)\
	PRETTY_PRINT_DEFINE_WITH(type, name, code)\
	PRETTY_PRINT_DEFINE_SIZE(type, name)\
	PRETTY_PRINT_SCALAR(type, name)\
	PRETTY_PRINT_ARRAY(type, name)\
	PRETTY_PRINT_NULL_ARRAY(type, name)

#define PRETTY_PRINT_WITH_CUSTOM_SIZE(type, name, code)\
	PRETTY_PRINT_DEFINE_WITH(type, name, code)\
	int pretty_print_##name##_size(type)\
	PRETTY_PRINT_SCALAR(type, name)\
	PRETTY_PRINT_ARRAY(type, name)\
	PRETTY_PRINT_NULL_ARRAY(type, name)

#define PRETTY_PRINT_DEFINE_WITH(type, name, code)\
	int pretty_print_##name##_with(voidfunc_t __xprintf, void *__output, type value) {\
		int __result = 0, __delta;\
		code;\
		return __result;\
	}

#define PRETTY_PRINT_DEFINE_SIZE(type, name)\
	int pretty_print_##name##_size(type value) {\
		size_t result;\
		initialize_stdnull();\
		result = pretty_print_##name##_with((voidfunc_t)fprintf, stdnull, value);\
		return result;\
	}

#define PRETTY_PRINT_SCALAR(type, name)\
	PRETTY_PRINT_HEADERS_SCALAR(type, name)\
	int pretty_print_##name##_to_buffer(char *buffer, type value) {\
		return pretty_print_##name##_with((voidfunc_t)sprintf, buffer, value);\
	}\
	int pretty_print_##name##_to_file(FILE *file, type value) {\
		return pretty_print_##name##_with((voidfunc_t)fprintf, file, value);\
	}\
	int pretty_print_##name(type value) {\
		return pretty_print_##name##_to_file(stdout, value);\
	}\
	char *pretty_print_##name##_as_string(type value) {\
		size_t size = pretty_print_##name##_size(value);\
		char *buffer = malloc(sizeof(char) * (size + 1));\
		pretty_print_##name##_to_buffer(buffer, value);\
		buffer[size] = 0;\
		return buffer;\
	}\

#define PRETTY_PRINT_ARRAY(type, name)\
	int pretty_print_##name##_array_size(size_t size, type *array) {\
		int result;\
		size_t i;\
		if (!size) return 2;\
		result = 0;\
		for (i = 0; i < size; i++) {\
			result += pretty_print_##name##_size(array[i]) + 2;\
		}\
		return result;\
	}\
	int pretty_print_##name##_array_with(voidfunc_t __xprintf, void *__output, size_t size, type *array) {\
		int __result = 0, __delta;\
		size_t i;\
		OUTPUT("[");\
		for (i = 0; i < size; i++) {\
			OUTPUT_CUSTOM(name, array[i]);\
			if (i < size - 1) {\
				OUTPUT(", ");\
			}\
		}\
		OUTPUT("]");\
		return __result;\
	}\
	int pretty_print_##name##_array_to_buffer(char *buffer, size_t size, type *array) {\
		return pretty_print_##name##_array_with((voidfunc_t)sprintf, buffer, size, array);\
	}\
	int pretty_print_##name##_array_to_file(FILE *file, size_t size, type *array) {\
		return pretty_print_##name##_array_with((voidfunc_t)fprintf, file, size, array);\
	}\
	int pretty_print_##name##_array(size_t size, type *array) {\
		return pretty_print_##name##_array_to_file(stdout, size, array);\
	}\
	char *pretty_print_##name##_array_as_string(size_t size, type *array) {\
		int length = pretty_print_##name##_array_size(size, array);\
		char *buffer = malloc(sizeof(char) * (length + 1)); \
		pretty_print_##name##_array_to_buffer(buffer, size, array);\
		buffer[length] = 0;\
		return buffer;\
	}

#define PRETTY_PRINT_NULL_ARRAY(type, name)\
	int pretty_print_##name##_null_array_size(type *array) {\
		if (*array == (type)((uintptr_t)NULL)) return 2;\
		int result;\
		result = 0;\
		for (; *array != (type)((uintptr_t)NULL); array++) {\
			result += pretty_print_##name##_size(*array) + 2;\
		}\
		return result;\
	}\
	int pretty_print_##name##_null_array_with(voidfunc_t __xprintf, void *__output, type *array) {\
		int __result = 0, __delta;\
		OUTPUT("[");\
		for (; *array != (type)((uintptr_t)NULL); array++) {\
			OUTPUT_CUSTOM(name, *array);\
			if (*(array + 1) != (type)((uintptr_t)NULL)) {\
				OUTPUT(", ");\
			}\
		}\
		OUTPUT("]");\
		return __result;\
	}\
	PRETTY_PRINT_SCALAR(type *, name##_null_array)

#define PRETTY_PRINT_INT(type, name, format)\
	PRETTY_PRINT(type, name, {\
		OUTPUT("%" #format "d", value);\
	})\
	PRETTY_PRINT(unsigned type, unsigned_##name, {\
		OUTPUT("%" #format "ud", value);\
	})\
	PRETTY_PRINT(unsigned type, name##_hex, {\
		OUTPUT("%" #format "ux", value);\
	})\

typedef int (*xprintf_t)(void *, char *, ...);
typedef void (*voidfunc_t)(void);

extern FILE *stdnull;
extern void initialize_stdnull();

PRETTY_PRINT_INT_HEADERS(char, byte)
PRETTY_PRINT_INT_HEADERS(short, short)
PRETTY_PRINT_INT_HEADERS(int, int)
PRETTY_PRINT_INT_HEADERS(long, long)
PRETTY_PRINT_INT_HEADERS(long long, long_long)

PRETTY_PRINT_HEADERS(void *, pointer)
PRETTY_PRINT_HEADERS(char, char)
PRETTY_PRINT_HEADERS(char *, string)
