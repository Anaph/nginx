#ifndef _NGX_CONFIG_LIB_CACHE_H_INCLUDED_
#define _NGX_CONFIG_LIB_CACHE_H_INCLUDED_

#include <ngx_config.h>
#include <ngx_core.h>

typedef struct {
    ngx_str_t   zone_name;
    size_t      max_size;
    ngx_uint_t  inactive_seconds;
    ngx_flag_t  use_stale;
} ngx_config_lib_cache_zone_conf_t;

#endif /* _NGX_CONFIG_LIB_CACHE_H_INCLUDED_ */
