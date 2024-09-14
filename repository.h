#ifndef PLUG_REPOSITORY_H_
#define PLUG_REPOSITORY_H_

typedef struct Repository_Kind_File {
	char *author;
	char *src;
	char *license;
} Repository_File;

typedef struct Repository_Kind_Github {
	char *author;
	char *repo;
	char *branch;
	char *version;
	char *license;
} Repository_Github;

typedef enum {
	Repository_Kind_File,
	Repository_Kind_Github,
} Repository_Kind;

typedef struct Repository {
	char *name;
	Repository_Kind kind;
	union {
		Repository_File file;
		Repository_Github github;
	};
} Repository;

#endif // PLUG_REPOSITORY_H_
