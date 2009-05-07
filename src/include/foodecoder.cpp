#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <errno.h>
#include <cassert>
//#include <ltdl.h>
#include "foodecoder.hpp"

void decoderErrorInit (DecoderError *error)
{
	error->type = ERROR_OK;
	error->err = NULL;
}

void decoderError (DecoderError *error, const DecoderErrorType type, const int add_errno, const char *format, ...)
{
	char errno_buf[256] = "";
	char err_str[256];
	va_list va;

	if (error->err)
	{
		delete error->err;
	}

	error->type = type;

	va_start (va, format);
	vsnprintf (err_str, sizeof(err_str), format, va);
	err_str[sizeof(err_str)-1] = 0;

	if (add_errno)
	{
		strerror_r(add_errno, errno_buf, sizeof(errno_buf));
	}

	error->err = new char [(strlen(err_str) + strlen(errno_buf) + 1)];
	strcpy (error->err, err_str);
	strcat (error->err, errno_buf);

	va_end (va);
}

void decoderErrorClear (DecoderError *error)
{
	error->type = ERROR_OK;
	if (error->err)
	{
		delete[] error->err;
		error->err = NULL;
	}
}

void decoderErrorCopy (DecoderError *dst, const DecoderError *src)
{
	dst->type = src->type;
	dst->err = strdup (src->err);
}

