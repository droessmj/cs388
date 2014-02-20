#!/bin/bash

cat > input

while getopts ":rv" opt; do
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




if [ "$sortReverse" = true ] && [ "$frequency" = true ]
	then 
	  sort -r input | uniq -c | awk '{print $2"\t"$1}' > sorted
	else 
		if [ "$sortReverse" = true ]
			then sort -u -r input > sorted
			else
				if [ "$frequency" = true ]
					then sort input | uniq -c | awk '{print $2"\t"$1}' > sorted
					else sort -u input > sorted
				fi
		fi
fi

cat sorted 
rm sorted
rm input
