#include <stdlib.h>

#define AXIS2_WANT_STRFUNC

#include <axis2_uri.h>

typedef struct schemes_t schemes_t;

/** Structure to store various schemes and their default ports */
struct schemes_t 
{
    /** The name of the scheme */
    const axis2_char_t *name;
    /** The default port for the scheme */
    axis2_port_t default_port;
};

/* Some WWW schemes and their default ports; this is basically /etc/services */
/* This will become global when the protocol abstraction comes */
/* As the schemes are searched by a linear search, */
/* they are sorted by their expected frequency */
static schemes_t schemes[] =
{
    {"http",     AXIS2_URI_HTTP_DEFAULT_PORT},
    {"ftp",      AXIS2_URI_FTP_DEFAULT_PORT},
    {"https",    AXIS2_URI_HTTPS_DEFAULT_PORT},
    {"gopher",   AXIS2_URI_GOPHER_DEFAULT_PORT},
    {"ldap",     AXIS2_URI_LDAP_DEFAULT_PORT},
    {"nntp",     AXIS2_URI_NNTP_DEFAULT_PORT},
    {"snews",    AXIS2_URI_SNEWS_DEFAULT_PORT},
    {"imap",     AXIS2_URI_IMAP_DEFAULT_PORT},
    {"pop",      AXIS2_URI_POP_DEFAULT_PORT},
    {"sip",      AXIS2_URI_SIP_DEFAULT_PORT},
    {"rtsp",     AXIS2_URI_RTSP_DEFAULT_PORT},
    {"wais",     AXIS2_URI_WAIS_DEFAULT_PORT},
    {"z39.50r",  AXIS2_URI_WAIS_DEFAULT_PORT},
    {"z39.50s",  AXIS2_URI_WAIS_DEFAULT_PORT},
    {"prospero", AXIS2_URI_PROSPERO_DEFAULT_PORT},
    {"nfs",      AXIS2_URI_NFS_DEFAULT_PORT},
    {"tip",      AXIS2_URI_TIP_DEFAULT_PORT},
    {"acap",     AXIS2_URI_ACAP_DEFAULT_PORT},
    {"telnet",   AXIS2_URI_TELNET_DEFAULT_PORT},
    {"ssh",      AXIS2_URI_SSH_DEFAULT_PORT},
    { NULL, 0xFFFF }     /* unknown port */
};

AXIS2_EXTERN axis2_port_t AXIS2_CALL 
axis2_uri_port_of_scheme(
        const axis2_char_t *scheme_str)
{
    schemes_t *scheme;

    if (scheme_str) 
    {
        for (scheme = schemes; scheme->name != NULL; ++scheme) 
        {
            if (AXIS2_STRCASECMP(scheme_str, scheme->name) == 0) 
            {
                return scheme->default_port;
            }
        }
    }
    return 0;
}

/* Unparse a axis2_uri_t structure to an URI string.
 * Optionally suppress the password for security reasons.
 */
