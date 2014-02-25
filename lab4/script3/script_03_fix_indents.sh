#!/bin/bash

cat > input

while getopts ":i:w:" opt; do
	case $opt in
		i)
			conversion=$OPTARG
			if [ "$conversion" != "spaces" ] && [ "$conversion" != "tabs" ]
				then echo "Error: please enter spaces or tabs" >&2; exit 1
			fi
		  ;;

		w)
			number=$OPTARG
			re='^[0-9]+$'
			if ! [[ $number =~ $re ]]
				then echo "Error: Not a number" >&2; exit 1
			fi
		  ;;
		\?)
			echo "Invalid option: -$opt"
		  ;;
	esac
done

space=" "
spaces=""
for i in `seq 1 $number`
	do 
	  spaces=$spaces$space
done

if [ "$conversion" == "spaces" ]
	then sed "s/\t/$spaces/g" input > output
	else sed "s/$spaces/\t/g" input > output
fi

cat output
rm output
rm input
