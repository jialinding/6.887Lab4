#!/usr/bin/env bash

function get_checksum() {
   FILE=$1

   if [ -x "$(command -v md5sum)" ]; then
      # Linux
      MD5_RESULT=`md5sum ${FILE} | awk '{ print $1 }'`
   else
      # OS X
      MD5_RESULT=`md5 -q ${FILE}`
   fi
}


function download_file() {
   FILE=$1;
   CHECKSUM=$2;
   URL=$3;

   # Check if file already exists
   if [ -f ${FILE} ]; then
       echo "$FILE already exists, skipping download."
   else
      # Does not exists -> download
       wget -O - ${URL} > ${FILE}
       get_checksum ${FILE}
       if [ "${MD5_RESULT}" != "${CHECKSUM}" ]; then
           echo "error checksum does not match: run download again"
           exit -1
       else
           echo ${FILE} "checksum ok"
       fi
   fi
}

download_file keys.bin 415c17a62d6e8f5238d4cf8f5854aae7 https://www.dropbox.com/s/stfjg0lw1lg7qrb/longitudes-50M-shuffled.data?dl=1