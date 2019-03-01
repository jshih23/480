#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	struct stat buf;
	int res = lstat("..", &buf);
	printf("%d\n", buf.st_size);
	printf("%d\n", res);
	return 0;
}