AXIS2_EXTERN axis2_char_t * AXIS2_CALL 
axis2_uri_to_string(
        const axis2_env_t *env, 
        const axis2_uri_t *uptr, 
        unsigned flags)
{
    axis2_char_t *ret = "";

    /* If suppressing the site part, omit both user name & scheme://hostname */
    if (!(flags & AXIS2_URI_UNP_OMITSITEPART)) 
    {

        /* Construct a "user:password@" string, honoring the passed
         * AXIS2_URI_UNP_ flags: */
        if (uptr->user || uptr->password) 
        {
            ret = axis2_strcat(env,
                      (uptr->user     && !(flags & AXIS2_URI_UNP_OMITUSER))
                          ? uptr->user : "",
                      (uptr->password && !(flags & AXIS2_URI_UNP_OMITPASSWORD))
                          ? ":" : "",
                      (uptr->password && !(flags & AXIS2_URI_UNP_OMITPASSWORD))
                          ? ((flags & AXIS2_URI_UNP_REVEALPASSWORD)
                              ? uptr->password : "XXXXXXXX")
                          : "",
                      ((uptr->user     && !(flags & AXIS2_URI_UNP_OMITUSER)) ||
                       (uptr->password && !(flags & AXIS2_URI_UNP_OMITPASSWORD)))
                          ? "@" : "", 
                      NULL);
        }

        /* Construct scheme://site string */
        if (uptr->hostname) 
        {
            int is_default_port;
            const axis2_char_t *lbrk = "", *rbrk = "";

            if (strchr(uptr->hostname, ':')) 
            { /* v6 literal */
                lbrk = "[";
                rbrk = "]";
            }

            is_default_port =
                (uptr->port_str == NULL ||
                 uptr->port == 0 ||
                 uptr->port == axis2_uri_port_of_scheme(uptr->scheme));

            if (uptr->scheme) 
            {
                ret = axis2_strcat(env,
                              uptr->scheme, "://", ret,
                              lbrk, uptr->hostname, rbrk,
                              is_default_port ? "" : ":",
                              is_default_port ? "" : uptr->port_str,
                              NULL);
            }
            else 
            {
                /* A violation of RFC2396, but it is clear from section 3.2
                 * that the : belongs above to the scheme, while // belongs
                 * to the authority, so include the authority prefix while
                 * omitting the "scheme:" that the user neglected to pass us.
                 */
                ret = axis2_strcat(env,
                              "//", ret, lbrk, uptr->hostname, rbrk,
                              is_default_port ? "" : ":",
                              is_default_port ? "" : uptr->port_str,
                              NULL);
            }
        }
    }

    /* Should we suppress all path info? */
    if (!(flags & AXIS2_URI_UNP_OMITPATHINFO)) 
    {
        /* Append path, query and fragment strings: */
        ret = axis2_strcat(env,
                            ret,
                          (uptr->path)
                              ? uptr->path : "",
                          (uptr->query    && !(flags & AXIS2_URI_UNP_OMITQUERY))
                              ? "?" : "",
                          (uptr->query    && !(flags & AXIS2_URI_UNP_OMITQUERY))
                              ? uptr->query : "",
                          (uptr->fragment && !(flags & AXIS2_URI_UNP_OMITQUERY))
                              ? "#" : NULL,
                          (uptr->fragment && !(flags & AXIS2_URI_UNP_OMITQUERY))
                              ? uptr->fragment : NULL,
                          NULL);
    }
    return ret;
}

/* Here is the hand-optimized parse_uri_components().  There are some wild
 * tricks we could pull in assembly language that we don't pull here... like we
 * can do word-at-time scans for delimiter characters using the same technique
 * that fast AXIS2_MEMCHR()s use.  But that would be way non-portable. -djg
 */

/* We have a axis2_table_t that we can index by character and it tells us if the
 * character is one of the interesting delimiters.  Note that we even get
 * compares for NUL for free -- it's just another delimiter.
 */

#define T_COLON           0x01        /* ':' */
#define T_SLASH           0x02        /* '/' */
#define T_QUESTION        0x04        /* '?' */
#define T_HASH            0x08        /* '#' */
#define T_NUL             0x80        /* '\0' */

