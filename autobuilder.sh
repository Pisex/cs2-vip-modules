#!/bin/bash

# Set the script according to the directory you run it in
MODULES_DIR="/root/cs2-vip-modules/"

# Delete build folders from folders
for folder in VIPS VIP_AntiFlash VIP_BaseFunctions VIP_Bhop VIP_BuyTeamWeapon VIP_FastDefuse VIP_FastPlant VIP_Healthshot VIP_Items VIP_KillScreen VIP_RainbowModel VIP_Respawn VIP_ShowDamage VIP_Smoke VIP_Tag VIP_Test; do
    rm -r "$MODULES_DIR$folder"/build
    mkdir "$MODULES_DIR$folder"/build
done

# recreating the build folder and going into it
for folder in VIPS VIP_AntiFlash VIP_BaseFunctions VIP_Bhop VIP_BuyTeamWeapon VIP_FastDefuse VIP_FastPlant  VIP_Healthshot VIP_Items VIP_KillScreen VIP_RainbowModel VIP_Respawn VIP_ShowDamage VIP_Smoke VIP_Tag VIP_Test; do
    if [ -d "$MODULES_DIR$folder/source" ]; then
        cd "$MODULES_DIR$folder/source" || exit
    elif [ -d "$MODULES_DIR$folder/src" ]; then
        cd "$MODULES_DIR$folder/src" || exit
    else
        echo "Niether 'source' nor 'src' directory found in $folder"
        continue
    fi
    mkdir build
    cd build || exit
    python3 ../configure.py --enable-optimize --symbol-files --sdks cs2 --hl2sdk-manifests hl2sdk-manifests
    ambuild
    cd "$MODULES_DIR"
done

# Zip file creation and naming
for folder in VIPS VIP_AntiFlash VIP_BaseFunctions VIP_Bhop VIP_BuyTeamWeapon VIP_FastDefuse VIP_FastPlant VIP_Healthshot VIP_Items VIP_KillScreen VIP_RainbowModel VIP_Respawn VIP_ShowDamage VIP_Smoke VIP_Tag VIP_Test; do
    if [ -d "$MODULES_DIR$folder/source/build/package/addons" ]; then
        cd "$MODULES_DIR$folder/source/build/package/addons" || exit
        zip -r "$MODULES_DIR$folder"/build/"$folder".zip .
        mv "$MODULES_DIR$folder"/build/"$folder".zip "$MODULES_DIR$folder"/build/
    elif [ -d "$MODULES_DIR$folder/src/build/package/addons" ]; then
        cd "$MODULES_DIR$folder/src/build/package/addons" || exit
        zip -r "$MODULES_DIR$folder"/build/"$folder".zip .
        mv "$MODULES_DIR$folder"/build/"$folder".zip "$MODULES_DIR$folder"/build/
    fi
done


# Coming back and cleaning the build folders
for folder in VIPS VIP_AntiFlash VIP_BaseFunctions VIP_Bhop VIP_BuyTeamWeapon VIP_FastDefuse VIP_FastPlant VIP_Healthshot VIP_Items VIP_KillScreen VIP_RainbowModel VIP_Respawn VIP_ShowDamage VIP_Smoke VIP_Tag VIP_Test; do
    if [ -d "$MODULES_DIR$folder/source/build" ]; then
        rm -r "$MODULES_DIR$folder"/source/build
    elif [ -d "$MODULES_DIR$folder/src/build" ]; then
        rm -r "$MODULES_DIR$folder"/src/build
    fi
done
