/*
* $Id: print.c,v 1.3 2005/10/28 02:56:22 jms Exp $
*
* Revision History
* ===================
* $Log: print.c,v $
* Revision 1.3  2005/10/28 02:56:22  jms
* add platform-specific printf formats to allow for DSS_HUGE data type
*
* Revision 1.2  2005/01/03 20:08:59  jms
* change line terminations
*
* Revision 1.1.1.1  2004/11/24 23:31:47  jms
* re-establish external server
*
* Revision 1.4  2004/02/18 16:26:49  jms
* 32/64 bit changes for overflow handling needed additional changes when ported back to windows
*
* Revision 1.3  2004/02/18 14:05:53  jms
* porting changes for LINUX and 64 bit RNG
*
* Revision 1.2  2004/01/22 05:49:29  jms
* AIX porting (AIX 5.1)
*
* Revision 1.1.1.1  2003/08/07 17:58:34  jms
* recreation after CVS crash
*
* Revision 1.2  2003/08/07 17:58:34  jms
* Convery RNG to 64bit space as preparation for new large scale RNG
*
* Revision 1.1.1.1  2003/04/03 18:54:21  jms
* initial checkin
*
*
*/
/* generate flat files for data load */
#include <stdio.h>
#ifndef VMS
#include <sys/types.h>
#endif
#if defined(SUN)
#include <unistd.h>
#endif
#include <math.h>

#include "dss.h"
#include "dsstypes.h"
#include "table_column.h"
#include <string.h>

#define EOL_HANDLING

/*
 * Function Prototypes
 */
FILE *print_prep PROTO((int table, int update, int add));
int pr_drange PROTO((int tbl, DSS_HUGE min, DSS_HUGE cnt, long num));

FILE *
print_prep(int table, int update, int add)
{
	char upath[128];
	FILE *res;

	if (updates)
		{
		if (update > 0) /* updates */
			if ( insert_segments )
				{
				int this_segment;
				if(strcmp(tdefs[table].name,"orders.csv"))
					this_segment=++insert_orders_segment;
				else 
					this_segment=++insert_lineitem_segment;
				sprintf(upath, "%s%c%s.u%d.%d", 
					env_config(PATH_TAG, PATH_DFLT),
					PATH_SEP, tdefs[table].name, update%10000,this_segment);
				}
			else
				{
				sprintf(upath, "%s%c%s.u%d",
				env_config(PATH_TAG, PATH_DFLT),
				PATH_SEP, tdefs[table].name, update);
				}
		else /* deletes */
			if ( delete_segments )
				{
				++delete_segment;
				sprintf(upath, "%s%cdelete.u%d.%d",
					env_config(PATH_TAG, PATH_DFLT), PATH_SEP, -update%10000,
					delete_segment);
				}
			else
				{
				sprintf(upath, "%s%cdelete.%d",
				env_config(PATH_TAG, PATH_DFLT), PATH_SEP, -update);
				}
        if(add == 1){
          return(fopen(upath, "a"));
        }else{
		      return(fopen(upath, "w"));
        }
    }
    if(add == 1){
      res = tbl_open(table, "a");
    }else{
      res = tbl_open(table, "w");
    }
    
    OPEN_CHECK(res, tdefs[table].name);
    return(res);
}

int
dbg_print(int format, FILE *target, void *data, int len, int sep)
{
	int dollars,
		cents;

	switch(format)
	{
	case DT_STR:
		fprintf(target, "%s", (char *)data);
		break;
#ifdef MVS
	case DT_VSTR:
		/* note: only used in MVS, assumes columnar output */
		fprintf(target, "%c%c%-*s", 
			(len >> 8) & 0xFF, len & 0xFF, len, (char *)data);
		break;
#endif /* MVS */
	case DT_INT:
		fprintf(target, "%ld", (long)data);
		break;
	case DT_HUGE:
		fprintf(target, HUGE_FORMAT, *(DSS_HUGE *)data);
		break;
	case DT_KEY:
		fprintf(target, "%ld", (long)data);
		break;
	case DT_MONEY:
		cents = (int)*(DSS_HUGE *)data;
		if (cents < 0)
			{
			fprintf(target, "-");
			cents = -cents;
			}
		dollars = cents / 100;
		cents %= 100;
		fprintf(target, "%d.%02d", dollars, cents);
		break;
	case DT_CHR:
		fprintf(target, "%c", *(char *)data);
		break;
	}

#ifdef EOL_HANDLING
	if (sep)
#endif /* EOL_HANDLING */
	fprintf(target, "%c", SEPARATOR);
	
	return(0);
}

