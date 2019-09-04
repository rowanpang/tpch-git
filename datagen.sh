#!/bin/bash

basedir=`pwd`
bindir=$basedir/tpch_gen

function timer(){
    if [[ $# -eq 0 ]]; then
    	echo $(date '+%s')
    else
    	local  stime=$1
    	etime=$(date '+%s')

    	if [[ -z "$stime" ]]; then stime=$etime; fi

        dt=$((etime - stime))
        ds=$((dt % 60))
        dm=$(((dt / 60) % 60))
        dh=$((dt / 3600))
        printf '%d:%02d:%02d' $dh $dm $ds
    fi
}

function dataGen(){
    factor=$1
    [ -z $factor ] && factor=1

    ddir=$basedir/data/${factor}g/
    [ -d $ddir ] && rm -rf $ddir
    mkdir -p $ddir

    cd $ddir
    echo "`date +%Y%m%d-%H:%M:%S`,gendata for factor:$factor------"
    t=$(timer)
    $bindir/dbgen -s $factor -f -b $bindir/dists.dss 2>/dev/null
    for f in `ls *.tbl 2>/dev/null`;do
	echo "trunk last SP for $f"
	sed -e 's/|$//' $f > ${f/tbl/csv};
    done
    cd $basedir
    echo "`date +%Y%m%d-%H:%M:%S`,done,spend $(timer $t)-----"
    echo
}

dataGen $@
