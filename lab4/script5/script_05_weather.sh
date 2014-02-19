#!/bin/bash

echo $0
echo $1
echo $2
echo $3
echo $4
echo $5

while getopts "f:" opt; do
	case $opt in
		f)
			fields=$OPTARG
			#if there is an f flag, airport codes are all the args after
			args=$3
			nextArg=$3
			num=3
#			while [ "$nextArg" != "" ]
#				do
#				   if [ "$nextArg" != "$3" ]
#					then $args=$args$nextArg
#				   fi
#				   $num+=1
#				   nextArg="$"$num""
#			done
		  ;;
		\?)
			echo "Invalid option: -$opt"
		  ;;
	esac
done

if ! grep -e "-f" $* 
	then echo "no -f flag"
fi

#for location in $locations
#   do
#	curl -s http://w1.weather.gov/xml/current_obs/$location.xml >> output
#done

#parse output

#cat output
#rm output
