#ifndef _FOODECODER_HPP_
#define _FOODECODER_HPP_

enum DecoderErrorType
{
	ERROR_OK, /*!< There was no error. */
	ERROR_STREAM, /*!< Recoverable error in the stream. */
	ERROR_FATAL /*!< Fatal error in the stream - further decoding can't be performed. */
};

struct DecoderError
{
	DecoderErrorType type; /*!< Type of the error. */
	char *err;  /*!< malloc()ed error string or NULL. */
};

void decoderErrorInit (DecoderError *);
void decoderError (DecoderError *, const DecoderErrorType, const int, const char *, ...);

#endif // _FOODECODER_HPP_

