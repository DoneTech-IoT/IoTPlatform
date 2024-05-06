#!/bin/bash
usage()
{
   echo "USAGE: [-D] [-H] [-h] "
   echo "       -h = Don't panic! SEZ@Done helps you :D    "
    
#    exit 1
if [ ! -z $1 ] ; then
    exit $1
fi
}

#--->>> SEZ@Done color map ---
color_reset=$'\E'"[00m"        #RESET
color_failed=$'\E'"[0;31m"     #RED
color_success=$'\E'"[0;32m"    #GREEN
color_yellow=$'\E'"[1;33m"     #YELLOW (ORANGE)
color_blue=$'\E'"[1;34m"       #BLUE
color_magenta=$'\E'"[1;35m"    #MAGENTA
color_cyan=$'\E'"[1;36m"       #CYAN
color_gray=$'\E'"[1;37m"       #GRAY
#--->>>

ESP_IDF_PATH="/home/ehsan/espressif-all/esp-idf"
ESP_IDF_VERSION="esp-idf-v5.2"
#ESP_MATTER_PATH="/root/rk3566-src/RK356X_Android11.0"
#ESP_MATTER_VERSION=""

default_src()
{ 
 
 echo -n "${color_yellow}---------------------------------------" && echo "${color_reset}"
 echo -n "${color_yellow} >>> SEZ@Done copy default sources <<< " && echo "${color_reset}"
 echo -n "${color_yellow}---------------------------------------" && echo "${color_reset}"

 
 

 cp -rv ./default-resources/device/rockchip/common/device.mk $SRC_ROOT/device/rockchip/common/
 cp -rv ./default-resources/device/rockchip/common/BoardConfig.mk $SRC_ROOT/device/rockchip/common/

 rm -rfv $DEVICE_ROOT/system.prop

 rm -rfv $DEVICE_ROOT/preinstall*


}

humanization()
{
 echo -n "${color_yellow}--------------------------------------------" && echo "${color_reset}"
 echo -n "${color_yellow} >>> SEZ@Done copy Humanization sources <<< " && echo "${color_reset}"
 echo -n "${color_yellow}--------------------------------------------" && echo "${color_reset}"

 cp -rv ./huma $SRC_ROOT/vendor/
 cp -rv ./partner_gms $SRC_ROOT/vendor/


 cp -rv ./changed-resources/build.sh $SRC_ROOT/
 cp -rv ./changed-resources/kernel/arch/arm64/boot/dts/rockchip/rk3566-firefly-aiojd4-mipi101_JDM101014_BC45_A1.dts $SRC_ROOT/kernel/arch/arm64/boot/dts/rockchip/
 cp -rv ./changed-resources/$DEVICE_ADDR/rk3566_firefly_aiojd4_mipi.mk $DEVICE_ROOT/
 cp -rv ./changed-resources/$DEVICE_ADDR/dt-overlay.in $DEVICE_ROOT/
 cp -arv ./changed-resources/$DEVICE_ADDR/huma_logo $DEVICE_ROOT/
 cp -arv ./changed-resources/$DEVICE_ADDR/overlay $DEVICE_ROOT/
 cp -arv ./changed-resources/device/rockchip/common/overlay $SRC_ROOT/device/rockchip/common
 cp -rv ./changed-resources/$DEVICE_ADDR/BoardConfig.mk $DEVICE_ROOT/


}

# check pass argument
while getopts "DHh" arg
do
    case $arg in
        D)
            default_src
        ;;
        H)
            humanization
        ;;

        h)
            usage 0;;
        ?)
            usage 1;;

esac
done
