#ifndef _NGX_CONFIG_LIB_LB_H_INCLUDED_
#define _NGX_CONFIG_LIB_LB_H_INCLUDED_

#include <ngx_config.h>
#include <ngx_core.h>

typedef enum {
    NGX_CONFIG_LIB_LB_ROUND_ROBIN = 0,
    NGX_CONFIG_LIB_LB_LEAST_CONN,
    NGX_CONFIG_LIB_LB_IP_HASH,
    NGX_CONFIG_LIB_LB_HASH
} ngx_config_lib_lb_method_e;

typedef struct {
    ngx_str_t   addr;
    ngx_uint_t  port;
    ngx_uint_t  weight;
    ngx_uint_t  max_fails;
    ngx_uint_t  fail_timeout_seconds;
    ngx_flag_t  backup;
} ngx_config_lib_upstream_peer_conf_t;

typedef struct {
    ngx_str_t                            name;
    ngx_config_lib_lb_method_e           method;
    ngx_uint_t                           npeers;
    ngx_config_lib_upstream_peer_conf_t *peers;
} ngx_config_lib_upstream_conf_t;

#endif /* _NGX_CONFIG_LIB_LB_H_INCLUDED_ */
