#ifndef _NGX_CONFIG_LIB_INTERNAL_H_INCLUDED_
#define _NGX_CONFIG_LIB_INTERNAL_H_INCLUDED_

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_config_lib.h>

void ngx_config_lib_set_error(u_char *err, size_t errlen, const char *message);
ngx_int_t ngx_config_lib_copy_str(ngx_pool_t *pool, ngx_str_t *dst,
    const ngx_str_t *src);

ngx_int_t ngx_config_lib_validate(const ngx_config_lib_update_t *update,
    u_char *err, size_t errlen);

ngx_int_t ngx_config_lib_copy_cache_zones(ngx_pool_t *pool,
    ngx_config_lib_conf_t *dst, const ngx_config_lib_update_t *src, u_char *err,
    size_t errlen);
ngx_int_t ngx_config_lib_copy_upstreams(ngx_pool_t *pool,
    ngx_config_lib_conf_t *dst, const ngx_config_lib_update_t *src, u_char *err,
    size_t errlen);
ngx_int_t ngx_config_lib_copy_rate_limits(ngx_pool_t *pool,
    ngx_config_lib_conf_t *dst, const ngx_config_lib_update_t *src, u_char *err,
    size_t errlen);
ngx_int_t ngx_config_lib_copy_socket_listeners(ngx_pool_t *pool,
    ngx_config_lib_conf_t *dst, const ngx_config_lib_update_t *src, u_char *err,
    size_t errlen);
ngx_int_t ngx_config_lib_copy_http_servers(ngx_pool_t *pool,
    ngx_config_lib_conf_t *dst, const ngx_config_lib_update_t *src, u_char *err,
    size_t errlen);

#endif /* _NGX_CONFIG_LIB_INTERNAL_H_INCLUDED_ */
