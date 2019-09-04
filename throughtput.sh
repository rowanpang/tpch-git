#!/bin/bash
export PG_HOME=/home/pangwz/megawise.q4.2/megawise/
export LD_LIBRARY_PATH=/usr/local/gcc-6.3.0/lib64/
if [ ! -n $LD_LIBRARY_PATH ]; then
	export LD_LIBRARY_PATH=$PG_HOME/lib:/usr/local/lib
else
	export LD_LIBRARY_PATH=$PG_HOME/lib:$LD_LIBRARY_PATH:/usr/local/lib
fi

basedir=`pwd`
x22sqldir="$basedir/tpch"
dbNamePrefix="tpch-ZDB"
tpchOUT="$basedir/tpchOut"

sPower="
0 14 2 9 20 6 17 18 8 21 13 3 22 16 4 11 15 1 10 19 5 7 12
"

throughputSNUM=5
sThroughput="
1 21 3 18 5 11 7 6 20 17 12 16 15 13 10 2 8 14 19 9 22 1 4
2 6 17 14 16 19 10 9 2 15 8 5 22 12 7 13 18 1 4 20 3 11 21
3 8 5 4 6 17 7 1 18 22 14 9 10 15 11 20 2 21 19 13 16 12 3
4 5 21 14 19 15 17 12 6 4 9 8 16 11 2 10 18 1 13 7 22 3 20
5 21 15 4 6 7 16 19 18 14 22 11 13 3 1 2 5 8 20 12 17 10 9
6 10 3 15 13 6 8 9 7 4 11 22 18 12 1 5 16 2 14 19 20 17 21
7 18 8 20 21 2 4 22 17 1 11 9 19 3 13 5 7 10 16 6 14 15 12
8 19 1 15 17 5 8 9 12 14 7 4 3 20 16 6 22 10 13 2 21 18 11
9 8 13 2 20 17 3 6 21 18 11 19 10 15 4 22 1 7 12 9 14 5 16
10 6 15 18 17 12 1 7 2 22 13 21 10 14 9 3 16 20 19 11 4 8 5
11 15 14 18 17 10 20 16 11 1 8 4 22 5 12 3 9 21 2 13 6 19 7
12 1 7 16 17 18 22 12 6 8 9 11 4 2 5 20 21 13 10 19 3 14 15
13 21 17 7 3 1 10 12 22 9 16 6 11 2 4 5 14 8 20 13 18 15 19
14 2 9 5 4 18 1 20 15 16 17 7 21 13 14 19 8 22 11 10 3 12 6
15 16 9 17 8 14 11 10 12 6 21 7 3 15 5 22 20 1 13 19 2 4 18
16 1 3 6 5 2 16 14 22 17 20 4 9 10 11 15 8 12 19 18 13 7 21
17 3 16 5 11 21 9 2 15 10 18 17 7 8 19 14 13 1 4 22 20 6 12
18 14 4 13 5 21 11 8 6 3 17 2 20 1 19 10 9 12 18 15 7 22 16
19 4 12 22 14 5 15 16 2 8 10 17 9 21 7 3 6 13 18 11 20 19 1
20 16 15 14 13 4 22 18 19 7 1 12 17 5 10 20 3 9 21 11 2 6 8
21 20 14 21 12 15 17 4 19 13 10 11 1 16 5 18 7 8 22 9 6 3 2
22 16 14 13 2 21 10 11 4 1 22 18 12 19 5 7 8 6 3 15 20 9 17
23 18 15 9 14 12 2 8 11 22 21 16 1 6 17 5 10 19 4 20 13 3 7
24 7 3 10 14 13 21 18 6 20 4 9 8 22 15 2 1 5 12 19 17 11 16
25 18 1 13 7 16 10 14 2 19 5 21 11 22 15 8 17 20 3 4 12 6 9
26 13 2 22 5 11 21 20 14 7 10 4 9 19 18 6 3 1 8 15 12 17 16
27 14 17 21 8 2 9 6 4 5 13 22 7 15 3 1 18 16 11 10 12 20 19
28 10 22 1 12 13 18 21 20 2 14 16 7 15 3 4 17 5 19 6 8 9 11
29 10 8 9 18 12 6 1 5 20 11 17 22 16 3 13 2 15 21 14 19 7 4
30 7 17 22 5 3 10 13 18 9 1 14 15 21 19 16 12 8 6 11 20 4 2
31 2 9 21 3 4 7 1 11 16 5 20 19 18 8 17 13 10 12 15 6 14 22
32 15 12 8 4 22 13 16 17 18 3 7 5 6 1 9 11 21 10 14 20 19 2
33 15 16 2 11 17 7 5 14 20 4 21 3 10 9 12 8 13 6 18 19 22 1
34 1 13 11 3 4 21 6 14 15 22 18 9 7 5 10 20 12 16 17 8 19 2
35 14 17 22 20 8 16 5 10 1 13 2 21 12 9 4 18 3 7 6 19 15 11
36 9 17 7 4 5 13 21 18 11 3 22 1 6 16 20 14 15 10 8 2 12 19
37 13 14 5 22 19 11 9 6 18 15 8 10 7 4 17 16 3 1 12 2 21 20
38 20 5 4 14 11 1 6 16 8 22 7 3 2 12 21 19 17 13 10 15 18 9
39 3 7 14 15 6 5 21 20 18 10 4 16 19 1 13 9 8 17 11 12 22 2
40 13 15 17 1 22 11 3 4 7 20 14 21 9 8 2 18 16 6 10 12 5 19
"
function querying(){
    sqlis="$1"
    dbName=$2

    for qi in $sqlis;do
	qii=`printf "%02d" $qi`
	sqlf=$x22sqldir/query${qii}.sql
	bName=`basename $sqlf`
	bName=${bName%%.*}
	outdir="$outdirBase/$bName"

	[ -d $outdir ] && rm -rf $outdir
	mkdir -p $outdir

	sql="\timing on
`cat $sqlf`
"
	of="$outdir/${sdoing}.txt"
	ofs="$outdir/${sdoing}.txt.sort"

	escape=`echo "$sql" | $PG_HOME/bin/psql -f - -o $of -d $dbName 2>/dev/null \
		| awk '/Time: /{print $2}'`

	if [ -z "$escape" ];then
	    echo "spend abnormal"
	    break;
	fi

	let "Total=0"
	if [ $qii == "15" ];then
	    for t in `echo $escape`;do
		Total=`echo "$Total + $t" | bc`
	    done
	    escape=$Total
	fi

	echo "$qii,spend $escape ms"
	sort $of -o $ofs

	spends[$qi]=$escape
    done

    echo ${spends[@]}
}

