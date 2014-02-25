#!/bin/bash

case "$1" in
	sum)
	  awk '{s+=$1} END {print s}'			
	;;
	average)
	  awk '{s+=$1; num+=1} END {print s/num}'			
	;;
	median)
  	  sort -g | awk '{count[NR] = $1;}END {if (NR % 2) {print count[(NR + 1) / 2];} else {print (count[(NR / 2)] + count[(NR / 2) + 1]) / 2.0;}}' 
	;;
	mode)
	  sort -n | uniq -c | sort -r | sed -n '1p' | sed 's/^.......//' 
	;;
	min)
	  sort -g | sed -n '1p'
	;;
	max)
	  sort -g -r | sed -n '1p'
	;;
esac

