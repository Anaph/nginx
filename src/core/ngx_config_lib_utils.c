#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_config_lib_internal.h>

void
ngx_config_lib_set_error(u_char *err, size_t errlen, const char *message)
{
    if (err == NULL || errlen == 0) {
        return;
    }

    ngx_cpystrn(err, (u_char *) message, errlen);
}

ngx_int_t
ngx_config_lib_copy_str(ngx_pool_t *pool, ngx_str_t *dst, const ngx_str_t *src)
{
    if (src->len == 0 || src->data == NULL) {
        dst->len = 0;
        dst->data = NULL;
        return NGX_OK;
    }

    dst->data = ngx_pnalloc(pool, src->len);
    if (dst->data == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(dst->data, src->data, src->len);
    dst->len = src->len;

    return NGX_OK;
}
