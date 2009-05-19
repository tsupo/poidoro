/*
 * poidoro.c
 *  COLORS �̓��L(�|�C�h�����A���Ւ�)�ɓ��e����v���O����
 *      �u�|�C�h�����{�b�g�v�A���́u�|�C�h���{�b�g�v
 *
 *      written by H.Tsujimura   13 Feb 2005
 *
 * $Log: /comm/poidoro/poidoro.c $
 * 
 * 1     09/05/14 4:46 tsupo
 * (1) �r���h���̃f�B���N�g���\���𐮗�
 * (2) VSS�T�[�o���_��ύX
 * 
 * 16    06/02/24 22:28 Tsujimura543
 * -v �I�v�V�����w�莞�̏o�͓��e���t�B�b�N�X
 * 
 * 15    06/02/24 22:26 Tsujimura543
 * checkPoidoro() �� response �� malloc() ������� if ���̏����������΂�����
 * �o�O (�f�O���[�h) ���C��
 * 
 * 14    06/02/22 12:14 Tsujimura543
 * poidoro() �����t�@�N�^�����O
 * 
 * 13    06/02/22 11:12 Tsujimura543
 * (1) COLORS �̃f�U�C���ύX�ɑΉ�
 * (2) �����̃|�C�h���������J�݂̏ꍇ�ɁA�����Ɏ����̓��L��T�������錻��
 * �ɑΏ�
 * 
 * 12    06/02/21 21:19 Tsujimura543
 * (1) ���\�[�X���[�N(�t�@�C���̕��Y��)�ɑΏ�
 * (2) �����̃|�C�h�����������΂��ɒT������悤�ɂ���
 * 
 * 11    06/02/21 20:46 Tsujimura543
 * 1��������N�����Ă���d���e���Ȃ��悤�ɂ���@�\��ǉ�
 * 
 * 10    06/02/21 18:55 Tsujimura543
 * checkPoidoro() ���C��(�]���֐����C��)
 * 
 * 9     06/02/21 16:53 Tsujimura543
 * ���m�̃|�C�h�����𔭌�����@�\���t�B�b�N�X
 * 
 * 8     06/02/21 15:35 Tsujimura543
 * (1) �딚�h�~�@�\����������
 * (2) http���N�G�X�g���o���񐔂��\�Ȍ��茸�炵��
 * (3) ���̑��A�o�O�t�B�b�N�X
 * 
 * 7     06/02/16 20:27 Tsujimura543
 * ���e����R�����g�̓��e���u���ԁv�Ō��߂Ă����̂��u�����v�Ō��߂�悤��
 * �ύX
 * 
 * 6     06/02/16 20:17 Tsujimura543
 * Sleep() ����ӏ��𑝂₵�Ă݂�
 * 
 * 5     06/02/16 20:07 Tsujimura543
 * ���m�̃|�C�h������T������@�\��ǉ�
 * 
 * 4     06/02/15 11:43 Tsujimura543
 * �A�����e�h�~�@�\��ǉ�
 * 
 * 3     06/02/14 21:26 Tsujimura543
 * ���e����R�����g�Ɏ����̃|�C�h������URL����ɕt�L����悤�ɂ���
 * 
 * 2     06/02/14 20:32 Tsujimura543
 * ���e�Ώۂ̃|�C�h�����Ɋւ������ݒ�t�@�C������ǂݎ���悤�ɂ���
 * 
 * 1     06/02/14 18:08 Tsujimura543
 * �ŏ��̔�
 */

#include "xmlRPC.h"
#include <time.h>

#ifndef	lint
static char	*rcs_id =
"$Header: /comm/poidoro/poidoro.c 1     09/05/14 4:46 tsupo $";
#endif

int
loginColors( const char *username,  // (I) COLORS ID (���[���A�h���X)
             const char *password,  // (I) �p�X���[�h
             char       *cookie );  // (O) �N�b�L�[


#define CLRS_HOME   "<a href=\"f_home.php?"


struct poidoroTarget {
    char    *did;
    char    *uid;
    char    *username;
    BOOL    alive;
}   pt[] = {
    { "20043", "547",   "tsupo", TRUE  },
    { NULL,    NULL,    NULL,    FALSE },
};

typedef enum poidoroStat {
    _continue = 0,
    _break,
    _proceed
}   POI_STAT;

POI_STAT
getUidDid( const char *str, char *uid, char *did )
{
    POI_STAT    ret = _continue;
    const char  *p  = str;
    const char  *q;
    char        *r;

    /* uid */
    q = strstr( p, "?uid=" );
    if ( !q )
        q = strstr( p, "&uid=" );
    if ( !q )
        return ( ret );
    q = strchr( q, '=' ) + 1;
    r = &(uid[0]);
    while ( (*q >= '0') && (*q <= '9') )
        *r++ = *q++;
    *r = '\0';

    /* did */
    q = strstr( p, "?did=" );
    if ( !q )
        q = strstr( p, "&did=" );
    if ( !q )
        return ( ret );
    q = strchr( q, '=' ) + 1;
    r = &(did[0]);
    while ( (*q >= '0') && (*q <= '9') )
        *r++ = *q++;
    *r = '\0';
    ret = _proceed;

    return ( ret );
}

