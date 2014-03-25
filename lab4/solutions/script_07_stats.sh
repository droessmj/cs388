#!/bin/sh

sum() {
    awk '
	    BEGIN   { sum = 0	}
		    { sum += $0 }
	    END	    { print sum	}

    ' < /dev/stdin
}

average() {
    awk '
	    BEGIN   { sum  = 0
		      line = 0
		    }
		    { sum += $0 
		      line += 1
		    }
	    END	    { print sum/line }	
    ' < /dev/stdin
}

median() {
    sort < /dev/stdin | awk '
	    function ceil(n) { return (n == int(n)) ? n : int(n) + 1 }
		    { line[NR] = $0; }
	    END	    { print line[ceil(NR/2)] }
    '
}

mode() {
    awk '
		    { numbers[$1] += 1 
		    }
	    END	    { 
		mode = 0
		for (number in numbers) {
		    if (numbers[number] > mode) {
		    	mode = number
		    }
		}
		print mode
	    }	
    ' < /dev/stdin
}

min() {
    awk '
	    BEGIN   { min = 10000000 }
		    { if ($1 < min) { min = $1 } }
	    END	    { 
		print min
	    }	
    ' < /dev/stdin

}

max() {
    awk '
	    BEGIN   { max = -10000000 }
		    { if ($1 > max) { max = $1 } }
	    END	    { 
		print max
	    }	
    ' < /dev/stdin
}

case $1 in 
    sum)	sum	;;
    average)    average	;;
    median)	median  ;;
    mode)	mode    ;;
    min)	min	;;
    max)	max	;;
    *)		echo "usage: $(basename $0) [sum | average | median | mode | min | max]"
esac
