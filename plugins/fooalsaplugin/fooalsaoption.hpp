#ifndef _FOOALSAOPTION_HPP_
#define _FOOALSAOPTION_HPP_

const int OPTIONS_MAX = 128;
const int OPTION_NAME_MAX = 32;

static int options_num = 0;

enum option_type
{
	OPTION_INT,
	OPTION_STR,
	OPTION_ANY
};

union option_value
{
	char *str;
	int num;
};

struct option
{
	char name[OPTION_NAME_MAX];
	option_type type;
	option_value value;
	int ignore_in_config;
	int set_in_config;
};

static struct option options[OPTIONS_MAX];

static int find_option (const char *, option_type);
char *options_get_str (const char *);

#endif // _FOOALSAOPTION_HPP_

