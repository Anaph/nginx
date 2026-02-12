#ifndef _NGX_CONFIG_LIB_HTTP_H_INCLUDED_
#define _NGX_CONFIG_LIB_HTTP_H_INCLUDED_

#include <ngx_config.h>
#include <ngx_core.h>

typedef struct {
    ngx_str_t   zone_name;
    ngx_uint_t  rate_rps;
    ngx_uint_t  burst;
    ngx_flag_t  nodelay;
} ngx_config_lib_rate_limit_conf_t;

typedef struct {
    ngx_str_t  listen_address;
    ngx_uint_t listen_port;
    ngx_str_t  root;
    ngx_str_t  upstream;
    ngx_str_t  cache_zone;
    ngx_str_t  rate_limit_zone;
    ngx_flag_t websocket_upgrade;
    ngx_flag_t rtsp_proxy;
} ngx_config_lib_http_server_conf_t;

#endif /* _NGX_CONFIG_LIB_HTTP_H_INCLUDED_ */
