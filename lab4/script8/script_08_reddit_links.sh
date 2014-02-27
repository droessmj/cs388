#!/bin/bash

for arg in $*
  do
	#get the json for the input site
	curl -s "$arg/.json" | python -m json.tool | grep '"title":\|"url":' > output
	#modify the output to the correct format
	sed 's/"title"/\n&/g' output | sed 's/"url": "//' | sed 's/",//' | awk '$1 = /"title":/ ? ++increment"." : $1"\n"'
done

rm output
