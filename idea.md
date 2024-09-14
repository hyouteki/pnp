## github without release
author
repo
branch
file/dir
license ["custom", "MIT", "GPL-3.0"]


usage: pnp <SUBCOMMAND> [<ARG>]

SUBCOMMAND:
    build                         build the project based off '.pnp.deps.toml'
	search      <SEARCH_ARGS>     search for a package in pnp repository
	install     <PACKAGE_NAME>    install a package
	uninstall   <PACKAGE_NAME>    uninstall a package
	list                          lists local packages
	help                          displays help
	info        <PACKAGE_NAME>    retrieves package information
	find [alias for search]       search for a package in pnp repository