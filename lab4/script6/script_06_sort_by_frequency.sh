#!/bin/bash

cat > input

while getopts ":rv:" opt; do
	case $opt in
		r)
		  	sortReverse=true
 	 	  ;;
		v)
			frequency=true
		  ;;
		\?)
			echo "Invalid option: -$opt"
		  ;;
	esac
done

if [ "$sortReverse" = true && "frequency" = true ]
	then 
	   sort -r input > sorted
	else 
		if [ "$sortReverse" = true ]
			then sort -r input > sorted
			else sort input > sorted
		fi
fi



cat sorted 
rm sorted

