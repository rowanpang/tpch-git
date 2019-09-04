#!/bin/bash

export PG_HOME="/home/inspur/megawise/zdb/v1.2/"
export LD_LIBRARY_PATH=/usr/local/bin/gcc-6.3/lib64/
if [ ! -n $LD_LIBRARY_PATH ]; then
    export LD_LIBRARY_PATH=$PG_HOME/lib:/usr/local/lib
else
    export LD_LIBRARY_PATH=$PG_HOME/lib:$LD_LIBRARY_PATH:/usr/local/lib
fi

#$PG_HOME/bin/psql -h /tmp -f ${PG_HOME}/script/sql/tpchCreateLoader.sql test

basedir=`pwd`
x22sqldir="$basedir/tpch_22sql"
dbNamePrefix="tpch-ZDB"
tpchOUT="$basedir/tpchOut"

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
	justQuery $factor
    fi
}

function doBoth(){
    factor=$1
    [ -z $factor ] && factor=1
    dataInit $factor
    22query $factor
}

function justQuery(){
    factor=$1
    [ -z $factor ] && factor=1
    22query $factor
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

    sqlTbl=`sed "s#data/tpch_1G/##" $tblSql | sed "s#@path@#${ddir}#"`
    echo "$sqlTbl"
    echo "$sqlTbl" | $PG_HOME/bin/psql -f - $dbName
}

function 22query(){
    factor=$1
    if [ -z $factor ];then
	echo "dataInit error,no factor!"
	exit -1
    fi
    dbName="${dbNamePrefix}-${factor}G"
    echo "`date +%Y%m%d-%H:%M:%S`,do 22query for tpch database $dbName"
    outdirbase="$tpchOUT/${factor}g"

    sqldir="$x22sqldir"
    for f in `ls $sqldir/query*.sql`;do
	bName=`basename $f`
	bName=${bName%%.*}
	outdir="$outdirbase/$bName"

	echo "`date +%Y%m%d-%H:%M:%S`,do $bName for $dbName"

	[ -d $outdir ] && rm -rf $outdir
	mkdir -p $outdir

	sql="\timing on
`cat $f`
"
	for i in {1..10};do
	    of=$outdir/$i.txt
	    ofs=$outdir/$i.txt.sort
	    escape=`echo "$sql" | $PG_HOME/bin/psql -f - -o $of -d $dbName 2>/dev/null | awk '/Time: / {print $2}'`
	    let "Total=0"
	    if [ $bName == "query15" ];then
		for t in `echo $escape`;do
		    Total=`echo "$Total + $t" | bc`
		done

		escape=$Total
	    fi
	    if [ -z "$escape" ];then
		echo "break abnormal"
		break;
	    fi

	    echo "`printf "%02d" $i`,spend $escape ms"
	    sort $of -o $ofs

	    if [ $bName == "query22" ];then
		sleep 3
	    fi
	done
    done
}

autodo $@
#doBoth $@
