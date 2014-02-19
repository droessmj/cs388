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

hash="#"
pounds=""
for i in `seq 1 $number`
	do 
	  pounds=$pounds$hash
done

echo $pounds

if [ "$conversion" == "spaces" ]
	then sed "s/\t/$pounds/g" input > output
	else sed "s/\$pounds/t/g" input > output
fi

cat output
rm output
