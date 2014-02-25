#!/bin/bash 

while getopts ":cW" opt; do
	case $opt in
	  c)
		stripSpecial=true
		sed -e "s/\$2/#/" > modText  
	   ;;
	  W)
		stripEmpty=false
	   ;;
	  \?)
	 	echo "Invalid option: -$opt"
	   ;;
	esac
done

if [ "$stripSpecial" = true ]
	then echo
	else sed -e '/^#/d' > modText
fi


if [ "$stripEmpty" = false ]
	then cat modText > output
	else sed -e '/^\s*$/d' modText > output
fi

cat output
rm modText
rm output
