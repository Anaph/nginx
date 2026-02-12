#ifndef _NGX_CONFIG_LIB_H_INCLUDED_
#define _NGX_CONFIG_LIB_H_INCLUDED_

#include <ngx_config.h>
#include <ngx_core.h>

#include <ngx_config_lib_main.h>
#include <ngx_config_lib_cache.h>
#include <ngx_config_lib_lb.h>
#include <ngx_config_lib_tls.h>
#include <ngx_config_lib_socket.h>
#include <ngx_config_lib_http.h>

typedef struct {
    ngx_config_lib_main_conf_t             main;
    ngx_uint_t                             ncache_zones;
    ngx_config_lib_cache_zone_conf_t      *cache_zones;
    ngx_uint_t                             nupstreams;
    ngx_config_lib_upstream_conf_t        *upstreams;
    ngx_uint_t                             nrate_limits;
    ngx_config_lib_rate_limit_conf_t      *rate_limits;
    ngx_config_lib_tls_conf_t              tls;
    ngx_uint_t                             nsocket_listeners;
    ngx_config_lib_socket_listener_conf_t *socket_listeners;
    ngx_uint_t                             nhttp_servers;
    ngx_config_lib_http_server_conf_t     *http_servers;
} ngx_config_lib_conf_t;

typedef struct {
    const char *prefix;
    const char *error_log;
} ngx_config_lib_options_t;

typedef struct {
    ngx_pool_t            *pool;
    ngx_log_t             *log;
    ngx_config_lib_conf_t *active;
    ngx_pool_t            *active_pool;
    ngx_uint_t             generation;
    ngx_uint_t             initialized;
} ngx_config_lib_t;

typedef struct {
    ngx_config_lib_main_conf_t                    main;
    ngx_uint_t                                    ncache_zones;
    const ngx_config_lib_cache_zone_conf_t       *cache_zones;
    ngx_uint_t                                    nupstreams;
    const ngx_config_lib_upstream_conf_t         *upstreams;
    ngx_uint_t                                    nrate_limits;
    const ngx_config_lib_rate_limit_conf_t       *rate_limits;
    ngx_config_lib_tls_conf_t                     tls;
    ngx_uint_t                                    nsocket_listeners;
    const ngx_config_lib_socket_listener_conf_t  *socket_listeners;
    ngx_uint_t                                    nhttp_servers;
    const ngx_config_lib_http_server_conf_t      *http_servers;
} ngx_config_lib_update_t;

ngx_int_t ngx_config_lib_init(ngx_config_lib_t *lib,
    const ngx_config_lib_options_t *options, u_char *err, size_t errlen);
ngx_int_t ngx_config_lib_reload(ngx_config_lib_t *lib,
    const ngx_config_lib_update_t *update, u_char *err, size_t errlen);
const ngx_config_lib_conf_t *ngx_config_lib_get(const ngx_config_lib_t *lib);
ngx_uint_t ngx_config_lib_generation(const ngx_config_lib_t *lib);
void ngx_config_lib_close(ngx_config_lib_t *lib);

#endif /* _NGX_CONFIG_LIB_H_INCLUDED_ */
