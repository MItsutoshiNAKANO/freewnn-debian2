/*
 *  $Id: jslib.h,v 1.6 2008/10/26 10:27:53 aonoto Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2008
 *
 * Maintainer:  FreeWnn Project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 *       Nihongo Henkan  Library Header File
 */

#ifndef WNN_JSLIB_H
#define WNN_JSLIB_H

#ifndef _WNN_SETJMP
#define _WNN_SETJMP
#include <setjmp.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define JLIB_VERSION    0x4003

#ifndef w_char
#define w_char  unsigned short
#endif /* w_char */

/* 前端ベクタ(品詞番号)の定義 */
#define WNN_BUN_SENTOU  -1      /* 大文節の先頭になれる奴 */
#define WNN_ALL_HINSI   -2      /* なんでもええよ */
/* 終端ベクタの定義 */
#define WNN_VECT_KANREN 0       /* 連文節変換の終端ベクター */
#define WNN_VECT_KANTAN 1       /* 単文節変換の終端ベクター */
#define WNN_VECT_KANZEN 1       /* 全候補取出しの終端ベクター 
                                   単文節変換時のものと等しい。 */
#define WNN_VECT_BUNSETSU 2     /* 連文節変換時の、各文節の終端ベクター */
#define WNN_VECT_NO     -1      /* 終端ベクタ1 無し */
/* 文節の終端の接続情報 */
#define WNN_CONNECT_BK  1       /* 指定された終端ベクターに接続できた。 */
#define WNN_NOT_CONNECT_BK      0       /* */
/* 文節の前端の接続情報 */
#define WNN_CONNECT     1       /* 指定された(品詞、付属語)に接続できた。 */
#define WNN_SENTOU      2       /* 大文節の先頭 */
#define WNN_NOT_CONNECT 3       /* 先頭になれないし、前に接続できない */
#define WNN_GIJI        4       /* 疑似文節を作った。 */

/* 疑似文節の直前に選択した候補 */
#define WNN_HIRAGANA    -1      /* ひらがな */  /* 読みのまま */
#define WNN_KATAKANA    -11     /* カタカナ */
/* 数字 */
#define WNN_NUM_HAN     -2      /* 半角数字 */  /* 読みのまま */
#define WNN_NUM_ZEN     -12     /* 全角数字 */  /* １２３ */
#define WNN_NUM_KAN     -13     /* 漢数字 */    /* 一二三 */
#define WNN_NUM_KANSUUJI -15    /* 漢数字 */    /* 百二十三 */
#define WNN_NUM_KANOLD  -16     /* 漢数字 */    /* 壱百弐拾参 */
#define WNN_NUM_HANCAN  -17     /* 半角数字 */  /* 1,234 */
#define WNN_NUM_ZENCAN  -18     /* 全角数字 */  /* １，２３４ */
/* 英数 */
#define WNN_ALP_HAN     -4      /* 半角 */      /* 読みのまま */
#define WNN_ALP_ZEN     -30     /* 全角 */
/* 記号 */
#define WNN_KIG_HAN     -5      /* 半角 */      /* 読みのまま */
#define WNN_KIG_JIS     -40     /* 全角(JIS) */
#define WNN_KIG_ASC     -41     /* 全角(ASC) */

#define WNN_FT_DICT_FILE 1
#define WNN_FT_HINDO_FILE 2
#define WNN_FT_FUZOKUGO_FILE 3

#define WNN_STATIC_DICT 1       /* static dict */
#define WNN_UD_DICT 2           /* updatable dict */
#define WNN_REV_DICT 3          /* Updatable Dictonary with Reverse Index */
#define WNN_REG_DICT 4          /* Regular Expressino Dictonary */

/* for CHINESE PIN-IN : with Si Sheng (四声) */
#define CWNN_REV_DICT           0x103   /* Updatable with Reverse Index */

/* for CHINESE: Bi Xing (筆形) */
#define BWNN_REV_DICT           0x203   /* Updatable with Reverse Index */

#define WNN_DIC_RDONLY  1
#define WNN_DIC_RW      0

#define WNN_DIC_ADD_REV 1       /* 逆変換 */
#define WNN_DIC_ADD_NOR 0       /* 通常の変換 */

