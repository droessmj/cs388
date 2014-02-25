#!/bin/bash 

cat > input

while getopts ":cW" opt; do
	case $opt in
	  c)
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

if ! [ "$stripSpecial" = true ]
	then sed -e 's/#.*$//'
fi


if ! [ "$stripEmpty" = false ]
	then sed -e '/^\s*$/d'
fi

cat > output
cat output
rm output
