#!/bin/bash

echo "Detecting package manager and installing SDL2 dependencies..."

if command -v pacman &> /dev/null; then
    # For Arch Linux / CachyOS
    echo "pacman detected. Installing packages for Arch-based system..."
    sudo pacman -Syu --needed base-devel sdl2 sdl2_image sdl2_ttf sdl2_mixer

elif command -v apt &> /dev/null; then
    # For Debian / Ubuntu / WSL on Windows
    echo "apt detected. Installing packages for Debian/Ubuntu..."
    sudo apt update
    sudo apt install -y build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev

else
    echo "Could not detect package manager. Please install SDL2 manually."
    exit 1
fi

echo "Installation complete! Now you can run make."