/* ���̓��L���|�C�h�����Ȃ̂��ǂ����𔻒f */
BOOL
checkPoidoro(
        const char *_did,
        const char *_uid,
        const char *_username,
        int        advantage,
        char       *cookie,
        BOOL       verbose
    )
{
    char    url[MAX_URLLENGTH];
    char    *response;
    BOOL    ret = FALSE;
    int     poidoro = advantage;

    response = (char *)malloc( 65536 * 16 );
    if ( !response )
        return ( ret );

    if ( strlen( _username ) <= 2 )
        poidoro -= 2500;

    sprintf( url, "http://clrs.jp/view_diary.php?did=%s&uid=%s", _did, _uid );
    http_getEx( url, response, cookie );
    if ( *response ) {
        char    *p, *q, *r = NULL;
        char    title[BUFSIZ];

        title[0] = '\0';
        p = response;
        if ( p )
            r = strstr( p, "</form>" );
        while ( *p && r && (p < r) ) {
            q = strstr( p,
                "<td colspan=\"2\" height=\"22\" width=\"300\" align=\"left\""
                " background=\"http://img.clrs.jp/images/box/bg_bar.gif\">"
                        "<strong>" );
            if ( q && (q < r) ) {
                p = q + 115;
                q = strstr( p, "</strong>" );
                if ( q ) {
                    strncpy( title, p, q - p );
                    title[q - p] = '\0';
                    if ( strstr( title, "�|�C�h����" ) ||
                         strstr( title, "�ۂ��ǂ뒠" ) ||
                         strstr( title, "�߲��ے�" )   ||
                         strstr( title, "�|�C�h����" ) ||
                         strstr( title, "�ۂ��ǂ뒟" ) ||
                         strstr( title, "�߲��ے�" )      )
                        poidoro += 10;
                    else
                    if ( strstr( title, "�|�C�h��" ) ||
                         strstr( title, "�ۂ��ǂ�" ) ||
                         strstr( title, "�߲���" )   ||
                         strstr( title, "�|�C�D" )   ||
                         strstr( title, "�ۂ��D" )   ||
                         strstr( title, "�߲�D" )       )
                        poidoro += 5;
                    else
                    if ( strstr( title, "�|�C���g�D�_" ) ||
                         strstr( title, "�ۂ���ƓD�_" ) ||
                         strstr( title, "�߲�ēD�_" )       )
                        poidoro += 3;

                    if ( strstr( title, "�Ȃ�" )       ||
                         strstr( title, "����܂���" ) ||
                         strstr( title, "�o��" )       ||
                         strstr( title, "��" )         ||
                         strstr( title, "��" )         ||
                         strstr( title, "��" )         ||
                         strstr( title, "�f" )         ||
                         strstr( title, "��" )         ||
                         strstr( title, "��" )         ||
                         strstr( title, "��" )         ||
                         strstr( title, "(@)" )           )
                        poidoro -= 500;

                    p = q + 10;
                }
            }

            break;
        }

        while ( *p && r && (p < r) ) {
            q = strstr( p, "http://clrs.jp/view_diary.php?did=" );
            if ( !q || (q > r) )
                q = strstr( p, "http://clrs.jp/view_diary.php?uid=" );
            if ( q && (q < r) ) {
                char    *s, *t;

                s = strstr( q, "#comment" );
                t = strchr( q, '"');
                if ( s && t && (s < r) && (s < t) ) {
                    poidoro += 1;
                    p = t + 1;
                }
                else
                    p = q + 1;
                poidoro += 1;
                continue;
            }

            break;
        }

        if ( verbose )
            fprintf( stderr, "\t�|�C�h���x: %d [%s]\n",
                     poidoro, title[0] ? title : "(����)" );
        if ( poidoro >= 25 )
            ret = TRUE;
    }

    free( response );
    return ( ret );
}

/* �w�肳�ꂽ���L���|�C�h�������e�[�u���ɓo�^ */
POI_STAT
_addPoidoro(
        struct poidoroTarget *tp,
        int                  num,
        const char           *_did,
        const char           *_uid,
        const char           *_username,
        BOOL                 alive,
        POI_STAT             ret
    )
{
    int len;

    /* username */
    len = strlen( _username );
    if ( tp[num].username )
        free( tp[num].username );
    tp[num].username = (char *)malloc( len + 1 );
    if ( tp[num].username == NULL )
        return ( ret );
    strcpy( tp[num].username, _username ); 

    /* uid */
    len = strlen( _uid );
    if ( tp[num].uid )
        free( tp[num].uid );
    tp[num].uid = (char *)malloc( len + 1 );
    if ( tp[num].uid == NULL )
      return ( ret );
    strcpy( tp[num].uid, _uid ); 

    /* did */
    len = strlen( _did );
    if ( tp[num].did )
        free( tp[num].did );
    tp[num].did = (char *)malloc( len + 1 );
    if ( tp[num].did == NULL )
        return ( ret );
    strcpy( tp[num].did, _did );

    /* alive */
    tp[num].alive = alive;
    ret = _proceed;

    return ( ret );
}

/* �V�����|�C�h�������|�C�h�����e�[�u���ɓo�^ */
POI_STAT
addPoidoro(
        struct poidoroTarget *tp,
        int                  curNum,
        const char           *_did,
        const char           *_uid,
        const char           *_username,
        int                  advantage,
        char                 *cookie,
        BOOL                 verbose
    )
{
    POI_STAT    ret   = _continue;
    BOOL        found = FALSE;
    int         i;

    // ���łɃ|�C�h�������e�[�u���ɓo�^�ς݂��ǂ����̃`�F�b�N
    for ( i = 0; i < curNum; i++ ) {
        if ( !strcmp( tp[i].did, _did ) ) {
            found = TRUE;
            break;
        }
    }
    if ( found )
        return ( ret );

    // ���Y���L���{���Ƀ|�C�h�����Ȃ̂��ǂ����m�F
    if ( verbose )
        fprintf( stderr,
                 "    �V���|�C�h�������ǂ������`�F�b�N: did=%s&uid=%s ...\n",
                 _did, _uid );
    if ( checkPoidoro( _did, _uid, _username, advantage, cookie,
                       verbose )                                 == FALSE ) {
        ret = _addPoidoro( tp, curNum, _did, _uid, _username, FALSE,
                           _proceed );
        return ( ret );
    }

    for ( i = 0; i < curNum; i++ ) {
        if ( !strcmp( tp[i].uid, _uid ) ) {
            int did1, did2;

            did1 = atol( tp[i].did );
            did2 = atol( _did );
            if ( did1 < did2 ) {
                _addPoidoro( tp, i, _did, _uid, _username, FALSE, _continue );
                ret = _continue;
            }
            else
                ret = _addPoidoro( tp, curNum, _did, _uid, _username, FALSE,
                                   _proceed );
            found = TRUE;
            break;
        }

    }
    if ( found == TRUE )
        return ( ret );

    if ( verbose )
        fprintf( stderr, "\t    �V���|�C�h�����𔭌�: did=%s&uid=%s\n",
                 _did, _uid );
    ret = _addPoidoro( tp, curNum, _did, _uid, _username, TRUE, _break );
    return ( ret );
}