int
pr_cust(customer_t *c, int mode)
{
static FILE *fp = NULL;
        
   if (fp == NULL)
        fp = print_prep(CUST, 0, 1);

   PR_STRT(fp);
   PR_HUGE(fp, &c->custkey);
   if (scale <= 3000)
   PR_VSTR(fp, c->name, C_NAME_LEN);
   else
   PR_VSTR(fp, c->name, C_NAME_LEN + 3);
   PR_VSTR(fp, c->address, c->alen);
   PR_HUGE(fp, &c->nation_code);
   PR_STR(fp, c->phone, PHONE_LEN);
   PR_MONEY(fp, &c->acctbal);
   PR_STR(fp, c->mktsegment, C_MSEG_LEN);
   PR_VSTR_LAST(fp, c->comment, c->clen);
   PR_END(fp);

   return(0);
}

int
pr_cust_table_column(int mode){
  static FILE *fp = NULL;
        
  if (fp == NULL)
        fp = print_prep(CUST, mode, 0);

  PR_STRT(fp);
  PR_STR(fp, C_CUSTKEY, (int)strlen(C_CUSTKEY));
  PR_STR(fp, C_NAME, (int)strlen(C_NAME));
  PR_STR(fp, C_ADDRESS, (int)strlen(C_ADDRESS));
  PR_STR(fp, C_NATIONKEY, (int)strlen(C_NATIONKEY));
  PR_STR(fp, C_PHONE, (int)strlen(C_PHONE));
  PR_STR(fp, C_ACCTBAL, (int)strlen(C_ACCTBAL));
  PR_STR(fp, C_MKTSEGMENT, (int)strlen(C_MKTSEGMENT));
  PR_STR_LAST(fp, C_COMMENT, (int)strlen(C_COMMENT));
  PR_END(fp);
  fclose(fp);
  
  return(0);
}

/*
 * print the numbered order 
 */
int
pr_order(order_t *o, int mode)
{
    static FILE *fp_o = NULL;
    static int last_mode = 0;
        
    if (fp_o == NULL || mode != last_mode)
        {
        if (fp_o) 
            fclose(fp_o);
        fp_o = print_prep(ORDER, mode, 1);
        last_mode = mode;
        }
    PR_STRT(fp_o);
    PR_HUGE(fp_o, &o->okey);
    PR_HUGE(fp_o, &o->custkey);
    PR_CHR(fp_o, &o->orderstatus);
    PR_MONEY(fp_o, &o->totalprice);
    PR_STR(fp_o, o->odate, DATE_LEN);
    PR_STR(fp_o, o->opriority, O_OPRIO_LEN);
    PR_STR(fp_o, o->clerk, O_CLRK_LEN);
    PR_INT(fp_o, o->spriority);
    PR_VSTR_LAST(fp_o, o->comment, o->clen);
    PR_END(fp_o);

    return(0);
}

int
pr_order_table_column(int mode){
  static FILE *fp = NULL;
        
  if (fp == NULL)
        fp = print_prep(ORDER, mode, 0);

  PR_STRT(fp);
  PR_STR(fp, O_ORDERKEY, (int)strlen(O_ORDERKEY));
  PR_STR(fp, O_CUSTKEY, (int)strlen(O_CUSTKEY));
  PR_STR(fp, O_ORDERSTATUS, (int)strlen(O_ORDERSTATUS));
  PR_STR(fp, O_TOTALPRICE, (int)strlen(O_TOTALPRICE));
  PR_STR(fp, O_ORDERDATE, (int)strlen(O_ORDERDATE));
  PR_STR(fp, O_ORDERPRIORITY, (int)strlen(O_ORDERPRIORITY));
  PR_STR(fp, O_CLERK, (int)strlen(O_CLERK));
  PR_STR(fp, O_SHIPPRIORITY, (int)strlen(O_SHIPPRIORITY));
  PR_STR_LAST(fp, O_COMMENT, (int)strlen(O_COMMENT));
  PR_END(fp);
  fclose(fp);

  return(0);
}

/*
 * print an order's lineitems
 */
