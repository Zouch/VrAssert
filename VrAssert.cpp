#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG

#define BREAKPOINT(arg) __debugbreak()

void clearInputBuffer()
{
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}

bool handleAssert()
{
	fprintf(stderr, "Assert handling: break (b, default), continue (c), ignore (i), quit (q): ");
	char value = getchar();
	clearInputBuffer();

	switch (value)
	{
	case 'c':
		return false;

	case 'i':
		return true;

	case 'q':
		exit(42);
		break;
	default:
		BREAKPOINT(0);
		return false;
	}
}

// Allow with and without a message assert calls
// https://stackoverflow.com/a/28074198/4717805
// It might be great to find out if there is a way to allow 
// a call with any number of parameters,
// but this does not seem to work properly
#define PRINT_8(_1, _2, _3, _4, _5, _6, _7, _8) fprintf(stderr, ": "); fprintf(stderr, _1, _2, _3, _4, _5, _6, _7, _8)
#define PRINT_7(_1, _2, _3, _4, _5, _6, _7) fprintf(stderr, ": "); fprintf(stderr, _1, _2, _3, _4, _5, _6, _7)
#define PRINT_6(_1, _2, _3, _4, _5, _6) fprintf(stderr, ": "); fprintf(stderr, _1, _2, _3, _4, _5, _6)
#define PRINT_5(_1, _2, _3, _4, _5) fprintf(stderr, ": "); fprintf(stderr, _1, _2, _3, _4, _5)
#define PRINT_4(_1, _2, _3, _4) fprintf(stderr, ": "); fprintf(stderr, _1, _2, _3, _4)
#define PRINT_3(_1, _2, _3) fprintf(stderr, ": "); fprintf(stderr, _1, _2, _3)
#define PRINT_2(_1, _2) fprintf(stderr, ": "); fprintf(stderr, _1, _2)
#define PRINT_1(_1) PRINT_2("%s", _1)
#define PRINT_0() 

#define PRINT_CHOOSER(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, ...) _f8
#define PRINT_RECOMPOSER(args) PRINT_CHOOSER args
#define CHOOSE_FROM_ARGCOUNT(...) PRINT_RECOMPOSER ((__VA_ARGS__, PRINT_8, PRINT_7, PRINT_6, PRINT_5, PRINT_4, PRINT_3, PRINT_2, PRINT_1, ))
#define NO_ARG_EXPANDER() ,,,,,,,, PRINT_0
#define CHOOSER(...) CHOOSE_FROM_ARGCOUNT(NO_ARG_EXPANDER __VA_ARGS__ ())

#define VrAssert(x, ...)																\
	do {																				\
		static bool ignoreLater = false;												\
		if (!(x) && !ignoreLater) {														\
			fprintf(stderr, "%s:%i: Assertion `%s` failed", __FILE__, __LINE__, #x);	\
			CHOOSER(__VA_ARGS__)(__VA_ARGS__);											\
			fprintf(stderr, "\n");														\
			ignoreLater = handleAssert();												\
		}																				\
	} while (false)

#else
#define VrAssert(x)
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
		VrAssert(false, "Swaggy P");
	}

	printf("Ok\n");

    return 0;
}
