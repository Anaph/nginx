#include <assert.h>

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_config_lib.h>

static ngx_str_t
str(const char *s)
{
    ngx_str_t v;

    v.len = ngx_strlen(s);
    v.data = (u_char *) s;

    return v;
}

int
main(void)
{
    ngx_config_lib_t                    lib;
    ngx_config_lib_options_t            options;
    ngx_config_lib_cache_zone_conf_t    cache_zones[1];
    ngx_config_lib_upstream_peer_conf_t peers[2];
    ngx_config_lib_upstream_conf_t      upstreams[1];
    ngx_config_lib_rate_limit_conf_t    rate_limits[1];
    ngx_config_lib_socket_listener_conf_t listeners[3];
    ngx_config_lib_http_server_conf_t   servers[2];
    ngx_config_lib_update_t             update;
    ngx_config_lib_update_t             bad_tls_update;
    const ngx_config_lib_conf_t        *cfg;
    u_char                              err[256];

    ngx_memzero(&options, sizeof(options));
    assert(ngx_config_lib_init(&lib, &options, err, sizeof(err)) == NGX_OK);

    cache_zones[0].zone_name = str("asset_cache");
    cache_zones[0].max_size = 512 * 1024 * 1024;
    cache_zones[0].inactive_seconds = 3600;
    cache_zones[0].use_stale = 1;

    peers[0].addr = str("10.0.0.11");
    peers[0].port = 8080;
    peers[0].weight = 3;
    peers[0].max_fails = 3;
    peers[0].fail_timeout_seconds = 5;

    peers[1].addr = str("10.0.0.12");
    peers[1].port = 8080;
    peers[1].weight = 2;
    peers[1].backup = 1;

    upstreams[0].name = str("app_pool");
    upstreams[0].method = NGX_CONFIG_LIB_LB_LEAST_CONN;
    upstreams[0].npeers = 2;
    upstreams[0].peers = peers;

    rate_limits[0].zone_name = str("api_limit");
    rate_limits[0].rate_rps = 100;
    rate_limits[0].burst = 40;
    rate_limits[0].nodelay = 1;

    listeners[0].protocol = NGX_CONFIG_LIB_SOCKET_WEBSOCKET;
    listeners[0].address = str("0.0.0.0");
    listeners[0].port = 9001;
    listeners[0].upstream = str("app_pool");

    listeners[1].protocol = NGX_CONFIG_LIB_SOCKET_WIREGUARD;
    listeners[1].address = str("0.0.0.0");
    listeners[1].port = 51820;
    listeners[1].upstream = str("vpn_backend");

    listeners[2].protocol = NGX_CONFIG_LIB_SOCKET_RTSP;
    listeners[2].address = str("0.0.0.0");
    listeners[2].port = 8554;
    listeners[2].upstream = str("stream_backend");

    servers[0].listen_address = str("0.0.0.0");
    servers[0].listen_port = 443;
    servers[0].root = str("/srv/www");
    servers[0].upstream = str("app_pool");
    servers[0].cache_zone = str("asset_cache");
    servers[0].rate_limit_zone = str("api_limit");
    servers[0].websocket_upgrade = 1;

    servers[1].listen_address = str("0.0.0.0");
    servers[1].listen_port = 8443;
    servers[1].root = str("/srv/rtsp");
    servers[1].upstream = str("stream_backend");
    servers[1].cache_zone = str("asset_cache");
    servers[1].rate_limit_zone = str("api_limit");
    servers[1].rtsp_proxy = 1;

    ngx_memzero(&update, sizeof(update));
    update.main.worker_processes = 4;
    update.main.worker_connections = 8192;
    update.main.daemon = 0;
    update.main.master_process = 1;
    update.ncache_zones = 1;
    update.cache_zones = cache_zones;
    update.nupstreams = 1;
    update.upstreams = upstreams;
    update.nrate_limits = 1;
    update.rate_limits = rate_limits;
    update.tls.enabled = 1;
    update.tls.certificate = str("/etc/nginx/tls/fullchain.pem");
    update.tls.certificate_key = str("/etc/nginx/tls/key.pem");
    update.tls.ciphers = str("HIGH:!aNULL:!MD5");
    update.tls.protocols = str("TLSv1.2 TLSv1.3");
    update.tls.session_timeout_seconds = 600;
    update.nsocket_listeners = 3;
    update.socket_listeners = listeners;
    update.nhttp_servers = 2;
    update.http_servers = servers;

    assert(ngx_config_lib_reload(&lib, &update, err, sizeof(err)) == NGX_OK);
    assert(ngx_config_lib_generation(&lib) == 1);

    cfg = ngx_config_lib_get(&lib);
    assert(cfg != NULL);
    assert(cfg->ncache_zones == 1);
    assert(cfg->nupstreams == 1);
    assert(cfg->upstreams[0].method == NGX_CONFIG_LIB_LB_LEAST_CONN);
    assert(cfg->nrate_limits == 1);
    assert(cfg->tls.enabled == 1);
    assert(cfg->nsocket_listeners == 3);
    assert(cfg->socket_listeners[1].protocol == NGX_CONFIG_LIB_SOCKET_WIREGUARD);
    assert(cfg->nhttp_servers == 2);
    assert(cfg->http_servers[0].websocket_upgrade == 1);

    bad_tls_update = update;
    bad_tls_update.tls.certificate.len = 0;

    assert(ngx_config_lib_reload(&lib, &bad_tls_update, err, sizeof(err))
           == NGX_ERROR);
    assert(ngx_config_lib_generation(&lib) == 1);

    ngx_config_lib_close(&lib);

    return 0;
}