function doStream(){
    cycle=$1
    sIdx=$2
    factor=$3
    streamsName=$4
    eval streams=\$$streamsName
    if [ -z "$sIdx" ] || [ -z "$cycle" ];then
	echo "doStream error! please specify stream cycle and idx"
	exit -1
    fi

    if [ -z $factor ];then
	echo "dataInit error,no factor!"
	exit -1
    fi

    sNum=`echo "$streams" | wc -l`
    let "sNum -= 2"
    #echo "prdefined $streamsName num: "$sNum

    dbName="${dbNamePrefix}-${factor}G"
    outdirBase="${tpchOUT}/${factor}g"

    let "lineNum = sIdx+1"
    stream=`echo "$streams" | sed -n "${lineNum}p"`
    sqlis=`echo "$stream" | cut -d " " -f 2-`

    let "sdoing = cycle*$sNum + sIdx"
    echo "`date +%Y%m%d-%H:%M:%S` do $streamsName test for db $dbName,cur stream $sdoing"
    echo "idx $sIdx, $sqlis"

    querying "$sqlis" "$dbName"
}

function doPowerTest() {
    sf=$1

    sNum=`echo "$sPower" | wc -l`
    let "sNum -= 2"

    echo "-----current $FUNCNAME,prdefined sqlNum:$sNum ------"
    doStream 0 1 $sf ${!sPower@}
}

function doThroughputTest() {
    sf=$1
    sNeed=$2

    sNum=`echo "$sThroughput" | wc -l`
    let "sNum -= 2"
    echo "-----current $FUNCNAME,prdefined sqlNum:$sNum ------"
    echo "total streams $sNeed"

    doing=0
    while [ true ];do
	let "cycle = $doing/sNum"
	let "idx = $doing%sNum+1"
    	doStream $cycle $idx $sf ${!sThroughput@}

	let "doing += 1"
	if [ $doing -ge $sNeed ];then
	    break;
	fi
	echo
    done
}

function dataInit(){
    factor=$1
    if [ -z $factor ];then
	echo "dataInit error,no factor!"
	exit -1
    fi
    dbName="${dbNamePrefix}-${factor}G"
    ddir="$basedir/data/${factor}g"
    tblSql="$x22sqldir/create_tpch_table.sql"

    echo "`date +%Y%m%d-%H:%M:%S`,do init for tpch database $dbName"
    echo "------dpath:$ddir-------"
    echo "--------sql:$tblSql------"
    if ! [ -d $ddir ];then
	echo "no data to load ,exit"
	exit
    fi

    $PG_HOME/bin/dropdb --if-exists $dbName
    $PG_HOME/bin/createdb $dbName

    $PG_HOME/bin/psql -c "CREATE EXTENSION zdb_fdw;" $dbName
    $PG_HOME/bin/psql -c "CREATE SERVER zdb_server FOREIGN DATA WRAPPER  zdb_fdw;" $dbName

    sed "s#@path@#${ddir}#" $tblSql | $PG_HOME/bin/psql -f - $dbName
}

function doQuery(){
    factor=$1
    [ -z $factor ] && factor=1
    doPowerTest 1
    doThroughputTest $factor $throughputSNUM
}

function doBoth(){
    factor=$1
    [ -z $factor ] && factor=1
    dataInit $factor
    doQuery $factor
}

function autodo(){
    factor=$1
    [ -z $factor ] && factor=1
    dbName="${dbNamePrefix}-${factor}G"
    echo -n "$dbName,"

    if [ `$PG_HOME/bin/psql -c "\l" postgres 2>&1 | grep -c "$dbName"` -lt 1 ];then
	echo "-----doBoth----"
	doBoth $factor
    else
	echo "----jusetQuery----"
	doQuery $factor
    fi
}

#autodo 1
doQuery 1