/* header file for dic_syurui */
#define WNN_FILE_NOT_EXIST -1
#define WNN_NOT_A_JISHO 0
/* KURI
#define WNN_USER_DIC 1
#define WNN_SYSTEM_DIC 3
*/
#define WNN_HINDO_FILE 4
#define WNN_FUZOKUGO_FILE 5

/* set_hindo operation */
#define WNN_HINDO_NOP -2
#define WNN_IMA_ON -3
#define WNN_IMA_OFF -4
#define WNN_HINDO_INC -3
#define WNN_HINDO_DECL -4
#define WNN_ENTRY_NO_USE -1

/*      Wnn constant
*/
#define WNN_HOSTLEN 16
#define WNN_ENVNAME_LEN 32

/*      js_who  */
#define WNN_MAX_ENV_OF_A_CLIENT 32

struct wnn_jwho
{
  int sd;                       /* jserver 内のソケットディスクリプタ */
  char user_name[64];           /* ユーザ名 */
  char host_name[64];           /* ホスト名 */
  int env[WNN_MAX_ENV_OF_A_CLIENT];     /* このクライアントが使用している
                                           環境番号の列。空いているところには、-1 が入る */
};
typedef struct wnn_jwho WNN_JWHO;

/*      js_env_list     */
#define WNN_MAX_JISHO_OF_AN_ENV 30
#define WNN_MAX_FILE_OF_AN_ENV 60

struct wnn_env_info
{
  int env_id;                   /* 環境番号 */
  char env_name[WNN_ENVNAME_LEN];       /* 環境名 */
  int ref_count;                /* 参照数 */
  /* struct wnn_param; */
  int fzk_fid;                  /* 付属語のファイル番号 */
  int jishomax;                 /* 使用している辞書の個数 */
  int jisho[WNN_MAX_JISHO_OF_AN_ENV];   /* 使用している辞書の辞書番号。
                                           最初のjishomax個だけ意味がある */
  int file[WNN_MAX_FILE_OF_AN_ENV];     /* この環境につながっている
                                           ファイル番号(空いているところには、-1 が入る) */
};
typedef struct wnn_env_info WNN_ENV_INFO;

#define WNN_COMMENT_LEN 512     /* jisho no comment no nagasa */
#define WNN_F_NAMELEN 100
#define WNN_PASSWD_LEN 16       /* File Passwd Length */


struct wnn_dic_info
{
  int dic_no;                   /* 辞書番号 */
  int body;                     /* 辞書本体のファイル番号 */
  int hindo;                    /* 頻度のファイル番号 */
  int rw;                       /* 辞書が登録可能かどうか
                                   (WNN_DIC_RW, WNN_DIC_RDONLY) */
  int hindo_rw;                 /* 頻度が更新可能かどうか
                                   (WNN_DIC_RW, WNN_DIC_RDONLY) */
  int enablef;                  /* 辞書が使用中かどうか
                                   (1 = 使用中, 0 = 使用中断) */
  int nice;                     /* 辞書の変換時の優先度 */
  int rev;                      /* 逆変換か、どうか(1 = 逆変換, 0 = 正変換) */
/* added H.T */
  w_char comment[WNN_COMMENT_LEN];      /* 辞書のコメント */
  char fname[WNN_F_NAMELEN];    /* 辞書のファイル名 */
  char hfname[WNN_F_NAMELEN];   /* 頻度のファイル名 */
  char passwd[WNN_PASSWD_LEN];  /* 辞書のパスワード */
  char hpasswd[WNN_PASSWD_LEN]; /* 頻度のパスワード */
  int type;                     /* 辞書の種類(WNN_UD_DICT,WNN_STATIC_DICT) */
  int gosuu;                    /* 辞書の語数 */
  int localf;
  int hlocalf;
};

typedef struct wnn_dic_info WNN_DIC_INFO;

struct wnn_file_stat
{
  int type;                     /* ファイルの種類
                                   WNN_STATIC_DICT              固定形式辞書
                                   WNN_UD_DICT          登録可能形式辞書
                                   WNN_HINDO_FILE               頻度ファイル
                                   WNN_FUZOKUGO_FILE    付属語ファイル
                                   WNN_NOT_A_JISHO              その他   */
};

