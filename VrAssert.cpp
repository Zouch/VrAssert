#include <stdio.h>
#include <stdlib.h>

#if defined(__clang__) || defined(__GNUC__)
#	define BREAKPOINT() asm volatile ("int $3")
#elif defined(_MSC_VER)
#	define BREAKPOINT() __debugbreak()
#endif

#if defined(_DEBUG)
#	define MACRO_BEGIN do {
#	define MACRO_END } while (0)
#else
#	define MACRO_BEGIN if (true) {
#	define MACRO_END } else {}
#endif

#if defined(_DEBUG) || defined(LOG_ASSERT)

namespace
{
void clearInputBuffer()
{
	char lChar;
	while ((lChar = getchar()) != '\n' && lChar != EOF) {}
}

bool handleAssert()
{
#if defined(_DEBUG)
	fprintf(stderr, "Assert handling: break (b, default), continue (c), ignore (i), quit (q): ");
	char lValue = getchar();
	clearInputBuffer();

	switch (lValue)
	{
	case 'c':
		return false;

	case 'i':
		return true;

	case 'q':
		exit(42);
		break;
	default:
		BREAKPOINT();
		return false;
	}
#else
	return false;
#endif
}

FILE* getAssertFile()
{
#if !defined(_DEBUG) && defined(LOG_ASSERT_FILE)
	static FILE* sFile = fopen(LOG_ASSERT_FILE, "w");
#else
	static FILE* sFile = stderr;
#endif

	return sFile;
}
}

// Allow with and without a message assert calls
// https://stackoverflow.com/a/28074198/4717805
#define PRINT_N(...) fprintf(lFile, ": "); fprintf(lFile, __VA_ARGS__)
#define PRINT_0()

#define PRINT_CHOOSER(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, ...) _f8
#define PRINT_RECOMPOSER(pArgs) PRINT_CHOOSER pArgs
#define CHOOSE_FROM_ARGCOUNT(...) PRINT_RECOMPOSER ((__VA_ARGS__, PRINT_N, PRINT_N, PRINT_N, PRINT_N, PRINT_N, PRINT_N, PRINT_N, PRINT_N, ))
#define NO_ARG_EXPANDER() ,,,,,,,, PRINT_0
#define CHOOSER(...) CHOOSE_FROM_ARGCOUNT(NO_ARG_EXPANDER __VA_ARGS__ ())

#define VrAssert(x, ...)																\
	MACRO_BEGIN																			\
	static bool sIgnoreLater = false;													\
	if (!(x) && !sIgnoreLater) {														\
		FILE* lFile = getAssertFile();													\
		fprintf(lFile, "%s:%i: Assertion `%s` failed", __FILE__, __LINE__, #x);			\
		CHOOSER(__VA_ARGS__)(__VA_ARGS__);												\
		fprintf(lFile, "\n");															\
		sIgnoreLater = handleAssert();													\
	}																					\
	MACRO_END

#else
#define VrAssert(...)
#endif

int main()
{
	VrAssert(false);
	VrAssert(false, "Yolo");
	VrAssert(false, "Yolo %i", 2);
	VrAssert(false, "Yolo %i %i", 2, 3);
	VrAssert(false, "Yolo %i %i %i", 2, 3, 4);
	VrAssert(false, "Yolo %i %i %i %i", 2, 3, 4, 5);
	VrAssert(false, "Yolo %i %i %i %i %i", 2, 3, 4, 5, 6);

	for (int i = 0; i < 5; ++i)
	{
		VrAssert(false, "Iteration %d", i);
	}

	VrAssert(false, "Ok");

	return 0;
}
