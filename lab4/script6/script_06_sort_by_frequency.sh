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
	  #sort both on reverse and display frequency
	  sort -r input | uniq -c | sort  | awk '{print $2"\t"$1}'
	else 
		if [ "$sortReverse" = true ]
			#sort on reverse but do not display frequency
			then sort  input | uniq -c | sort | awk '{print $2}'
			else
				if [ "$frequency" = true ]
				        #show frequency but not reverse order
					then sort input | uniq -c | sort -r | awk '{print $2"\t"$1}'
					#do not show frequency and do not reverse
					else sort input | uniq -c | sort -r | awk '{print $2}'
				fi
		fi
fi

rm input