typedef struct wnn_file_stat WNN_FILE_STAT;

/*      */
extern int wnn_errorno;         /* Wnnのエラーはこの変数に報告される */

extern char *wnn_dic_types[];   /* "固定","登録","逆変換","正規" */

extern char *cwnn_dic_types[];  /* "固定","登録","逆変換","正規" */
extern char *bwnn_dic_types[];  /* "固定","登録","逆変換","正規" */

#define FILE_ALREADY_READ -2

/* この構造体は、ライブラリ内部で用いられる */
struct wnn_jserver_id
{
  int sd;
  char js_name[40];
  int js_dead;
  jmp_buf js_dead_env;          /* サーバが死んだ時に飛んでいくenv */
  int js_dead_env_flg;          /* jd_server_dead_envが有効か否か  */
};

typedef struct wnn_jserver_id WNN_JSERVER_ID;

/* この構造体は、ライブラリ内部で用いられる */
struct wnn_env
{
  int env_id;
  WNN_JSERVER_ID *js_id;
  char lang[16];                /* for exsample "ja_JP" */
};

typedef struct wnn_env WNN_ENV;

struct wnn_param
{
  int n;                        /* Ｎ(大)文節解析のＮ */
  int nsho;                     /* 大文節中の小文節の最大数 */
  int p1;                       /* 自立語の頻度のパラメータ */
  int p2;                       /* 小文節長のパラメータ */
  int p3;                       /* 自立語長のパラメータ */
  int p4;                       /* 今使ったよビットのパラメータ */
  int p5;                       /* 辞書のパラメータ */
  int p6;                       /* 小文節の評価値のパラメータ */
  int p7;                       /* 大文節長のパラメータ */
  int p8;                       /* 小文節数のパラメータ */
  int p9;                       /* 疑似品詞 数字の頻度 */
  int p10;                      /* 疑似品詞 カナの頻度 *//* CWNN:英数の頻度 */
  int p11;                      /* 疑似品詞 英数の頻度 *//* CWNN:記号の頻度 */
  int p12;                      /* 疑似品詞 記号の頻度 *//* CWNN:開括弧の頻度 */
  int p13;                      /* 疑似品詞 閉括弧の頻度 *//* CWNN:閉括弧の頻度 */
  int p14;                      /* 疑似品詞 付属語の頻度 *//* BWNN:No of koho */
  int p15;                      /* 疑似品詞 開括弧の頻度 *//* CWNN:Not used */
};

#ifdef nodef
struct wnn_dic_info_struct
{
  int body_fid;
  int hindo_fid;
  int enablef;
  int rw;
  int hindo_rw;
  int nice;
};

typedef struct wnn_dic_info_struct WNN_DIC_INFO_STRUCT;
#endif

struct wnn_file_info_struct
{
  int fid;                      /* ファイル番号 */
  char name[WNN_F_NAMELEN];     /* ファイル名 */
  int localf;                   /* サーバ・サイトのファイルかどうか
                                   1: サーバ・サイト
                                   0: クライアント・サイト   */
  int type;                     /* ファイルの種類 */
  int ref_count;                /* (環境からの)参照数 */
};

typedef struct wnn_file_info_struct WNN_FILE_INFO_STRUCT;

#define WNN_VECT_L      ((256+8*4-1)/(8*4) + 5) /***** !!!!! ****/

struct wnn_sho_bunsetsu
{
  int end;                      /* 候補文節の end char index */
  int start;                    /* 候補文節の top char index */
  int jiriend;                  /* 候補文節の自立語 end char index */
  int dic_no;                   /* 自立語の辞書内のエントリ番号 */
  int entry;                    /* 候補文節の自立語辞書 entry */

