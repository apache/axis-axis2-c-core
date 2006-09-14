#include <tcpmon_util.h>
#include <axis2_string.h>

typedef struct tcpmon_util_allocator
{
    int allocated;
    int index;
    axis2_char_t* buffer;
} tcpmon_util_allocator_t;


static void add_string( const axis2_env_t* env, 
                      tcpmon_util_allocator_t* allocator, 
                      axis2_char_t* string);

static void add_axis2_char_t( const axis2_env_t* env, 
                      tcpmon_util_allocator_t* allocator, 
                      axis2_char_t c,
                      int turns);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
tcpmon_util_format_as_xml (const axis2_env_t* env, axis2_char_t* data)
{
    tcpmon_util_allocator_t* allocator = NULL;
    axis2_char_t* p = NULL;
    axis2_char_t* flag = NULL;
    axis2_char_t tmp;
    int tabs = 0;
    int xml_part_found = 0;
    int indent_on_next_it = 0;
   
    allocator = (tcpmon_util_allocator_t*) AXIS2_MALLOC(env->
        allocator, sizeof(tcpmon_util_allocator_t));
    allocator-> buffer = NULL;
    allocator-> allocated = 0;
    allocator-> index = 0;

    for ( p = flag = data, tabs = 0 ; *p ; p ++)
    {
        /** dont do anything until this is seen */
        for(  ;!xml_part_found && *p && !( *p == '<' && *(p+1) == '?'); p ++)
        {
            /** just a new line */
            if ( *p =='\n')
            {
                tmp = *(p+1);
                *(p+1) = '\0';
                add_string ( env, allocator, flag );
                *(p+1) = tmp;
                flag = p+1;
            }
        }
        xml_part_found = 1; /* we come here after we found xml */
        /** ignore cases nothing to process */
        for(; *p ==' ' || *p  == '\n' || *p == '\r' || *p =='\t' || 
                (*p > 'a' && *p < 'z') || /* simple letters */
                (*p > 'A' && *p < 'Z') || /* capital letters */
                (*p > '0' && *p < '9'); p ++)
        {
            /** new lines handle specially to keept the indentation */
            if ( *p =='\n' )
            {
                /* surly there will be an iteration in next line, so dont do explicitly */
                if ( indent_on_next_it)
                {
                    flag = p+1;
                }
                else
                {
                   tmp = *(p+1);
                   *(p+1) = '\0';
                   add_string ( env, allocator, flag );
                   *(p+1) = tmp;
                   flag = p+1;
                   add_axis2_char_t (env, allocator, '\n', 1);
                   add_axis2_char_t ( env, allocator, '\t', tabs );
                }
            }
        } 
        indent_on_next_it = 0;
        /** found a comment */
        if ( *p == '<'  && *(p+1) =='!'  /**just continue since string conts */
                        && *(p+2) =='-'
                        && *(p+3) =='-')
        {
            for ( ; *p ; p ++ )
            {

                /** end the comment */
                if ( *p == '-' && *(p+1) =='-' 
                              && *(p+2) =='>' )
                {
                    break;
                }
                /** just a new line */
                if ( *p =='\n')
                {
                    tmp = *(p+1);
                    *(p+1) = '\0';
                    add_string ( env, allocator, flag );
                    *(p+1) = tmp;
                    flag = p+1;
                    add_axis2_char_t (env, allocator, '\n', 1);
                    add_axis2_char_t ( env, allocator, '\t', tabs );
                }
            }
            /** end the strig insdie the comment:| */
            if ( !*p )
            {
                break;
            }
        }
        /** found a quote*/
        if ( *p == '"'  )
        {
            for( ; *p && *p != '"'; p ++ );
            if ( !*p)
            {
                break;
            }
        }

        /** open tag */ 
        if ( *p =='<'  && !( *(p+1) =='/' || *(p+1) == '?' ) )
        {
            tabs ++;
        }
        /** close tag */
        if ( *p =='<'  &&  *(p+1) =='/' )
        {
            tabs --;
            tmp = *p;
            *p = '\0';
            add_string ( env, allocator, flag );
            add_axis2_char_t (env, allocator, '\n', 1);
            add_axis2_char_t (env, allocator, '\t', tabs);
            *p = tmp;
            flag = p;
        }
        /** found '>' go to a new line */
        if ( *p =='>' )
        {
            /** ignore spaces */
            for( ; *p && *p == ' '; p ++ );
            if ( !*p)
            {
                break;
            }

            /** places like <this/> */
            if( p != data && *(p-1) == '/')
            {
                tabs --;
            }

            /** these are special case - new line would be set on next iterate */
            if ( *(p+1) =='<' && *(p+2) =='/' )
            {
            }
            else
            {
                tmp = *(p+1);
                *(p+1) = '\0';
                add_string ( env, allocator, flag );
                *(p+1) = tmp;
                flag = p+1;
    
                add_axis2_char_t (env, allocator, '\n', 1);
                add_axis2_char_t (env, allocator, '\t', tabs);
                indent_on_next_it = 1;
            }
        }
    }
    add_axis2_char_t (env, allocator, '\0', 1);
    /**(allocator-> buffer + allocator-> index) = '\0';*/
    return allocator-> buffer;
}

void add_string( const axis2_env_t* env,
               tcpmon_util_allocator_t* allocator, 
               axis2_char_t* string )
{
    int additional_len = 0;
    void* dest = NULL;
    void* src = NULL;
    int count = 0;

    additional_len = AXIS2_STRLEN ( string ) +1;
    if ( allocator-> index + additional_len  >= allocator-> allocated )
    {
        if ( allocator-> allocated == 0 )
        {
            allocator-> buffer =
                    AXIS2_MALLOC ( env-> allocator, additional_len );
        }
        else
        {
            allocator-> buffer =
                    AXIS2_REALLOC ( env-> allocator, allocator-> buffer, 
                                allocator-> allocated + additional_len);
        }
        allocator-> allocated += additional_len;
    }
    
    /* copy memory */
    dest = allocator-> buffer + allocator-> index;
    src = string;
    count = additional_len; /* this is with the terminating zero */
    memcpy ( dest, src, count );
   
    allocator-> index += count -1;
}

void add_axis2_char_t( const axis2_env_t* env,
               tcpmon_util_allocator_t* allocator, 
               axis2_char_t c,
               int turns)
{
    int additional_len = 0;

    additional_len = turns + 1;
    if ( allocator-> index + additional_len  >= allocator-> allocated )
    {
        if ( allocator-> allocated == 0 )
        {
            allocator-> buffer =
                    AXIS2_MALLOC ( env-> allocator, additional_len );
        }
        else
        {
            allocator-> buffer =
                    AXIS2_REALLOC ( env-> allocator, allocator-> buffer, 
                                allocator-> allocated + additional_len);
        }
        allocator-> allocated += additional_len;
    }
    
    /* copy memory */
    memset ( allocator-> buffer + allocator-> index, c, turns );

    allocator-> index += turns;
    
}
