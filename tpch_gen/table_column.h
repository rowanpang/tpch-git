#ifndef _TABLE_COLUMN_H_
#define  _TABLE_COLUMN_H_

/*
*	cust_table_column
*/

#define C_CUSTKEY   "C_CUSTKEY|int|data|primary_key"
#define C_NAME   "C_NAME|string|data"
#define C_ADDRESS   "C_ADDRESS|string|data"
#define C_NATIONKEY   "CC_NATIONKEY|int|data"
#define C_PHONE   "C_PHONE|string|data|char16"
#define C_ACCTBAL   "C_ACCTBAL|double|data"
#define C_MKTSEGMENT   "C_MKTSEGMENT|string|data|char16"
#define C_COMMENT   "C_COMMENT|string|data"


/*
*	order_table_column
*/

#define O_ORDERKEY   "O_ORDERKEY|int|data|primary_key"
#define O_CUSTKEY   "O_CUSTKEY|int|data"
#define O_ORDERSTATUS   "O_ORDERSTATUS|string|data|char1"
#define O_TOTALPRICE   "O_TOTALPRICE|double|data"
#define O_ORDERDATE   "O_ORDERDATE|string|data|date"
#define O_ORDERPRIORITY   "O_ORDERPRIORITY|string|data|char16"
#define O_CLERK   "O_CLERK|string|data|char16"
#define O_SHIPPRIORITY   "O_SHIPPRIORITY|int|data"
#define O_COMMENT   "O_COMMENT|string|data"


/*
*	line_table_column
*/

#define L_ORDERKEY   "L_ORDERKEY|int|data"
#define L_PARTKEY    "L_PARTKEY|int|data"
#define L_SUPPKEY   "L_SUPPKEY|int|data"
#define L_LINENUMBER   "L_LINENUMBER|int|data"
#define L_QUANTITY   "L_QUANTITY|double|data"
#define L_EXTENDEDPRICE   "L_EXTENDEDPRICE|double|data"
#define L_DISCOUNT   "L_DISCOUNT|double|data"
#define L_TAX   "L_TAX|double|data"
#define L_RETURNFLAG   "L_RETURNFLAG|string|data|char1"
#define L_LINESTATUS   "L_LINESTATUS|string|data|char1"
#define L_SHIPDATE   "L_SHIPDATE|string|data|date"
#define L_COMMITDATE   "L_COMMITDATE|string|data|date"
#define L_RECEIPTDATE   "L_RECEIPTDATE|string|data|date"
#define L_SHIPINSTRUCT   "L_SHIPINSTRUCT|string|data|char32"
#define L_SHIPMODE   "L_SHIPMODE|string|data|char16"
#define L_COMMENT   "L_COMMENT|string|data"


/*
*	part_table_column
*/

#define P_PARTKEY   "P_PARTKEY|int|data|primary_key"
#define P_NAME   "P_NAME|string|data"
#define P_MFGR   "P_MFGR|string|data|char32"
#define P_BRAND   "P_BRAND|string|data|char16"
#define P_TYPE   "P_TYPE|string|data"
#define P_SIZE_S   "P_SIZE|int|data"
#define P_CONTAINER   "P_CONTAINER|string|data|char16"
#define P_RETAILPRICE   "P_RETAILPRICE|double|data"
#define P_COMMENT   "P_COMMENT|string|data"


/*
*	psupp_table_column
*/

#define PS_PARTKEY   "PS_PARTKEY|int|data|primary_key"
#define PS_SUPPKEY   "PS_SUPPKEY|int|data|primary_key"
#define PS_AVAILQTY   "PS_AVAILQTY|int|data"
#define PS_SUPPLYCOST   "PS_SUPPLYCOST|double|data"
#define PS_COMMENT   "PS_COMMENT|string|data"

/*
*	supp_table_column
*/

#define S_SUPPKEY   "S_SUPPKEY|int|data|primary_key"
#define S_NAME   "S_NAME|string|data|char32"
#define S_ADDRESS   "S_ADDRESS|string|data"
#define S_NATIONKEY   "S_NATIONKEY|int|data"
#define S_PHONE   "S_PHONE|string|data|char16"
#define S_ACCTBAL   "S_ACCTBAL|double|data"
#define S_COMMENT   "S_COMMENT|string|data"

/*
*	nation_table_column
*/

#define N_NATIONKEY   "N_NATIONKEY|int|data|primary_key"
#define N_NAME   "N_NAME|string|data|char32"
#define N_REGIONKEY   "N_REGIONKEY|int|data"
#define N_COMMENT   "N_COMMENT|string|data"

/*
*	region_table_column
*/

#define R_REGIONKEY   "R_REGIONKEY|int|data|primary_key"
#define R_NAME   "R_NAME|string|data|char32"
#define R_COMMENT   "R_COMMENT|string|data"

#endif