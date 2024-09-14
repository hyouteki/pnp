#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper.h"
#include "hashset.h"
#include "repository.h"
#include "tomlc99/toml.h"

static char *arg_package_name;
static String_Hashset hashset;

void usage() {
	printf("usage: pnp <SUBCOMMAND> [<ARG>]\n"
		   "\n"
		   "SUBCOMMAND:\n"
		   "    build                         build the project based off '.pnp.deps.toml'\n"
		   "    search      <SEARCH_ARGS>     search for a package in pnp repository\n"
		   "    install     <PACKAGE_NAME>    install a package\n"
		   "    uninstall   <PACKAGE_NAME>    uninstall a package\n"
		   "    list                          lists local packages\n"
		   "    help                          displays help\n"
		   "    info        <PACKAGE_NAME>    retrieves package information\n"
		   "    update                        updates the local pnp repository\n"
		   "    find [alias for search]       search for a package in pnp repository\n");
}

void parse_args(int argc, char *argv[]) {
	if (argc < 2) Error_Usage("insufficient args", "");
	if (strcmp(argv[1], "help") == 0) {
		usage();
		exit(EXIT_SUCCESS);
	}
	String_Hashset_Insert(&hashset, argv[2]);
}

char *toml_read_string(toml_table_t *toml_repo, char *field) {
	toml_datum_t toml_field = toml_string_in(toml_repo , field);
	if (!toml_field.ok) Error("cannot read repository.%s", field);
	return toml_field.u.s;
}

Repository *parse_repository(toml_table_t *toml_repo) {
	Repository *repo = (Repository *)malloc(sizeof(Repository));

	repo->name = toml_read_string(toml_repo, "name");
	
	char *type = toml_read_string(toml_repo, "type");
	if (strcmp(type, "file") == 0) {
		repo->file.author = toml_read_string(toml_repo, "author");
		repo->file.src = toml_read_string(toml_repo, "src");
		repo->file.license = toml_read_string(toml_repo, "license");
		repo->kind = Repository_Kind_File;
	}

	return repo;
}

void parse_toml(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) Error("cannot open file '%s'", filename);

    char error_buffer[200];
    toml_table_t *root = toml_parse_file(file, error_buffer, sizeof(error_buffer));
    fclose(file);
    if (!root) Error_Toml("cannot parse file", error_buffer);

    toml_array_t *repositories = toml_array_in(root, "repository");
    if (!repositories) Error("cannot find repository array", "");

	size_t i = 0;
    while (1) {
        toml_table_t *repository = toml_table_at(repositories, i++);
        if (!repository) break;

        printf("type: %s\n", toml_read_string(repository, "type"));
		toml_free(repository);
    }
}

int main(int argc, char *argv[]) {
	parse_args(argc, argv);
    parse_toml("pnp_repository.toml");
    return 0;
}
