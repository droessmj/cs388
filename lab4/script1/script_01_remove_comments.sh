#!/bin/bash 

cat > input

while getopts ":cW" opt; do
	case $opt in
	  c)
		if [ -f tmp.file ]
			then sed '/regexSpecified/' tmp.file >  tmp.txt 
			else sed '/regexSpecified/' > tmp.txt
	   	fi
	   ;;
	  W)
		if [ -f tmp.txt ]
			then cat tmp > tmp.file
			else cat input > tmp.file
	   	fi
	   ;;
	  \?)
	 	echo "Invalid option: -$opt"
	   ;;
	esac
done



if ! grep -e "-c" $* 
	then sed '/^#/d' input > tmp.txt
	else echo "-c option entered"
fi

if ! grep -e "-W" $*
   then
	if  [ -f tmp.txt ]
		then  sed '/^\s*$/d' tmp.txt > tmp.file
		else  sed '/^\s*$/d' input > tmp.file
	fi
   else echo "-W option entered"
fi

cat tmp.file
rm tmp.file
rm tmp.txt
