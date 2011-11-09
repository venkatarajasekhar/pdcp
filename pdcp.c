/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:   dietoad@gmail.com                                                           |
  +----------------------------------------------------------------------+
*/

/* $Id: header dietoad */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include<mysql.h>
#include "php_pdcp.h"
#define VERSION "0.01 "
#define AUTHOR "dietoad"
#define SUPPORT "firedtoad@gmail.com && dietoad@gmail.com"
#define MYSQL_TYPE "mysql link"
#define MYSQL_PTYPE "mysql link persistent"
#define MYSQL_HOST "127.0.0.1"
#define MYSQL_USER "root"
#define MYSQL_PASSWORD ""
#define MYSQL_PORT 3306
#define INIT_MAX_CONNECTION 8
#define LOADFACTOR 1.3
#define TRY_MAX 3
typedef struct _php_mysql_conn {
	MYSQL conn;
	int active_result_id;
} php_mysql_conn;

/* If you declare any globals in php_pdcp.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(pdcp)
*/

/* True global resources - no need for thread safety here */
static int le_pdcp,le_ppdcp,mysql_type,mysql_ptype,init_lock,max_connection;
static php_mysql_conn *g_connections[INIT_MAX_CONNECTION];
static int use_index[INIT_MAX_CONNECTION];
/* {{{ pdcp_functions[]
 *
 * Every user visible function must have an entry in pdcp_functions[].
 */
zend_function_entry pdcp_functions[] = {
	PHP_FE(pdcp_init,	NULL)
	PHP_FE(pdcp_get_connection,	NULL)
	PHP_FE(pdcp_release,	NULL)
	PHP_FE(pdcp_debug,	NULL)
	{NULL, NULL, NULL}	/* Must be the last line in pdcp_functions[] */
};
/* }}} */

/* {{{ pdcp_module_entry
 */
zend_module_entry pdcp_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"pdcp",
	pdcp_functions,
	PHP_MINIT(pdcp),
	PHP_MSHUTDOWN(pdcp),
	PHP_RINIT(pdcp),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(pdcp),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(pdcp),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PDCP
ZEND_GET_MODULE(pdcp)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("pdcp.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_pdcp_globals, pdcp_globals)
    STD_PHP_INI_ENTRY("pdcp.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_pdcp_globals, pdcp_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_pdcp_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_pdcp_init_globals(zend_pdcp_globals *pdcp_globals)
{
	pdcp_globals->global_value = 0;
	pdcp_globals->global_string = NULL;
}
*/
/* }}} */



static void _pdcp_close(zend_rsrc_list_entry *rsrc TSRMLS_DC){
	php_printf("_pdcp_close_called\n");;
}


static void _pdcp_p_close(zend_rsrc_list_entry *rsrc TSRMLS_DC){
	php_printf("_pdcp_p_close_called\n");;
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pdcp)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	le_pdcp=zend_register_list_destructors_ex(_pdcp_close,NULL,"pdcp",module_number);
	le_ppdcp=zend_register_list_destructors_ex(NULL,_pdcp_p_close,"pdcp_persistent",module_number);
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pdcp)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	int i=0;
    php_mysql_conn *con;
	for(;i<INIT_MAX_CONNECTION;++i){
		con=g_connections[i];
		if(con!=NULL){
			if(mysql_ping(&con->conn)){
				mysql_close(&con->conn);
			}
			pefree(con,1);
		}
	}
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(pdcp)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(pdcp)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pdcp)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pdcp support", "enabled");
	php_info_print_table_row(2,"Version",VERSION);
	php_info_print_table_row(2,"Author",AUTHOR);
	php_info_print_table_row(2,"Support",SUPPORT);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */



/* {{{ proto  pdcp_intit(char *type,char *host,char *user,char *password)
 */
