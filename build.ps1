$build_name = "pnp"
$cc = "gcc"
$cflags = @("-Wall", "-Wextra", "-Wno-format", "-Wno-format-extra-args")
$src_dir = "src"
$deps = @("tomlc99/toml.c")
$main_file = "main.c"
$build_output = "./" + $build_name

$main_path = Join-Path $src_dir $main_file
$dep_paths = $deps | ForEach-Object { Join-Path $src_dir $_ }

& $cc $main_path $cflags $dep_paths -o $build_output
if ($LASTEXITCODE -ne 0) {
    Write-Host "error: build failed with exit code $LASTEXITCODE"
} else {
    & $build_output build
}
