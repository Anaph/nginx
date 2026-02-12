#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_config_lib_internal.h>

ngx_int_t
ngx_config_lib_copy_cache_zones(ngx_pool_t *pool, ngx_config_lib_conf_t *dst,
    const ngx_config_lib_update_t *src, u_char *err, size_t errlen)
{
    ngx_uint_t                        i;
    ngx_config_lib_cache_zone_conf_t *zones;

    if (src->ncache_zones == 0) {
        return NGX_OK;
    }

    zones = ngx_pcalloc(pool,
                        sizeof(ngx_config_lib_cache_zone_conf_t)
                        * src->ncache_zones);
    if (zones == NULL) {
        ngx_config_lib_set_error(err, errlen, "failed to allocate cache zones");
        return NGX_ERROR;
    }

    for (i = 0; i < src->ncache_zones; i++) {
        zones[i] = src->cache_zones[i];

        if (ngx_config_lib_copy_str(pool, &zones[i].zone_name,
                                    &src->cache_zones[i].zone_name)
            != NGX_OK)
        {
            ngx_config_lib_set_error(err, errlen, "failed to copy cache zone");
            return NGX_ERROR;
        }
    }

    dst->cache_zones = zones;
    dst->ncache_zones = src->ncache_zones;

    return NGX_OK;
}

ngx_int_t
ngx_config_lib_copy_upstreams(ngx_pool_t *pool, ngx_config_lib_conf_t *dst,
    const ngx_config_lib_update_t *src, u_char *err, size_t errlen)
{
    ngx_uint_t                           i, j;
    ngx_config_lib_upstream_conf_t      *upstreams;
    ngx_config_lib_upstream_peer_conf_t *peers;

    if (src->nupstreams == 0) {
        return NGX_OK;
    }

    upstreams = ngx_pcalloc(pool,
                            sizeof(ngx_config_lib_upstream_conf_t)
                            * src->nupstreams);
    if (upstreams == NULL) {
        ngx_config_lib_set_error(err, errlen, "failed to allocate upstreams");
        return NGX_ERROR;
    }

    for (i = 0; i < src->nupstreams; i++) {
        upstreams[i] = src->upstreams[i];

        if (ngx_config_lib_copy_str(pool, &upstreams[i].name,
                                    &src->upstreams[i].name)
            != NGX_OK)
        {
            ngx_config_lib_set_error(err, errlen, "failed to copy upstream name");
            return NGX_ERROR;
        }

        peers = ngx_pcalloc(pool,
                            sizeof(ngx_config_lib_upstream_peer_conf_t)
                            * src->upstreams[i].npeers);
        if (peers == NULL) {
            ngx_config_lib_set_error(err, errlen,
                                     "failed to allocate upstream peers");
            return NGX_ERROR;
        }

        for (j = 0; j < src->upstreams[i].npeers; j++) {
            peers[j] = src->upstreams[i].peers[j];

            if (ngx_config_lib_copy_str(pool, &peers[j].addr,
                                        &src->upstreams[i].peers[j].addr)
                != NGX_OK)
            {
                ngx_config_lib_set_error(err, errlen,
                                         "failed to copy upstream peer address");
                return NGX_ERROR;
            }
        }

        upstreams[i].peers = peers;
    }

    dst->upstreams = upstreams;
    dst->nupstreams = src->nupstreams;

    return NGX_OK;
}

