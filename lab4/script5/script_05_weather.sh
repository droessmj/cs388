#!/bin/bash

while getopts ":f:" opt; do
	case $opt in
		f)
			fields=$OPTARG
			echo $fields
		  ;;
		\?)
			echo "Invalid option: -$opt"
		  ;;
	esac
done

#for location in $locations
#   do
#	curl -s http://w1.weather.gov/xml/current_obs/$location.xml >> output
#done

#parse output

#cat output
#rm output
