#!/bin/sh

curl -s $1/.json | python -m json.tool | awk '
    BEGIN	{
	FS    = ": "
	count = 0
    }

    /"title"/	{ 
	gsub(/^"/, "", $2)
	gsub(/",/, "", $2)
	Titles[count] = $2
    }
    
    /"url"/	{ 
	gsub(/^"/, "", $2)
	gsub(/",/, "", $2)
	URLs[count] = $2
	count++
    }

    END		{
	printf "Reddit Links:\n", title_count
	for (i = 0; i < count; i++) {
	    printf "%2d. %s\n", i + 1, Titles[i]
	    printf "    %s\n\n", URLs[i]
	}
    }
'
