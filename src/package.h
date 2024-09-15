#ifndef PLUG_PACKAGE_H_
#define PLUG_PACKAGE_H_

typedef struct Package_Kind_File {
	char *author;
	char *src;
	char *license;
} Package_File;

typedef struct Package_Kind_Github {
	char *author;
	char *repo;
	char *branch;
	char *version;
	char *license;
} Package_Github;

typedef enum {
	Package_Kind_File,
	Package_Kind_Github,
} Package_Kind;

typedef struct Package {
	char *name;
	Package_Kind kind;
	union {
		Package_File file;
		Package_Github github;
	};
} Package;

#endif // PLUG_PACKAGE_H_
