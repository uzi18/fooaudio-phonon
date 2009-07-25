#ifndef _FOODECODER_HPP_
#define _FOODECODER_HPP_

enum DecoderErrorType
{
	ERROR_OK, /*!< There was no error. */
	ERROR_STREAM, /*!< Recoverable error in the stream. */
	ERROR_FATAL /*!< Fatal error in the stream - further decoding can't be performed. */
};

class DecoderError
{
public:
	DecoderError();
	DecoderError(const DecoderErrorType, const int, const char *, ...);

	void init();
	void clear();
	void copy(const DecoderError *);

	char * getErr();
	DecoderErrorType getType();

#warning "poprawić na prywatne"
	char *err;  /*!< malloc()ed error string or NULL. */
	DecoderErrorType type; /*!< Type of the error. */

};

class Decoder
{
public:
	Decoder();
};

#endif // _FOODECODER_HPP_

