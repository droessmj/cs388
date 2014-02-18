#!/bin/bash 

input=/dev/stdin
while getopts ":cW" opt; do
	case $opt in
	  c)
		sed '/regexSpecified/' input > tmp
	   ;;
	  W)
		if [-f tmp]
			then tmp > tmp.file
			else input > tmp.file
	   	fi
	   ;;
	  \?)
	 	echo "Invalid option: -$opt"
	   ;;
	esac
done

if ! -c
	then sed '/^#/d' input > tmp
fi

if ! -W
	if  [-f tmp]
		then  sed '/^\s*$/d' tmp > tmp.file
		else  sed '/^\s*$/d' input > tmp.file
	fi
fi

cat tmp.file
rm tmp.file
rm tmp
