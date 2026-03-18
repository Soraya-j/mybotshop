#!/usr/bin/env bash
# ==============================================================================
# Compile Python to Binary using Nuitka
# ==============================================================================
# Usage:
#   cd src/mybotshop/go2_webserver/go2_webserver
#   ../scripts/compile_binary.sh
#
# Or from package root:
#   cd go2_webserver && ../scripts/compile_binary.sh
# ==============================================================================

RED='\033[0;31m'
DGREEN='\033[0;32m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
NC='\033[0m'

echo -e "${DGREEN}-------------------------------------------------------------------------------"
echo -e "  __  ____     ______   ____ _______ _____ _    _  ____  _____           _____ "
echo -e " |  \/  \ \   / /  _ \ / __ \__   __/ ____| |  | |/ __ \|  __ \         / ____|"
echo -e " | \  / |\ \_/ /| |_) | |  | | | | | (___ | |__| | |  | | |__) |  _   _| |  __ "
echo -e " | |\/| | \   / |  _ <| |  | | | |  \___ \|  __  | |  | |  ___/  | | | | | |_ |"
echo -e " | |  | |  | |  | |_) | |__| | | |  ____) | |  | | |__| | |      | |_| | |__| |"
echo -e " |_|  |_|  |_|  |____/ \____/  |_| |_____/|_|  |_|\____/|_|       \__,_|\_____|"
echo -e ""
echo -e "-------------------------------------------------------------------------------"
echo -e "Python Binary Generator (Nuitka)"
echo -e "-------------------------------------------------------------------------------${NC}"

# Files to exclude from compilation
EXCLUDE_FILES="__init__.py libroscustom.py"

# Check for nuitka
if ! python3 -c "import nuitka" 2>/dev/null; then
    echo -e "${RED}Nuitka not installed. Install with: pip3 install nuitka ordered-set --break-system-packages${NC}"
    exit 1
fi

# Compile each .py file
for pyfile in *.py; do
    # Skip excluded files
    skip=false
    for exclude in $EXCLUDE_FILES; do
        if [[ "$pyfile" == "$exclude" ]]; then
            skip=true
            break
        fi
    done

    if $skip; then
        echo -e "${BLUE}Skipping: $pyfile${NC}"
        continue
    fi

    echo -e "${GREEN}Compiling: $pyfile${NC}"
    python3 -m nuitka --module --remove-output --no-pyi-file "$pyfile" 2>&1 | grep -E "(Successfully|ERROR)" || true
done

# Rename .so files (remove cpython-* suffix)
for f in *.cpython-*.so; do
    if [[ -f "$f" ]]; then
        base_name="${f%%.cpython-*}.so"
        mv "$f" "$base_name"
        echo -e "${GREEN}Renamed: $f -> $base_name${NC}"
    fi
done

# Delete original .py files (except excluded ones)
for pyfile in *.py; do
    skip=false
    for exclude in $EXCLUDE_FILES; do
        if [[ "$pyfile" == "$exclude" ]]; then
            skip=true
            break
        fi
    done

    if ! $skip; then
        rm -f "$pyfile"
        echo -e "${GREEN}Deleted: $pyfile${NC}"
    fi
done

# Cleanup build artifacts
rm -rf *.build build *.c

echo -e "${DGREEN}-------------------------------------------------------------------------------"
echo -e "Compilation complete!"
echo -e "Now run: colcon build --packages-select go2_webserver"
echo -e "-------------------------------------------------------------------------------${NC}"
