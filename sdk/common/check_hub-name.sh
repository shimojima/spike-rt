#!/bin/bash
if [ "$#" -ne 2 ]; then
  exit
fi

if [ -e ${1} ]; then
  name=`cat ${1}`
  name=${name:0:15}
  for i in $(seq ${#name} 15); do name+="\0"; done
  echo -en $name > ${2:-HUB_NAME}
fi
