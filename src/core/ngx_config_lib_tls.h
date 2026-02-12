#ifndef _NGX_CONFIG_LIB_TLS_H_INCLUDED_
#define _NGX_CONFIG_LIB_TLS_H_INCLUDED_

#include <ngx_config.h>
#include <ngx_core.h>

typedef struct {
    ngx_flag_t  enabled;
    ngx_str_t   certificate;
    ngx_str_t   certificate_key;
    ngx_str_t   ciphers;
    ngx_str_t   protocols;
    ngx_uint_t  session_timeout_seconds;
} ngx_config_lib_tls_conf_t;

#endif /* _NGX_CONFIG_LIB_TLS_H_INCLUDED_ */
