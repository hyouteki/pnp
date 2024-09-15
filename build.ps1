$project_name = "pnp"
$cc = "gcc"
$cflags = @("-Wall", "-Wextra", "-Wno-format", "-Wno-format-extra-args")
$deps = @("tomlc99/toml.c")

& $cc "${project_name}.c" $cflags $deps -o $project_name
if ($LASTEXITCODE -ne 0) {
    Write-Host "error: build failed with exit code $LASTEXITCODE"
} else {
	& ./$project_name build
}
