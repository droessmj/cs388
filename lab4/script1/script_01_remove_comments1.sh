#!/bin/bash 

while getopts ":cW" opt; do
	case $opt in
	  c)
		echo special strip selected
		stripSpecial = true
		sed -e "/$OPTARG//*$"  
	   ;;
	  W)
		stripEmpty=false
	   ;;
	  \?)
	 	echo "Invalid option: -$opt"
	   ;;
	esac
done

if [ "$stripSpecial" = false ]
	then echo
	else sed -e 's/#.*$//'
fi


if [ "$stripEmpty" = false ]
	then echo 
	else sed -e '/^\s*$/d'
fi

cat > output
cat output
rm output