int
pr_line(order_t *o, int mode)
{
    static FILE *fp_l = NULL;
    static int last_mode = 0;
    long      i;
        
    if (fp_l == NULL || mode != last_mode)
        {
        if (fp_l) 
            fclose(fp_l);
        fp_l = print_prep(LINE, mode, 1);
        last_mode = mode;
        }

    for (i = 0; i < o->lines; i++)
        {
        PR_STRT(fp_l);
        PR_HUGE(fp_l, &o->l[i].okey);
        PR_HUGE(fp_l, &o->l[i].partkey);
        PR_HUGE(fp_l, &o->l[i].suppkey);
        PR_HUGE(fp_l, &o->l[i].lcnt);
        PR_HUGE(fp_l, &o->l[i].quantity);
        PR_MONEY(fp_l, &o->l[i].eprice);
        PR_MONEY(fp_l, &o->l[i].discount);
        PR_MONEY(fp_l, &o->l[i].tax);
        PR_CHR(fp_l, &o->l[i].rflag[0]);
        PR_CHR(fp_l, &o->l[i].lstatus[0]);
        PR_STR(fp_l, o->l[i].sdate, DATE_LEN);
        PR_STR(fp_l, o->l[i].cdate, DATE_LEN);
        PR_STR(fp_l, o->l[i].rdate, DATE_LEN);
        PR_STR(fp_l, o->l[i].shipinstruct, L_INST_LEN);
        PR_STR(fp_l, o->l[i].shipmode, L_SMODE_LEN);
        PR_VSTR_LAST(fp_l, o->l[i].comment,o->l[i].clen);
        PR_END(fp_l);
        }

   return(0);
}

int
pr_line_table_column(int mode){
  static FILE *fp = NULL;
        
  if (fp == NULL)
        fp = print_prep(LINE, mode, 0);

  PR_STRT(fp);
  PR_STR(fp, L_ORDERKEY, (int)strlen(L_ORDERKEY));
  PR_STR(fp, L_PARTKEY, (int)strlen(L_PARTKEY));
  PR_STR(fp, L_SUPPKEY, (int)strlen(L_SUPPKEY));
  PR_STR(fp, L_LINENUMBER, (int)strlen(L_LINENUMBER));
  PR_STR(fp, L_QUANTITY, (int)strlen(L_QUANTITY));
  PR_STR(fp, L_EXTENDEDPRICE, (int)strlen(L_EXTENDEDPRICE));
  PR_STR(fp, L_DISCOUNT, (int)strlen(L_DISCOUNT));
  PR_STR(fp, L_TAX, (int)strlen(L_TAX));
  PR_STR(fp, L_RETURNFLAG, (int)strlen(L_RETURNFLAG));
  PR_STR(fp, L_LINESTATUS, (int)strlen(L_LINESTATUS));
  PR_STR(fp, L_SHIPDATE, (int)strlen(L_SHIPDATE));
  PR_STR(fp, L_COMMITDATE, (int)strlen(L_COMMITDATE));
  PR_STR(fp, L_RECEIPTDATE, (int)strlen(L_RECEIPTDATE));
  PR_STR(fp, L_SHIPINSTRUCT, (int)strlen(L_SHIPINSTRUCT));
  PR_STR(fp, L_SHIPMODE, (int)strlen(L_SHIPMODE));
  PR_STR_LAST(fp, L_COMMENT, (int)strlen(L_COMMENT));
  PR_END(fp);
  fclose(fp);

  return(0);
}

/*
 * print the numbered order *and* its associated lineitems
 */
int
pr_order_line(order_t *o, int mode)
{
    tdefs[ORDER].name = tdefs[ORDER_LINE].name;
    pr_order(o, mode);
    pr_line(o, mode);

    return(0);
}

int
pr_order_line_table_column(int mode){

  pr_order_table_column(mode);
  pr_line_table_column(mode);
  return(0);
}

/*
 * print the given part
 */
