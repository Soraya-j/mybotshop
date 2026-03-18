#!/bin/bash

# ========================
# Common Shell Script Utilities
# Shared functions for build and setup scripts
# ========================

# Color definitions
COLOR_ERROR='\033[0;31m'     # Red
COLOR_SUCCESS='\033[0;32m'   # Green
COLOR_WARNING='\033[1;33m'   # Yellow
COLOR_INFO='\033[0;34m'      # Blue
COLOR_DEBUG='\033[0;36m'     # Cyan
COLOR_RESET='\033[0m'        # Reset

# ========================
# Output Functions
# ========================

print_success() {
    echo -e "${COLOR_SUCCESS}[SUCCESS]${COLOR_RESET} $1"
}

print_warning() {
    echo -e "${COLOR_WARNING}[WARNING]${COLOR_RESET} $1"
}

print_error() {
    echo -e "${COLOR_ERROR}[ERROR]${COLOR_RESET} $1"
}

print_info() {
    echo -e "${COLOR_INFO}[INFO]${COLOR_RESET} $1"
}

print_debug() {
    echo -e "${COLOR_DEBUG}[DEBUG]${COLOR_RESET} $1"
}

print_separator() {
    echo -e "${COLOR_INFO}-------------------------------------------------------------------${COLOR_RESET}"
}

print_header() {
    print_separator
    echo -e "${COLOR_INFO}$1${COLOR_RESET}"
}

# ========================
# Utility Functions
# ========================

ask_confirmation() {
    local message="$1"
    echo -e -n "${COLOR_WARNING}$message (y/n): ${COLOR_RESET}"
    read -r response
    case "$response" in
        [yY]) return 0 ;;
        [nN]) return 1 ;;
        *)
            print_error "Please enter 'y' or 'n'"
            ask_confirmation "$message"
            ;;
    esac
}

# Get absolute directory of the calling script
get_script_dir() {
    echo "$( cd "$( dirname "${BASH_SOURCE[1]}" )" &> /dev/null && pwd )"
}

# Detect platform information
detect_platform() {
    export OS_TYPE="$(uname -s)"
    export ARCH_TYPE="$(uname -m)"
    print_debug "Platform: ${OS_TYPE} (${ARCH_TYPE})"
}

# Check if network is available
# Returns 0 if online, 1 if offline
is_online() {
    local host="${1:-github.com}"
    local timeout="${2:-3}"

    # Try multiple methods for better compatibility
    if command -v curl &> /dev/null; then
        # Use curl with timeout
        curl --connect-timeout "$timeout" --silent --head "$host" &> /dev/null
        return $?
    elif command -v wget &> /dev/null; then
        # Use wget with timeout
        wget --timeout="$timeout" --tries=1 --spider --quiet "$host" &> /dev/null
        return $?
    elif command -v ping &> /dev/null; then
        # Fallback to ping (may not work if ICMP is blocked)
        ping -c 1 -W "$timeout" "$host" &> /dev/null
        return $?
    else
        # No tool available, assume offline to be safe
        return 1
    fi
}

# Check network with user feedback
check_network_status() {
    local host="${1:-github.com}"

    if is_online "$host"; then
        print_info "Network available, checking for updates..."
        return 0
    else
        print_warning "Network unavailable or cannot access $host"
        print_info "Using local files (offline mode)"
        return 1
    fi
}

# ========================
# Platform Detection
# ========================

# Detect if running on NVIDIA Jetson platform and configure environment
detect_jetson() {
    if [ -f /etc/nv_tegra_release ]; then
        export IS_JETSON=true
        export JETSON_RELEASE=$(head -1 /etc/nv_tegra_release | grep -oP 'R\d+')
        print_debug "Jetson platform detected: ${JETSON_RELEASE}"

        # Setup CUDA environment for Jetson
        setup_jetson_cuda

        # Setup Python environment for Jetson
        setup_jetson_python
    else
        export IS_JETSON=false
    fi
}

# Setup CUDA environment for Jetson
setup_jetson_cuda() {
    local cuda_path=""
    if [ -d "/usr/local/cuda" ]; then
        cuda_path="/usr/local/cuda"
    elif [ -d "/usr/local/cuda-11" ]; then
        cuda_path="/usr/local/cuda-11"
    fi

    if [ -n "$cuda_path" ]; then
        # Add CUDA to PATH if not already present
        if [[ ":$PATH:" != *":$cuda_path/bin:"* ]]; then
            export PATH="$cuda_path/bin:$PATH"
        fi

        # Set CUDA compiler
        if [ -f "$cuda_path/bin/nvcc" ]; then
            export CUDACXX="$cuda_path/bin/nvcc"
            export CMAKE_CUDA_COMPILER="$cuda_path/bin/nvcc"
        fi

        # Set library path
        if [[ ":$LD_LIBRARY_PATH:" != *":$cuda_path/lib64:"* ]]; then
            export LD_LIBRARY_PATH="$cuda_path/lib64:$LD_LIBRARY_PATH"
        fi

        export CUDA_HOME="$cuda_path"
        print_debug "CUDA configured: $cuda_path"
    fi
}

# Setup Python environment for Jetson
# Jetson typically has Python 3.8 with dev libraries
setup_jetson_python() {
    local python_exec=""
    local python_include=""
    local python_lib=""

    # Check Python 3.8 first (common on Jetson with JetPack 5.x)
    if [ -f "/usr/bin/python3.8" ] && [ -d "/usr/include/python3.8" ]; then
        python_exec="/usr/bin/python3.8"
        python_include="/usr/include/python3.8"
        if [ -f "/usr/lib/aarch64-linux-gnu/libpython3.8.so" ]; then
            python_lib="/usr/lib/aarch64-linux-gnu/libpython3.8.so"
        fi
    # Fallback to Python 3.6 (older JetPack)
    elif [ -f "/usr/bin/python3.6" ] && [ -d "/usr/include/python3.6m" ]; then
        python_exec="/usr/bin/python3.6"
        python_include="/usr/include/python3.6m"
        if [ -f "/usr/lib/aarch64-linux-gnu/libpython3.6m.so" ]; then
            python_lib="/usr/lib/aarch64-linux-gnu/libpython3.6m.so"
        fi
    fi

    if [ -n "$python_exec" ]; then
        export JETSON_PYTHON_EXECUTABLE="$python_exec"
        export JETSON_PYTHON_INCLUDE_DIR="$python_include"
        export JETSON_PYTHON_LIBRARY="$python_lib"
        print_debug "Python configured: $python_exec"
    fi
}

# Get CMake arguments for Jetson Python
get_jetson_cmake_python_args() {
    if [ "$IS_JETSON" = true ] && [ -n "$JETSON_PYTHON_EXECUTABLE" ]; then
        echo "-DPython3_EXECUTABLE=$JETSON_PYTHON_EXECUTABLE -DPython3_INCLUDE_DIR=$JETSON_PYTHON_INCLUDE_DIR -DPython3_LIBRARY=$JETSON_PYTHON_LIBRARY"
    fi
}

# Auto-detect Jetson on script load
detect_jetson

