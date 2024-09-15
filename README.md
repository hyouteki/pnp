## PnPpm: Plug n Play Package Manager

### Philosophy

PnPpm (Plug n Play Package Manager) is designed to streamline package management by providing a unified way to download and manage packages from various sources. Whether your package is a single file from GitHub, an archive, a specific GitHub repository, a release, or even an archaic VCS release, PnPpm handles it all.

### Key Features
- **Flexible Package Sources**: Supports packages from various sources including single files, archives, repositories, releases, archaic version control systems, etc.
- **Automated Setup**: Generates `.gitignore`, setup guides, and auto-generated README files. Provides different setups for users who utilize PnPpm and those who don't, simplifying the build process with `pnp build`.
- **User Convenience**: Eliminates the need for users to manually navigate sites, download files, and follow complex setup instructions. 
- **Courtesy to Authors**: Acknowledges and respects the work of package authors by including proper documentation and setup procedures.

### Dependencies
- [curl](https://curl.se/download.html): Comes pre-installed on Windows 10 & 11.
- [git](https://git-scm.com/downloads): Download from the provided link.

### Getting started
Clone the repository and build:
```bash
git clone https://github.com/hyouteki/pnp --recursive --depth=1
cd pnp
```
On Windows
```bash
build.ps1
```
On GNU/Linux
```bash
chmod +x build.sh
./build.sh
```

### Courtesy
- [tomlc99 - cktan](https://github.com/cktan/tomlc99): TOML in c99; v1.0 compliant.