/* �V�����|�C�h�����𔭌����A�|�C�h�����e�[�u���ɓo�^ */
void
findPoidoro(
        const char           *response,
        const char           *targetString,
        struct poidoroTarget **tp,
        int                  *curNum,
        int                  *maxNum,
        int                  advantage,
        char                 *cookie,
        BOOL                 verbose
    )
{
    char        *p, *q, *r;
    static int  mm1 = 0, dd1 = 0; /* ���� */
    static int  mm2 = 0, dd2 = 0; /* ��� */

    if ( mm1 == 0 ) {
        time_t      t;
        struct tm   *tm;

        t = time( NULL );
        tm = localtime( &t );
        mm1 = tm->tm_mon + 1;
        dd1 = tm->tm_mday;

        t -= (24 * 60 * 60); /* ��� */
        tm = localtime( &t );
        mm2 = tm->tm_mon + 1;
        dd2 = tm->tm_mday;
    }

    p = strstr( response, targetString );
    if ( p ) {
        p += strlen( targetString );
        q = strstr( p, "</form>" );
        if ( q ) {
            if ( *tp == NULL ) {
                *tp = (struct poidoroTarget *)
                            malloc( 256 * sizeof (struct poidoroTarget) );
                *maxNum = 256;
            }
            else if ( *maxNum - *curNum < 128 ) {
                struct poidoroTarget    *np;
                np = (struct poidoroTarget *)
                            realloc( (void *)(*tp),
                                     (*maxNum + 256) *
                                        sizeof (struct poidoroTarget) );
                if ( np ) {
                    *tp = np;
                    *maxNum += 256;
                }
            }
            if ( *tp && (*maxNum > *curNum) ) {
                char        _username[80];
                char        _uid[32];
                char        _did[32];
                int         _mm, _dd;
                char        *s, *t, *u;
                POI_STAT    stat;

                while ( p < q ) {
                    /* username */
                    r = strstr( p, CLRS_HOME );
                    if ( !r || (r > q) )
                        break;
                    r += 20;
                    p = strchr( r, '>' );
                    if ( !p || (p > q) )
                        break;
                    p++;
                    r = strchr( p, '<' );
                    if ( !r || (r > q) )
                        break;
                    strncpy( _username, p, r - p );
                    _username[r - p] = '\0';

                    /* ���e���� */
                    p = r + 1;
                    r = strstr( p, "<td align=\"right\"><strong>" );
                    if ( !r || (r > q) )
                        break;
                    p = r + 26;
                    if ( (*p < '0') || (*p > '9') )
                        break;
                    if ( *p == '0' )
                       p++;
                    _mm = atol( p );
                    if ( (_mm != mm1) && (_mm != mm2) )
                        continue;

                    while ( (*p >= '0') && (*p <= '9') )
                        p++;
                    while ( (*p < '0') || (*p > '9') )
                        p++;
                    if ( (p >= q) || (*p < '0') || (*p > '9') )
                        break;
                    if ( *p == '0' )
                       p++;
                    _dd = atol( p );
                    if ( (_dd != dd1) && (_dd != dd2) )
                        continue;

                    s = strstr( p, "<hr>" );
                    if ( !s || (s > q) )
                        continue;

                    /* uid */
                    t = strstr( p, "view_diary.php?uid=" );
                    if ( !t || (t > s) )
                        t = strstr( p, "&amp;uid=" );
                    if ( !t || (t > s) )
                        continue;
                    t = strchr( t, '=' ) + 1;
                    u = &(_uid[0]);
                    while ( (*t >= '0') && (*t <= '9') )
                        *u++ = *t++;
                    *u = '\0';

                    /* did */
                    t = strstr( p, "view_diary.php?did=" );
                    if ( !t || (t > s) )
                        t = strstr( p, "&amp;did=" );
                    if ( !t || (t > s) )
                        continue;
                    t = strchr( t, '=' ) + 1;
                    u = &(_did[0]);
                    while ( (*t >= '0') && (*t <= '9') )
                        *u++ = *t++;
                    *u = '\0';
                    p = t;

                    (*tp)[*curNum].did = NULL;
                    (*tp)[*curNum].uid = NULL;
                    (*tp)[*curNum].username = NULL;
                    stat = addPoidoro( *tp, *curNum,
                                       _did, _uid, _username,
                                       advantage, cookie, verbose );
                    if ( stat == _continue )
                        continue;
                    if ( stat == _break )
                        break;

                    (*curNum)++;
                    if ( *curNum >= (*maxNum - 1) )
                        break;
                }

                (*tp)[*curNum].did = NULL;
                (*tp)[*curNum].uid = NULL;
                (*tp)[*curNum].username = NULL;
                (*tp)[*curNum].alive = FALSE;
            }
        }
    }
}

/* �w��|�C�h�������疢�m�̃|�C�h������T�� */
int
_findNewPoidoro(
        struct poidoroTarget **tp,
        int                  *curNum,
        int                  *maxNum,
        const char           *uid,
        const char           *did,
        char                 *cookie,
        BOOL                 verbose
    )
{
    char    url[MAX_URLLENGTH];
    char    *response;

    response = (char *)malloc( 65536 * 16 );
    if ( !response )
        return ( *curNum );

    /* �w��|�C�h������ get */
    memset( response, 0x00, 65536 * 16 );
    sprintf( url, "http://clrs.jp/view_diary.php?uid=%s&did=%s",
             uid, did );
    if ( verbose )
        fprintf( stderr, "�����|�C�h����: %s ��T���� ...\n", url );
    http_getEx( url, response, cookie );
    if ( *response ) {
        /* �|�C�h��������� */
        findPoidoro( response,
                     "<form action=\"delete_comment.php",
                     tp, curNum, maxNum, 0, cookie, verbose );
    }
    free( response );

    return ( *curNum );
}

