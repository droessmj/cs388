#!/bin/bash

for arg in $*
  do
	if [ "$arg" = "sum" ]
		then
			awk '{s+=$1} END {print s}'			
	fi
	
	if [ "$arg" = "average" ]
		then
			awk '{s+=$1; num+=1} END {print s/num}'			
	fi
	
	if [ "$arg" = "median" ]
		then
			sort -g | awk '{count[NR] = $1;}END {if (NR % 2) {print count[(NR + 1) / 2];} else {print (count[(NR / 2)] + count[(NR / 2) + 1]) / 2.0;}}' 
	fi

	if [ "$arg" = "mode" ]
		then
			sort -k2 -n 
	fi

	if [ "$arg" = "min" ]
		then
			sort -g | sed -n '1p'
	fi

	if [ "$arg" = "max" ]
		then
			sort -g -r | sed -n '1p'
	fi

done


