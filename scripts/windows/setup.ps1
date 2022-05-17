$RUNNING_PATH = Get-Location

# Update submodules
git submodule update --init

# Build dependencies
&${PSScriptRoot}\build_library.ps1

Set-Location $RUNNING_PATH