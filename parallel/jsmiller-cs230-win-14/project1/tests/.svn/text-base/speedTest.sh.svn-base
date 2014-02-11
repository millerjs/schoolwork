#!/bin/bash

d=`date`
outfile="results/output_from_"${d// /_}".dat"
binary="$1"

echo "" > $outfile

chunks(){

    for c in $chunks
    do 
        echo ../floyd -r $s -t $t -p -n $c
        ../floyd -r $s -t $t -p -n $c >> $outfile
    done

}

threads(){
    s=$1
    for t in $threads
    do
        chunks $s $t 
    done
}

run(){

    for s in $sizes
    do 
        echo ../floyd -r $s
        ../floyd -r $s -s >> $outfile
    done

    for s in $sizes
    do 
        threads $s
    done
}

chunks="1 2 4 8 16 32 64 128 512"
sizes="16 32 64 128 256 512 1024"
# threads="1 2 4 8 16 32 64"
threads="1 3 7 15 31 63"

# run all tests
run

