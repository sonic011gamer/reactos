/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */

#include <wine/config.h>
#include <wine/port.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <windef.h>
#include <winbase.h>
#include <ntsecapi.h>
#include <bcrypt.h>

#include <wine/debug.h>
#include <wine/unicode.h>
#include <wine/library.h>

#ifdef SONAME_LIBMBEDTLS
#include <mbedtls/md.h>
#include <mbedtls/md5.h>
#include <mbedtls/sha1.h>
#include <mbedtls/sha256.h>
#include <mbedtls/sha512.h>
#endif

#include "bcrypt_internal.h"

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

static HINSTANCE instance;

#if defined(HAVE_GNUTLS_HASH) && !defined(HAVE_COMMONCRYPTO_COMMONDIGEST_H)
WINE_DECLARE_DEBUG_CHANNEL(winediag);

static void *libgnutls_handle;
#define MAKE_FUNCPTR(f) static typeof(f) * p##f
MAKE_FUNCPTR(gnutls_global_deinit);
MAKE_FUNCPTR(gnutls_global_init);
MAKE_FUNCPTR(gnutls_global_set_log_function);
MAKE_FUNCPTR(gnutls_global_set_log_level);
MAKE_FUNCPTR(gnutls_hash);
MAKE_FUNCPTR(gnutls_hash_deinit);
MAKE_FUNCPTR(gnutls_hash_init);
MAKE_FUNCPTR(gnutls_hmac);
MAKE_FUNCPTR(gnutls_hmac_deinit);
MAKE_FUNCPTR(gnutls_hmac_init);
MAKE_FUNCPTR(gnutls_perror);
#undef MAKE_FUNCPTR

static void gnutls_log( int level, const char *msg )
{
    TRACE( "<%d> %s", level, msg );
}

