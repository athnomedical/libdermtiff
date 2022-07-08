#Requires -RunAsAdministrator

$RUNNING_PATH = Get-Location

# Build dependencies
&${PSScriptRoot}\install_dependency.ps1

Set-Location $RUNNING_PATH