  int hinsi;                    /* 自立語品詞 */
  int status;                   /* 大文節の先頭か */
  int status_bkwd;              /* usiro の文節に接続できるか */
  int hindo;                    /* 候補自立語の頻度 */
  int ima;                      /* 候補自立語の今使ったよビット */
  int kangovect;                /* 接続ベクトルテーブルへのポインタ */
  int hyoka;                    /* 小文節評価値 */
  w_char *kanji;                /* 自立語文字列 */
  w_char *yomi;                 /* 自立語の読み文字列 */
  w_char *fuzoku;               /* 付属語文字列 */
/*
 *頻度については、頻度ファイルが指定されている時は、
 *hindo = 頻度ファイルの(実)頻度値 + 辞書内の(実)頻度値
 *ima = 頻度ファイルの今使ったよビット
 *
 *頻度ファイルが指定されていない時には、
 *hindo = 辞書内の(実)頻度値、ima = 辞書内の今使ったよビット
 *である。ここで、実頻度値とは、計算機内で7ビットに圧縮された値である。
 *仮想頻度値ではなく実頻度値を返すのは、変換結果のデバッグのためである。
 */
};

struct wnn_dai_bunsetsu
{
  int end;                      /* 候補文節の end char index */
  int start;                    /* 候補文節の top char index */
  struct wnn_sho_bunsetsu *sbn; /* 小文節解析結果へのポインタ */
  int hyoka;                    /* 大文節評価値 */
  int sbncnt;                   /* 小文節数 (次候補の場合は、次候補数)
                                   DSD_SBNは、*sbn から sbncnt だけある */
};

struct wnn_jdata
{
  int dic_no;                   /* 辞書番号 */
  int serial;                   /* 辞書内のエントリ番号 */
  int hinshi;                   /* 品詞番号(品詞番号と品詞名の対応は、manual/etc
                                   の下を参照) */
  int hindo;                    /* 頻度 */
  int ima;                      /* 今使ったよビット */
  int int_hindo;                /* 辞書内頻度 */
  int int_ima;                  /* 辞書内、今使ったよビット */
  w_char *yomi;                 /* 読みの文字列 */
  w_char *kanji;                /* 漢字文字列 */
  w_char *com;                  /* エントリのコメント文字列 */
/*
 *頻度については、頻度ファイルが指定されている時は、
 *hindo = 頻度ファイルの(仮想)頻度値、ima = 頻度ファイルの今使ったよビット
 *int_hindo = 辞書内の(仮想)頻度値、int_ima = 辞書内の今使ったよビット
 *
 *頻度ファイルが指定されていない時には、
 *hindo = 辞書内の(仮想)頻度値、ima = 辞書内の今使ったよビット
 *int_hindo = -1、int_ima = -1
 *
 *ただし、どちらの場合でも、エントリが使用中止の状態の時には、
 *hindo = -1, ima = 0 あるいは、
 *int_hindo = -1, int_ima = 0 となる。
 *ここで、(仮想)頻度値とは、計算機内で7ビットに圧縮された頻度値から、
 *実際の値を想定した値である。
 */
};

struct wnn_ret_buf
{
  int size;                     /* buf から alloc されている大きさ */
  char *buf;                    /* malloc などでとられた領域 */
};


#define WNN_F_UNIQ_LEN  (sizeof(struct wnn_file_uniq))

/* この構造体は、ライブラリ内部で用いられる */
struct wnn_file_head
{
  struct wnn_file_uniq
  {
    int time;
    int dev;
    int inode;
    char createhost[WNN_HOSTLEN];
  }
  file_uniq;
  struct wnn_file_uniq file_uniq_org;
  int file_type;
  char file_passwd[WNN_PASSWD_LEN];
};

#define WNN_HINSI_NAME_LEN 64

#define WNN_FILE_HEADER_LEN     (WNN_PASSWD_LEN + 8 + WNN_FILE_STRING_LEN + \
                                 sizeof(struct wnn_file_uniq) * 2 + 4)
                                /*
                                   8 is for future use 
                                   4 is for file_type.
                                 */

#ifndef JSERVER
/*
  JSLIB function declaration
*/
#define js_open(server, timeout)        js_open_lang(server, "ja_JP", timeout)
#define js_connect(server,env_name)     js_connect_lang(server, env_name, "ja_JP")

#ifndef FRWNN_PARAMS
/*
 * I currently think ANSI-C is not sufficiant on building your clients
 * linked with libwnn. (aonoto)
 */
