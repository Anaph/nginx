#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_config_lib_internal.h>

ngx_int_t
ngx_config_lib_validate(const ngx_config_lib_update_t *update, u_char *err,
    size_t errlen)
{
    ngx_uint_t  i, j;

    if (update == NULL) {
        ngx_config_lib_set_error(err, errlen, "update must not be NULL");
        return NGX_ERROR;
    }

    if (update->main.worker_processes == 0
        || update->main.worker_connections == 0)
    {
        ngx_config_lib_set_error(err, errlen,
                                 "worker settings must be greater than 0");
        return NGX_ERROR;
    }

    if (update->ncache_zones && update->cache_zones == NULL) {
        ngx_config_lib_set_error(err, errlen, "cache_zones must not be NULL");
        return NGX_ERROR;
    }

    if (update->nupstreams && update->upstreams == NULL) {
        ngx_config_lib_set_error(err, errlen, "upstreams must not be NULL");
        return NGX_ERROR;
    }

    if (update->nrate_limits && update->rate_limits == NULL) {
        ngx_config_lib_set_error(err, errlen, "rate_limits must not be NULL");
        return NGX_ERROR;
    }

    if (update->nsocket_listeners && update->socket_listeners == NULL) {
        ngx_config_lib_set_error(err, errlen,
                                 "socket_listeners must not be NULL");
        return NGX_ERROR;
    }

    if (update->nhttp_servers && update->http_servers == NULL) {
        ngx_config_lib_set_error(err, errlen, "http_servers must not be NULL");
        return NGX_ERROR;
    }

    if (update->tls.enabled) {
        if (update->tls.certificate.len == 0
            || update->tls.certificate_key.len == 0)
        {
            ngx_config_lib_set_error(err, errlen,
                                     "TLS enabled requires certificate and key");
            return NGX_ERROR;
        }
    }

    for (i = 0; i < update->nupstreams; i++) {
        if (update->upstreams[i].npeers == 0 || update->upstreams[i].peers == NULL) {
            ngx_config_lib_set_error(err, errlen,
                                     "upstream must have at least one peer");
            return NGX_ERROR;
        }

        for (j = 0; j < update->upstreams[i].npeers; j++) {
            if (update->upstreams[i].peers[j].port == 0
                || update->upstreams[i].peers[j].port > 65535)
            {
                ngx_config_lib_set_error(err, errlen,
                                         "upstream peer has invalid port");
                return NGX_ERROR;
            }
        }
    }

    for (i = 0; i < update->nsocket_listeners; i++) {
        if (update->socket_listeners[i].port == 0
            || update->socket_listeners[i].port > 65535)
        {
            ngx_config_lib_set_error(err, errlen,
                                     "socket listener has invalid port");
            return NGX_ERROR;
        }
    }

    for (i = 0; i < update->nhttp_servers; i++) {
        if (update->http_servers[i].listen_port == 0
            || update->http_servers[i].listen_port > 65535)
        {
            ngx_config_lib_set_error(err, errlen,
                                     "http server has invalid port");
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}
