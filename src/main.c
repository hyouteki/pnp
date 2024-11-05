#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "helper.h"
#include "hashmap.h"
#include "package.h"
#include "tomlc99/toml.h"

#define PNP_CONF_NAME ".pnp.toml"
#ifdef _WIN32
#define REDIRECT_NULL " > NUL 2>&1"
#else
#define REDIRECT_NULL " > /dev/null 2>&1" 
#endif

typedef enum {
	Pnp_Subcommand_Build,
	Pnp_Subcommand_Search,
	Pnp_Subcommand_Install,
	Pnp_Subcommand_Uninstall,
	Pnp_Subcommand_List,
	Pnp_Subcommand_Info,
	Pnp_Subcommand_GenerateGitignore,
	Pnp_Subcommand_GenerateSetupGuide,
	Pnp_Subcommand_GenerateCourtesy,
	Pnp_Subcommand_GenerateReadme,
	Pnp_Subcommand_None,
} Pnp_Subcommand_Kind;

typedef struct BuildFlags {
	int generate_readme;
	int generate_gitignore;
	int generate_setup_guide;
	int generate_courtesy;
} BuildFlags;

BuildFlags flags = {0};

static char *subcommand_package_name;
static String_Hashmap hashmap;
static Pnp_Subcommand_Kind subcommand = Pnp_Subcommand_None;

static void usage();
static char *get_package_name(int, char *[]);
static void parse_args(int, char*[]);
static char *toml_read_string(toml_table_t *, const char *);
static Package *parse_package(toml_table_t *);
static void parse_pnp_repository(const char *);
static void parse_pnp_toml_configuration(toml_array_t *);
static void parse_pnp_conf();
static void perform_subcommand_build(Package *);
static void perform_subcommand(Package *);
static char *join_paths(const char *, const char *);
static int path_exists(const char *);

static void usage() {
    printf("usage: pnp <SUBCOMMAND> [<ARG>]\n"
           "\n"
           "SUBCOMMANDS:\n"
           "    build                         Build the project based on '"PNP_CONF_NAME"'\n"
           "    search      <PACKAGE_NAME>    Search for a package in the pnp repository\n"
           "    install     <PACKAGE_NAME>    Install a package\n"
           "    uninstall   <PACKAGE_NAME>    Uninstall a package\n"
           "    list                          List local packages\n"
           "    help                          Display help\n"
           "    info        <PACKAGE_NAME>    Retrieve package information\n"
           "    update                        Update the local pnp repository\n"
           "    find [alias for search]       Search for a package in the pnp repository\n"
           "    generate-gitignore            Generate .gitignore based on '"PNP_CONF_NAME"'\n"
           "    generate-setup-guide          Generate SETUP.md based on '"PNP_CONF_NAME"'\n"
           "    generate-courtesy             Generate COURTESY.md based on '"PNP_CONF_NAME"'\n"
           "    generate-readme               Generate README.md based on '"PNP_CONF_NAME"'\n");
}

static char *get_package_name(int argc, char *argv[]) {
	if (argc < 3) Error_Usage("expected <PACKAGE_NAME>", "");
	return argv[2];
}

static void parse_args(int argc, char *argv[]) {
	if (argc < 2) Error_Usage("expected SUBCOMMAND", "");
	if (strcmp(argv[1], "help") == 0) {
		usage();
		exit(EXIT_SUCCESS);
	} else if (strcmp(argv[1], "build") == 0) {
		subcommand = Pnp_Subcommand_Build;
		parse_pnp_conf();
	} else if (strcmp(argv[1], "search") == 0 || strcmp(argv[1], "find") == 0) {
		subcommand = Pnp_Subcommand_Search;
		subcommand_package_name = get_package_name(argc, argv);
		String_Hashmap_Insert(&hashmap, subcommand_package_name, NULL);
	}
}

static char *toml_read_string(toml_table_t *toml_table, const char *field) {
	toml_datum_t toml_field = toml_string_in(toml_table , field);
	if (!toml_field.ok) Error("cannot read property '%s'", field);
	return toml_field.u.s;
}

static Package *parse_package(toml_table_t *toml_package) {
	Package *package = (Package *)malloc(sizeof(Package));
	package->name = toml_read_string(toml_package, "name");
	char *type = toml_read_string(toml_package, "type");
	if (strcmp(type, "file") == 0) {
		package->file.author = toml_read_string(toml_package, "author");
		package->file.src = toml_read_string(toml_package, "src");
		package->file.license = toml_read_string(toml_package, "license");
		package->kind = Package_Kind_File;
	} else if (strcmp(type, "github") == 0) {
		package->github.author = toml_read_string(toml_package, "author");
		package->github.repo = toml_read_string(toml_package, "repo");
		package->github.branch = toml_read_string(toml_package, "branch");
		package->github.version = toml_read_string(toml_package, "version");
		package->github.license = toml_read_string(toml_package, "license");
		package->kind = Package_Kind_Github;
	} else Error_NoExit("cannot invalid package type '%s'", type);
	return package;
}

