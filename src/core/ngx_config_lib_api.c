#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_config_lib_internal.h>

ngx_int_t
ngx_config_lib_init(ngx_config_lib_t *lib,
    const ngx_config_lib_options_t *options, u_char *err, size_t errlen)
{
    ngx_memzero(lib, sizeof(ngx_config_lib_t));

    ngx_debug_init();

    if (ngx_strerror_init() != NGX_OK) {
        ngx_config_lib_set_error(err, errlen, "ngx_strerror_init() failed");
        return NGX_ERROR;
    }

    ngx_time_init();

#if (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();
    ngx_process = NGX_PROCESS_SINGLE;

    lib->log = ngx_log_init((u_char *) (options ? options->prefix : NULL),
                            (u_char *) (options ? options->error_log : NULL));
    if (lib->log == NULL) {
        ngx_config_lib_set_error(err, errlen, "ngx_log_init() failed");
        return NGX_ERROR;
    }

    lib->pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, lib->log);
    if (lib->pool == NULL) {
        ngx_config_lib_set_error(err, errlen, "ngx_create_pool() failed");
        return NGX_ERROR;
    }

    lib->initialized = 1;

    return NGX_OK;
}

ngx_int_t
ngx_config_lib_reload(ngx_config_lib_t *lib, const ngx_config_lib_update_t *update,
    u_char *err, size_t errlen)
{
    ngx_pool_t            *pool;
    ngx_config_lib_conf_t *snapshot;

    if (!lib->initialized) {
        ngx_config_lib_set_error(err, errlen, "library is not initialized");
        return NGX_ERROR;
    }

    if (ngx_config_lib_validate(update, err, errlen) != NGX_OK) {
        return NGX_ERROR;
    }

    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, lib->log);
    if (pool == NULL) {
        ngx_config_lib_set_error(err, errlen,
                                 "failed to create snapshot pool");
        return NGX_ERROR;
    }

    snapshot = ngx_pcalloc(pool, sizeof(ngx_config_lib_conf_t));
    if (snapshot == NULL) {
        ngx_destroy_pool(pool);
        ngx_config_lib_set_error(err, errlen, "failed to allocate snapshot");
        return NGX_ERROR;
    }

    snapshot->main = update->main;
    snapshot->tls = update->tls;

    if (ngx_config_lib_copy_str(pool, &snapshot->tls.certificate,
                                &update->tls.certificate)
        != NGX_OK
        || ngx_config_lib_copy_str(pool, &snapshot->tls.certificate_key,
                                   &update->tls.certificate_key)
           != NGX_OK
        || ngx_config_lib_copy_str(pool, &snapshot->tls.ciphers,
                                   &update->tls.ciphers)
           != NGX_OK
        || ngx_config_lib_copy_str(pool, &snapshot->tls.protocols,
                                   &update->tls.protocols)
           != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ngx_config_lib_set_error(err, errlen, "failed to copy tls settings");
        return NGX_ERROR;
    }

    if (ngx_config_lib_copy_cache_zones(pool, snapshot, update, err, errlen)
        != NGX_OK
        || ngx_config_lib_copy_upstreams(pool, snapshot, update, err, errlen)
           != NGX_OK
        || ngx_config_lib_copy_rate_limits(pool, snapshot, update, err, errlen)
           != NGX_OK
        || ngx_config_lib_copy_socket_listeners(pool, snapshot, update,
                                                err, errlen)
           != NGX_OK
        || ngx_config_lib_copy_http_servers(pool, snapshot, update,
                                            err, errlen)
           != NGX_OK)
    {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    if (lib->active_pool != NULL) {
        ngx_destroy_pool(lib->active_pool);
    }

    lib->active = snapshot;
    lib->active_pool = pool;
    lib->generation++;

    return NGX_OK;
}

const ngx_config_lib_conf_t *
ngx_config_lib_get(const ngx_config_lib_t *lib)
{
    return lib->active;
}

ngx_uint_t
ngx_config_lib_generation(const ngx_config_lib_t *lib)
{
    return lib->generation;
}

void
ngx_config_lib_close(ngx_config_lib_t *lib)
{
    if (lib->active_pool) {
        ngx_destroy_pool(lib->active_pool);
    }

    if (lib->pool) {
        ngx_destroy_pool(lib->pool);
    }

    ngx_memzero(lib, sizeof(ngx_config_lib_t));
}
