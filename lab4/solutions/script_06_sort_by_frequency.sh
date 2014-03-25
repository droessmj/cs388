#!/bin/sh

SORT_FLAGS="-r -n"
VERBOSE=0

usage() {
    cat <<EOF
usage: $(basename $0)

    -r    Sort in ascending order rather than descending.
    -v    Output the frequency along with each item.
EOF
}

while getopts "rv" options; do
    case "${options}" in
    r)	SORT_FLAGS="-n";;
    v)	VERBOSE=1;;
    *)	usage; exit 1;;
    esac
done

shift $((${OPTIND} - 1))

sort_input() {
    awk '
    { names[$1] += 1 }
    END {
	for (name in names) {
	    print names[name], name
	}
    }
    ' | sort ${SORT_FLAGS} < /dev/stdin
}

if [ ${VERBOSE} -eq 0 ]; then
    sort_input | awk '{print $2}' 
else
    sort_input
fi	
