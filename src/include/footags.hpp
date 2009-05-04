#ifndef FOOTAGS_HPP
#define FOOTAGS_HPP

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