int
pr_part(part_t *part, int mode)
{
static FILE *p_fp = NULL;

    if (p_fp == NULL)
        p_fp = print_prep(PART, 0, 1);

   PR_STRT(p_fp);
   PR_HUGE(p_fp, &part->partkey);
   PR_VSTR(p_fp, part->name,part->nlen);
   PR_STR(p_fp, part->mfgr, P_MFG_LEN);
   PR_STR(p_fp, part->brand, P_BRND_LEN);
   PR_VSTR(p_fp, part->type,part->tlen);
   PR_HUGE(p_fp, &part->size);
   PR_STR(p_fp, part->container, P_CNTR_LEN);
   PR_MONEY(p_fp, &part->retailprice);
   PR_VSTR_LAST(p_fp, part->comment,part->clen);
   PR_END(p_fp);

   return(0);
}

int
pr_part_table_column(int mode){
  static FILE *fp = NULL;
        
  if (fp == NULL)
        fp = print_prep(PART, mode, 0);

  PR_STRT(fp);
  PR_STR(fp, P_PARTKEY, (int)strlen(P_PARTKEY));
  PR_STR(fp, P_NAME, (int)strlen(P_NAME));
  PR_STR(fp, P_MFGR, (int)strlen(P_MFGR));
  PR_STR(fp, P_BRAND, (int)strlen(P_BRAND));
  PR_STR(fp, P_TYPE, (int)strlen(P_TYPE));
  PR_STR(fp, P_SIZE_S, (int)strlen(P_SIZE_S));
  PR_STR(fp, P_CONTAINER, (int)strlen(P_CONTAINER));
  PR_STR(fp, P_RETAILPRICE, (int)strlen(P_RETAILPRICE));
  PR_STR_LAST(fp, P_COMMENT, (int)strlen(P_COMMENT));
  PR_END(fp);
  fclose(fp);

  return(0);
}

/*
 * print the given part's suppliers
 */
int
pr_psupp(part_t *part, int mode)
{
    static FILE *ps_fp = NULL;
    long      i;

    if (ps_fp == NULL)
        ps_fp = print_prep(PSUPP, mode, 1);

   for (i = 0; i < SUPP_PER_PART; i++)
      {
      PR_STRT(ps_fp);
      PR_HUGE(ps_fp, &part->s[i].partkey);
      PR_HUGE(ps_fp, &part->s[i].suppkey);
      PR_HUGE(ps_fp, &part->s[i].qty);
      PR_MONEY(ps_fp, &part->s[i].scost);
      PR_VSTR_LAST(ps_fp, part->s[i].comment,part->s[i].clen);
      PR_END(ps_fp);
      }

   return(0);
}

int
pr_psupp_table_column(int mode){
  static FILE *fp = NULL;
        
  if (fp == NULL)
        fp = print_prep(PSUPP, mode, 0);

  PR_STRT(fp);
  PR_STR(fp, PS_PARTKEY, (int)strlen(PS_PARTKEY));
  PR_STR(fp, PS_SUPPKEY, (int)strlen(PS_SUPPKEY));
  PR_STR(fp, PS_AVAILQTY, (int)strlen(PS_AVAILQTY));
  PR_STR(fp, PS_SUPPLYCOST, (int)strlen(PS_SUPPLYCOST));
  PR_STR_LAST(fp, PS_COMMENT, (int)strlen(PS_COMMENT));
  PR_END(fp);
  fclose(fp);

  return(0);
}

/*
 * print the given part *and* its suppliers
 */
int
pr_part_psupp(part_t *part, int mode)
{
    tdefs[PART].name = tdefs[PART_PSUPP].name;
    pr_part(part, mode);
    pr_psupp(part, mode);

    return(0);
}

int
pr_part_psupp_table_column(int mode){

  pr_part_table_column(mode);
  pr_psupp_table_column(mode);
  return(0);
}

int
pr_supp(supplier_t *supp, int mode)
{
static FILE *fp = NULL;
        
   if (fp == NULL)
        fp = print_prep(SUPP, mode, 1);

   PR_STRT(fp);
   PR_HUGE(fp, &supp->suppkey);
   PR_STR(fp, supp->name, S_NAME_LEN);
   PR_VSTR(fp, supp->address, supp->alen);
   PR_HUGE(fp, &supp->nation_code);
   PR_STR(fp, supp->phone, PHONE_LEN);
   PR_MONEY(fp, &supp->acctbal);
   PR_VSTR_LAST(fp, supp->comment, supp->clen);
   PR_END(fp);

   return(0);
}

