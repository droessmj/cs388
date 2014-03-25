#!/bin/sh

TARGET='spaces'
WIDTH=4

usage() {
    cat <<EOF
usage: $(basename $0)"

    -i [spaces|tabs]    Specifies the conversion type.  If spaces, then we
			are converting tabs to spaces, otherwise convert
			spaces into tabs.
    -w #                Specifies how many spaces corresponds to one tab.
EOF
}

while getopts "i:w:" option; do
    case "${option}" in
    i)	TARGET=${OPTARG};;
    w)	WIDTH=${OPTARG};;
    *)	usage; exit 1;;
    esac
done

shift $((${OPTIND} - 1))

spaces=""
for i in $(seq $WIDTH); do
    spaces=${spaces}" "
done

if [ "${TARGET}" = "spaces" ]; then
    sed 's|\t|'"${spaces}"'|g'
else
    sed 's|'"${spaces}"'|\t|g'
fi