static void parse_pnp_repository(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) Error("cannot open file '%s'", filename);

    char error_buffer[200];
    toml_table_t *root = toml_parse_file(file, error_buffer, sizeof(error_buffer));
    fclose(file);
    if (!root) Error_Toml("cannot parse file", error_buffer);

    toml_array_t *packages = toml_array_in(root, "package");
    if (!packages) Error("cannot find package array", "");

	size_t i = 0;
    while (1) {
        toml_table_t *toml_package = toml_table_at(packages, i++);
        if (!toml_package) break;
		Package *package = parse_package(toml_package);
		perform_subcommand(package);
		//TODO: free package and its content
		toml_free(toml_package);
    }
#ifdef _WIN32
	toml_free(root);
#endif
}

static void parse_pnp_toml_configuration(toml_array_t *configuration) {
	for (int i = 0; i < toml_array_nelem(configuration); i++) {
		toml_table_t* entry = toml_table_at(configuration, i);

		if (toml_bool_in(entry, "generate-readme").u.b) flags.generate_readme = 1;
		if (toml_bool_in(entry, "generate-gitignore").u.b) flags.generate_gitignore = 1;
		if (toml_bool_in(entry, "generate-setup_guide").u.b) flags.generate_setup_guide = 1;
		if (toml_bool_in(entry, "generate-courtesy").u.b) flags.generate_courtesy = 1;
	}
}

static void parse_pnp_conf() {
    FILE *file = fopen(PNP_CONF_NAME, "r");
    if (!file) Error("cannot open file '%s'", PNP_CONF_NAME);

    char error_buffer[200];
    toml_table_t *root = toml_parse_file(file, error_buffer, sizeof(error_buffer));
    fclose(file);
    if (!root) Error_Toml("cannot parse file", error_buffer);

	toml_array_t *configuration = toml_array_in(root, "configuration");
	if (configuration) parse_pnp_toml_configuration(configuration);
	
    toml_array_t *dependencies = toml_array_in(root, "dependency");
    if (!dependencies) Error("cannot find dependency array", "");

	size_t i = 0;
    while (1) {
        toml_table_t *dependency = toml_table_at(dependencies, i++);
        if (!dependency) break;
		char *dep_name = toml_read_string(dependency, "name");
		char *dep_path = toml_read_string(dependency, "path");
		String_Hashmap_Insert(&hashmap, dep_name, dep_path);
		free(dep_name);
		free(dep_path);
		toml_free(dependency);
    }
#ifdef _WIN32
	toml_free(root);
#endif
}

static void perform_subcommand_build(Package *package) {
	if (!String_Hashmap_Find(&hashmap, package->name)) return;
	char *relative_path = String_Hashmap_Lookup(&hashmap, package->name);
	printf("info: build '%s' at '%s'\n", package->name, relative_path);
	char *path_from_root = join_paths("./", relative_path);
	free(relative_path);
	char *download_path = join_paths(path_from_root, package->name);
	free(path_from_root);
	if (path_exists(download_path)) {
		printf("info: path exists; ignoring build for this package\n");
		return;
	}
	char snprintf_buffer[1024];
	int code;
	switch (package->kind) {
	case Package_Kind_File: {
		code = snprintf(snprintf_buffer, sizeof(snprintf_buffer),
						"curl %s -o %s -s", package->file.src, download_path);
	} break;
	case Package_Kind_Github:
		code = snprintf(snprintf_buffer, sizeof(snprintf_buffer),
						"git clone https://github.com/%s/%s --single-branch --branch %s %s %s",
						package->github.author, package->github.repo, package->github.branch,
						download_path, REDIRECT_NULL);
		break;
	default:
		Error("unknown Package_Kind '%d'", package->kind);
	}
	printf("info: cmd: %s\n", snprintf_buffer);
	if (code < 0) Error("snprintf error");
	system(snprintf_buffer);
}

static void perform_subcommand(Package *package) {
	switch (subcommand) {
	case Pnp_Subcommand_Build:
		perform_subcommand_build(package);
		break;
	default:
		Error("unknown SUBMCOMMAND", "");
	}
}

static char *join_paths(const char *path1, const char *path2) {
	size_t len1 = strlen(path1), len2 = strlen(path2);
    size_t maxlen = len1 + len2 + 2; // for '/' and '\0'
	
	char *joined_path = (char *)malloc(sizeof(char)*maxlen);
    if (!joined_path) Error("malloc failed");
	strcpy(joined_path, path1);

	if (path1[len1-1] != '/' && path2[0] != '/') strcat(joined_path, "/");
    else if (path1[len1-1] == '/' && path2[0] == '/') joined_path[len1-1] = '\0';
    strcat(joined_path, path2);

	return joined_path;
}

int path_exists(const char *path) {
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

int main(int argc, char *argv[]) {
	parse_args(argc, argv);
    parse_pnp_repository("pnp_repository.toml");
    return 0;
}
