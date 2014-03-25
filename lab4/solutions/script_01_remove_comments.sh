#!/bin/sh

DELIMITER='#'
EMPTY_LINES=0

usage() {
    echo "usage: $(basename $0)"
    echo "   -c  Use this as the comment delimiter."
    echo "   -W  Don't strip empty lines."
}

while getopts "c:W" option; do
    case "${option}" in
    c)	DELIMITER=${OPTARG};;
    W)	EMPTY_LINES=1;;
    *)	usage; exit 1;;
    esac
done

shift $((${OPTIND} - 1))

sed -e "s|${DELIMITER}.*$||" | {
    if [ ${EMPTY_LINES} -eq 1 ]; then
    	cat
    else
    	sed '/^\s*$/d'
    fi
}
