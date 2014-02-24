#!/bin/bash

touch hash_file

if [[ -d $1 ]]
 then
	for child in $1/*
	do
 	  	if [[ -d $child ]] 
			then echo $child is directory
	  	fi
		
		hash=$(md5sum $child)
		
		if grep $hash hash_file
		  then echo $child
		  else echo $hash >> hash_file
		fi	
	done  
 else
	echo Passed in argument is not a directory
fi
rm hash_file
