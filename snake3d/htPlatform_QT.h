/*
 * htPlatforms.h
 *
 *  Created on: 2013-12-31
 *      Author: Wang Yang
 *  Description: load some library and do some essential initialization before compiling.
 */

#ifndef HTPLATFORMS_H_
#define HTPLATFORMS_H_

#include <QOpenGLFunctions_ES2>
#include <qgl.h>

#ifdef _MSC_VER

#if _MSC_VER > 1600 && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include <windows.h>

#define SHOW_CONSOLE

#define F_RED		FOREGROUND_INTENSITY | FOREGROUND_RED
#define F_BLUE		FOREGROUND_INTENSITY | FOREGROUND_BLUE
#define F_GREEN		FOREGROUND_INTENSITY | FOREGROUND_GREEN
#define F_YELLOW	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
#define F_PURPLE	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
#define F_CYAN		FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE

#define B_RED		BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED
#define B_BLUE		BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_BLUE
#define B_GREEN		BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_GREEN
#define B_YELLOW	BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN
#define B_PURPLE	BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE
#define B_CYAN		BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE

#define LOG_INFO(...) \
do{\
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);\
printf(__VA_ARGS__);\
fflush(stdout);\
}while(0)

#define LOG_ERROR(...) \
do{\
SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), F_YELLOW | B_RED);\
fprintf(stderr, __VA_ARGS__);\
fflush(stderr);\
}while(0)
#define LOG_CODE(...) __VA_ARGS__

#elif defined(Q_OS_OSX)

#define _HT_SHADER_VERSION_ 120

#if (defined(DEBUG) || defined(_DEBUG))
#include <cstdio>
#define LOG_ERROR(str, ...) \
	do{\
	fprintf(stderr, "❌❌❌");\
	fprintf(stderr, str, ##__VA_ARGS__);\
	fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
	fprintf(stderr, "❌❌❌\n");\
	fflush(stderr);\
	}while(0)

#define LOG_CODE(...) __VA_ARGS__
#define LOG_INFO(...) do{printf(__VA_ARGS__); fflush(stdout);}while(0)

#else

#define LOG_ERROR(...)
#define LOG_CODE(...)
#define LOG_INFO(...)

#endif

#elif defined(Q_OS_ANDROID)

#include <android/log.h>

#ifndef LOG_TAG
#define LOG_TAG "htSDK"
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define  LOG_INFO(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOG_ERROR(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_CODE(...) __VA_ARGS__
#else
#define  LOG_INFO(...)
#define  LOG_ERROR(...)
#define LOG_CODE(...)
#endif


#elif defined(Q_OS_IOS)

#if (defined(DEBUG) || defined(_DEBUG))
#include <cstdio>
#define LOG_ERROR(str, ...) \
    do{\
    fprintf(stderr, "❌❌❌");\
    fprintf(stderr, str, ##__VA_ARGS__);\
    fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
    fprintf(stderr, "❌❌❌\n");\
    fflush(stderr);\
    }while(0)

#define LOG_CODE(...) __VA_ARGS__
#define LOG_INFO(...) do{printf(__VA_ARGS__); fflush(stdout);}while(0)

#else

#define LOG_ERROR(...)
#define LOG_CODE(...)
#define LOG_INFO(...)

#endif


#else


#if (defined(DEBUG) || defined(_DEBUG))
#include <cstdio>
#define LOG_ERROR(str, ...) \
	do{\
	fprintf(stderr, "❌❌❌");\
	fprintf(stderr, str, ##__VA_ARGS__);\
	fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
	fprintf(stderr, "❌❌❌\n");\
    fflush(stderr);\
	}while(0)

#define LOG_CODE(...) __VA_ARGS__
#define LOG_INFO(...) do{printf(__VA_ARGS__); fflush(stdout);}while(0)

#else

#define LOG_ERROR(...)
#define LOG_CODE(...)
#define LOG_INFO(...)

#endif

#endif



#endif /* HTPLATFORMS_H_ */
