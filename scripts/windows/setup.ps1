$RUNNING_PATH = Get-Location
$REPO_PATH = Resolve-Path (Join-Path $PSScriptRoot ..\..\)
function Build ($library) {
    Write-Host "Build and install ${library}" -ForegroundColor Green
    Set-Location ${REPO_PATH}deps\$library
    mkdir build -Force
    Set-Location .\build\
    cmake ..
    cmake --build . --config Release
    cmake --install .
}

# Build and install ZLIB
build("zlib")

Set-Location $RUNNING_PATH