#!/vendor/bin/sh
BT_MAC_VENDOR_PREFIX="ac:d6:18:"
BDADDR_PATH="/data/vendor/bluetooth/bdaddr"

if [[ ! -f "${BDADDR_PATH}" ]] || [[ ! -s "${BDADDR_PATH}" ]]; then
    SERIALNO=`printf '%d' $((16#$1))`

    MAC_SUFFIX=`awk -v seed=${SERIALNO} -v min=0 -v max=255 'BEGIN{srand(seed); printf("%02x:%02x:%02x", int(min+rand()*(max-min+1)), int(min+rand()*(max-min+1)), int(min+rand()*(max-min+1)))}' | tr '[:upper:]' '[:lower:]'`
    MAC="${BT_MAC_VENDOR_PREFIX}${MAC_SUFFIX}"

    echo "${MAC}" > "${BDADDR_PATH}"

    setprop persist.vendor.service.bdroid.bdaddr "${MAC}"
fi