/* �w��L�[���[�h�̎w��{�[�h���疢�m�̃|�C�h������T�� */
int
_findNewPoidoroFromKeyword(
        struct poidoroTarget **tp,
        int                  *curNum,
        int                  *maxNum,
        const char           *kid,
        const char           *bid,
        char                 *cookie,
        BOOL                 verbose
    )
{
    char    url[MAX_URLLENGTH];
    char    *response;

    response = (char *)malloc( 65536 * 16 );
    if ( !response )
        return ( *curNum );

    /* �w��{�[�h�� get */
    memset( response, 0x00, 65536 * 16 );
    sprintf( url, "http://clrs.jp/view_msgboard.php?bid=%s&kid=%s",
             bid, kid );
    if ( verbose )
        fprintf( stderr, "�|�C�h�����W�X�^�[: %s ��T���� ...\n", url );
    http_getEx( url, response, cookie );
    if ( *response ) {
        /* �{�[�h����� */
        findPoidoro( response,
                     "<form action=\"delete_msgcomment.php",
                     tp, curNum, maxNum, 10, cookie, verbose );
    }
    free( response );

    return ( *curNum );
}

/* �V�����L���疢�m�̃|�C�h������T�� */
int
_findNewPoidoroFromNewDiary(
        struct poidoroTarget **tp,
        int                  *curNum,
        int                  *maxNum,
        char                 *cookie,
        BOOL                 verbose
    )
{
    char    url[MAX_URLLENGTH];
    char    *response;
    int     maxPage = 10;   /* �`�F�b�N����y�[�W�� */
    int     i;

    response = (char *)malloc( 65536 * 16 );
    if ( !response )
        return ( *curNum );

    /* �V�����L�ꗗ�� get */
    for ( i = 0; i < maxPage; i++ ) {
        if ( i > 0 )
            Sleep( 2500 );
        memset( response, 0x00, 65536 * 16 );
        if ( i == 0 )
            sprintf( url, "http://clrs.jp/search_user.php?mode=searchexecd" );
        else
            sprintf( url,
                    "http://clrs.jp/search_user.php?page=%d&mode=searchexecd",
                     i + 1 );
        if ( verbose )
            fprintf( stderr, "�V�����L�ꗗ: %s ��T���� ...\n", url );
        http_getEx( url, response, cookie );
        if ( *response ) {
            if ( *tp == NULL ) {
                *tp = (struct poidoroTarget *)
                            malloc( 256 * sizeof (struct poidoroTarget) );
                *maxNum = 256;
            }
            else if ( *maxNum - *curNum < 128 ) {
                struct poidoroTarget    *np;
                np = (struct poidoroTarget *)
                            realloc( (void *)(*tp),
                                     (*maxNum + 256) *
                                        sizeof (struct poidoroTarget) );
                if ( np ) {
                    *tp = np;
                    *maxNum += 256;
                }
            }
            if ( *tp && (*maxNum > *curNum) ) {
                char        *p, *q, *r;
                char        title[256];
                char        _username[80];
                char        _uid[32];
                char        _did[32];
                POI_STAT    stat;

                /* �V�����L�ꗗ����� */
                p = response;
                while ( *p ) {
                    q = strstr( p, "<img src=\"images/box/p_green.gif\"" );
                    if ( !q )
                        break;
                    p = q + 34;
                    stat = getUidDid( p, _uid, _did );
                    if ( stat == _continue )
                        continue;

                    q = strchr( p, '>' );
                    if ( !q )
                        continue;
                    p = q++;
                    r = strchr( q, '<' );
                    if ( !r )
                        continue;
                    p = r;
                    strncpy( title, q, r - q );
                    title[r - q] = '\0';
                    if ( !strstr( title, "�|�C�h����" ) &&
                         !strstr( title, "�߲��ے�"   ) &&
                         !strstr( title, "�ۂ��ǂ뒠" ) &&
                         !strstr( title, "�|�C�h����" ) &&
                         !strstr( title, "�߲��ے�"   ) &&
                         !strstr( title, "�ۂ��ǂ뒟" )    )
                        continue;

                    r = p;
                    do {
                        q = strstr( r, CLRS_HOME );
                        if ( !q )
                            break;
                        q += 25;
                        r = strchr( q, '>' );
                        if ( !r )
                            break;
                        r++;
                    }
                    while ( *r == '<' );
                    if ( !q || !r || !(*r) )
                        continue;
                    q = strchr( r, '<' );
                    if ( !q )
                        continue;
                    strncpy( _username, r, q - r );
                    _username[q - r] = '\0';

                    (*tp)[*curNum].did = NULL;
                    (*tp)[*curNum].uid = NULL;
                    (*tp)[*curNum].username = NULL;
                    stat = addPoidoro( *tp, *curNum,
                                       _did, _uid, _username,
                                       5, cookie, verbose );
                    if ( stat == _continue )
                        continue;
                    if ( stat == _break )
                        break;

                    (*curNum)++;
                    if ( *curNum >= (*maxNum - 1) )
                        break;
                }

                (*tp)[*curNum].did = NULL;
                (*tp)[*curNum].uid = NULL;
                (*tp)[*curNum].username = NULL;
                (*tp)[*curNum].alive = FALSE;
            }
        }
    }
    free( response );

    return ( *curNum );
}

/* ���m�̃|�C�h�����𔭌����� */
int
findNewPoidoro(
        struct poidoroTarget **tp,
        int                  *num,
        char                 *cookie,
        BOOL                 verbose
    )
{
    int                     i;
    int                     maxNum = 0;
    struct poidoroTarget    *p;

    if ( !num || (*num < 0) )
        return ( 0 );

    /* �����̃|�C�h������T�� */
    for ( p = &(pt[0]); p->did; p++ ) {
        if ( !strcmp( p->did, "0" ) )
            continue;
        _findNewPoidoro( tp, num, &maxNum,
                         p->uid, p->did, cookie, verbose );
    }

    /* ���e�Ώۃ|�C�h������T�� */
    for ( i = 0; i < *num; i++ ) {
        if ( (*tp)[i].alive == FALSE )
            continue;
        _findNewPoidoro( tp, num, &maxNum,
                         (*tp)[i].uid, (*tp)[i].did, cookie, verbose );
        Sleep( 2500 );
    }

    /* �|�C�h�����W�X�^�[ ��T�� */
    // http://clrs.jp/view_msgboard.php?bid=728&kid=1003
    _findNewPoidoroFromKeyword( tp, num, &maxNum,
                                "1003", "728", cookie, verbose );

    /* �V�����L��T�� */
    _findNewPoidoroFromNewDiary( tp, num, &maxNum, cookie, verbose);

    return ( *num );
}

