DROP FOREIGN TABLE IF EXISTS NATION;
CREATE FOREIGN TABLE NATION  ( N_NATIONKEY  INTEGER NOT NULL,
                            N_NAME       CHAR(25) NOT NULL,
                            N_REGIONKEY  INTEGER NOT NULL,
                            N_COMMENT    VARCHAR(152))
SERVER zdb_server;
DROP FOREIGN TABLE IF EXISTS REGION;
CREATE FOREIGN TABLE REGION  ( R_REGIONKEY  INTEGER NOT NULL,
                            R_NAME       CHAR(25) NOT NULL,
                            R_COMMENT    VARCHAR(152))
SERVER zdb_server;
DROP FOREIGN TABLE IF EXISTS PART;
CREATE FOREIGN TABLE PART  ( P_PARTKEY     INTEGER NOT NULL,
                          P_NAME        VARCHAR(55) NOT NULL,
                          P_MFGR        CHAR(25) NOT NULL,
                          P_BRAND       CHAR(10) NOT NULL,
                          P_TYPE        VARCHAR(25) NOT NULL,
                          P_SIZE        INTEGER NOT NULL,
                          P_CONTAINER   CHAR(10) NOT NULL,
                          P_RETAILPRICE DECIMAL(15,2) NOT NULL,
                          P_COMMENT     VARCHAR(23) NOT NULL )
SERVER zdb_server;
DROP FOREIGN TABLE IF EXISTS SUPPLIER;
CREATE FOREIGN TABLE SUPPLIER ( S_SUPPKEY     INTEGER NOT NULL,
                             S_NAME        CHAR(25) NOT NULL,
                             S_ADDRESS     VARCHAR(40) NOT NULL,
                             S_NATIONKEY   INTEGER NOT NULL,
                             S_PHONE       CHAR(15) NOT NULL,
                             S_ACCTBAL     DECIMAL(15,2) NOT NULL,
                             S_COMMENT     VARCHAR(101) NOT NULL)
SERVER zdb_server;
DROP FOREIGN TABLE IF EXISTS PARTSUPP;
CREATE FOREIGN TABLE PARTSUPP ( PS_PARTKEY     INTEGER NOT NULL,
                             PS_SUPPKEY     INTEGER NOT NULL,
                             PS_AVAILQTY    INTEGER NOT NULL,
                             PS_SUPPLYCOST  DECIMAL(15,2)  NOT NULL,
                             PS_COMMENT     VARCHAR(199) NOT NULL )
SERVER zdb_server;
DROP FOREIGN TABLE IF EXISTS CUSTOMER;
CREATE FOREIGN TABLE CUSTOMER ( C_CUSTKEY     INTEGER NOT NULL,
                             C_NAME        VARCHAR(25) NOT NULL,
                             C_ADDRESS     VARCHAR(40) NOT NULL,
                             C_NATIONKEY   INTEGER NOT NULL,
                             C_PHONE       CHAR(15) NOT NULL,
                             C_ACCTBAL     DECIMAL(15,2)   NOT NULL,
                             C_MKTSEGMENT  CHAR(10) NOT NULL,
                             C_COMMENT     VARCHAR(117) NOT NULL)
SERVER zdb_server;
DROP FOREIGN TABLE IF EXISTS ORDERS;
CREATE FOREIGN TABLE ORDERS  ( O_ORDERKEY       INTEGER NOT NULL,
                           O_CUSTKEY        INTEGER NOT NULL,
                           O_ORDERSTATUS    CHAR(1) NOT NULL,
                           O_TOTALPRICE     DECIMAL(15,2) NOT NULL,
                           O_ORDERDATE      DATE NOT NULL,
                           O_ORDERPRIORITY  CHAR(15) NOT NULL,
                           O_CLERK          CHAR(15) NOT NULL,
                           O_SHIPPRIORITY   INTEGER NOT NULL,
                           O_COMMENT        VARCHAR(79) NOT NULL)
SERVER zdb_server;
DROP FOREIGN TABLE IF EXISTS LINEITEM;
CREATE FOREIGN TABLE LINEITEM ( L_ORDERKEY    INTEGER NOT NULL,
                             L_PARTKEY     INTEGER NOT NULL,
                             L_SUPPKEY     INTEGER NOT NULL,
                             L_LINENUMBER  INTEGER NOT NULL,
                             L_QUANTITY    DECIMAL(15,2) NOT NULL,
                             L_EXTENDEDPRICE  DECIMAL(15,2) NOT NULL,
                             L_DISCOUNT    DECIMAL(15,2) NOT NULL,
                             L_TAX         DECIMAL(15,2) NOT NULL,
                             L_RETURNFLAG  CHAR(1) NOT NULL,
                             L_LINESTATUS  CHAR(1) NOT NULL,
                             L_SHIPDATE    DATE NOT NULL,
                             L_COMMITDATE  DATE NOT NULL,
                             L_RECEIPTDATE DATE NOT NULL,
                             L_SHIPINSTRUCT CHAR(25) NOT NULL,
                             L_SHIPMODE     CHAR(10) NOT NULL,
                             L_COMMENT      VARCHAR(44) NOT NULL)
SERVER zdb_server;
copy nation from '@path@/data/tpch_1G/nation.csv' with CSV HEADER DELIMITER ',';
copy region from '@path@/data/tpch_1G/region.csv' with CSV HEADER DELIMITER ',';
copy part from '@path@/data/tpch_1G/part.csv' with CSV HEADER DELIMITER ',';
copy supplier from '@path@/data/tpch_1G/supplier.csv' with CSV HEADER DELIMITER ',';
copy partsupp from '@path@/data/tpch_1G/partsupp.csv' with CSV HEADER DELIMITER ',';
copy customer from '@path@/data/tpch_1G/customer.csv' with CSV HEADER DELIMITER ',';
copy orders from '@path@/data/tpch_1G/orders.csv' with CSV HEADER DELIMITER ',';
copy lineitem from '@path@/data/tpch_1G/lineitem.csv' with CSV HEADER DELIMITER ',';
