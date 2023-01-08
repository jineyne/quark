#pragma once

#define VARARGS __cdecl

#define GET_VARARGS_RESULT(msg, msgsize, len, lastarg, fmt, result) \
	{ \
		va_list ap; \
		va_start(ap, lastarg); \
		result = FCString::Vsprintf(msg, msgsize, fmt, ap); \
		if (result >= msgsize) \
		{ \
			result = -1; \
		} \
		va_end(ap); \
	}

#define GET_VARARGS_RESULT_WIDE(msg, msgsize, len, lastarg, fmt, result) \
	{ \
		va_list ap; \
		va_start(ap, lastarg); \
		result = FCStringWide::GetVarArgs(msg, msgsize, fmt, ap); \
		if (result >= msgsize) \
		{ \
			result = -1; \
		} \
		va_end(ap); \
	}
#define GET_VARARGS_RESULT_ANSI(msg, msgsize, len, lastarg, fmt, result) \
	{ \
		va_list ap; \
		va_start(ap, lastarg); \
		result = FCStringAnsi::GetVarArgs(msg, msgsize, fmt, ap); \
		if (result >= msgsize) \
		{ \
			result = -1; \
		} \
		va_end(ap); \
	}

#define GET_VARARGS_RESULT_VALIST(msg, msgsize, len, lastarg, fmt, result, ap) \
	{ \
		result = FCString::Vsprintf(msg, msgsize, fmt, ap); \
		if (result >= msgsize) \
		{ \
			result = -1; \
		} \
	}

#define GET_VARARGS_RESULT_VALIST_WIDE(msg, msgsize, len, lastarg, fmt, result) \
	{ \
		result = FCStringWide::GetVarArgs(msg, msgsize, fmt, ap); \
		if (result >= msgsize) \
		{ \
			result = -1; \
		} \
	}
#define GET_VARARGS_RESULT_VALIST_ANSI(msg, msgsize, len, lastarg, fmt, result) \
	{ \
		result = FCStringAnsi::GetVarArgs(msg, msgsize, fmt, ap); \
		if (result >= msgsize) \
		{ \
			result = -1; \
		} \
	}