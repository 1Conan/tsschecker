#!/bin/bash
cd /var/shsh
declare -a DIRS=("shsh" "shsh_ota" "shsh_beta_ota")
for dir in "${DIRS[@]}"
do
	mkdir $dir 2>/dev/null
done

rm /tmp/ota.json 2>/dev/null
rm /tmp/firmware.json 2>/dev/null
rm /tmp/bbgcid.json 2>/dev/null

while read device; do
  	if [ -z "$device" ]; then
		continue
	fi
	modelWithBoard=$(echo $device | cut -d ' ' -f 1)
	model=$(echo $modelWithBoard | cut -d '-' -f 1)
	board=$(echo $modelWithBoard | cut -d '-' -f 2)
	ecid=$(echo $device | cut -d ' ' -f 2)

	i=2
	ap=""
	sp=""
	hw=""
	
	if [ "$board" != "$model" ] && [ -n "$board" ]; then
		hw="--boardconfig $board"
	fi
	
	cnt="true";
	while [ "$cnt" == "true" ];
	do
		cnt="false"
		apnonce=$(echo $device | cut -d ':' -f $i)
		apn=""
		sp=""
		if [ "$apnonce" != "$device" ] && [ -n "$apnonce" ]; then
			apn=$apnonce
			sp="/$apnonce"
			ap="--apnonce $apnonce"
			i=$(($i+1))
			mkdir "shsh$sp" 2>/dev/null
			cnt="true"
		fi
		
		echo saving blobs for $modelWithBoard $ecid $apn
	        echo -n "saving normal blob ... "
	        ret=$(tsschecker -d $model -e $ecid -s --save-path "shsh$sp" -l $ap $hw);code=$?
	        echo "$ret" >>/tmp/tsschecker_saveblobs_fullog.log
		echo -n $(echo $ret | grep -o "iOS .* for device" | rev | cut -c 12- | rev )
		if [ $code -eq 0 ]; then echo " ok"; else echo " failed"; echo $ret;fi
	done

	echo -n "saving ota blob ... "
	ret=$(tsschecker -d $model -e $ecid -s --save-path shsh_ota -l -o $hw);code=$?
	echo "$ret" >>/tmp/tsschecker_saveblobs_fullog.log
	echo -n $(echo $ret | grep -o "iOS .* for device" | rev | cut -c 12- | rev )
	if [ $code -eq 0 ]; then echo " ok"; else echo " failed"; echo $ret;fi
	echo -n "saving beta ota blob ... "
	ret=$(tsschecker -d $model -e $ecid -s --save-path shsh_beta_ota -l -o --beta $hw);code=$?
	echo "$ret" >>/tmp/tsschecker_saveblobs_fullog.log
	echo -n $(echo $ret | grep -o "iOS .* for device" | rev | cut -c 12- | rev )
	if [ $code -eq 0 ]; then echo " ok"; else echo " failed"; echo $ret;fi	
done <devices.txt