ngx_int_t
ngx_config_lib_copy_rate_limits(ngx_pool_t *pool, ngx_config_lib_conf_t *dst,
    const ngx_config_lib_update_t *src, u_char *err, size_t errlen)
{
    ngx_uint_t                       i;
    ngx_config_lib_rate_limit_conf_t *limits;

    if (src->nrate_limits == 0) {
        return NGX_OK;
    }

    limits = ngx_pcalloc(pool,
                         sizeof(ngx_config_lib_rate_limit_conf_t)
                         * src->nrate_limits);
    if (limits == NULL) {
        ngx_config_lib_set_error(err, errlen,
                                 "failed to allocate rate limits");
        return NGX_ERROR;
    }

    for (i = 0; i < src->nrate_limits; i++) {
        limits[i] = src->rate_limits[i];

        if (ngx_config_lib_copy_str(pool, &limits[i].zone_name,
                                    &src->rate_limits[i].zone_name)
            != NGX_OK)
        {
            ngx_config_lib_set_error(err, errlen,
                                     "failed to copy rate limit zone");
            return NGX_ERROR;
        }
    }

    dst->rate_limits = limits;
    dst->nrate_limits = src->nrate_limits;

    return NGX_OK;
}

ngx_int_t
ngx_config_lib_copy_socket_listeners(ngx_pool_t *pool, ngx_config_lib_conf_t *dst,
    const ngx_config_lib_update_t *src, u_char *err, size_t errlen)
{
    ngx_uint_t                            i;
    ngx_config_lib_socket_listener_conf_t *listeners;

    if (src->nsocket_listeners == 0) {
        return NGX_OK;
    }

    listeners = ngx_pcalloc(pool,
                            sizeof(ngx_config_lib_socket_listener_conf_t)
                            * src->nsocket_listeners);
    if (listeners == NULL) {
        ngx_config_lib_set_error(err, errlen,
                                 "failed to allocate socket listeners");
        return NGX_ERROR;
    }

    for (i = 0; i < src->nsocket_listeners; i++) {
        listeners[i] = src->socket_listeners[i];

        if (ngx_config_lib_copy_str(pool, &listeners[i].address,
                                    &src->socket_listeners[i].address)
            != NGX_OK
            || ngx_config_lib_copy_str(pool, &listeners[i].upstream,
                                       &src->socket_listeners[i].upstream)
               != NGX_OK)
        {
            ngx_config_lib_set_error(err, errlen,
                                     "failed to copy socket listener");
            return NGX_ERROR;
        }
    }

    dst->socket_listeners = listeners;
    dst->nsocket_listeners = src->nsocket_listeners;

    return NGX_OK;
}

ngx_int_t
ngx_config_lib_copy_http_servers(ngx_pool_t *pool, ngx_config_lib_conf_t *dst,
    const ngx_config_lib_update_t *src, u_char *err, size_t errlen)
{
    ngx_uint_t                        i;
    ngx_config_lib_http_server_conf_t *servers;

    if (src->nhttp_servers == 0) {
        return NGX_OK;
    }

    servers = ngx_pcalloc(pool,
                          sizeof(ngx_config_lib_http_server_conf_t)
                          * src->nhttp_servers);
    if (servers == NULL) {
        ngx_config_lib_set_error(err, errlen,
                                 "failed to allocate http servers");
        return NGX_ERROR;
    }

    for (i = 0; i < src->nhttp_servers; i++) {
        servers[i] = src->http_servers[i];

        if (ngx_config_lib_copy_str(pool, &servers[i].listen_address,
                                    &src->http_servers[i].listen_address)
            != NGX_OK
            || ngx_config_lib_copy_str(pool, &servers[i].root,
                                       &src->http_servers[i].root)
               != NGX_OK
            || ngx_config_lib_copy_str(pool, &servers[i].upstream,
                                       &src->http_servers[i].upstream)
               != NGX_OK
            || ngx_config_lib_copy_str(pool, &servers[i].cache_zone,
                                       &src->http_servers[i].cache_zone)
               != NGX_OK
            || ngx_config_lib_copy_str(pool, &servers[i].rate_limit_zone,
                                       &src->http_servers[i].rate_limit_zone)
               != NGX_OK)
        {
            ngx_config_lib_set_error(err, errlen,
                                     "failed to copy http server config");
            return NGX_ERROR;
        }
    }

    dst->http_servers = servers;
    dst->nhttp_servers = src->nhttp_servers;

    return NGX_OK;
}
