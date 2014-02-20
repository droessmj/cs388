#!/bin/bash

while getopts "f:" opt; do
	case $opt in
		f)
			fields=$OPTARG
			shift $(( ${OPTIND} - 1 ));
		  
 	 	  ;;
		\?)
			echo "Invalid option: -$opt"
		  ;;
	esac
done


for location in $*
   do
	curl -s http://w1.weather.gov/xml/current_obs/$location.xml >> weatherOutput
	echo $location
	IFS=',' read -a fieldArray <<< "$fields"
	for field in "${fieldArray[@]}"
	  do
		grep -F "<$field>" weatherOutput >> output
	done
	cat output
	rm weatherOutput
	rm output
done

