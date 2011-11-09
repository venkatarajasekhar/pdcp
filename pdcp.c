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
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 297205 2010-03-30 21:09:07Z johannes $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_pdcp.h"

/* If you declare any globals in php_pdcp.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(pdcp)
*/

/* True global resources - no need for thread safety here */
static int le_pdcp,le_ppdcp;

/* {{{ pdcp_functions[]
 *
 * Every user visible function must have an entry in pdcp_functions[].
 */
zend_function_entry pdcp_functions[] = {
	PHP_FE(pdcp_intit,	NULL)
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
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */



/* {{{ proto  pdcp_intit(char *type,char *host,char *user,char *password)
 */
PHP_FUNCTION(pdcp_intit)
{
	if (ZEND_NUM_ARGS() != 0) {
		WRONG_PARAM_COUNT;
	}
	php_error(E_WARNING, "dbcp_intit: not yet implemented");
}
/* }}} */

/* {{{ proto  pdcp_get_connection(char *type)
 */
PHP_FUNCTION(pdcp_get_connection)
{
	if (ZEND_NUM_ARGS() != 0) {
		WRONG_PARAM_COUNT;
	}
	php_error(E_WARNING, "dbcp_get_connection: not yet implemented");
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


static void show_debug_info(HashTable *ht){
	TSRMLS_FETCH();
	HashPosition pos;
	zval **data;

	for(zend_hash_internal_pointer_reset_ex(ht,&pos);
		zend_hash_get_current_data_ex(ht,(void**)&data,&pos)==SUCCESS;
		zend_hash_move_forward_ex(ht,&pos)){
			php_var_dump(data,2 TSRMLS_CC);
	}

}
/* {{{ proto  pdcp_debug()
   
 */
PHP_FUNCTION(pdcp_debug)
{
	show_debug_info(&EG(regular_list));
	//show_debug_info(&EG(persistent_list));
	//show_debug_info(EG(ini_directives));
	
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