/* �A�����e�`�F�b�N */
POI_STAT
checkPostOnLast( const char *response )
{
    POI_STAT    ret = _proceed;
    const char  *p, *q, *r, *u, *v;

    p = strstr( response, "<form action=\"delete_comment.php" );
    if ( p ) {
        p += 32;
        q = strstr( p, "</form>" );
        if ( q ) {
            char    commenter[80];

            commenter[0] = '\0';
            do {
                r = strstr( p, CLRS_HOME );
                if ( r && (r < q) ) {
                    r += 24;
                    u = strchr( r, '>' );
                    if ( u && (u < q) ) {
                        u++;
                        v = strchr( u, '<' );
                        if ( v && (v < q) ) {
                            strncpy( commenter, u, v - u );
                            commenter[v - u] = '\0';
                            p = v + 1;
                        }
                        else
                            p = u;
                    }
                    else
                        p = r;
                }
                else
                    break;
            } while ( r && (p < q) );

            if ( !strcmp( commenter, pt[0].username ) )
                ret = _continue;// ���̂܂ܓ��e����ƘA�����e�ɂȂ��Ă��܂�
        }
    }

    return ( ret );
}

/* ��d���e�`�F�b�N */
POI_STAT
checkPostOnToday( const char *response, const struct tm *tm )
{
    POI_STAT    ret = _proceed;
    char        target[256];
    const char  *p, *q;
    int         len;
    BOOL        postedOnToday = FALSE;

    sprintf( target, "<a href=\"f_home.php?id=%s\">%s</a>",
             pt[0].uid, pt[0].username );
    len = strlen( target );
    p = response;
    q = strstr( p, target );
    while ( p && q ) {
        q += len;
        p = strstr( q, target );
        if ( !p ) {
            p = strstr( q,
                        "<input type=\"checkbox\" name=\"commid[]\" "
                        "value=\"" );
            if ( q ) {
                int cnt = 0;

                q += 46;
                while ( *q ) {
                    if ( *q == ',' ) {
                        cnt++;
                        if ( cnt >= 2 )
                            break;
                    }
                    q++;
                }
                if ( cnt == 2 ) {
                    int mm, dd;

                    mm = 0;
                    dd = 0;

                    p = strchr( q, '/' );
                    if ( p ) {
                        p++;
                        if ( *p == '0' )
                            p++;
                        mm = atol( p );

                        q = strchr( p, '/' );
                        if ( q ) {
                            q++;
                            if ( *q == '0' )
                                q++;
                            dd = atol( q );
                        }
                    }

                    if ( (mm == tm->tm_mon + 1) &&
                         (dd == tm->tm_mday)       )
                        postedOnToday = TRUE;
                }
            }
            break;
        }
    }
    if ( postedOnToday == TRUE )
        ret = _continue;    // ���̂܂ܓ��e����Ɠ�d���e�ɂȂ��Ă��܂�

    return ( ret );
}

/* �A�����e(������Ɍ��肵�Ȃ�)�A(�������)��d���e�̃`�F�b�N */
POI_STAT
checkPost(
        const struct poidoroTarget *pp,
        const struct tm            *tm,
        char                       *cookie,
        BOOL                       verbose
    )
{
    POI_STAT    ret = _break;
    char        url[MAX_URLLENGTH];
    char        *response;

    response = (char *)malloc( 65536 * 16 );
    if ( !response )
        return ( ret );

    // ���e�Ώۂ̓��L���擾
    sprintf( url,
             "http://clrs.jp/view_diary.php?did=%s&uid=%s",
             pp->did, pp->uid );
    memset( response, 0x00, 65536 * 16 );
    if ( verbose )
        fprintf( stderr, "*** �`�F�b�N�Ώۂ̓��L���擾: did = %s, uid = %s\n",
                 pp->did, pp->uid );
    http_getEx( url, response, cookie );
    if ( response[0] == '\0' )
        ret = _continue;
    else {
        if ( verbose )
            fprintf( stderr, "**** �A�����e�`�F�b�N: did = %s, uid = %s\n",
                     pp->did, pp->uid );
        // �A�����e����̂�����邽�߂ɁA�Ō�̃R�����g���e�҂�
        // username ���m�F (�Ō�̃R�����g���e�҂�������������
        // ���e���X�L�b�v)
        ret = checkPostOnLast( response );
        if ( ret == _proceed ) {
            if ( verbose )
                fprintf( stderr,
                         "**** ��d���e�`�F�b�N: did = %s, uid = %s\n",
                         pp->did, pp->uid );
            // ���łɍ����A���e�ς݂̏ꍇ�͍ēx���e���Ȃ��悤�ɂ���
            ret = checkPostOnToday( response, tm );
            if ( ret != _proceed )
                if ( verbose )
                    fprintf( stderr,
                             " �� ���̂܂ܓ��e����Ɠ�d���e�ɂȂ邽�߁A"
                             "���e�𒆎~���܂��B\n" );
        }
        else {
            if ( verbose )
                fprintf( stderr,
                         " �� ���̂܂ܓ��e����ƘA�����e�ɂȂ邽�߁A"
                         "���e�𒆎~���܂��B\n" );
        }
    }

    free( response );
    return ( ret );
}

