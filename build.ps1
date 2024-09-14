$project_name = "pnp"
$cc = "gcc"

& $cc "${project_name}.c" tomlc99/toml.c -o $project_name
if ($LASTEXITCODE -ne 0) {
    Write-Host "error: build failed with exit code $LASTEXITCODE"
} else {
	& ./$project_name info fasm-mode
}
