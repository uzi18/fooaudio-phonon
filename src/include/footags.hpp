#ifndef FOOTAGS_HPP
#define FOOTAGS_HPP

enum tags_select
{
	TAGS_COMMENTS = 0x01,
	TAGS_TIME = 0x02
};

struct FileTags
{
	char *title;
	char *artist;
	char *album;
	int track;
	int time;
	int filled; /* Which tags are filled: TAGS_COMMENTS, TAGS_TIME. */
};

#endif // FOOTAGS_HPP