#if __STDC__		/* Sufficient? */
#define FRWNN_PARAMS(paramlist)	paramlist
#else
#define FRWNN_PARAMS(paramlist)	()
#endif
#endif

extern WNN_JSERVER_ID *js_open_lang FRWNN_PARAMS((register char *server, register char *lang, register int timeout));
extern int js_close      FRWNN_PARAMS((WNN_JSERVER_ID *server));
/* extern WNN_JSERVER_ID *js_change_current_jserver (); */ /* Not exist? */
extern struct wnn_env *js_connect_lang FRWNN_PARAMS((WNN_JSERVER_ID *server, register char *env_name, char *lang));
extern int js_disconnect FRWNN_PARAMS((register struct wnn_env *env));
extern int js_env_list   FRWNN_PARAMS((WNN_JSERVER_ID *server, struct wnn_ret_buf *ret));
extern int js_param_set  FRWNN_PARAMS((struct wnn_env *env, register struct wnn_param *para));
extern int js_param_get  FRWNN_PARAMS((struct wnn_env *env, register struct wnn_param *para));
extern char *js_get_lang FRWNN_PARAMS((struct wnn_env *env));
/**************************************/
extern int js_access     FRWNN_PARAMS((struct wnn_env *env, char *path, int amode));
extern int js_dic_add    FRWNN_PARAMS((struct wnn_env *env, int fid, int hfid, int rev,int jnice, int rw, int hrw, char *pw1, char *pw2));
extern int js_dic_delete      FRWNN_PARAMS((struct wnn_env *env, int dicno));
extern int js_dic_file_create FRWNN_PARAMS((struct wnn_env *env, char *fn, int type, w_char *comment, char *passwd, char *hpasswd));
extern int js_dic_file_create_client FRWNN_PARAMS((struct wnn_env *env, char *fn, int type, w_char *comment, char *passwd, char *hpasswd));
extern int js_dic_info     FRWNN_PARAMS((struct wnn_env *env, int dic_no, register WNN_DIC_INFO *ret));
extern int js_dic_list     FRWNN_PARAMS((struct wnn_env *env, struct wnn_ret_buf *ret));
extern int js_dic_list_all FRWNN_PARAMS((WNN_JSERVER_ID *server, struct wnn_ret_buf *ret));
extern int js_dic_use      FRWNN_PARAMS((struct wnn_env *env, int dic_no, int flag));
extern int js_env_exist     FRWNN_PARAMS((register WNN_JSERVER_ID *server, register char *env_name));
extern int js_env_sticky    FRWNN_PARAMS((register struct wnn_env *env));
extern int js_env_un_sticky FRWNN_PARAMS((register struct wnn_env *env));
extern int js_file_comment_set FRWNN_PARAMS((struct wnn_env *env, int fid, w_char *comment));
extern int js_file_discard FRWNN_PARAMS((struct wnn_env *env, int fid));
extern int js_file_info    FRWNN_PARAMS((struct wnn_env *env, int fid, register WNN_FILE_INFO_STRUCT *file));
extern int js_file_list    FRWNN_PARAMS((struct wnn_env *env, struct wnn_ret_buf *ret));
extern int js_file_list_all FRWNN_PARAMS((WNN_JSERVER_ID *server, struct wnn_ret_buf *ret));
extern int js_file_loaded  FRWNN_PARAMS((WNN_JSERVER_ID *server, char *path));
extern int js_file_loaded_local FRWNN_PARAMS((WNN_JSERVER_ID *server, char *path));
extern int js_file_password_set FRWNN_PARAMS((struct wnn_env *env, int fid, int which, char *old_passwd, char *new_passwd));
extern int js_file_read    FRWNN_PARAMS((struct wnn_env *env, char *fn));
extern int js_file_receive FRWNN_PARAMS((struct wnn_env *env, int fid, char *fn));
extern int js_file_remove  FRWNN_PARAMS((WNN_JSERVER_ID *server, char *n, char *pwd));
extern int js_file_remove_client FRWNN_PARAMS((WNN_JSERVER_ID *server, char *n, char *pwd));
extern int js_file_send    FRWNN_PARAMS((struct wnn_env *env, char *fn));
extern int js_file_stat    FRWNN_PARAMS((struct wnn_env *env, char *path, WNN_FILE_STAT *s));
extern int js_file_write   FRWNN_PARAMS((struct wnn_env *env, int fid, char *fn));
extern void js_flush FRWNN_PARAMS((struct wnn_env *env));
extern int js_fuzokugo_get FRWNN_PARAMS((struct wnn_env *env));
extern int js_fuzokugo_set FRWNN_PARAMS((struct wnn_env *env, int fid));
extern int js_hindo_file_create FRWNN_PARAMS((struct wnn_env *env, int fid, char *fn, w_char *comment, char *hpasswd));
extern int js_hindo_file_create_client FRWNN_PARAMS((struct wnn_env *env, int fid, char *fn, w_char *com, char *hpasswd));
extern int js_hindo_set    FRWNN_PARAMS((struct wnn_env *env, int dic, int entry, int ima, int hindo));
extern int js_hinsi_dicts  FRWNN_PARAMS((struct wnn_env *env, int no, struct wnn_ret_buf *rb));
extern int js_hinsi_list   FRWNN_PARAMS((struct wnn_env *env, int dic_no, w_char *name, struct wnn_ret_buf *rb));
extern int js_hinsi_name   FRWNN_PARAMS((WNN_JSERVER_ID *server, int no,  struct wnn_ret_buf *rb));
extern int js_hinsi_number FRWNN_PARAMS((WNN_JSERVER_ID *server, w_char *name));
extern int js_hinsi_table_set FRWNN_PARAMS((struct wnn_env *env, int dic_no, w_char *hinsi_table));
extern int js_isconnect FRWNN_PARAMS((register struct wnn_env *env));
extern int js_kanren FRWNN_PARAMS((struct wnn_env *env, w_char *yomi, int hinsi, w_char *fzk, int vec, int vec1, int vec2, struct wnn_ret_buf *rb));
extern int js_kantan_dai FRWNN_PARAMS((struct wnn_env *env, w_char *yomi, int hinsi, w_char *fzk, int vec, int vec1, struct wnn_ret_buf *rb));
extern int js_kantan_sho FRWNN_PARAMS((struct wnn_env *env, w_char *yomi, int hinsi, w_char *fzk, int vec, int vec1, struct wnn_ret_buf *rb));
extern int js_kanzen_dai FRWNN_PARAMS((struct wnn_env *env, w_char *yomi, int hinsi, w_char *fzk, int vec, int vec1, struct wnn_ret_buf *rb));
extern int js_kanzen_sho FRWNN_PARAMS((struct wnn_env *env, w_char *yomi, int hinsi, w_char *fzk, int vec, int vec1, struct wnn_ret_buf *rb));
extern int js_kill    FRWNN_PARAMS((WNN_JSERVER_ID *server));
extern int js_mkdir   FRWNN_PARAMS((struct wnn_env *env, char *path));
extern int js_version FRWNN_PARAMS((WNN_JSERVER_ID *server, int *serv, int *libv));
extern int js_who     FRWNN_PARAMS((WNN_JSERVER_ID *server, struct wnn_ret_buf *ret));
extern int js_word_add    FRWNN_PARAMS((struct wnn_env *env, int dic_no, w_char *yomi, w_char *kanji, w_char *comment, int hinshi, int init_hindo));
extern int js_word_comment_set FRWNN_PARAMS((struct wnn_env *env, int dic_no, int entry, w_char *comment));
extern int js_word_delete FRWNN_PARAMS((struct wnn_env *env, int dic_no, int entry));
extern int js_word_info   FRWNN_PARAMS((struct wnn_env *env, int dic_no, int entry, struct wnn_ret_buf *ret));
extern int js_word_search FRWNN_PARAMS((struct wnn_env *env, int dic_no, w_char *yomi, struct wnn_ret_buf *ret));
extern int js_word_search_by_env FRWNN_PARAMS((struct wnn_env *env, w_char *yomi, struct wnn_ret_buf *ret));

#endif	/* !JSERVER */

#ifdef __cplusplus
}
#endif

#endif  /* WNN_JSLIB_H */
