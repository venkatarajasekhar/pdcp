# **pdcp(php database connection pool)** #

---

## **Introduction** ##
> ` A php database connection pool implement,provider much faster connection speed over mysql postgres oracle db2 sybase or more...`

---

# Compile PDCP in Linux #
```
$/path/to/phpize 
$./configure --with-pdcp --with-php-config=/path/to/php-config/
$make && make install
```

---

# HOW TO USE #
```
<?php
 /*
   check if the pdcp extension is loaded
 */
if(extenstion_loaded('pdcp')){
     if(!pdcp_is_init()){
         if(!pdcp_init('mysql','localhost','username','password',32)){
             exit('pdcp_init connection pool failed!');
          }   
          $con=pdcp_get_connection('mysql');
          mysql_select_db('pdcp',$con);
          //do some query and other database logic
     ｝
}
```

---

## Support ##
**dietoad@gmail.com** **me**

---

**firedtoad@gmial.com** **me too**
# functions #

---

## in c proto ##
```
bool pdcp_is_init();
bool pdcp_init(char* type,char* host,char *user,char *password,int pool_size=32);
mysql_link* pdcp_get_connection(char *type);
bool pdcp_release();
void pdcp_debug();
```

---

## In php proto ##
```
bool pdcp_is_init();
bool pdcp_init($type,$host,$user,$password,$pool_size=32);
mysql resource pdcp_get_connection($type='mysql');
bool pdcp_release();
void pdcp_debug();
```