PHP_FUNCTION(pdcp_init)
{
    int i=0;
	php_mysql_conn *con;
	 char *mysql_error_str;
	max_connection=INIT_MAX_CONNECTION;
	if(init_lock==1){
		php_printf("already inited!\n");
		RETURN_BOOL(1);
	}
	for(;i<max_connection;++i){

		con=(php_mysql_conn*)pemalloc(sizeof(php_mysql_conn),1);
		
		mysql_init(&con->conn);
		if(mysql_real_connect(&con->conn,MYSQL_HOST,MYSQL_USER,MYSQL_PASSWORD,NULL,MYSQL_PORT,NULL,0)==NULL){
           php_printf("connect failed!\n");
            mysql_error_str=estrdup(mysql_error(&con->conn));
		   php_printf("error %s \n",mysql_error_str);
		   efree(mysql_error_str);

		}
		if(mysql_ping(&con->conn)==SUCCESS){
            con->active_result_id=i;
			g_connections[i]=con;
			php_printf("g_index=%d\n",g_connections[i]->active_result_id);
		}
	}
	mysql_type=zend_fetch_list_dtor_id(MYSQL_TYPE);
	mysql_ptype=zend_fetch_list_dtor_id(MYSQL_PTYPE);
	if(mysql_type<=0||mysql_ptype<=0){
		php_printf("no mysql extension support\n");
	}
	init_lock=1;
	RETURN_BOOL(1);
}
/* }}} */





/* {{{ proto  pdcp_get_connection(char *type)
 */

PHP_FUNCTION(pdcp_get_connection)
{
   int index=0,try_time=0;
   zval *ret;
   php_mysql_conn * con;
  // srand((unsigned)time(NULL));
  // php_printf("index=%f",(rand()/(RAND_MAX+1))*max_connection);
   
   do{
   
   index=rand()%max_connection;
   if(index<0||index==max_connection){
      continue;
   }
     con=g_connections[index];
    
   if(con!=NULL){
        php_printf("con is not null mysql_type=%d\n",mysql_type);
	MAKE_STD_ZVAL(ret);
   // ZEND_REGISTER_RESOURCE(ret,con,mysql_type);
	RETURN_ZVAL(ret,0,0);
    return;
   }else{
	  php_printf("try failed %d time index=%d \n",try_time,index);
   }
   if(++try_time==TRY_MAX){
	   break;
   }
   }while(1);
RETURN_BOOL(0);
}
/* }}} */

/* {{{ proto  pbcp_release(char *type)
 */
PHP_FUNCTION(pdcp_release)
{
	if (ZEND_NUM_ARGS() != 0) {
		WRONG_PARAM_COUNT;
	}
	php_error(E_WARNING, "dbcp_release: not yet implemented");
}
/* }}} */


static void show_debug_info(HashTable *ht,int fc,int cl){
	TSRMLS_FETCH();
	HashPosition pos;
	zend_function *fe;
	zend_class_entry *ce;
	zend_bool bret=1;
	zval **data;
	if(zend_hash_num_elements(ht)==0){
       return;
	}
	for(zend_hash_internal_pointer_reset_ex(ht,&pos);
		bret;
		zend_hash_move_forward_ex(ht,&pos)){
			if(fc==1){

				if(zend_hash_get_current_data_ex(ht,(void**)&fe,&pos)==SUCCESS){
                  php_printf("function name=%s\n",fe->common.function_name);
				}else{
                 bret=0;
				} 
			}else if(cl==1){

				if(zend_hash_get_current_data_ex(ht,(void**)&ce,&pos)==SUCCESS){
				  php_printf("function name=%s\n",ce->name);
				}else{
					bret=0;
				} 
			}else{
				if(zend_hash_get_current_data_ex(ht,(void**)&data,&pos)==SUCCESS){
			      php_var_dump(data,2 TSRMLS_CC);
				}else{
					bret=0;
				} 
			}
	}

}
/* {{{ proto  pdcp_debug()
   
 */
PHP_FUNCTION(pdcp_debug)
{
	int i=0;
	//php_printf("<pre>\n");
	show_debug_info(&EG(regular_list),0,0);
	show_debug_info(&EG(persistent_list),0,0);
	
	//show_debug_info(EG(active_symbol_table),0,0);
	//show_debug_info(&EG(included_files));
	//show_debug_info(EG(class_table));
	//show_debug_info(EG(zend_constants),0,0);
	//show_debug_info(EG(function_table),1,0);
	//php_printf("</pre>\n");
	//show_debug_info(EG(ini_directives));
	if(init_lock==0){
		return;
	}
	for(;i<max_connection;++i){
		if(g_connections[i]!=NULL)
		php_printf("con->active_result_id=%d\n",g_connections[i]->active_result_id);
	}
	
}

/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
