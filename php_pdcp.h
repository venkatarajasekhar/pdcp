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

#ifndef PHP_PDCP_H
#define PHP_PDCP_H

extern zend_module_entry pdcp_module_entry;
#define phpext_pdcp_ptr &pdcp_module_entry

#ifdef PHP_WIN32
#define PHP_PDCP_API __declspec(dllexport)
#else
#define PHP_PDCP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(pdcp);
PHP_MSHUTDOWN_FUNCTION(pdcp);
PHP_RINIT_FUNCTION(pdcp);
PHP_RSHUTDOWN_FUNCTION(pdcp);
PHP_MINFO_FUNCTION(pdcp);

PHP_FUNCTION(pdcp_init);
PHP_FUNCTION(pdcp_is_init);
PHP_FUNCTION(pdcp_get_connection);
PHP_FUNCTION(pdcp_release);
PHP_FUNCTION(pdcp_debug);
typedef unsigned int php_uint32;
extern PHPAPI int  php_register_info_logo(char *logo_string, const char *mimetype, const unsigned char *data, int size);
extern PHPAPI void php_var_dump(zval **struc, int level TSRMLS_DC);
extern PHPAPI void php_debug_zval_dump(zval **struc, int level TSRMLS_DC);
PHPAPI void php_mt_srand(php_uint32 seed TSRMLS_DC);
PHPAPI php_uint32 php_mt_rand(TSRMLS_D);


/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     
*/
ZEND_BEGIN_MODULE_GLOBALS(pdcp)
	long  global_value;
	char *global_string;

ZEND_END_MODULE_GLOBALS(pdcp)


/* In every utility function you add that needs to use variables 
   in php_pdcp_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as PDCP_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define PDCP_G(v) TSRMG(pdcp_globals_id, zend_pdcp_globals *, v)
#else
#define PDCP_G(v) (pdcp_globals.v)
#endif

#endif	/* PHP_PDCP_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
