#!/bin/bash

for arg in $*
  do
	curl -s "$arg/.json" | python -m json.tool | grep '"title":\|"url":' > output
	sed 's/"title"/\n&/g' output | sed 's/"url": "//' | sed 's/",//' | awk '$1 = /"title":/ ? ++increment : $1"\n"'
done

rm output