#if AXIS2_CHARSET_EBCDIC
/* Delimiter table for the EBCDIC character set */
static const unsigned char uri_delims[256] = 
{
    T_NUL,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,T_SLASH,0,0,0,0,0,0,0,0,0,0,0,0,0,T_QUESTION,
    0,0,0,0,0,0,0,0,0,0,T_COLON,T_HASH,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
#else
/* Delimiter table for the ASCII character set */
static const unsigned char uri_delims[256] = 
{
    T_NUL,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,T_HASH,0,0,0,0,0,0,0,0,0,0,0,T_SLASH,
    0,0,0,0,0,0,0,0,0,0,T_COLON,0,0,0,0,T_QUESTION,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
#endif


/* it works like this:
    if (uri_delims[ch] & NOTEND_foobar) {
        then we're not at a delimiter for foobar
    }
*/

/* Note that we optimize the scheme scanning here, we cheat and let the
 * compiler know that it doesn't have to do the & masking.
 */
#define NOTEND_SCHEME     (0xff)
#define NOTEND_HOSTINFO   (T_SLASH | T_QUESTION | T_HASH | T_NUL)
#define NOTEND_PATH       (T_QUESTION | T_HASH | T_NUL)

/* parse_uri_components():
 * Parse a given URI, fill in all supplied fields of a uri_components
 * structure. This eliminates the necessity of extracting host, port,
 * path, query info repeatedly in the modules.
 * Side effects:
 *  - fills in fields of uri_components *uptr
 *  - none on any of the r->* fields
 */
AXIS2_EXTERN axis2_uri_t *AXIS2_CALL
axis2_uri_parse_string(
        const axis2_env_t *env, 
        const axis2_char_t *uri)
{
    axis2_uri_t *uptr = NULL;

    const axis2_char_t *s;
    const axis2_char_t *s1;
    const axis2_char_t *hostinfo;
    axis2_char_t *endstr;
    int port;
    int v6_offset1 = 0, v6_offset2 = 0;
    
    uptr = AXIS2_MALLOC(env->allocator, sizeof(struct axis2_uri_t));

    if (!uptr)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /* Initialize the structure. parse_uri() and parse_uri_components()
     * can be called more than once per request.
     */
    memset (uptr, '\0', sizeof(*uptr));
    uptr->is_initialized = 1;

    /* We assume the processor has a branch predictor like most --
     * it assumes forward branches are untaken and backwards are taken.  That's
     * the reason for the gotos.
     */
    if (uri[0] == '/') 
    {
        /* RFC2396 #4.3 says that two leading slashes mean we have an
         * authority component, not a path!  Fixing this looks scary
         * with the gotos here.  But if the existing logic is valid,
         * then presumably a goto pointing to deal_with_authority works.
         *
         * RFC2396 describes this as resolving an ambiguity.  In the
         * case of three or more slashes there would seem to be no
         * ambiguity, so it is a path after all.
         */
        if (uri[1] == '/' && uri[2] != '/') 
        {
            s = uri + 2 ;
            goto deal_with_authority ;
        }

deal_with_path:
        /* we expect uri to point to first character of path ... remember
         * that the path could be empty -- http://foobar?query for example
         */
        s = uri;
        while ((uri_delims[*(unsigned char *)s] & NOTEND_PATH) == 0) 
        {
            ++s;
        }
        if (s != uri) 
        {
            uptr->path = AXIS2_STRMEMDUP(uri, s - uri, env);
        }
        if (*s == 0) 
        {
            return uptr;
        }
        if (*s == '?') 
        {
            ++s;
            s1 = strchr(s, '#');
            if (s1) {
                uptr->fragment = AXIS2_STRDUP(s1 + 1, env);
                uptr->query = AXIS2_STRMEMDUP(s, s1 - s, env);
            }
            else 
            {
                uptr->query = AXIS2_STRDUP(s, env);
            }
            return uptr;
        }
        /* otherwise it's a fragment */
        uptr->fragment = AXIS2_STRDUP(s + 1, env);
        return uptr;
    }

    /* find the scheme: */
    s = uri;
    while ((uri_delims[*(unsigned char *)s] & NOTEND_SCHEME) == 0) 
    {
        ++s;
    }
    /* scheme must be non-empty and followed by :// */
    if (s == uri || s[0] != ':' || s[1] != '/' || s[2] != '/') 
    {
        goto deal_with_path;        /* backwards predicted taken! */
    }

    uptr->scheme = AXIS2_STRMEMDUP(uri, s - uri, env);
    s += 3;

deal_with_authority:
    hostinfo = s;
    while ((uri_delims[*(unsigned char *)s] & NOTEND_HOSTINFO) == 0) 
    {
        ++s;
    }
    uri = s;        /* whatever follows hostinfo is start of uri */
    uptr->hostinfo = AXIS2_STRMEMDUP(hostinfo, uri - hostinfo, env);

    /* If there's a username:password@host:port, the @ we want is the last @...
     * too bad there's no memrchr()... For the C purists, note that hostinfo
     * is definately not the first character of the original uri so therefore
     * &hostinfo[-1] < &hostinfo[0] ... and this loop is valid C.
     */
    do 
    {
        --s;
    } while (s >= hostinfo && *s != '@');
    if (s < hostinfo) 
    {
        /* again we want the common case to be fall through */
deal_with_host:
        /* We expect hostinfo to point to the first character of
         * the hostname.  If there's a port it is the first colon,
         * except with IPv6.
         */
        if (*hostinfo == '[') 
        {
            v6_offset1 = 1;
            v6_offset2 = 2;
            s = AXIS2_MEMCHR(hostinfo, ']', uri - hostinfo);
            if (s == NULL) {
                return AXIS2_FAILURE;
            }
            if (*++s != ':') {
                s = NULL; /* no port */
            }
        }
        else 
        {
            s = AXIS2_MEMCHR(hostinfo, ':', uri - hostinfo);
        }
        if (s == NULL) {
            /* we expect the common case to have no port */
            uptr->hostname = AXIS2_STRMEMDUP(
                                            hostinfo + v6_offset1,
                                            uri - hostinfo - v6_offset2, env);
            goto deal_with_path;
        }
        uptr->hostname = AXIS2_STRMEMDUP(
                                        hostinfo + v6_offset1,
                                        s - hostinfo - v6_offset2, env);
        ++s;
        uptr->port_str = AXIS2_STRMEMDUP(s, uri - s, env);
        if (uri != s) 
        {
            port = strtol(uptr->port_str, &endstr, 10);
            uptr->port = port;
            if (*endstr == '\0') {
                goto deal_with_path;
            }
            /* Invalid characters after ':' found */
            return AXIS2_FAILURE;
        }
        uptr->port = axis2_uri_port_of_scheme(uptr->scheme);
        goto deal_with_path;
    }

    /* first colon delimits username:password */
    s1 = AXIS2_MEMCHR(hostinfo, ':', s - hostinfo);
    if (s1) 
    {
        uptr->user = AXIS2_STRMEMDUP(hostinfo, s1 - hostinfo, env);
        ++s1;
        uptr->password = AXIS2_STRMEMDUP(s1, s - s1, env);
    }
    else 
    {
        uptr->user = AXIS2_STRMEMDUP(hostinfo, s - hostinfo, env);
    }
    hostinfo = s + 1;
    goto deal_with_host;
    return uptr;
}

/* Special case for CONNECT parsing: it comes with the hostinfo part only */
/* See the INTERNET-DRAFT document "Tunneling SSL Through a WWW Proxy"
 * currently at http://www.mcom.com/newsref/std/tunneling_ssl.html
 * for the format of the "CONNECT host:port HTTP/1.0" request
 */
AXIS2_EXTERN axis2_uri_t *AXIS2_CALL
axis2_uri_parse_hostinfo(
        const axis2_env_t *env, 
        const axis2_char_t *hostinfo)
{
    axis2_uri_t *uptr = NULL;
    const axis2_char_t *s;
    axis2_char_t *endstr;
    const axis2_char_t *rsb;
    int v6_offset1 = 0;

    uptr = AXIS2_MALLOC(env->allocator, sizeof(struct axis2_uri_t));
    
    /* Initialize the structure. parse_uri() and parse_uri_components()
     * can be called more than once per request.
     */
    memset(uptr, '\0', sizeof(*uptr));
    uptr->is_initialized = 1;
    uptr->hostinfo = AXIS2_STRDUP(hostinfo, env);

    /* We expect hostinfo to point to the first character of
     * the hostname.  There must be a port, separated by a colon
     */
    if (*hostinfo == '[') 
    {
        if ((rsb = strchr(hostinfo, ']')) == NULL ||
            *(rsb + 1) != ':') {
            return NULL;
        }
        /* literal IPv6 address */
        s = rsb + 1;
        ++hostinfo;
        v6_offset1 = 1;
    }
    else 
    {
        s = strchr(hostinfo, ':');
    }
    if (s == NULL) 
    {
        return NULL;
    }
    uptr->hostname = AXIS2_STRNDUP(hostinfo, s - hostinfo - v6_offset1, env);
    ++s;
    uptr->port_str = AXIS2_STRDUP(s, env);
    if (*s != '\0') 
    {
        uptr->port = (unsigned short) strtol(uptr->port_str, &endstr, 10);
        if (*endstr == '\0') 
        {
            return uptr;
        }
        /* Invalid characters after ':' found */
    }
    return NULL;
}

/* Resolve relative to a base.  This means host/etc, and (crucially) path */
AXIS2_EXTERN axis2_uri_t *AXIS2_CALL 
axis2_uri_resolve_relative(
        const axis2_env_t *env,
        const axis2_uri_t* base,
        axis2_uri_t* uptr)
{
    if ( uptr == NULL || base == NULL || ! base->is_initialized
               || ! uptr->is_initialized ) 
    {
        return NULL;
    }
    /* The interesting bit is the path.  */
    if ( uptr->path == NULL ) 
    {
        if ( uptr->hostname == NULL ) 
        {
            /* is this compatible with is_initialised?  Harmless in any case */
            uptr->path = base->path ? base->path : AXIS2_STRDUP("/", env) ;
        }
        else 
        {
            /* deal with the idiosyncracy of APR allowing path==NULL
             * without risk of breaking back-compatibility
             */
             uptr->path = AXIS2_STRDUP("/", env) ;
        }
    }
    else if ( uptr->path[0] != '/' ) 
    {
        size_t baselen ;
        const char* basepath = base->path ? base->path :"/" ;
        const char* path = uptr->path ;
        const char* base_end = strrchr(basepath, '/') ;

        /* if base is nonsensical, bail out */
        if ( basepath[0] != '/' ) 
        {
            return NULL;
        }
        /* munch "up" components at the start, and chop them from base path */
        while ( !strncmp(path, "../", 3) ) 
        {
            while ( base_end > basepath ) 
            {
                if ( *--base_end == '/' ) 
                {
                    break ;
                }
            }
            path += 3 ;
        }
        /* munch "here" components at the start */
        while ( !strncmp(path, "./", 2) ) 
        {
              path += 2 ;
        }
        baselen = base_end-basepath+1 ;
        uptr->path = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * baselen + strlen(path) + 1 ) ;
        memcpy(uptr->path, basepath, baselen) ;
        strcpy(uptr->path+baselen, path) ;
    }
        
    /* The trivial bits are everything-but-path */
    if ( uptr->scheme == NULL ) 
    {
        uptr->scheme = base->scheme ;
    }
    if ( uptr->hostinfo == NULL ) 
    {
        uptr->hostinfo = base->hostinfo ;
    }
    if ( uptr->user == NULL ) 
    {
        uptr->user = base->user ;
    }
    if ( uptr->password == NULL ) 
    {
        uptr->password = base->password ;
    }
    if ( uptr->hostname == NULL ) 
    {
        uptr->hostname = base->hostname ;
    } 
    if ( uptr->port_str == NULL ) 
    {
        uptr->port_str = base->port_str ;
    }
    if ( uptr->hostent == NULL ) 
    {
        uptr->hostent = base->hostent ;
    }
    if ( ! uptr->port ) 
    {
        uptr->port = base->port ;
    }
    
    return uptr ;
}

AXIS2_EXTERN axis2_uri_t *AXIS2_CALL 
axis2_uri_parse_relative(
        const axis2_env_t *env,
        const axis2_uri_t* base,
        const char* uri)
{
    axis2_uri_t *uptr = NULL;

    uptr = axis2_uri_parse_string(env, uri) ;
    if (NULL == uptr && AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error) ) 
    {
        return uptr ;
    }
    uptr = axis2_uri_resolve_relative(env, base, uptr) ;

    return uptr;
}