/* �|�C�h�����ɓ��e���� */
int
poidoro(
        struct poidoroTarget *tp,
        char                 *cookie,
        BOOL                 verbose
    )
{
    char    *request;
    char    *response;
    char    *p, *q;
    char    checkCode[64];
    char    comment[1024];
    char    url[MAX_URLLENGTH];
    int     ret = 0;

    request  = (char *)malloc( 8192 );
    response = (char *)malloc( 65536 * 16 );
    if ( request && response ) {
        struct poidoroTarget    *pp;
        time_t                  t;
        struct tm               *tm;
        int                     rdm;
        POI_STAT                stat;

        t = time( NULL );
        srand( (unsigned)t );

        for ( pp = tp ? tp : &(pt[0]); pp->did; pp++ ) {
            if ( (pp->alive == FALSE) || !strcmp( pp->did, "0" ) )
                continue;

            t  = time( NULL );
            tm = localtime( &t );

            // �A�����e�A��d���e�`�F�b�N
            if ( verbose )
                fprintf( stderr, "** ���e�󋵃`�F�b�N: did: %s, uid: %s\n",
                         pp->did, pp->uid );
            stat = checkPost( pp, tm, cookie, verbose );
            if ( stat == _break )
                break;
            if ( stat == _continue )
                continue;

            rdm = rand();
            memset( request,  0x00, 8192 );
            memset( response, 0x00, 65536 * 16 );

            // ���e���
            sprintf( url,
                     "http://clrs.jp/add_comment.php?did=%s&uid=%s",
                     pp->did, pp->uid );
            if ( verbose )
                fprintf( stderr, "%s �ɃR�����g�𓊍e\n", url );
            if ( !strcmp( pp->username, pt[0].username ) )
                sprintf( comment, "�|�C�h�����肪�Ƃ�!!" );
            else if ( (rdm % 8) == 0 )
                sprintf( comment,
                         "�ۂ��ǂ���!!\r\n"
                        "http://clrs.jp/view_diary.php?uid=%s&did=%s#comment",
                         pt[0].uid, pt[0].did );
            else if ( (rdm % 8) == 1 )
                sprintf( comment, "%s\r\n"
                        "http://clrs.jp/view_diary.php?uid=%s&did=%s#comment",
                (tm->tm_hour >=  4) && (tm->tm_hour < 10) ? "���͂悤���܂�" :
                (tm->tm_hour >= 10) && (tm->tm_hour < 18) ? "����ɂ���"     :
                                                            "����΂��",
                         pt[0].uid, pt[0].did );
            else if ( (rdm % 8) == 2 )
                sprintf( comment, "���������C�Ƀ|�C���g�Q�b�g!! (^^;\r\n"
                        "http://clrs.jp/view_diary.php?uid=%s&did=%s#comment",
                         pt[0].uid, pt[0].did );
            else if ( (rdm % 8) == 3 )
                sprintf( comment,
                         "���̃|�C�h��������낵��!!\r\n"
                        "http://clrs.jp/view_diary.php?uid=%s&did=%s#comment",
                         pt[0].uid, pt[0].did );
            else if ( (rdm % 8) == 4 )
                sprintf( comment, "�؂��؂�\r\n"
                        "http://clrs.jp/view_diary.php?uid=%s&did=%s#comment",
                         pt[0].uid, pt[0].did );
            else if ( (rdm % 8) == 5 )
                sprintf( comment, "�|�C�h�����Ă����܂��B\r\n"
                        "http://clrs.jp/view_diary.php?uid=%s&did=%s#comment",
                         pt[0].uid, pt[0].did );
            else if ( (rdm % 8) == 6 )
                sprintf( comment, "�|�C�|�C!!\r\n"
                        "http://clrs.jp/view_diary.php?uid=%s&did=%s#comment",
                         pt[0].uid, pt[0].did );
            else
                sprintf( comment, "(�E�́E)\r\n"
                        "http://clrs.jp/view_diary.php?uid=%s&did=%s#comment",
                         pt[0].uid, pt[0].did );

            sprintf( request,
                     "magicword=%s&",
                     encodeURL("����") );
            sprintf( request + strlen(request),
                     "body=%s&",
                     encodeURL(comment) );
            sprintf( request + strlen(request),
                     "Submit=%s",
                     encodeURL("�m�@�F") );

            http_postEx( url,
                         "application/x-www-form-urlencoded",
                         request, response, cookie );
            if ( response[0] == '\0' )
                break;

            // �m�F���
            sprintf( url,
                  "http://clrs.jp/add_comment.php?mode=addexec&did=%s&uid=%s",
                     pp->did, pp->uid );

            checkCode[0] = '\0';
            p = strstr( response, "name=\"checkcode\"" );
            if ( p ) {
                p += 16;
                p = strstr( p, "value=\"" );
                if ( p ) {
                    p += 7;
                    q = strchr( p, '"' );
                    if ( q ) {
                        strncpy( checkCode, p, q - p );
                        checkCode[q - p] = '\0';
                    }
                }
            }
            sprintf( request,
                     "magicword=%s&"
                     "checkcode=%s&",
                     encodeURL("����"), checkCode );
            sprintf( request + strlen(request),
                     "body=%s&",
                     encodeURL(comment) );
            sprintf( request + strlen(request),
                     "Submit=%s",
                     encodeURL("�m�@�F") );

            http_postEx( url,
                         "application/x-www-form-urlencoded",
                         request, response, cookie );
            if ( response[0] == '\0' )
                break;

            ret++;
            Sleep( 2500 );
        }
    }

    encodeURL( NULL );
    if ( response )
        free( response );
    if ( request )
        free( request );

    return ( ret );
}

