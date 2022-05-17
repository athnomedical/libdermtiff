$RUNNING_PATH = Get-Location
$REPO_PATH = Resolve-Path (Join-Path $PSScriptRoot ..\..\)

# Build and install library.
function InstallLibrary ($library) {
    Write-Host "Build and install ${library}" -ForegroundColor Green
    Set-Location ${REPO_PATH}deps\${library}
    mkdir build -Force
    Set-Location .\build\
    cmake ..
    cmake --build . --config Release
    cmake --install .

    # Copy shared object to bin/
    mkdir ${REPO_PATH}bin -Force
    Copy-Item ${REPO_PATH}deps\${library}\build\Release\${library}.dll ${REPO_PATH}bin\

    # Clean library repo
    Set-Location ..\
    git restore .
    git clean -f -d
}

# Build and install ZLIB
InstallLibrary("zlib")

Set-Location $RUNNING_PATH