int
pr_supp_table_column(int mode){
  static FILE *fp = NULL;
        
  if (fp == NULL)
        fp = print_prep(SUPP, mode, 0);

  PR_STRT(fp);
  PR_STR(fp, S_SUPPKEY, (int)strlen(S_SUPPKEY));
  PR_STR(fp, S_NAME, (int)strlen(S_NAME));
  PR_STR(fp, S_ADDRESS, (int)strlen(S_ADDRESS));
  PR_STR(fp, S_NATIONKEY, (int)strlen(S_NATIONKEY));
  PR_STR(fp, S_PHONE, (int)strlen(S_PHONE));
  PR_STR(fp, S_ACCTBAL, (int)strlen(S_ACCTBAL));
  PR_STR_LAST(fp, S_COMMENT, (int)strlen(S_COMMENT));
  PR_END(fp);
  fclose(fp);

  return(0);
}

int
pr_nation(code_t *c, int mode)
{
static FILE *fp = NULL;
        
   if (fp == NULL)
        fp = print_prep(NATION, mode, 1);

   PR_STRT(fp);
   PR_HUGE(fp, &c->code);
   PR_STR(fp, c->text, NATION_LEN);
   PR_INT(fp, c->join);
   PR_VSTR_LAST(fp, c->comment, c->clen);
   PR_END(fp);

   return(0);
}

int
pr_nation_table_column(int mode){
  static FILE *fp = NULL;
        
  if (fp == NULL)
        fp = print_prep(NATION, mode, 0);

  PR_STRT(fp);
  PR_STR(fp, N_NATIONKEY, (int)strlen(N_NATIONKEY));
  PR_STR(fp, N_NAME, (int)strlen(N_NAME));
  PR_STR(fp, N_REGIONKEY, (int)strlen(N_REGIONKEY));
  PR_STR_LAST(fp, N_COMMENT, (int)strlen(N_COMMENT));
  PR_END(fp);
  fclose(fp);

  return(0);
}

int
pr_region(code_t *c, int mode)
{
static FILE *fp = NULL;
        
   if (fp == NULL)
        fp = print_prep(REGION, mode, 1);

   PR_STRT(fp);
   PR_HUGE(fp, &c->code);
   PR_STR(fp, c->text, REGION_LEN);
   PR_VSTR_LAST(fp, c->comment, c->clen);
   PR_END(fp);

   return(0);
}

int
pr_region_table_column(int mode){
  static FILE *fp = NULL;
        
  if (fp == NULL)
        fp = print_prep(REGION, mode, 0);

  PR_STRT(fp);
  PR_STR(fp, R_REGIONKEY, (int)strlen(R_REGIONKEY));
  PR_STR(fp, R_NAME, (int)strlen(R_NAME));
  PR_STR_LAST(fp, R_COMMENT, (int)strlen(R_COMMENT));
  PR_END(fp);
  fclose(fp);

  return(0);
}

/* 
 * NOTE: this routine does NOT use the BCD2_* routines. As a result,
 * it WILL fail if the keys being deleted exceed 32 bits. Since this
 * would require ~660 update iterations, this seems an acceptable
 * oversight
 */
int
pr_drange(int tbl, DSS_HUGE min, DSS_HUGE cnt, long num)
{
    static int  last_num = 0;
    static FILE *dfp = NULL;
    DSS_HUGE child = -1;
    DSS_HUGE start, last, new;

	static DSS_HUGE rows_per_segment=0;
	static DSS_HUGE rows_this_segment=0;

    if (last_num != num)
        {
        if (dfp)
            fclose(dfp);
        dfp = print_prep(tbl, -num, 1);
        if (dfp == NULL)
            return(-1);
        last_num = num;
		rows_this_segment=0;
        }

    start = MK_SPARSE(min, num/ (10000 / UPD_PCT));
    last = start - 1;
    for (child=min; cnt > 0; child++, cnt--)
	{
		new = MK_SPARSE(child, num/ (10000 / UPD_PCT));
		if (delete_segments)
		{

			if(rows_per_segment==0) 
				rows_per_segment = (cnt / delete_segments) + 1;
			if((++rows_this_segment) > rows_per_segment)
			{
				fclose(dfp);
				dfp = print_prep(tbl, -num, 1);
				if (dfp == NULL) return(-1);
				last_num = num;
				rows_this_segment=1;
			}
		}
		PR_STRT(dfp);
		PR_HUGE(dfp, &new);
		PR_END(dfp);
		start = new;
		last = new;
	}
    
    return(0);
}



