#ifndef _UCHAR_H_
#define	_UCHAR_H_

typedef	signed char		__int8_t;
typedef	unsigned char		__uint8_t;
typedef	short			__int16_t;
typedef	unsigned short		__uint16_t;
typedef	int			__int32_t;
typedef	unsigned int		__uint32_t;
typedef	long			__int64_t;
typedef	unsigned long		__uint64_t;

/* Clang already provides these types as built-ins, but only in C++ mode. */
#if !defined(__clang__) || !defined(__cplusplus)
typedef	__uint16_t __char16_t;
typedef	__uint32_t __char32_t;
#endif
/* In C++11, char16_t and char32_t are built-in types. */
#if defined(__cplusplus) && __cplusplus >= 201103L
#define	_CHAR16_T_DECLARED
#define	_CHAR32_T_DECLARED
#endif

#ifndef _CHAR16_T_DECLARED
typedef	__char16_t	char16_t;
#define	_CHAR16_T_DECLARED
#endif

#ifndef _CHAR32_T_DECLARED
typedef	__char32_t	char32_t;
#define	_CHAR32_T_DECLARED
#endif


#endif
