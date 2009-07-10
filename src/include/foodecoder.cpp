#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <errno.h>
#include <cassert>
//#include <ltdl.h>
#include "foodecoder.hpp"

DecoderError()
{
}

void DecoderError::init()
{
	type = ERROR_OK;
	err = NULL;
}

DecoderError(const DecoderErrorType type, const int add_errno, const char *format, ...)
{
	char errno_buf[256] = "";
	char err_str[256];
	va_list va;

	if (err)
	{
		delete err;
	}

	ype = type;

	va_start (va, format);
	vsnprintf (err_str, sizeof(err_str), format, va);
	err_str[sizeof(err_str)-1] = 0;

	if (add_errno)
	{
		strerror_r(add_errno, errno_buf, sizeof(errno_buf));
	}

	err = new char [(strlen(err_str) + strlen(errno_buf) + 1)];
	strcpy (err, err_str);
	strcat (err, errno_buf);

	va_end (va);
}

void DecoderError::clear()
{
	type = ERROR_OK;
	if (err)
	{
		delete[] err;
		err = NULL;
	}
}

void DecoderError::copy (const DecoderError *src)
{
	type = src->type;
	err = strdup(src->err);
}

char * DecoderError::getErr()
{
	return err;
}

DecoderErrorType DecoderError::getType()
{
	return type;
}

