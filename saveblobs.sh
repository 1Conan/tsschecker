#!/bin/bash

declare -a DIRS=("shsh" "shsh_ota" "shsh_beta_ota")
for dir in "${DIRS[@]}"
do
	mkdir $dir 2>/dev/null
done

rm /tmp/ota.json
rm /tmp/firmware.json
rm /tmp/bbgcid.json

while read device; do
  	if [ -z "$device" ]; then
		continue
	fi
	model=$(echo $device | cut -d ' ' -f 1)
	ecid=$(echo $device | cut -d ' ' -f 2)
	echo saving blobs for $model $ecid
	echo -n "saving normal blob ... "
	ret=$(tsschecker -d $model -e $ecid -s --save-path shsh -l);code=$?
	echo -n $(echo $ret | grep -o "iOS .* for device" | rev | cut -c 12- | rev )
	if [ $code -eq 1 ]; then echo " ok"; else echo " failed"; echo $ret;fi
	echo -n "saving ota blob ... "
	ret=$(tsschecker -d $model -e $ecid -s --save-path shsh_ota -l -o);code=$?
	echo -n $(echo $ret | grep -o "iOS .* for device" | rev | cut -c 12- | rev )
	if [ $code -eq 1 ]; then echo " ok"; else echo " failed"; echo $ret;fi
	echo -n "saving beta ota blob ... "
	ret=$(tsschecker -d $model -e $ecid -s --save-path shsh_beta_ota -l -o --beta);code=$?
	echo -n $(echo $ret | grep -o "iOS .* for device" | rev | cut -c 12- | rev )
	if [ $code -eq 1 ]; then echo " ok"; else echo " failed"; echo $ret;fi	
done <devices.txt



DATE=$(date +"%Y-%m-%d")
echo "backing up shsh files"
for dir in "${DIRS[@]}"
do
	MOVED="no"
	for i in $dir/*.shsh;
	do

		DST=$(echo $i | rev | cut -c 6- | rev)
		if ! [[ "$DST" =~ "--" ]]; then
			DST="$DST--$DATE.shsh"
			echo moving $DST
			mv $i $DST
			MOVED="yes"
		fi
	done

	if [ $MOVED == "no" ]; then
		echo "nothing to move in "$dir
	fi

done
