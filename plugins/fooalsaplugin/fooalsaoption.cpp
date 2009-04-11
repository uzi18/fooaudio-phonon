#include <cstring>
#include <iostream>

using namespace std;

#include "fooalsaoption.hpp"

static int find_option (const char *name, enum option_type type)
{
	for (int i = 0; i < options_num; i++)
	{
		if (!strcasecmp(options[i].name, name))
		{
			if (type != OPTION_ANY && options[i].type != type)
			{
				return -1;
			}

			return i;
		}
	}

	return -1;
}

char *options_get_str (const char *name)
{
	int i = find_option(name, OPTION_STR);

	if (i == -1)
	{
		cerr << "Tried to get wrong option " << name << "!" << endl;
	}

	return options[i].value.str;
}

