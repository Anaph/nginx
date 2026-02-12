#ifndef _NGX_CONFIG_LIB_SOCKET_H_INCLUDED_
#define _NGX_CONFIG_LIB_SOCKET_H_INCLUDED_

#include <ngx_config.h>
#include <ngx_core.h>

typedef enum {
    NGX_CONFIG_LIB_SOCKET_HTTP = 0,
    NGX_CONFIG_LIB_SOCKET_WEBSOCKET,
    NGX_CONFIG_LIB_SOCKET_VPN,
    NGX_CONFIG_LIB_SOCKET_WIREGUARD,
    NGX_CONFIG_LIB_SOCKET_RTSP
} ngx_config_lib_socket_proto_e;

typedef struct {
    ngx_config_lib_socket_proto_e  protocol;
    ngx_str_t                      address;
    ngx_uint_t                     port;
    ngx_str_t                      upstream;
    ngx_flag_t                     proxy_protocol;
} ngx_config_lib_socket_listener_conf_t;

#endif /* _NGX_CONFIG_LIB_SOCKET_H_INCLUDED_ */