/* �ݒ�t�@�C������|�C�h�����Ɋւ������ǂݎ�� */
int
readSetting(
        const char           *settingFilename,
        struct poidoroTarget **tp,
        BOOL                 verbose
    )
{
    FILE    *fp;
    int     ret = 0;

    if ( ( fp = fopen( settingFilename, "r" ) ) != NULL ) {
        char    *p;
        char    buf[BUFSIZ];

        while ( ( p = fgets( buf, BUFSIZ - 1, fp ) ) != NULL ) {
            while ( (*p == ' ') || (*p == '\t') )
                p++;
            if ( (*p == '\0') || (*p == '#') || (*p == '\n') || (*p == '\r') )
                continue;
            ret++;
        }

        if ( ret > 0 ) {
            *tp = (struct poidoroTarget *)
                        malloc( (ret + 1) * sizeof (struct poidoroTarget) );
            if ( tp ) {
                char    *q;
                int     n = 0;
                int     len;

                rewind( fp );
                while ( ( p = fgets( buf, BUFSIZ - 1, fp ) ) != NULL ) {
                    while ( (*p == ' ') || (*p == '\t') )
                        p++;
                    if ( (*p == '\0') || (*p == '#')  ||
                         (*p == '\n') || (*p == '\r')    )
                        continue;

                    /* did */
                    q = strchr( p, ',' );
                    if ( !q )
                        q = strchr( p, '\t' );
                    if ( !q )
                        q = strchr( p, ' ' );
                    if ( q ) {
                        len = q - p;
                        (*tp)[n].did = (char *)malloc( len + 1 );
                        if ( (*tp)[n].did == NULL )
                            break;
                        strncpy( (*tp)[n].did, p, q - p ); 
                        (*tp)[n].did[q - p] = '\0';
                    }

                    /* uid */
                    p = q + 1;
                    while ( (*p == ' ') || (*p == '\t') )
                        p++;
                    q = strchr( p, ',' );
                    if ( !q )
                        q = strchr( p, '\t' );
                    if ( !q )
                        q = strchr( p, ' ' );
                    if ( q ) {
                        len = q - p;
                        (*tp)[n].uid = (char *)malloc( len + 1 );
                        if ( (*tp)[n].uid == NULL )
                            break;
                        strncpy( (*tp)[n].uid, p, q - p ); 
                        (*tp)[n].uid[q - p] = '\0';
                    }

                    /* username */
                    p = q + 1;
                    while ( (*p == ' ') || (*p == '\t') )
                        p++;
                    q = strchr( p, '\n' );
                    if ( !q )
                        q = strchr( p, '\r' );
                    if ( !q )
                        q = strchr( p, '#' );
                    if ( !q )
                        q = strchr( p, '\t' );
                    if ( !q )
                        q = strchr( p, ' ' );
                    if ( !q )
                        q = strchr( p, ',' );
                    if ( q ) {
                        len = q - p;
                        (*tp)[n].username = (char *)malloc( len + 1 );
                        if ( (*tp)[n].username == NULL )
                            break;
                        strncpy( (*tp)[n].username, p, q - p ); 
                        (*tp)[n].username[q - p] = '\0';
                    }

                    (*tp)[n].alive = TRUE;
                    n++;
                }

                (*tp)[n].did = NULL;
                (*tp)[n].uid = NULL;
                (*tp)[n].username = NULL;
                (*tp)[n].alive = FALSE;
                if ( verbose )
                    fprintf( stderr,
                             "%d���̃|�C�h��������ǂݍ��݂܂����B\n", n );
            }
        }

        fclose( fp );
    }

    return ( ret );
}

/* �|�C�h�������i�[�̈��������� */
void
freeSetting(
        const char           *settingFilename,
        struct poidoroTarget *tp,
        int                  num,
        BOOL                 verbose
    )
{
    if ( tp ) {
        int     i;
        FILE    *fp;

        if ( ( fp = fopen( settingFilename, "w" ) ) != NULL ) {
            fputs( "# did uid username\n", fp );
            for ( i = 0; i < num; i++ ) {
                if ( tp[i].alive )
                    fprintf( fp, "%s %s %s\n",
                             tp[i].did, tp[i].uid, tp[i].username );
            }
            fclose( fp );
            if ( verbose )
                fprintf( stderr,
                         "%d���̃|�C�h�������������o���܂����B\n", num );
        }

        for ( i = num - 1; i >= 0; i-- ) {
            free( tp[i].username );
            free( tp[i].uid );
            free( tp[i].did );
        }
        free( tp );
    }
}

/* �����̃|�C�h������ URL �����߂� */
void
getOwnPoidoro(
        const char *username,
        const char *uid,
        char       *did,
        char       *cookie,
        BOOL       verbose
    )
{
    char        url[MAX_URLLENGTH];
    char        *response;
    int         page = 0;
    BOOL        done = FALSE;
    char        _uid[32];
    char        _did[32];
    char        *p, *q;
    POI_STAT    stat;
    int         cnt;

    response = (char *)malloc( 65536 * 16 );
    if ( !response )
        return;

    do {
        if ( ++page == 1 )
            strcpy( url, "http://clrs.jp/list_diary.php" );
        else
            sprintf( url, "http://clrs.jp/list_diary.php?uid=%s&page=%d",
                     uid, page );
        if ( verbose )
            fprintf( stderr, "%s ���擾 ...", url );
        memset( response, 0x00, 65536 * 16 );
        http_getEx( url, response, cookie );
        if ( !(*response) )
            break;
        p = response;
        if ( verbose )
            fprintf( stderr, "... ��͒� ...\n" );
        cnt = 0;
        while ( *p ) {
            q = strstr( p, "<td align=\"right\" nowrap>" );
            if ( !q )
                break;
            cnt++;
            p = q + 25;
            stat = getUidDid( p, _uid, _did );
            if ( stat == _continue )
                continue;
            if ( verbose )
                fprintf( stderr, "  username=%s, uid=%s, did=%s",
                         username, _uid, _did );
            if ( checkPoidoro( _did, _uid, username, 0, cookie,
                               FALSE )                          == FALSE ) {
                if ( verbose )
                    fprintf( stderr, " �� �|�C�h�����ł͂���܂���\n" );
                continue;
            }
            if ( verbose )
                fprintf( stderr, " �� �|�C�h�������Ǝv���܂�\n" );
            strcpy( did, _did );
            done = TRUE;
            break;
        }
        if ( cnt == 0 )
            break;
    } while ( done == FALSE );

    free( response );
}

void
setNickname( const char *nickname, const char *uid, const char *did )
{
    pt[0].username = (char *)nickname;
    pt[0].uid      = (char *)uid;
    pt[0].did      = (char *)did;
    pt[0].alive    = TRUE;
}

