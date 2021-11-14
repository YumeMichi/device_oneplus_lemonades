#! /vendor/bin/sh

export PATH=/vendor/bin

# Make sure persist directory for HVDCP is gone.
rm -rf /mnt/vendor/persist/hvdcp_opti

prefix="/sys/class/"
#List of folder for ownership update.
arr=( "power_supply/battery/" "power_supply/usb/" "power_supply/main/" "power_supply/pc_port/" "power_supply/dc/" "power_supply/bms/" "usbpd/usbpd0/" "qcom-battery/" )
for i in "${arr[@]}"
do
    for j in `ls "$prefix""$i"`
    do
        # Skip directories to prevent possible security issues.
        if [[ -d "$prefix""$i""$j" ]]
        then
            continue
        else
            chown -h system.system "$prefix""$i""$j"
        fi
    done
done