static BOOL gnutls_initialize(void)
{
    int ret;

    if (!(libgnutls_handle = wine_dlopen( SONAME_LIBGNUTLS, RTLD_NOW, NULL, 0 )))
    {
        ERR_(winediag)( "failed to load libgnutls, no support for crypto hashes\n" );
        return FALSE;
    }

#define LOAD_FUNCPTR(f) \
    if (!(p##f = wine_dlsym( libgnutls_handle, #f, NULL, 0 ))) \
    { \
        ERR( "failed to load %s\n", #f ); \
        goto fail; \
    }

    LOAD_FUNCPTR(gnutls_global_deinit)
    LOAD_FUNCPTR(gnutls_global_init)
    LOAD_FUNCPTR(gnutls_global_set_log_function)
    LOAD_FUNCPTR(gnutls_global_set_log_level)
    LOAD_FUNCPTR(gnutls_hash);
    LOAD_FUNCPTR(gnutls_hash_deinit);
    LOAD_FUNCPTR(gnutls_hash_init);
    LOAD_FUNCPTR(gnutls_hmac);
    LOAD_FUNCPTR(gnutls_hmac_deinit);
    LOAD_FUNCPTR(gnutls_hmac_init);
    LOAD_FUNCPTR(gnutls_perror)
#undef LOAD_FUNCPTR

    if ((ret = pgnutls_global_init()) != GNUTLS_E_SUCCESS)
    {
        pgnutls_perror( ret );
        goto fail;
    }

    if (TRACE_ON( bcrypt ))
    {
        pgnutls_global_set_log_level( 4 );
        pgnutls_global_set_log_function( gnutls_log );
    }

    return TRUE;

fail:
    wine_dlclose( libgnutls_handle, NULL, 0 );
    libgnutls_handle = NULL;
    return FALSE;
}

static void gnutls_uninitialize(void)
{
    pgnutls_global_deinit();
    wine_dlclose( libgnutls_handle, NULL, 0 );
    libgnutls_handle = NULL;
}
#elif defined(SONAME_LIBMBEDTLS) && !defined(HAVE_COMMONCRYPTO_COMMONDIGEST_H) && !defined(__REACTOS__)
WINE_DECLARE_DEBUG_CHANNEL(winediag);

void *libmbedtls_handle;

#define MAKE_FUNCPTR(f) static typeof(f) * p##f
MAKE_FUNCPTR(mbedtls_md_init);
MAKE_FUNCPTR(mbedtls_md_setup);
MAKE_FUNCPTR(mbedtls_md_update);
MAKE_FUNCPTR(mbedtls_md_hmac_starts);
MAKE_FUNCPTR(mbedtls_md_hmac_finish);
MAKE_FUNCPTR(mbedtls_md_free);
MAKE_FUNCPTR(mbedtls_md5_init);
MAKE_FUNCPTR(mbedtls_md5_starts);
MAKE_FUNCPTR(mbedtls_md5_update);
MAKE_FUNCPTR(mbedtls_md5_finish);
MAKE_FUNCPTR(mbedtls_md5_free);
MAKE_FUNCPTR(mbedtls_sha1_init);
MAKE_FUNCPTR(mbedtls_sha1_starts);
MAKE_FUNCPTR(mbedtls_sha1_update);
MAKE_FUNCPTR(mbedtls_sha1_finish);
MAKE_FUNCPTR(mbedtls_sha1_free);
MAKE_FUNCPTR(mbedtls_sha256_init);
MAKE_FUNCPTR(mbedtls_sha256_starts);
MAKE_FUNCPTR(mbedtls_sha256_update);
MAKE_FUNCPTR(mbedtls_sha256_finish);
MAKE_FUNCPTR(mbedtls_sha256_free);
MAKE_FUNCPTR(mbedtls_sha512_init);
MAKE_FUNCPTR(mbedtls_sha512_starts);
MAKE_FUNCPTR(mbedtls_sha512_update);
MAKE_FUNCPTR(mbedtls_sha512_finish);
MAKE_FUNCPTR(mbedtls_sha512_free);
#undef MAKE_FUNCPTR

#define mbedtls_md_init             pmbedtls_md_init
#define mbedtls_md_setup            pmbedtls_md_setup
#define mbedtls_md_update           pmbedtls_md_update
#define mbedtls_md_hmac_starts      pmbedtls_md_hmac_starts
#define mbedtls_md_hmac_finish      pmbedtls_md_hmac_finish
#define mbedtls_md_free             pmbedtls_md_free
#define mbedtls_md5_init            pmbedtls_md5_init
#define mbedtls_md5_starts          pmbedtls_md5_starts
#define mbedtls_md5_update          pmbedtls_md5_update
#define mbedtls_md5_finish          pmbedtls_md5_finish
#define mbedtls_md5_free            pmbedtls_md5_free
#define mbedtls_sha1_init           pmbedtls_sha1_init
#define mbedtls_sha1_starts         pmbedtls_sha1_starts
#define mbedtls_sha1_update         pmbedtls_sha1_update
#define mbedtls_sha1_finish         pmbedtls_sha1_finish
#define mbedtls_sha1_free           pmbedtls_sha1_free
#define mbedtls_sha256_init         pmbedtls_sha256_init
#define mbedtls_sha256_starts       pmbedtls_sha256_starts
#define mbedtls_sha256_update       pmbedtls_sha256_update
#define mbedtls_sha256_finish       pmbedtls_sha256_finish
#define mbedtls_sha256_free         pmbedtls_sha256_free
#define mbedtls_sha512_init         pmbedtls_sha512_init
#define mbedtls_sha512_starts       pmbedtls_sha512_starts
#define mbedtls_sha512_update       pmbedtls_sha512_update
#define mbedtls_sha512_finish       pmbedtls_sha512_finish
#define mbedtls_sha512_free         pmbedtls_sha512_free

static BOOL mbedtls_initialize(void)
{
    if (!(libmbedtls_handle = wine_dlopen( SONAME_LIBMBEDTLS, RTLD_NOW, NULL, 0 )))
    {
        ERR_(winediag)( "failed to load libmbedtls, no support for crypto hashes\n" );
        return FALSE;
    }

#define LOAD_FUNCPTR(f) \
    if (!(p##f = wine_dlsym( libmbedtls_handle, #f, NULL, 0 ))) \
    { \
        ERR( "failed to load %s\n", #f ); \
        goto fail; \
    }

    LOAD_FUNCPTR(mbedtls_md_init)
    LOAD_FUNCPTR(mbedtls_md_setup)
    LOAD_FUNCPTR(mbedtls_md_update)
    LOAD_FUNCPTR(mbedtls_md_hmac_starts)
    LOAD_FUNCPTR(mbedtls_md_hmac_finish)
    LOAD_FUNCPTR(mbedtls_md_free);
    LOAD_FUNCPTR(mbedtls_md5_init)
    LOAD_FUNCPTR(mbedtls_md5_starts)
    LOAD_FUNCPTR(mbedtls_md5_update)
    LOAD_FUNCPTR(mbedtls_md5_finish)
    LOAD_FUNCPTR(mbedtls_md5_free);
    LOAD_FUNCPTR(mbedtls_sha1_init)
    LOAD_FUNCPTR(mbedtls_sha1_starts)
    LOAD_FUNCPTR(mbedtls_sha1_update)
    LOAD_FUNCPTR(mbedtls_sha1_finish)
    LOAD_FUNCPTR(mbedtls_sha1_free);
    LOAD_FUNCPTR(mbedtls_sha256_init)
    LOAD_FUNCPTR(mbedtls_sha256_starts)
    LOAD_FUNCPTR(mbedtls_sha256_update)
    LOAD_FUNCPTR(mbedtls_sha256_finish)
    LOAD_FUNCPTR(mbedtls_sha256_free);
    LOAD_FUNCPTR(mbedtls_sha512_init)
    LOAD_FUNCPTR(mbedtls_sha512_starts)
    LOAD_FUNCPTR(mbedtls_sha512_update)
    LOAD_FUNCPTR(mbedtls_sha512_finish)
    LOAD_FUNCPTR(mbedtls_sha512_free);
#undef LOAD_FUNCPTR

    return TRUE;

fail:
    wine_dlclose( libmbedtls_handle, NULL, 0 );
    libmbedtls_handle = NULL;
    return FALSE;
}

static void mbedtls_uninitialize(void)
{
    wine_dlclose( libmbedtls_handle, NULL, 0 );
    libmbedtls_handle = NULL;
}
#endif /* SONAME_LIBMBEDTLS && !HAVE_COMMONCRYPTO_COMMONDIGEST_H && !__REACTOS__ */


/* ordered by class, keep in sync with enum alg_id */
static const struct
{
    const WCHAR *name;
    ULONG        class;
    ULONG        object_length;
    ULONG        hash_length;
    ULONG        block_bits;
}
builtin_algorithms[] =
{
    {  BCRYPT_3DES_ALGORITHM,       BCRYPT_CIPHER_INTERFACE,                522,    0,    0 },
    {  BCRYPT_AES_ALGORITHM,        BCRYPT_CIPHER_INTERFACE,                654,    0,    0 },
    {  BCRYPT_SHA256_ALGORITHM,     BCRYPT_HASH_INTERFACE,                  286,   32,  512 },
    {  BCRYPT_SHA384_ALGORITHM,     BCRYPT_HASH_INTERFACE,                  382,   48, 1024 },
    {  BCRYPT_SHA512_ALGORITHM,     BCRYPT_HASH_INTERFACE,                  382,   64, 1024 },
    {  BCRYPT_SHA1_ALGORITHM,       BCRYPT_HASH_INTERFACE,                  278,   20,  512 },
    {  BCRYPT_MD5_ALGORITHM,        BCRYPT_HASH_INTERFACE,                  274,   16,  512 },
    {  BCRYPT_MD4_ALGORITHM,        BCRYPT_HASH_INTERFACE,                  270,   16,  512 },
    {  BCRYPT_MD2_ALGORITHM,        BCRYPT_HASH_INTERFACE,                  270,   16,  128 },
    {  BCRYPT_RSA_ALGORITHM,        BCRYPT_ASYMMETRIC_ENCRYPTION_INTERFACE, 0,      0,    0 },
    {  BCRYPT_ECDH_P256_ALGORITHM,  BCRYPT_SECRET_AGREEMENT_INTERFACE,      0,      0,    0 },
    {  BCRYPT_ECDH_P384_ALGORITHM,  BCRYPT_SECRET_AGREEMENT_INTERFACE,      0,      0,    0 },
    {  BCRYPT_RSA_SIGN_ALGORITHM,   BCRYPT_SIGNATURE_INTERFACE,             0,      0,    0 },
    {  BCRYPT_ECDSA_P256_ALGORITHM, BCRYPT_SIGNATURE_INTERFACE,             0,      0,    0 },
    {  BCRYPT_ECDSA_P384_ALGORITHM, BCRYPT_SIGNATURE_INTERFACE,             0,      0,    0 },
    {  BCRYPT_DSA_ALGORITHM,        BCRYPT_SIGNATURE_INTERFACE,             0,      0,    0 },
    {  BCRYPT_RNG_ALGORITHM,        BCRYPT_RNG_INTERFACE,                   0,      0,    0 },
};

static inline BOOL is_symmetric_key( struct key *key )
{
    return builtin_algorithms[key->alg_id].class == BCRYPT_CIPHER_INTERFACE;
}

static inline BOOL is_asymmetric_encryption_key( struct key *key )
{
    return builtin_algorithms[key->alg_id].class == BCRYPT_ASYMMETRIC_ENCRYPTION_INTERFACE;
}

static inline BOOL is_agreement_key( struct key *key )
{
    return builtin_algorithms[key->alg_id].class == BCRYPT_SECRET_AGREEMENT_INTERFACE;
}

static inline BOOL is_signature_key( struct key *key )
{
    return builtin_algorithms[key->alg_id].class == BCRYPT_SIGNATURE_INTERFACE || key->alg_id == ALG_ID_RSA;
}


NTSTATUS WINAPI BCryptAddContextFunction( ULONG table, const WCHAR *ctx, ULONG iface, const WCHAR *func, ULONG pos )
{
    FIXME( "%#lx, %s, %#lx, %s, %lu: stub\n", table, debugstr_w(ctx), iface, debugstr_w(func), pos );
    return STATUS_SUCCESS;
}

NTSTATUS WINAPI BCryptAddContextFunctionProvider( ULONG table, const WCHAR *ctx, ULONG iface, const WCHAR *func,
                                                  const WCHAR *provider, ULONG pos )
{
    FIXME( "%#lx, %s, %#lx, %s, %s, %lu: stub\n", table, debugstr_w(ctx), iface, debugstr_w(func),
           debugstr_w(provider), pos );
    return STATUS_SUCCESS;
}

NTSTATUS WINAPI BCryptRemoveContextFunction( ULONG table, const WCHAR *ctx, ULONG iface, const WCHAR *func )
{
    FIXME( "%#lx, %s, %#lx, %s: stub\n", table, debugstr_w(ctx), iface, debugstr_w(func) );
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS WINAPI BCryptRemoveContextFunctionProvider( ULONG table, const WCHAR *ctx, ULONG iface, const WCHAR *func,
                                                     const WCHAR *provider )
{
    FIXME( "%#lx, %s, %#lx, %s, %s: stub\n", table, debugstr_w(ctx), iface, debugstr_w(func), debugstr_w(provider) );
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS WINAPI BCryptEnumContextFunctions( ULONG table, const WCHAR *ctx, ULONG iface, ULONG *buflen,
                                            CRYPT_CONTEXT_FUNCTIONS **buffer )
{
    FIXME( "%#lx, %s, %#lx, %p, %p\n", table, debugstr_w(ctx), iface, buflen, buffer );
    return STATUS_NOT_IMPLEMENTED;
}

void WINAPI BCryptFreeBuffer( void *buffer )
{
    free( buffer );
}

NTSTATUS WINAPI BCryptRegisterProvider( const WCHAR *provider, ULONG flags, CRYPT_PROVIDER_REG *reg )
{
    FIXME( "%s, %#lx, %p: stub\n", debugstr_w(provider), flags, reg );
    return STATUS_SUCCESS;
}

NTSTATUS WINAPI BCryptUnregisterProvider( const WCHAR *provider )
{
    FIXME( "%s: stub\n", debugstr_w(provider) );
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS WINAPI BCryptEnumAlgorithms(ULONG dwAlgOperations, ULONG *pAlgCount,
                                     BCRYPT_ALGORITHM_IDENTIFIER **ppAlgList, ULONG dwFlags)
{
    FIXME("%08x, %p, %p, %08x - stub\n", dwAlgOperations, pAlgCount, ppAlgList, dwFlags);

    *ppAlgList=NULL;
    *pAlgCount=0;

    return STATUS_NOT_IMPLEMENTED;
}


static const struct {
    ULONG hash_length;
    const WCHAR *alg_name;
} alg_props[] = {
    /* ALG_ID_MD5    */ { 16, BCRYPT_MD5_ALGORITHM },
    /* ALG_ID_RNG    */ {  0, BCRYPT_RNG_ALGORITHM },
    /* ALG_ID_SHA1   */ { 20, BCRYPT_SHA1_ALGORITHM },
    /* ALG_ID_SHA256 */ { 32, BCRYPT_SHA256_ALGORITHM },
    /* ALG_ID_SHA384 */ { 48, BCRYPT_SHA384_ALGORITHM },
    /* ALG_ID_SHA512 */ { 64, BCRYPT_SHA512_ALGORITHM },
    /* ALG_ID_ECDSA_P256 */ { 0, BCRYPT_ECDSA_P256_ALGORITHM },
    /* ALG_ID_ECDSA_P384 */ { 0, BCRYPT_ECDSA_P384_ALGORITHM },
};

static const struct algorithm pseudo_algorithms[] =
{
    {{ MAGIC_ALG }, ALG_ID_MD2 },
    {{ MAGIC_ALG }, ALG_ID_MD4 },
    {{ MAGIC_ALG }, ALG_ID_MD5 },
    {{ MAGIC_ALG }, ALG_ID_SHA1 },
    {{ MAGIC_ALG }, ALG_ID_SHA256 },
    {{ MAGIC_ALG }, ALG_ID_SHA384 },
    {{ MAGIC_ALG }, ALG_ID_SHA512 },
    {{ 0 }}, /* RC4 */
    {{ MAGIC_ALG }, ALG_ID_RNG },
    {{ MAGIC_ALG }, ALG_ID_MD5, 0, BCRYPT_ALG_HANDLE_HMAC_FLAG },
    {{ MAGIC_ALG }, ALG_ID_SHA1, 0, BCRYPT_ALG_HANDLE_HMAC_FLAG },
    {{ MAGIC_ALG }, ALG_ID_SHA256, 0, BCRYPT_ALG_HANDLE_HMAC_FLAG },
    {{ MAGIC_ALG }, ALG_ID_SHA384, 0, BCRYPT_ALG_HANDLE_HMAC_FLAG },
    {{ MAGIC_ALG }, ALG_ID_SHA512, 0, BCRYPT_ALG_HANDLE_HMAC_FLAG },
    {{ MAGIC_ALG }, ALG_ID_RSA },
    {{ 0 }}, /* ECDSA */
    {{ 0 }}, /* AES_CMAC */
    {{ 0 }}, /* AES_GMAC */
    {{ MAGIC_ALG }, ALG_ID_MD2, 0, BCRYPT_ALG_HANDLE_HMAC_FLAG },
    {{ MAGIC_ALG }, ALG_ID_MD4, 0, BCRYPT_ALG_HANDLE_HMAC_FLAG },
    {{ MAGIC_ALG }, ALG_ID_3DES, CHAIN_MODE_CBC },
    {{ MAGIC_ALG }, ALG_ID_3DES, CHAIN_MODE_ECB },
    {{ MAGIC_ALG }, ALG_ID_3DES, CHAIN_MODE_CFB },
    {{ 0 }}, /* 3DES_112_CBC */
    {{ 0 }}, /* 3DES_112_ECB */
    {{ 0 }}, /* 3DES_112_CFB */
    {{ MAGIC_ALG }, ALG_ID_AES, CHAIN_MODE_CBC },
    {{ MAGIC_ALG }, ALG_ID_AES, CHAIN_MODE_ECB },
    {{ MAGIC_ALG }, ALG_ID_AES, CHAIN_MODE_CFB },
    {{ MAGIC_ALG }, ALG_ID_AES, CHAIN_MODE_CCM },
    {{ MAGIC_ALG }, ALG_ID_AES, CHAIN_MODE_GCM },
    {{ 0 }}, /* DES_CBC */
    {{ 0 }}, /* DES_ECB */
    {{ 0 }}, /* DES_CFB */
    {{ 0 }}, /* DESX_CBC */
    {{ 0 }}, /* DESX_ECB */
    {{ 0 }}, /* DESX_CFB */
    {{ 0 }}, /* RC2_CBC */
    {{ 0 }}, /* RC2_ECB */
    {{ 0 }}, /* RC2_CFB */
    {{ 0 }}, /* DH */
    {{ 0 }}, /* ECDH */
    {{ MAGIC_ALG }, ALG_ID_ECDH_P256 },
    {{ MAGIC_ALG }, ALG_ID_ECDH_P384 },
    {{ 0 }}, /* ECDH_P512 */
    {{ MAGIC_ALG }, ALG_ID_DSA },
    {{ MAGIC_ALG }, ALG_ID_ECDSA_P256 },
    {{ MAGIC_ALG }, ALG_ID_ECDSA_P384 },
    {{ 0 }}, /* ECDSA_P512 */
    {{ MAGIC_ALG }, ALG_ID_RSA_SIGN },
};

/* Algorithm pseudo-handles are denoted by having the lowest bit set.
 * An aligned algorithm pointer will never have this bit set.
 */
static inline BOOL is_alg_pseudo_handle( BCRYPT_ALG_HANDLE handle )
{
    return (((ULONG_PTR)handle & 1) == 1);
}

static struct object *get_object( BCRYPT_HANDLE handle, ULONG magic )
{
    ULONG idx;

    if (!handle) return NULL;

    if (!is_alg_pseudo_handle( handle ))
    {
        struct object *obj = handle;
        if (magic && obj->magic != magic) return NULL;
        return obj;
    }

    idx = (ULONG_PTR)handle >> 4;
    if (idx > ARRAY_SIZE(pseudo_algorithms) || !pseudo_algorithms[idx].hdr.magic)
    {
        FIXME( "pseudo-handle %p not supported\n", handle );
        return NULL;
    }
    return (struct object *)&pseudo_algorithms[idx];
}

static inline struct algorithm *get_alg_object( BCRYPT_ALG_HANDLE handle )
{
    return (struct algorithm *)get_object( handle, MAGIC_ALG );
}

static inline struct hash *get_hash_object( BCRYPT_HASH_HANDLE handle )
{
    return (struct hash *)get_object( handle, MAGIC_HASH );
}

static inline struct key *get_key_object( BCRYPT_KEY_HANDLE handle )
{
    return (struct key *)get_object( handle, MAGIC_KEY );
}

static inline struct secret *get_secret_object( BCRYPT_SECRET_HANDLE handle )
{
    return (struct secret *)get_object( handle, MAGIC_SECRET );
}

NTSTATUS WINAPI BCryptGenRandom(BCRYPT_ALG_HANDLE handle, UCHAR *buffer, ULONG count, ULONG flags)
{
    const DWORD supported_flags = BCRYPT_USE_SYSTEM_PREFERRED_RNG;
    struct algorithm *algorithm = handle;

    TRACE("%p, %p, %u, %08x - semi-stub\n", handle, buffer, count, flags);

    if (!algorithm)
    {
        /* It's valid to call without an algorithm if BCRYPT_USE_SYSTEM_PREFERRED_RNG
         * is set. In this case the preferred system RNG is used.
         */
        if (!(flags & BCRYPT_USE_SYSTEM_PREFERRED_RNG))
            return STATUS_INVALID_HANDLE;
    }
    else if (algorithm->hdr.magic != MAGIC_ALG || algorithm->id != ALG_ID_RNG)
        return STATUS_INVALID_HANDLE;

    if (!buffer)
        return STATUS_INVALID_PARAMETER;

    if (flags & ~supported_flags)
        FIXME("unsupported flags %08x\n", flags & ~supported_flags);

    if (algorithm)
        FIXME("ignoring selected algorithm\n");

    /* When zero bytes are requested the function returns success too. */
    if (!count)
        return STATUS_SUCCESS;

    if (algorithm || (flags & BCRYPT_USE_SYSTEM_PREFERRED_RNG))
    {
        if (RtlGenRandom(buffer, count))
            return STATUS_SUCCESS;
    }

    FIXME("called with unsupported parameters, returning error\n");
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS WINAPI BCryptOpenAlgorithmProvider( BCRYPT_ALG_HANDLE *handle, LPCWSTR id, LPCWSTR implementation, DWORD flags )
{
    struct algorithm *alg;
    enum alg_id alg_id;

    const DWORD supported_flags = BCRYPT_ALG_HANDLE_HMAC_FLAG;

    TRACE( "%p, %s, %s, %08x\n", handle, wine_dbgstr_w(id), wine_dbgstr_w(implementation), flags );

    if (!handle || !id) return STATUS_INVALID_PARAMETER;
    if (flags & ~supported_flags)
    {
        FIXME( "unsupported flags %08x\n", flags & ~supported_flags);
        return STATUS_NOT_IMPLEMENTED;
    }

    if (!strcmpW( id, BCRYPT_SHA1_ALGORITHM )) alg_id = ALG_ID_SHA1;
    else if (!strcmpW( id, BCRYPT_MD5_ALGORITHM )) alg_id = ALG_ID_MD5;
    else if (!strcmpW( id, BCRYPT_RNG_ALGORITHM )) alg_id = ALG_ID_RNG;
    else if (!strcmpW( id, BCRYPT_SHA256_ALGORITHM )) alg_id = ALG_ID_SHA256;
    else if (!strcmpW( id, BCRYPT_SHA384_ALGORITHM )) alg_id = ALG_ID_SHA384;
    else if (!strcmpW( id, BCRYPT_SHA512_ALGORITHM )) alg_id = ALG_ID_SHA512;
    else if (!strcmpW( id, BCRYPT_ECDSA_P256_ALGORITHM )) alg_id = ALG_ID_ECDSA_P256;
    else if (!strcmpW( id, BCRYPT_ECDSA_P384_ALGORITHM )) alg_id = ALG_ID_ECDSA_P384;
    else
    {
        FIXME( "algorithm %s not supported\n", debugstr_w(id) );
        return STATUS_NOT_IMPLEMENTED;
    }
    if (implementation && strcmpW( implementation, MS_PRIMITIVE_PROVIDER ))
    {
        FIXME( "implementation %s not supported\n", debugstr_w(implementation) );
        return STATUS_NOT_IMPLEMENTED;
    }

    if (!(alg = HeapAlloc( GetProcessHeap(), 0, sizeof(*alg) ))) return STATUS_NO_MEMORY;
    alg->hdr.magic = MAGIC_ALG;
    alg->id        = alg_id;
    alg->hmac      = flags & BCRYPT_ALG_HANDLE_HMAC_FLAG;

    *handle = alg;
    return STATUS_SUCCESS;
}

NTSTATUS WINAPI BCryptCloseAlgorithmProvider( BCRYPT_ALG_HANDLE handle, DWORD flags )
{
    struct algorithm *alg = handle;

    TRACE( "%p, %08x\n", handle, flags );

    if (!alg || alg->hdr.magic != MAGIC_ALG) return STATUS_INVALID_HANDLE;
    HeapFree( GetProcessHeap(), 0, alg );
    return STATUS_SUCCESS;
}

NTSTATUS WINAPI BCryptGetFipsAlgorithmMode(BOOLEAN *enabled)
{
    FIXME("%p - semi-stub\n", enabled);

    if (!enabled)
        return STATUS_INVALID_PARAMETER;

    *enabled = FALSE;
    return STATUS_SUCCESS;
}

#ifdef HAVE_COMMONCRYPTO_COMMONDIGEST_H
struct hash
{
    struct object hdr;
    enum alg_id   alg_id;
    BOOL hmac;
    union
    {
        CC_MD5_CTX    md5_ctx;
        CC_SHA1_CTX   sha1_ctx;
        CC_SHA256_CTX sha256_ctx;
        CC_SHA512_CTX sha512_ctx;
        CCHmacContext hmac_ctx;
    } u;
};

static NTSTATUS hash_init( struct hash *hash )
{
    switch (hash->alg_id)
    {
    case ALG_ID_MD5:
        CC_MD5_Init( &hash->u.md5_ctx );
        break;

    case ALG_ID_SHA1:
        CC_SHA1_Init( &hash->u.sha1_ctx );
        break;

    case ALG_ID_SHA256:
        CC_SHA256_Init( &hash->u.sha256_ctx );
        break;

    case ALG_ID_SHA384:
        CC_SHA384_Init( &hash->u.sha512_ctx );
        break;

    case ALG_ID_SHA512:
        CC_SHA512_Init( &hash->u.sha512_ctx );
        break;

    default:
        ERR( "unhandled id %u\n", hash->alg_id );
        return STATUS_NOT_IMPLEMENTED;
    }
    return STATUS_SUCCESS;
}

static NTSTATUS hmac_init( struct hash *hash, UCHAR *key, ULONG key_size )
{
    CCHmacAlgorithm cc_algorithm;
    switch (hash->alg_id)
    {
    case ALG_ID_MD5:
        cc_algorithm = kCCHmacAlgMD5;
        break;

    case ALG_ID_SHA1:
        cc_algorithm = kCCHmacAlgSHA1;
        break;

    case ALG_ID_SHA256:
        cc_algorithm = kCCHmacAlgSHA256;
        break;

    case ALG_ID_SHA384:
        cc_algorithm = kCCHmacAlgSHA384;
        break;

    case ALG_ID_SHA512:
        cc_algorithm = kCCHmacAlgSHA512;
        break;

    default:
        ERR( "unhandled id %u\n", hash->alg_id );
        return STATUS_NOT_IMPLEMENTED;
    }

    CCHmacInit( &hash->u.hmac_ctx, cc_algorithm, key, key_size );
    return STATUS_SUCCESS;
}


static NTSTATUS hash_update( struct hash *hash, UCHAR *input, ULONG size )
{
    switch (hash->alg_id)
    {
    case ALG_ID_MD5:
        CC_MD5_Update( &hash->u.md5_ctx, input, size );
        break;

    case ALG_ID_SHA1:
        CC_SHA1_Update( &hash->u.sha1_ctx, input, size );
        break;

    case ALG_ID_SHA256:
        CC_SHA256_Update( &hash->u.sha256_ctx, input, size );
        break;

    case ALG_ID_SHA384:
        CC_SHA384_Update( &hash->u.sha512_ctx, input, size );
        break;

    case ALG_ID_SHA512:
        CC_SHA512_Update( &hash->u.sha512_ctx, input, size );
        break;

    default:
        ERR( "unhandled id %u\n", hash->alg_id );
        return STATUS_NOT_IMPLEMENTED;
    }
    return STATUS_SUCCESS;
}

static NTSTATUS hmac_update( struct hash *hash, UCHAR *input, ULONG size )
{
    CCHmacUpdate( &hash->u.hmac_ctx, input, size );
    return STATUS_SUCCESS;
}

static NTSTATUS hash_finish( struct hash *hash, UCHAR *output, ULONG size )
{
    switch (hash->alg_id)
    {
    case ALG_ID_MD5:
        CC_MD5_Final( output, &hash->u.md5_ctx );
        break;

    case ALG_ID_SHA1:
        CC_SHA1_Final( output, &hash->u.sha1_ctx );
        break;

    case ALG_ID_SHA256:
        CC_SHA256_Final( output, &hash->u.sha256_ctx );
        break;

    case ALG_ID_SHA384:
        CC_SHA384_Final( output, &hash->u.sha512_ctx );
        break;

    case ALG_ID_SHA512:
        CC_SHA512_Final( output, &hash->u.sha512_ctx );
        break;

    default:
        ERR( "unhandled id %u\n", hash->alg_id );
        break;
    }
    return STATUS_SUCCESS;
}

static NTSTATUS hmac_finish( struct hash *hash, UCHAR *output, ULONG size )
{
    CCHmacFinal( &hash->u.hmac_ctx, output );

    return STATUS_SUCCESS;
}
#elif defined(HAVE_GNUTLS_HASH)
struct hash
{
    struct object    hdr;
    enum alg_id      alg_id;
    BOOL hmac;
    union
    {
        gnutls_hash_hd_t hash_handle;
        gnutls_hmac_hd_t hmac_handle;
    } u;
};

static NTSTATUS hash_init( struct hash *hash )
{
    gnutls_digest_algorithm_t alg;

    if (!libgnutls_handle) return STATUS_INTERNAL_ERROR;

    switch (hash->alg_id)
    {
    case ALG_ID_MD5:
        alg = GNUTLS_DIG_MD5;
        break;
    case ALG_ID_SHA1:
        alg = GNUTLS_DIG_SHA1;
        break;

    case ALG_ID_SHA256:
        alg = GNUTLS_DIG_SHA256;
        break;

    case ALG_ID_SHA384:
        alg = GNUTLS_DIG_SHA384;
        break;

    case ALG_ID_SHA512:
        alg = GNUTLS_DIG_SHA512;
        break;

    default:
        ERR( "unhandled id %u\n", hash->alg_id );
        return STATUS_NOT_IMPLEMENTED;
    }

    if (pgnutls_hash_init( &hash->u.hash_handle, alg )) return STATUS_INTERNAL_ERROR;
    return STATUS_SUCCESS;
}

static NTSTATUS hmac_init( struct hash *hash, UCHAR *key, ULONG key_size )
{
    gnutls_mac_algorithm_t alg;

    if (!libgnutls_handle) return STATUS_INTERNAL_ERROR;

    switch (hash->alg_id)
    {
    case ALG_ID_MD5:
        alg = GNUTLS_MAC_MD5;
        break;
    case ALG_ID_SHA1:
        alg = GNUTLS_MAC_SHA1;
        break;

    case ALG_ID_SHA256:
        alg = GNUTLS_MAC_SHA256;
        break;

    case ALG_ID_SHA384:
        alg = GNUTLS_MAC_SHA384;
        break;

    case ALG_ID_SHA512:
        alg = GNUTLS_MAC_SHA512;
        break;

    default:
        ERR( "unhandled id %u\n", hash->alg_id );
        return STATUS_NOT_IMPLEMENTED;
    }

    if (pgnutls_hmac_init( &hash->u.hmac_handle, alg, key, key_size )) return STATUS_INTERNAL_ERROR;
    return STATUS_SUCCESS;
}

static NTSTATUS hash_update( struct hash *hash, UCHAR *input, ULONG size )
{
    if (pgnutls_hash( hash->u.hash_handle, input, size )) return STATUS_INTERNAL_ERROR;
    return STATUS_SUCCESS;
}

static NTSTATUS hmac_update( struct hash *hash, UCHAR *input, ULONG size )
{
    if (pgnutls_hmac( hash->u.hmac_handle, input, size )) return STATUS_INTERNAL_ERROR;
    return STATUS_SUCCESS;
}

static NTSTATUS hash_finish( struct hash *hash, UCHAR *output, ULONG size )
{
    pgnutls_hash_deinit( hash->u.hash_handle, output );
    return STATUS_SUCCESS;
}

static NTSTATUS hmac_finish( struct hash *hash, UCHAR *output, ULONG size )
{
    pgnutls_hmac_deinit( hash->u.hmac_handle, output );
    return STATUS_SUCCESS;
}
#elif defined(SONAME_LIBMBEDTLS)
struct hash
{
    struct object hdr;
    BOOL hmac;
    enum alg_id   alg_id;
    union
    {
        mbedtls_md5_context    md5_ctx;
        mbedtls_sha1_context   sha1_ctx;
        mbedtls_sha256_context sha256_ctx;
        mbedtls_sha512_context sha512_ctx;
        mbedtls_md_context_t   hmac_ctx;
    } u;
};

static NTSTATUS hash_init( struct hash *hash )
{
#ifndef __REACTOS__
    if (!libmbedtls_handle) return STATUS_INTERNAL_ERROR;
#endif
    switch (hash->alg_id)
    {
    case ALG_ID_MD5:
        mbedtls_md5_init(&hash->u.md5_ctx);
        mbedtls_md5_starts(&hash->u.md5_ctx);
        break;

    case ALG_ID_SHA1:
        mbedtls_sha1_init(&hash->u.sha1_ctx);
        mbedtls_sha1_starts(&hash->u.sha1_ctx);
        break;

    case ALG_ID_SHA256:
        mbedtls_sha256_init(&hash->u.sha256_ctx);
        mbedtls_sha256_starts(&hash->u.sha256_ctx, FALSE);
        break;

    case ALG_ID_SHA384:
    case ALG_ID_SHA512:
        mbedtls_sha512_init(&hash->u.sha512_ctx);
        mbedtls_sha512_starts(&hash->u.sha512_ctx, hash->alg_id==ALG_ID_SHA384);
        break;

    default:
        ERR( "unhandled id %u\n", hash->alg_id );
        return STATUS_NOT_IMPLEMENTED;
    }

    return STATUS_SUCCESS;
}

static NTSTATUS hmac_init( struct hash *hash, UCHAR *key, ULONG key_size )
{
    const mbedtls_md_info_t *md_info;
    mbedtls_md_type_t md_type;
    int ret;
#ifndef __REACTOS__
    if (!libmbedtls_handle) return STATUS_INTERNAL_ERROR;
#endif
    mbedtls_md_init(&hash->u.hmac_ctx);
    switch (hash->alg_id)
    {
    case ALG_ID_MD5:
        md_type = MBEDTLS_MD_MD5;
        break;

    case ALG_ID_SHA1:
        md_type = MBEDTLS_MD_SHA1;
        break;

    case ALG_ID_SHA256:
        md_type = MBEDTLS_MD_SHA256;
        break;

    case ALG_ID_SHA384:
        md_type = MBEDTLS_MD_SHA384;
        break;

    case ALG_ID_SHA512:
        md_type = MBEDTLS_MD_SHA512;
        break;

    default:
        ERR("unhandled id %u\n", hash->alg_id);
        return STATUS_NOT_IMPLEMENTED;
    }
    if ((md_info = mbedtls_md_info_from_type(md_type)) == NULL)
    {
        mbedtls_md_free(&hash->u.hmac_ctx);
        return STATUS_INTERNAL_ERROR;
    }

    if ((ret = mbedtls_md_setup(&hash->u.hmac_ctx, md_info, 1)) != 0)
    {
        mbedtls_md_free(&hash->u.hmac_ctx);
        return STATUS_INTERNAL_ERROR;
    }

    mbedtls_md_hmac_starts(&hash->u.hmac_ctx, key, key_size);

    return STATUS_SUCCESS;
}

static NTSTATUS hash_update( struct hash *hash, UCHAR *input, ULONG size )
{
#ifndef __REACTOS__
    if (!libmbedtls_handle) return STATUS_INTERNAL_ERROR;
#endif
    switch (hash->alg_id)
    {
    case ALG_ID_MD5:
        mbedtls_md5_update(&hash->u.md5_ctx, input, size);
        break;

    case ALG_ID_SHA1:
        mbedtls_sha1_update(&hash->u.sha1_ctx, input, size);
        break;

    case ALG_ID_SHA256:
        mbedtls_sha256_update(&hash->u.sha256_ctx, input, size);
        break;

    case ALG_ID_SHA384:
    case ALG_ID_SHA512:
        mbedtls_sha512_update(&hash->u.sha512_ctx, input, size);
        break;

    default:
        ERR( "unhandled id %u\n", hash->alg_id );
        return STATUS_NOT_IMPLEMENTED;
    }

    return STATUS_SUCCESS;
}

static NTSTATUS hmac_update( struct hash *hash, UCHAR *input, ULONG size )
{
#ifndef __REACTOS__
    if (!libmbedtls_handle) return STATUS_INTERNAL_ERROR;
#endif
    mbedtls_md_update(&hash->u.hmac_ctx, input, size);

    return STATUS_SUCCESS;
}

static NTSTATUS hash_finish( struct hash *hash, UCHAR *output, ULONG size )
{
#ifndef __REACTOS__
    if (!libmbedtls_handle) return STATUS_INTERNAL_ERROR;
#endif
    switch (hash->alg_id)
    {
    case ALG_ID_MD5:
        mbedtls_md5_finish(&hash->u.md5_ctx, output);
        mbedtls_md5_free(&hash->u.md5_ctx);
        break;

    case ALG_ID_SHA1:
        mbedtls_sha1_finish(&hash->u.sha1_ctx, output);
        mbedtls_sha1_free(&hash->u.sha1_ctx);
        break;

    case ALG_ID_SHA256:
        mbedtls_sha256_finish(&hash->u.sha256_ctx, output);
        mbedtls_sha256_free(&hash->u.sha256_ctx);
        break;

    case ALG_ID_SHA384:
    case ALG_ID_SHA512:
        mbedtls_sha512_finish(&hash->u.sha512_ctx, output);
        mbedtls_sha512_free(&hash->u.sha512_ctx);
        break;

    default:
        ERR( "unhandled id %u\n", hash->alg_id );
        return STATUS_NOT_IMPLEMENTED;
    }

    return STATUS_SUCCESS;
}

static NTSTATUS hmac_finish( struct hash *hash, UCHAR *output, ULONG size )
{
#ifndef __REACTOS__
    if (!libmbedtls_handle) return STATUS_INTERNAL_ERROR;
#endif
    mbedtls_md_hmac_finish(&hash->u.hmac_ctx, output);
    mbedtls_md_free(&hash->u.hmac_ctx);

    return STATUS_SUCCESS;
}
#endif

#define OBJECT_LENGTH_MD5       274
#define OBJECT_LENGTH_SHA1      278
#define OBJECT_LENGTH_SHA256    286
#define OBJECT_LENGTH_SHA384    382
#define OBJECT_LENGTH_SHA512    382

static NTSTATUS generic_alg_property( enum alg_id id, const WCHAR *prop, UCHAR *buf, ULONG size, ULONG *ret_size )
{
    if (!strcmpW( prop, BCRYPT_HASH_LENGTH ))
    {
        *ret_size = sizeof(ULONG);
        if (size < sizeof(ULONG))
            return STATUS_BUFFER_TOO_SMALL;
        if(buf)
            *(ULONG*)buf = alg_props[id].hash_length;
        return STATUS_SUCCESS;
    }

    if (!strcmpW( prop, BCRYPT_ALGORITHM_NAME ))
    {
        *ret_size = (strlenW(alg_props[id].alg_name)+1)*sizeof(WCHAR);
        if (size < *ret_size)
            return STATUS_BUFFER_TOO_SMALL;
        if(buf)
            memcpy(buf, alg_props[id].alg_name, *ret_size);
        return STATUS_SUCCESS;
    }

    return STATUS_NOT_IMPLEMENTED;
}

static NTSTATUS get_alg_property( enum alg_id id, const WCHAR *prop, UCHAR *buf, ULONG size, ULONG *ret_size )
{
    NTSTATUS status;
    ULONG value;

    status = generic_alg_property( id, prop, buf, size, ret_size );
    if (status != STATUS_NOT_IMPLEMENTED)
        return status;

    switch (id)
    {
    case ALG_ID_MD5:
        if (!strcmpW( prop, BCRYPT_OBJECT_LENGTH ))
        {
            value = OBJECT_LENGTH_MD5;
            break;
        }
        FIXME( "unsupported md5 algorithm property %s\n", debugstr_w(prop) );
        return STATUS_NOT_IMPLEMENTED;

    case ALG_ID_RNG:
        if (!strcmpW( prop, BCRYPT_OBJECT_LENGTH )) return STATUS_NOT_SUPPORTED;
        FIXME( "unsupported rng algorithm property %s\n", debugstr_w(prop) );
        return STATUS_NOT_IMPLEMENTED;

    case ALG_ID_SHA1:
        if (!strcmpW( prop, BCRYPT_OBJECT_LENGTH ))
        {
            value = OBJECT_LENGTH_SHA1;
            break;
        }
        FIXME( "unsupported sha1 algorithm property %s\n", debugstr_w(prop) );
        return STATUS_NOT_IMPLEMENTED;

    case ALG_ID_SHA256:
        if (!strcmpW( prop, BCRYPT_OBJECT_LENGTH ))
        {
            value = OBJECT_LENGTH_SHA256;
            break;
        }
        FIXME( "unsupported sha256 algorithm property %s\n", debugstr_w(prop) );
        return STATUS_NOT_IMPLEMENTED;

    case ALG_ID_SHA384:
        if (!strcmpW( prop, BCRYPT_OBJECT_LENGTH ))
        {
            value = OBJECT_LENGTH_SHA384;
            break;
        }
        FIXME( "unsupported sha384 algorithm property %s\n", debugstr_w(prop) );
        return STATUS_NOT_IMPLEMENTED;

    case ALG_ID_SHA512:
        if (!strcmpW( prop, BCRYPT_OBJECT_LENGTH ))
        {
            value = OBJECT_LENGTH_SHA512;
            break;
        }
        FIXME( "unsupported sha512 algorithm property %s\n", debugstr_w(prop) );
        return STATUS_NOT_IMPLEMENTED;

    default:
        FIXME( "unsupported algorithm %u\n", id );
        return STATUS_NOT_IMPLEMENTED;
    }

    if (size < sizeof(ULONG))
    {
        *ret_size = sizeof(ULONG);
        return STATUS_BUFFER_TOO_SMALL;
    }
    if (buf) *(ULONG *)buf = value;
    *ret_size = sizeof(ULONG);

    return STATUS_SUCCESS;
}

static NTSTATUS get_hash_property( enum alg_id id, const WCHAR *prop, UCHAR *buf, ULONG size, ULONG *ret_size )
{
    NTSTATUS status;

    status = generic_alg_property( id, prop, buf, size, ret_size );
    if (status == STATUS_NOT_IMPLEMENTED)
        FIXME( "unsupported property %s\n", debugstr_w(prop) );
    return status;
}

NTSTATUS WINAPI BCryptGetProperty( BCRYPT_HANDLE handle, LPCWSTR prop, UCHAR *buffer, ULONG count, ULONG *res, ULONG flags )
{
    struct object *object = handle;

    TRACE( "%p, %s, %p, %u, %p, %08x\n", handle, wine_dbgstr_w(prop), buffer, count, res, flags );

    if (!object) return STATUS_INVALID_HANDLE;
    if (!prop || !res) return STATUS_INVALID_PARAMETER;

    switch (object->magic)
    {
    case MAGIC_ALG:
    {
        const struct algorithm *alg = (const struct algorithm *)object;
        return get_alg_property( alg->id, prop, buffer, count, res );
    }
    case MAGIC_HASH:
    {
        const struct hash *hash = (const struct hash *)object;
        return get_hash_property( hash->alg_id, prop, buffer, count, res );
    }
    default:
        WARN( "unknown magic %08x\n", object->magic );
        return STATUS_INVALID_HANDLE;
    }
}

NTSTATUS WINAPI BCryptCreateHash( BCRYPT_ALG_HANDLE algorithm, BCRYPT_HASH_HANDLE *handle, UCHAR *object, ULONG objectlen,
                                  UCHAR *secret, ULONG secretlen, ULONG flags )
{
    struct algorithm *alg = algorithm;
    struct hash *hash;
    NTSTATUS status;

    TRACE( "%p, %p, %p, %u, %p, %u, %08x - stub\n", algorithm, handle, object, objectlen,
           secret, secretlen, flags );
    if (flags)
    {
        FIXME( "unimplemented flags %08x\n", flags );
        return STATUS_NOT_IMPLEMENTED;
    }

    if (!alg || alg->hdr.magic != MAGIC_ALG) return STATUS_INVALID_HANDLE;
    if (object) FIXME( "ignoring object buffer\n" );

    if (!(hash = HeapAlloc( GetProcessHeap(), 0, sizeof(*hash) ))) return STATUS_NO_MEMORY;
    hash->hdr.magic = MAGIC_HASH;
    hash->alg_id    = alg->id;
    hash->hmac      = alg->hmac;

    if (hash->hmac)
    {
        status = hmac_init( hash, secret, secretlen );
    }
    else
    {
        status = hash_init( hash );
    }

    if (status != STATUS_SUCCESS)
    {
        HeapFree( GetProcessHeap(), 0, hash );
        return status;
    }

    *handle = hash;
    return STATUS_SUCCESS;
}

NTSTATUS WINAPI BCryptDestroyHash( BCRYPT_HASH_HANDLE handle )
{
    struct hash *hash = handle;

    TRACE( "%p\n", handle );

    if (!hash || hash->hdr.magic != MAGIC_HASH) return STATUS_INVALID_HANDLE;
    HeapFree( GetProcessHeap(), 0, hash );
    return STATUS_SUCCESS;
}

NTSTATUS WINAPI BCryptHashData( BCRYPT_HASH_HANDLE handle, UCHAR *input, ULONG size, ULONG flags )
{
    struct hash *hash = handle;

    TRACE( "%p, %p, %u, %08x\n", handle, input, size, flags );

    if (!hash || hash->hdr.magic != MAGIC_HASH) return STATUS_INVALID_HANDLE;
    if (!input) return STATUS_SUCCESS;

    if (hash->hmac)
    {
        return hmac_update( hash, input, size );
    }
    else
    {
        return hash_update( hash, input, size );
    }
}

NTSTATUS WINAPI BCryptFinishHash( BCRYPT_HASH_HANDLE handle, UCHAR *output, ULONG size, ULONG flags )
{
    struct hash *hash = handle;

    TRACE( "%p, %p, %u, %08x\n", handle, output, size, flags );

    if (!hash || hash->hdr.magic != MAGIC_HASH) return STATUS_INVALID_HANDLE;
    if (!output) return STATUS_INVALID_PARAMETER;

    if (hash->hmac)
    {
        return hmac_finish( hash, output, size );
    }
    else
    {
        return hash_finish( hash, output, size );
    }
}

NTSTATUS WINAPI BCryptHash( BCRYPT_ALG_HANDLE algorithm, UCHAR *secret, ULONG secretlen,
                            UCHAR *input, ULONG inputlen, UCHAR *output, ULONG outputlen )
{
    NTSTATUS status;
    BCRYPT_HASH_HANDLE handle;

    TRACE( "%p, %p, %u, %p, %u, %p, %u\n", algorithm, secret, secretlen,
           input, inputlen, output, outputlen );

    status = BCryptCreateHash( algorithm, &handle, NULL, 0, secret, secretlen, 0);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = BCryptHashData( handle, input, inputlen, 0 );
    if (status != STATUS_SUCCESS)
    {
        BCryptDestroyHash( handle );
        return status;
    }

    status = BCryptFinishHash( handle, output, outputlen, 0 );
    if (status != STATUS_SUCCESS)
    {
        BCryptDestroyHash( handle );
        return status;
    }

    return BCryptDestroyHash( handle );
}


static NTSTATUS key_asymmetric_create( enum alg_id alg_id, ULONG bitlen, struct key **ret_key )
{
    struct key *key;

    //TODO: __wine_unixlib_handle?????
    WARN("(!__wine_unixlib_handle)");
    /*if (!__wine_unixlib_handle)
    {
        ERR( "no encryption support\n" );
        return STATUS_NOT_IMPLEMENTED;
    }*/

    if (!(key = calloc( 1, sizeof(*key) ))) return STATUS_NO_MEMORY;
    key->hdr.magic  = MAGIC_KEY;
    key->alg_id     = alg_id;
    key->u.a.bitlen = bitlen;

    *ret_key = key;
    return STATUS_SUCCESS;
}

static BOOL is_equal_vector( const UCHAR *vector, ULONG len, const UCHAR *vector2, ULONG len2 )
{
    if (!vector && !vector2) return TRUE;
    if (len != len2) return FALSE;
    return !memcmp( vector, vector2, len );
}

static NTSTATUS key_symmetric_set_vector( struct key *key, UCHAR *vector, ULONG vector_len, BOOL force_reset )
{
    BOOL needs_reset = force_reset || !is_equal_vector( key->u.s.vector, key->u.s.vector_len, vector, vector_len );

    free( key->u.s.vector );
    key->u.s.vector = NULL;
    key->u.s.vector_len = 0;
    if (vector)
    {
        if (!(key->u.s.vector = malloc( vector_len ))) return STATUS_NO_MEMORY;
        memcpy( key->u.s.vector, vector, vector_len );
        key->u.s.vector_len = vector_len;
    }
    if (needs_reset)
    {
        //TODO: UNIX_CALL????
        WARN("UNIX_CALL( key_symmetric_vector_reset, key )");
        //UNIX_CALL( key_symmetric_vector_reset, key );
        return STATUS_NOT_IMPLEMENTED;
    }
    return STATUS_SUCCESS;
}

static struct key *create_symmetric_key( enum alg_id alg, enum chain_mode mode, ULONG block_size, UCHAR *secret,
                                         ULONG secret_len )
{
    struct key *ret;

    if (!(ret = calloc( 1, sizeof(*ret) ))) return NULL;
    InitializeCriticalSection( &ret->u.s.cs );
    ret->hdr.magic      = MAGIC_KEY;
    ret->alg_id         = alg;
    ret->u.s.mode       = mode;
    ret->u.s.block_size = block_size;

    if (!(ret->u.s.secret = malloc( secret_len )))
    {
        DeleteCriticalSection( &ret->u.s.cs );
        free( ret );
        return NULL;
    }
    memcpy( ret->u.s.secret, secret, secret_len );
    ret->u.s.secret_len = secret_len;

    return ret;
}

static ULONG get_block_size( struct algorithm *alg )
{
    ULONG ret = 0, size = sizeof(ret);
    get_alg_property( alg->id, BCRYPT_BLOCK_LENGTH, (UCHAR *)&ret, sizeof(ret), &size );
    return ret;
}

static NTSTATUS generate_symmetric_key( struct algorithm *alg, BCRYPT_KEY_HANDLE *ret_handle, UCHAR *secret,
                                        ULONG secret_len )
{
    BCRYPT_KEY_LENGTHS_STRUCT key_lengths;
    ULONG block_size, size;
    struct key *key;
    NTSTATUS status;

    if (!(block_size = get_block_size( alg ))) return STATUS_INVALID_PARAMETER;
    if (!get_alg_property( alg->id, BCRYPT_KEY_LENGTHS, (UCHAR *)&key_lengths, sizeof(key_lengths), &size ))
    {
        if (secret_len > (size = key_lengths.dwMaxLength / 8))
        {
            WARN( "secret_len %lu exceeds key max length %lu, setting to maximum\n", secret_len, size );
            secret_len = size;
        }
        else if (secret_len < (size = key_lengths.dwMinLength / 8))
        {
            WARN( "secret_len %lu is less than minimum key length %lu\n", secret_len, size );
            return STATUS_INVALID_PARAMETER;
        }
        else if (key_lengths.dwIncrement && (secret_len * 8 - key_lengths.dwMinLength) % key_lengths.dwIncrement)
        {
            WARN( "secret_len %lu is not a valid key length\n", secret_len );
            return STATUS_INVALID_PARAMETER;
        }
    }

    if (!(key = create_symmetric_key( alg->id, alg->mode, block_size, secret, secret_len ))) status = STATUS_NO_MEMORY;
    else
    {
        *ret_handle = key;
        status = STATUS_SUCCESS;
    }

    return status;
}

static NTSTATUS key_import( struct algorithm *alg, const WCHAR *type, BCRYPT_KEY_HANDLE *key, UCHAR *object,
                            ULONG object_len, UCHAR *input, ULONG input_len )
{
    ULONG len;

    if (!wcscmp( type, BCRYPT_KEY_DATA_BLOB ))
    {
        BCRYPT_KEY_DATA_BLOB_HEADER *header = (BCRYPT_KEY_DATA_BLOB_HEADER *)input;

        if (input_len < sizeof(BCRYPT_KEY_DATA_BLOB_HEADER)) return STATUS_BUFFER_TOO_SMALL;
        if (header->dwMagic != BCRYPT_KEY_DATA_BLOB_MAGIC) return STATUS_INVALID_PARAMETER;
        if (header->dwVersion != BCRYPT_KEY_DATA_BLOB_VERSION1)
        {
            FIXME( "unknown key data blob version %lu\n", header->dwVersion );
            return STATUS_INVALID_PARAMETER;
        }
        len = header->cbKeyData;
        if (len + sizeof(BCRYPT_KEY_DATA_BLOB_HEADER) > input_len) return STATUS_INVALID_PARAMETER;

        return generate_symmetric_key( alg, key, (UCHAR *)&header[1], len );
    }
    else if (!wcscmp( type, BCRYPT_OPAQUE_KEY_BLOB ))
    {
        if (input_len < sizeof(len)) return STATUS_BUFFER_TOO_SMALL;
        len = *(ULONG *)input;
        if (len + sizeof(len) > input_len) return STATUS_INVALID_PARAMETER;

        return generate_symmetric_key( alg, key, input + sizeof(len), len );
    }

    FIXME( "unsupported key type %s\n", debugstr_w(type) );
    return STATUS_NOT_IMPLEMENTED;
}

static NTSTATUS key_export( struct key *key, const WCHAR *type, UCHAR *output, ULONG output_len, ULONG *size )
{
    //struct key_asymmetric_export_params params;

    if (!wcscmp( type, BCRYPT_KEY_DATA_BLOB ))
    {
        BCRYPT_KEY_DATA_BLOB_HEADER *header = (BCRYPT_KEY_DATA_BLOB_HEADER *)output;
        ULONG req_size = sizeof(BCRYPT_KEY_DATA_BLOB_HEADER) + key->u.s.secret_len;

        *size = req_size;
        if (output_len < req_size) return STATUS_BUFFER_TOO_SMALL;
        if (output)
        {
            header->dwMagic   = BCRYPT_KEY_DATA_BLOB_MAGIC;
            header->dwVersion = BCRYPT_KEY_DATA_BLOB_VERSION1;
            header->cbKeyData = key->u.s.secret_len;
            memcpy( &header[1], key->u.s.secret, key->u.s.secret_len );
        }
        return STATUS_SUCCESS;
    }
    else if (!wcscmp( type, BCRYPT_OPAQUE_KEY_BLOB ))
    {
        ULONG len, req_size = sizeof(len) + key->u.s.secret_len;

        *size = req_size;
        if (output_len < req_size) return STATUS_BUFFER_TOO_SMALL;
        if (output)
        {
            *(ULONG *)output = key->u.s.secret_len;
            memcpy( output + sizeof(len), key->u.s.secret, key->u.s.secret_len );
        }
        return STATUS_SUCCESS;
    }
    else if (!wcscmp( type, BCRYPT_DSA_PRIVATE_BLOB ) || !wcscmp( type, LEGACY_DSA_V2_PRIVATE_BLOB ) ||
             !wcscmp( type, BCRYPT_ECCPRIVATE_BLOB ))
    {
        //TODO: UNIX_CALL????
        /*params.key     = key;
        params.flags   = 0;
        params.buf     = output;
        params.len     = output_len;
        params.ret_len = size;*/
        WARN("UNIX_CALL( key_asymmetric_export, &params )");
        //return UNIX_CALL( key_asymmetric_export, &params );
    }
    else if (!wcscmp( type, BCRYPT_RSAPRIVATE_BLOB ) || !wcscmp( type, BCRYPT_RSAFULLPRIVATE_BLOB ))
    {
        //TODO: UNIX_CALL????
        /*params.key     = key;
        params.flags   = (wcscmp( type, BCRYPT_RSAPRIVATE_BLOB )) ? KEY_EXPORT_FLAG_RSA_FULL : 0;
        params.buf     = output;
        params.len     = output_len;
        params.ret_len = size;*/
        WARN("UNIX_CALL( key_asymmetric_export, &params )");
        //return UNIX_CALL( key_asymmetric_export, &params );
    }
    else if (!wcscmp( type, BCRYPT_DSA_PUBLIC_BLOB ) || !wcscmp( type, LEGACY_DSA_V2_PUBLIC_BLOB ) ||
             !wcscmp( type, BCRYPT_ECCPUBLIC_BLOB ) || !wcscmp( type, BCRYPT_RSAPUBLIC_BLOB ))
    {
        //TODO: UNIX_CALL????
        /*params.key     = key;
        params.flags   = KEY_EXPORT_FLAG_PUBLIC;
        params.buf     = output;
        params.len     = output_len;
        params.ret_len = size;*/
        WARN("UNIX_CALL( key_asymmetric_export, &params )");
        //return UNIX_CALL( key_asymmetric_export, &params );
    }

    FIXME( "unsupported key type %s\n", debugstr_w(type) );
    return STATUS_NOT_IMPLEMENTED;
}

static NTSTATUS key_symmetric_encrypt( struct key *key,  UCHAR *input, ULONG input_len, void *padding, UCHAR *iv,
                                       ULONG iv_len, UCHAR *output, ULONG output_len, ULONG *ret_len, ULONG flags )
{
    //struct key_symmetric_set_auth_data_params auth_params;
    struct key_symmetric_encrypt_params encrypt_params;
    //struct key_symmetric_get_tag_params tag_params;
    ULONG bytes_left = input_len;
    UCHAR *buf;
    NTSTATUS status;

    if (key->u.s.mode == CHAIN_MODE_GCM)
    {
        BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO *auth_info = padding;

        if (!auth_info) return STATUS_INVALID_PARAMETER;
        if (!auth_info->pbNonce) return STATUS_INVALID_PARAMETER;
        if (!auth_info->pbTag) return STATUS_INVALID_PARAMETER;
        if (auth_info->cbTag < 12 || auth_info->cbTag > 16) return STATUS_INVALID_PARAMETER;
        if (auth_info->dwFlags & BCRYPT_AUTH_MODE_CHAIN_CALLS_FLAG)
            FIXME( "call chaining not implemented\n" );

        if ((status = key_symmetric_set_vector( key, auth_info->pbNonce, auth_info->cbNonce, TRUE )))
            return status;

        *ret_len = input_len;
        if (flags & BCRYPT_BLOCK_PADDING) return STATUS_INVALID_PARAMETER;
        if (input && !output) return STATUS_SUCCESS;
        if (output_len < *ret_len) return STATUS_BUFFER_TOO_SMALL;

        //TODO: UNIX_CALL?????
        /*auth_params.key = key;
        auth_params.auth_data = auth_info->pbAuthData;
        auth_params.len = auth_info->cbAuthData;*/
        WARN("UNIX_CALL( key_symmetric_set_auth_data, &auth_params )");
        //if ((status = UNIX_CALL( key_symmetric_set_auth_data, &auth_params ))) return status;

        encrypt_params.key = key;
        encrypt_params.input = input;
        encrypt_params.input_len = input_len;
        encrypt_params.output = output;
        encrypt_params.output_len = output_len;
        //TODO: UNIX_CALL?????
        WARN("UNIX_CALL( key_symmetric_encrypt, &encrypt_params )");
        //if ((status = UNIX_CALL( key_symmetric_encrypt, &encrypt_params ))) return status;

        //TODO: UNIX_CALL?????
        /*tag_params.key = key;
        tag_params.tag = auth_info->pbTag;
        tag_params.len = auth_info->cbTag;*/
        WARN("UNIX_CALL( key_symmetric_get_tag, &tag_params )");
        //return UNIX_CALL( key_symmetric_get_tag, &tag_params );
    }

    *ret_len = input_len;

    if (flags & BCRYPT_BLOCK_PADDING)
        *ret_len = (input_len + key->u.s.block_size) & ~(key->u.s.block_size - 1);
    else if (input_len & (key->u.s.block_size - 1))
        return STATUS_INVALID_BUFFER_SIZE;

    if (!output) return STATUS_SUCCESS;
    if (output_len < *ret_len) return STATUS_BUFFER_TOO_SMALL;
    if (key->u.s.mode == CHAIN_MODE_ECB && iv) return STATUS_INVALID_PARAMETER;
    if ((status = key_symmetric_set_vector( key, iv, iv_len, flags & BCRYPT_BLOCK_PADDING ))) return status;

    encrypt_params.key = key;
    encrypt_params.input = input;
    encrypt_params.input_len = key->u.s.block_size;
    encrypt_params.output = output;
    encrypt_params.output_len = key->u.s.block_size;
    while (bytes_left >= key->u.s.block_size)
    {
        //TODO: UNIX_CALL?????
        WARN("UNIX_CALL( key_symmetric_encrypt, &encrypt_params )");
        /*if ((status = UNIX_CALL( key_symmetric_encrypt, &encrypt_params )))
            return status;
        */
        if (key->u.s.mode == CHAIN_MODE_ECB && (status = key_symmetric_set_vector( key, NULL, 0, TRUE )))
            return status;
        bytes_left -= key->u.s.block_size;
        encrypt_params.input += key->u.s.block_size;
        encrypt_params.output += key->u.s.block_size;
    }

    if (flags & BCRYPT_BLOCK_PADDING)
    {
        if (!(buf = malloc( key->u.s.block_size ))) return STATUS_NO_MEMORY;
        memcpy( buf, encrypt_params.input, bytes_left );
        memset( buf + bytes_left, key->u.s.block_size - bytes_left, key->u.s.block_size - bytes_left );
        encrypt_params.input = buf;
        //TODO: UNIX_CALL?????
        WARN("UNIX_CALL( key_symmetric_encrypt, &encrypt_params )");
        //status = UNIX_CALL( key_symmetric_encrypt, &encrypt_params );
        free( buf );
    }

    return status;
}

NTSTATUS WINAPI BCryptGenerateSymmetricKey( BCRYPT_ALG_HANDLE handle, BCRYPT_KEY_HANDLE *ret_handle,
                                            UCHAR *object, ULONG object_len, UCHAR *secret, ULONG secret_len,
                                            ULONG flags )
{
    struct algorithm *alg = get_alg_object( handle );

    TRACE( "%p, %p, %p, %lu, %p, %lu, %#lx\n", handle, ret_handle, object, object_len, secret, secret_len, flags );
    if (object) FIXME( "ignoring object buffer\n" );

    //TODO: __wine_unixlib_handle?????
    WARN("__wine_unixlib_handle");
    /*if (!__wine_unixlib_handle)
    {
        ERR( "no encryption support\n" );
        return STATUS_NOT_IMPLEMENTED;
    }*/

    if (!alg) return STATUS_INVALID_HANDLE;
    return generate_symmetric_key( alg, ret_handle, secret, secret_len );
}

NTSTATUS WINAPI BCryptGenerateKeyPair( BCRYPT_ALG_HANDLE handle, BCRYPT_KEY_HANDLE *ret_handle, ULONG key_len,
                                       ULONG flags )
{
    struct algorithm *alg = get_alg_object( handle );
    struct key *key;
    NTSTATUS status;

    TRACE( "%p, %p, %lu, %#lx\n", handle, ret_handle, key_len, flags );

    if (!alg) return STATUS_INVALID_HANDLE;
    if (!ret_handle) return STATUS_INVALID_PARAMETER;

    if ((status = key_asymmetric_create( alg->id, key_len, &key ))) return status;
    *ret_handle = key;
    return STATUS_SUCCESS;
}

NTSTATUS WINAPI BCryptFinalizeKeyPair( BCRYPT_KEY_HANDLE handle, ULONG flags )
{
    struct key *key = get_key_object( handle );

    TRACE( "%p, %#lx\n", key, flags );

    if (!key) return STATUS_INVALID_HANDLE;
    //TODO: UNIX_CALL?????
    WARN("UNIX_CALL( key_asymmetric_generate, key )");
    //return UNIX_CALL( key_asymmetric_generate, key );
    return STATUS_NOT_SUPPORTED;
}

NTSTATUS WINAPI BCryptImportKey( BCRYPT_ALG_HANDLE handle, BCRYPT_KEY_HANDLE decrypt_key, const WCHAR *type,
                                 BCRYPT_KEY_HANDLE *ret_handle, UCHAR *object, ULONG object_len, UCHAR *input,
                                 ULONG input_len, ULONG flags )
{
    struct algorithm *alg = get_alg_object( handle );

    TRACE( "%p, %p, %s, %p, %p, %lu, %p, %lu, %#lx\n", handle, decrypt_key, debugstr_w(type), ret_handle, object,
          object_len, input, input_len, flags );

    if (!alg) return STATUS_INVALID_HANDLE;
    if (!ret_handle || !type || !input) return STATUS_INVALID_PARAMETER;
    if (decrypt_key)
    {
        FIXME( "decryption of key not yet supported\n" );
        return STATUS_NOT_IMPLEMENTED;
    }

    return key_import( alg, type, ret_handle, object, object_len, input, input_len );
}

NTSTATUS WINAPI BCryptExportKey( BCRYPT_KEY_HANDLE export_key_handle, BCRYPT_KEY_HANDLE encrypt_key_handle,
                                 const WCHAR *type, UCHAR *output, ULONG output_len, ULONG *size, ULONG flags )
{
    struct key *key = get_key_object( export_key_handle );

    TRACE( "%p, %p, %s, %p, %lu, %p, %#lx\n", export_key_handle, encrypt_key_handle, debugstr_w(type), output,
           output_len, size, flags );

    if (!key) return STATUS_INVALID_HANDLE;
    if (!type || !size) return STATUS_INVALID_PARAMETER;
    if (encrypt_key_handle)
    {
        FIXME( "encryption of key not yet supported\n" );
        return STATUS_NOT_IMPLEMENTED;
    }

    return key_export( key, type, output, output_len, size );
}

static NTSTATUS set_alg_property( struct algorithm *alg, const WCHAR *prop, UCHAR *value, ULONG size, ULONG flags )
{
    switch (alg->id)
    {
    case ALG_ID_3DES:
        if (!wcscmp( prop, BCRYPT_CHAINING_MODE ))
        {
            if (!wcscmp( (WCHAR *)value, BCRYPT_CHAIN_MODE_CBC ))
            {
                alg->mode = CHAIN_MODE_CBC;
                return STATUS_SUCCESS;
            }
            else
            {
                FIXME( "unsupported mode %s\n", debugstr_w((WCHAR *)value) );
                return STATUS_NOT_SUPPORTED;
            }
        }
        FIXME( "unsupported 3des algorithm property %s\n", debugstr_w(prop) );
        return STATUS_NOT_IMPLEMENTED;

    case ALG_ID_AES:
        if (!wcscmp( prop, BCRYPT_CHAINING_MODE ))
        {
            if (!wcscmp( (WCHAR *)value, BCRYPT_CHAIN_MODE_ECB ))
            {
                alg->mode = CHAIN_MODE_ECB;
                return STATUS_SUCCESS;
            }
            else if (!wcscmp( (WCHAR *)value, BCRYPT_CHAIN_MODE_CBC ))
            {
                alg->mode = CHAIN_MODE_CBC;
                return STATUS_SUCCESS;
            }
            else if (!wcscmp( (WCHAR *)value, BCRYPT_CHAIN_MODE_GCM ))
            {
                alg->mode = CHAIN_MODE_GCM;
                return STATUS_SUCCESS;
            }
            else if (!wcscmp( (WCHAR *)value, BCRYPT_CHAIN_MODE_CFB ))
            {
                alg->mode = CHAIN_MODE_CFB;
                return STATUS_SUCCESS;
            }
            else
            {
                FIXME( "unsupported mode %s\n", debugstr_w((WCHAR *)value) );
                return STATUS_NOT_IMPLEMENTED;
            }
        }
        FIXME( "unsupported aes algorithm property %s\n", debugstr_w(prop) );
        return STATUS_NOT_IMPLEMENTED;

    default:
        FIXME( "unsupported algorithm %u\n", alg->id );
        return STATUS_NOT_IMPLEMENTED;
    }
}

static NTSTATUS set_key_property( struct key *key, const WCHAR *prop, UCHAR *value, ULONG size, ULONG flags )
{
    if (!wcscmp( prop, BCRYPT_CHAINING_MODE ))
    {
        if (!wcscmp( (WCHAR *)value, BCRYPT_CHAIN_MODE_ECB ))
        {
            key->u.s.mode = CHAIN_MODE_ECB;
            return STATUS_SUCCESS;
        }
        else if (!wcscmp( (WCHAR *)value, BCRYPT_CHAIN_MODE_CBC ))
        {
            key->u.s.mode = CHAIN_MODE_CBC;
            return STATUS_SUCCESS;
        }
        else if (!wcscmp( (WCHAR *)value, BCRYPT_CHAIN_MODE_GCM ))
        {
            key->u.s.mode = CHAIN_MODE_GCM;
            return STATUS_SUCCESS;
        }
        else if (!wcscmp( (WCHAR *)value, BCRYPT_CHAIN_MODE_CFB ))
        {
            key->u.s.mode = CHAIN_MODE_CFB;
            return STATUS_SUCCESS;
        }
        else
        {
            FIXME( "unsupported mode %s\n", debugstr_w((WCHAR *)value) );
            return STATUS_NOT_IMPLEMENTED;
        }
    }
    else if (!wcscmp( prop, BCRYPT_KEY_LENGTH ))
    {
        if (size < sizeof(DWORD)) return STATUS_INVALID_PARAMETER;
        key->u.a.bitlen = *(DWORD*)value;
        return STATUS_SUCCESS;
    }

    FIXME( "unsupported key property %s\n", debugstr_w(prop) );
    return STATUS_NOT_IMPLEMENTED;
}



NTSTATUS WINAPI BCryptEncrypt( BCRYPT_KEY_HANDLE handle, UCHAR *input, ULONG input_len, void *padding, UCHAR *iv,
                               ULONG iv_len, UCHAR *output, ULONG output_len, ULONG *ret_len, ULONG flags )
{
    struct key *key = get_key_object( handle );
    //struct key_asymmetric_encrypt_params asymmetric_params;
    NTSTATUS ret;

    TRACE( "%p, %p, %lu, %p, %p, %lu, %p, %lu, %p, %#lx\n", handle, input, input_len, padding, iv, iv_len, output,
           output_len, ret_len, flags );

    if (!key) return STATUS_INVALID_HANDLE;

    if (is_symmetric_key( key ))
    {
        if (flags & ~BCRYPT_BLOCK_PADDING)
        {
            FIXME( "flags %#lx not implemented\n", flags );
            return STATUS_NOT_IMPLEMENTED;
        }
        EnterCriticalSection( &key->u.s.cs );
        ret = key_symmetric_encrypt( key, input, input_len, padding, iv, iv_len, output, output_len, ret_len, flags );
        LeaveCriticalSection( &key->u.s.cs );
    }
    else
    {
        if (flags & BCRYPT_PAD_NONE || flags & BCRYPT_PAD_OAEP)
        {
            FIXME( "flags %#lx not implemented\n", flags );
            return STATUS_NOT_IMPLEMENTED;
        }
        if (!is_asymmetric_encryption_key( key )) return STATUS_NOT_SUPPORTED;

        //TODO: UNIX_CALL????
        /*asymmetric_params.input = input;
        asymmetric_params.input_len = input_len;
        asymmetric_params.key = key;
        asymmetric_params.output = output;
        asymmetric_params.output_len = output_len;
        asymmetric_params.ret_len = ret_len;*/
        WARN("UNIX_CALL(key_asymmetric_encrypt, &asymmetric_params)");
        //ret = UNIX_CALL(key_asymmetric_encrypt, &asymmetric_params);
    }

    return ret;
}

NTSTATUS WINAPI BCryptSetProperty( BCRYPT_HANDLE handle, const WCHAR *prop, UCHAR *value, ULONG size, ULONG flags )
{
    struct object *object = get_object( handle, 0 );

    TRACE( "%p, %s, %p, %lu, %#lx\n", handle, debugstr_w(prop), value, size, flags );

    if (!handle) return STATUS_INVALID_HANDLE;
    if (is_alg_pseudo_handle( handle )) return STATUS_ACCESS_DENIED;

    switch (object->magic)
    {
    case MAGIC_ALG:
    {
        struct algorithm *alg = (struct algorithm *)object;
        return set_alg_property( alg, prop, value, size, flags );
    }
    case MAGIC_KEY:
    {
        struct key *key = (struct key *)object;
        return set_key_property( key, prop, value, size, flags );
    }
    default:
        WARN( "unknown magic %#lx\n", object->magic );
        return STATUS_INVALID_HANDLE;
    }
}

BOOL WINAPI DllMain( HINSTANCE hinst, DWORD reason, LPVOID reserved )
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        instance = hinst;
        DisableThreadLibraryCalls( hinst );
#if defined(HAVE_GNUTLS_HASH) && !defined(HAVE_COMMONCRYPTO_COMMONDIGEST_H)
        gnutls_initialize();
#elif defined(SONAME_LIBMBEDTLS) && !defined(HAVE_COMMONCRYPTO_COMMONDIGEST_H) && !defined(__REACTOS__)
        mbedtls_initialize();
#endif
        break;

    case DLL_PROCESS_DETACH:
        if (reserved) break;
#if defined(HAVE_GNUTLS_HASH) && !defined(HAVE_COMMONCRYPTO_COMMONDIGEST_H)
        gnutls_uninitialize();
#elif defined(SONAME_LIBMBEDTLS) && !defined(HAVE_COMMONCRYPTO_COMMONDIGEST_H) && !defined(__REACTOS__)
        mbedtls_uninitialize();
#endif
        break;
    }
    return TRUE;
}