void
getNickname(
        char *nickname,
        char *uid,
        char *did,
        char *cookie,
        BOOL verbose
    )
{
    char    url[MAX_URLLENGTH];
    char    *response;

    response = (char *)malloc( 65536 * 16 );
    if ( !response )
        return;

    if ( verbose )
        fprintf( stderr, "�g�b�v�y�[�W���擾 ..." );
    memset( response, 0x00, 65536 * 16 );
    strcpy( url, "http://clrs.jp/home.php" );
    http_getEx( url, response, cookie );
    if ( *response ) {
        char    *p, *q;

        if ( verbose )
            fprintf( stderr, "... ��͒� ..." );
        p = strstr( response, "<div class=\"my-name\">" );
        if ( p ) {
            p += 21;
            q = strchr( p, '<' );
            if ( q ) {
                strncpy( nickname, p, q - p );
                nickname[q - p] = '\0';
            }
        }

        p = strstr( response, "<li><a href=\"./f_home.php?id=" );
        if ( p ) {
            p += 29;
            q = strchr( p, '"' );
            if ( q ) {
                strncpy( uid, p, q - p );
                uid[q - p] = '\0';
            }
        }
    }
    free( response );

    // �����̃|�C�h������ URL �����߂�
    if ( verbose )
        fprintf( stderr, "\n  username = %s, uid = %s\n", nickname, uid );
    strcpy( did, "0" );
    getOwnPoidoro( nickname, uid, did, cookie, verbose );
    if ( verbose )
        fprintf( stderr, "    did = %s\n", did );

    if ( nickname[0] && uid[0] && did[0] )
        setNickname( nickname, uid, did );
}

void
usage( void )
{
    fprintf( stderr,
        "poidoro [-u COLORS���[�U��] [-p �p�X���[�h] [-n �j�b�N�l�[��]\n"
        "        [-v] [-f �ݒ�t�@�C����] [-F]\n"
        "           -u: COLORS���[�U�� (���[���A�h���X)\n"
        "               COLORS �� login ����Ƃ��̃��[���A�h���X���w�肷��\n"
        "               (�ȗ������ꍇ�́A���͗p�̃v�����v�g��\��)\n"
        "           -p: COLORS�p�X���[�h\n"
        "               COLORS �� login ����Ƃ��̃p�X���[�h���w�肷��\n"
        "               (�ȗ������ꍇ�́A���͗p�̃v�����v�g��\��)\n"
        "           -v: ���s�o�߂��ڂ����\��\n"
        "           -f: �ݒ�t�@�C����\n"
        "               (�f�t�H���g�� poidoro.inf)\n"
        "           -F: �V���|�C�h�����̔����̂ݎ��s����(�R�����g�̓��e��"
                       "���Ȃ�)\n" );
    exit( 255 );
}

/* �R�}���h���C�����������߂��� */
void
getArgument(
        int  argc,
        char *argv[],
        char *username,
        char *password,
        char *settingFilename,
        BOOL *doNotPost,
        BOOL *verbose
    )
{
    int i, j;

    for ( i = 1; i < argc; i++ ) {
        if ( argv[i][0] == '-' ) {
            for ( j = 1; argv[i][j]; j++ ) {
                switch ( argv[i][j] ) {
                case 'h':
                    if ( !strcmp( &argv[i][j + 1], "elp" ) )
                        usage();
                    break;

                case 'u':
                    if ( argv[i][j + 1] ) {
                        strcpy( username, &argv[i][j + 1] );
                        j = strlen( argv[i] ) - 1;
                    }
                    else if ( i + 1 < argc ) {
                        strcpy( username, argv[++i] );
                        j = strlen( argv[i] ) - 1;
                    }
                    break;

                case 'p':
                    if ( argv[i][j + 1] ) {
                        strcpy( password, &argv[i][j + 1] );
                        j = strlen( argv[i] ) - 1;
                    }
                    else if ( i + 1 < argc ) {
                        strcpy( password, argv[++i] );
                        j = strlen( argv[i] ) - 1;
                    }
                    break;

                case 'f':
                    if ( argv[i][j + 1] ) {
                        strcpy( settingFilename, &argv[i][2] );
                        j = strlen( argv[i] ) - 1;
                    }
                    else if ( i + 1 < argc ) {
                        strcpy( settingFilename, argv[++i] );
                        j = strlen( argv[i] ) - 1;
                    }
                    break;

                case 'F':
                    *doNotPost = ~(*doNotPost);
                    break;

                case 'v':
                    *verbose = ~(*verbose);
                    break;
                }
            }

            continue;
        }
    }
}

int
main( int argc, char *argv[] )
{
    int     ret = 0;
    char    uid[32];
    char    did[32];
    char    nickname[80];
    char    username[80];
    char    password[80];
    char    settingFilename[MAX_PATH];
    BOOL    doNotPost = FALSE;
    BOOL    verbose   = FALSE;

    uid[0]      = '\0';
    did[0]      = '\0';
    nickname[0] = '\0';
    username[0] = '\0';
    password[0] = '\0';
    settingFilename[0] = '\0';
    getArgument( argc, argv, username, password, settingFilename,
                 &doNotPost, &verbose );

    if ( settingFilename[0] == '\0' )
        strcpy( settingFilename, "poidoro.inf" );

    if ( username[0] == '\0' ) {
        if ( isatty( fileno( stdin ) ) ) {
            inputString( username,
                         "COLORS ���[�U��(���[���A�h���X): ",
                         FALSE );
        }
    }
    if ( password[0] == '\0' ) {
        if ( isatty( fileno( stdin ) ) ) {
            inputString( password,
                         "COLORS �p�X���[�h: ",
                         TRUE );
        }
    }

    if ( username[0] && password[0] ) {
        char    *cookie;
        cookie = (char *)malloc( 8192 );
        if ( cookie ) {
            struct poidoroTarget    *tp = NULL;
            int                     useProxy = isUsedProxy();
            int                     num;

            num = readSetting( settingFilename, &tp, verbose );
            setUseProxy( useProxy );
            memset( cookie, 0x00, 8192 );
            if ( loginColors( username, password, cookie ) ) {
                if ( verbose )
                    fprintf( stderr, "COLORS �� login ���܂����B\n" );
                getNickname( nickname, uid, did, cookie, verbose );
                findNewPoidoro( &tp, &num, cookie, verbose );
                                                  /* ���m�̃|�C�h������T�� */
                if ( !doNotPost )
                    ret = poidoro( tp, cookie, verbose );   /* ���e */
            }
            freeSetting( settingFilename, tp, num, verbose );
                                             /* �ݒ�t�@�C���X�V + ��Еt�� */
            if ( cookie[0] )
                if ( verbose )
                    fprintf( stderr, "COLORS ���� logout ���܂����B\n" );
            free( cookie );
        }
    }
    else
        usage();

    return ( ret );
}
