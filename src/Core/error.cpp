#include "error.h"
// Error Reporting Includes
#include <stdarg.h>
#include <string>

// Error Reporting Definitions
#define KAGUYA_ERROR_IGNORE 0
#define KAGUYA_ERROR_CONTINUE 1
#define KAGUYA_ERROR_ABORT 2
/*
static void processError(const char* format, va_list args,
	const char* errorType, int disposition) {
	// Report error
	if (disposition == KAGUYA_ERROR_IGNORE) return;

	// Build up an entire formatted error string and print it all at once;
	// this way, if multiple threads are printing messages at once, they
	// don't get jumbled up...
	std::string errorString;

	// Print line and position in input file, if available
	extern int line_num;
	if (line_num != 0) {
		extern std::string current_file;
		errorString += current_file;
		errorString += StringPrintf("(%d): ", line_num);
	}

	// PBRT_ERROR_CONTINUE, PBRT_ERROR_ABORT
	// Print formatted error message
	int width = std::max(20, TerminalWidth() - 2);
	errorString += errorType;
	errorString += ": ";
	int column = errorString.size();

	std::string message = StringVaprintf(format, args);

	const char* msgPos = message.c_str();
	while (true) {
		while (*msgPos != '\0' && isspace(*msgPos)) ++msgPos;
		if (*msgPos == '\0') break;

		const char* wordEnd = findWordEnd(msgPos);
		if (column + wordEnd - msgPos > width) {
			errorString += "\n    ";
			column = 4;
		}
		while (msgPos != wordEnd) {
			errorString += *msgPos++;
			++column;
		}
		errorString += ' ';
		++column;
	}

	// Print the error message (but not more than one time).
	static std::string lastError;
	static std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	if (errorString != lastError) {
		fprintf(stderr, "%s\n", errorString.c_str());
		lastError = errorString;
	}

	if (disposition == KAGUYA_ERROR_ABORT) {
#if defined(PBRT_IS_WINDOWS)
		__debugbreak();
#else
		abort();
#endif
	}
}*/

void Error(const char* format, ...) {
	va_list args;
	va_start(args, format);
	//processError(format, args, "Error", KAGUYA_ERROR_CONTINUE);
	va_end(args);
}

void Severe(const char* format, ...) {
	va_list args;
	va_start(args, format);
	//processError(format, args, "Fatal Error", KAGUYA_ERROR_ABORT);
	va_end(args);
}