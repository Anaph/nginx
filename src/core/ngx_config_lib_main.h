#ifndef _NGX_CONFIG_LIB_MAIN_H_INCLUDED_
#define _NGX_CONFIG_LIB_MAIN_H_INCLUDED_

#include <ngx_config.h>
#include <ngx_core.h>

typedef struct {
    ngx_uint_t  worker_processes;
    ngx_flag_t  daemon;
    ngx_flag_t  master_process;
    ngx_uint_t  worker_connections;
} ngx_config_lib_main_conf_t;

#endif /* _NGX_CONFIG_LIB_MAIN_H_INCLUDED_ */
