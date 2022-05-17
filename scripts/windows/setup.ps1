$RUNNING_PATH = Get-Location

# Build dependencies
&${PSScriptRoot}\build_library.ps1

Set-Location $RUNNING_PATH