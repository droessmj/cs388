#!/bin/sh

if [ $# -ne 1 ]; then
    echo "usage: $(basename $0) folder-to-search"
    exit 1
fi

find $1 -type f -exec md5sum \{\} \; | 
    sort | 
    awk '
    BEGIN	{ 
		    prev_cksum = ""
		    prev_fname = ""
		}
		{ 
		    if (prev_cksum == $1) {
		    	if ($1 in duplicates) {
			    duplicates[$1] = duplicates[$1] " " $2
			} else {
			    duplicates[$1] = prev_fname " " $2
			}
		    }
		    prev_cksum = $1
		    prev_fname = $2
		}
    END		{
		    for (duplicate in duplicates) {
		    	print duplicates[duplicate]
		    }
		}
'
