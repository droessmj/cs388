#!/bin/sh

FIELDS="weather|temp_f|windchill_f"

fetch_weather() {
    station=$1
    echo ${station}
    curl -s "http://w1.weather.gov/xml/current_obs/${station}.xml" |
	sed -nr '/<('"${FIELDS}"')>/s/.*>(.*)<.*/\1/p'
}

while getopts "f:" option; do
    case "${option}" in
	f)	FIELDS=$(echo ${OPTARG} | sed 's/,/|/g');;
    esac
done

shift $((${OPTIND} - 1))

for station in $@; do
    fetch_weather ${station}
done
