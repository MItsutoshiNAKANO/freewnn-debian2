/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2002, 2011
 *
 * Maintainer:  FreeWnn Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

static char rcs_id[] = "$Id: renbn_kai.c,v 1.5 2011/04/10 16:51:16 aonoto Exp $";

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

/* #include <stdio.h> */	/* for debug only  */
#include "commonhd.h"
#include "de_header.h"
#include "fzk.h"
#include "kaiseki.h"

static struct DSD_DBN *dcdbn_set (struct DSD_DBN *, struct DSD_SBN **, struct BZD *);
static struct DSD_SBN *dcdsbn_set (struct DSD_SBN *, struct SYO_BNSETSU *);
static int  cnt_syo (struct SYO_BNSETSU *);
static void cnt_bzd (struct BZD *, int *, int *);
static int  chk_yomi_endvect (int, int, int, int);
static int  set_kata_giji_sbn (int, int, int, int, struct SYO_BNSETSU **);
static int  set_kata_giji_bzd (int, int, int, int, struct BZD **, int);

int
renbn_kai (int yomi_sno,	/* ����ʸ���� start index */
	int yomi_eno,		/* ����ʸ���� end index (end char �μ�) */
	int beginvect,		/* ��ü�٥���(-1:ʸ����Ƭ��-2:�ʤ�Ǥ�)�ʻ�No. */
#ifndef NO_FZK
	w_char *fzkchar,
#endif /* NO_FZK */
	int endvect,		/* ��ü�٥��� */
	int endvect1,		/* ��ü�٥��� (ͽ��) */
	int endvect2,		/* bunsetsu ��ü�٥��� */
	int kaidbno,		/* ������ʸ��� */
	int kaisbno,		/* ���Ͼ�ʸ��� */
	struct DSD_DBN **dsd_dbn) /* ������ʸ����󥨥ꥢ pointer */
{
  int dbn_cnt;
  int sbn_cnt;
  struct DSD_SBN *dsd_sbn;      /* ������ʸ����󥨥ꥢ pointer */

  UINT dicidyno,       /* ���ꤷ����ʸ��� index */
    buncnt = 0;                 /* ���ꤷ����ʸ��� */
  struct BZD *rbzdptr;          /* ������оݤȤʤ�Ρ��� top pointer */
  struct BZD *brbzdptr,        /* work pointer */
   *dicide_bp = 0,              /* ���ꤷ����ʸ��� top pointer */
   *dicide_np,                  /* ���ꤷ����ʸ��ؤ� pointer */
   *wkbzdptr,                   /* work pointer         */
   *maxbzd;                     /* ������оݤȤʤäƤ�����ʸ��� */
  /* �����ɾ���ͤ���ĥΡ��ɤؤ� pointer */
  int rtmknode;
  extern int _status;

  dicidyno = yomi_sno - 1;
  rbzdptr = 0;
  rtmknode = 1;

  _status = 0;

  if (c_env->fzk_fid == -1)
    {
      wnn_errorno = WNN_FZK_FILE_NO_LOAD;
      return (-1);
    }
  ft = (struct FT *) files[c_env->fzk_fid].area;
  if (chk_yomi_endvect (yomi_sno, yomi_eno, endvect, endvect1) < 0)
    return (-1);
  while (dicidyno + 1 < yomi_eno)
    {
      if (rbzdptr == 0)
        {
          if (dbn_kai (dicidyno + 1, yomi_eno, beginvect,
#ifndef NO_FZK
                       fzkchar,
#endif /* NO_FZK */
                       endvect, endvect1, kaisbno, &rbzdptr) < 0)
            {
              log_err ("CONVERSION ERROR.");
              init_work_areas ();
              return (-1);      /* ERROR */
            }
          if (rbzdptr == 0)
            {
              if (set_kata_giji_bzd (dicidyno + 1, yomi_eno, endvect, endvect1, &rbzdptr, buncnt) < 0)
                return (-1);
              buncnt++;
              rtmknode = 0;
              if (dicide_bp == 0)
                dicide_bp = rbzdptr;
              else
                dicide_np->lnk_son = rbzdptr;
              break;
            }
        }
      /* 1 ʸ������ǽ�ˤʤ�ޤǥΡ��ɤ��� */
      if (kaidbno == 1)
        {
          /* ��ʸ���ɾ���ͤ�­����(son_v)�ϡ�mknode �Ǥ�������� kaidbno �� 1
             �Ĥޤ� 1ʸ����Ϥξ��ϡ��������̤�ʤ����餳���Ǥ���� */
          for (brbzdptr = rbzdptr->lnk_br; brbzdptr != 0; brbzdptr = brbzdptr->lnk_br)
            brbzdptr->son_v = brbzdptr->v_jc;
        }
      else
        {
          while ((int) (rbzdptr->kbcnt) < kaidbno && rtmknode > 0)
            {
              for (brbzdptr = rbzdptr; brbzdptr != 0; brbzdptr = brbzdptr->lnk_br)
                {
                  rtmknode = mknode (brbzdptr, yomi_eno, beginvect,
#ifndef NO_FZK
                                     fzkchar,
#endif /* NO_FZK */
                                     endvect2, kaisbno);
                  brbzdptr->son_v /= brbzdptr->kbcnt;
                  if (rtmknode == 0)
                    {
                      brbzdptr->son_v += brbzdptr->v_jc;
                    }
                  else if (rtmknode == -1)
                    {
                      log_err ("CONVERSION ERROR.");
                      init_work_areas ();       /* ���顼�Ǥ����Τ��� */
                      return (-1);      /* ERROR */
                    }
                }
            }
        }

      /* �����ɾ������ĥΡ��ɤ򸫤Ĥ��� */
      for (brbzdptr = rbzdptr->lnk_br, maxbzd = rbzdptr; brbzdptr != 0; brbzdptr = wkbzdptr)
        {
          wkbzdptr = brbzdptr->lnk_br;
          if (maxbzd->son_v <= brbzdptr->son_v)
            {
              clr_node (maxbzd);        /* garbage collection */
              maxbzd = brbzdptr;
            }
          else
            clr_node (brbzdptr);
        }

      /* 1 ʸ����� */
      buncnt++;                 /* ����ʸ�� count up */
      dicidyno = maxbzd->j_c;   /* ���ꤷ��ʸ��� top */
      /* kettei sita node no link */
      if (maxbzd->bend_m == yomi_sno)
        {
          dicide_bp = dicide_np = maxbzd;
        }
      else
        {
          dicide_np->lnk_son = maxbzd;
          dicide_np = maxbzd;
        }
      rbzdptr = maxbzd->lnk_son;
    }

  cnt_bzd (dicide_bp, &dbn_cnt, &sbn_cnt);
  if ((*dsd_dbn = get_dsd_dbn (dbn_cnt)) <= (struct DSD_DBN *) 0)
    {
      init_work_areas ();
      return (-1);
    }
  if ((dsd_sbn = get_dsd_sbn (sbn_cnt)) <= (struct DSD_SBN *) 0)
    {
      init_work_areas ();
      return (-1);
    }

  dcdbn_set (*dsd_dbn, &dsd_sbn, dicide_bp);
  return (buncnt);
}

/************************************************************/
/*      ñʸ�����(��ʸ��) routine                          */
/************************************************************/
int
tan_dai (int yomi_sno,		/* ����ʸ���� start index */
	int yomi_eno,		/* ����ʸ���� end index (end char �μ�) */
	int beginvect,		/* ��ü�٥���(-1:ʸ����Ƭ��-2:�ʤ�Ǥ�)�ʻ�No. */
#ifndef NO_FZK
	w_char *fzkchar,
#endif /* NO_FZK */
	int endvect,		/* ��ü�٥��� */
	int endvect1,		/* ��ü�٥��� */
	int kaisbno,		/* ���Ͼ�ʸ��� */
	struct DSD_DBN **dsd_dbn) /* ������ʸ����󥨥ꥢ pointer */
{
  int dbn_cnt;
  int sbn_cnt;
  struct DSD_SBN *dsd_sbn;      /* ������ʸ����󥨥ꥢ pointer */
  struct BZD *rbzdptr;          /* ������оݤȤʤ�Ρ��ɤ�
                                   �ȥåץݥ��� */
  struct BZD *brbzdptr, *wkbzdptr;
  struct BZD *maxbzd;           /* ������оݤȤʤäƤ���ʸ���
                                   �����ɾ���ͤ���ĥΡ��ɤؤΥݥ��� */
  extern int _status;

  if (c_env->fzk_fid == -1)
    {
      wnn_errorno = WNN_FZK_FILE_NO_LOAD;
      return (-1);
    }
  ft = (struct FT *) files[c_env->fzk_fid].area;

  rbzdptr = 0;
  _status = 0;
  if (chk_yomi_endvect (yomi_sno, yomi_eno, endvect, endvect1) < 0)
    return (-1);
  if (dbn_kai (yomi_sno, yomi_eno, beginvect,
#ifndef NO_FZK
               fzkchar,
#endif /* NO_FZK */
               endvect, endvect1, kaisbno, &rbzdptr) < 0)
    {
      init_work_areas ();
      return (-1);              /* ERROR */
    }

  /* �����ɾ���ͤ���ĥΡ��ɤ򸫤Ĥ��� */
  for (brbzdptr = rbzdptr, maxbzd = 0; brbzdptr != 0; brbzdptr = wkbzdptr)
    {
      wkbzdptr = brbzdptr->lnk_br;
      if (brbzdptr->j_c == yomi_eno - 1)
        {                       /* ʸ��Ĺ�� */
          if (maxbzd == 0)
            {
              maxbzd = brbzdptr;
            }
          else if (maxbzd->v_jc < brbzdptr->v_jc)
            {
              freebzd (maxbzd);
              maxbzd = brbzdptr;
            }
          else
            {
              freebzd (brbzdptr);
            }
        }
      else
        {
          clr_node (brbzdptr);
        }
    }

  if (maxbzd == 0)
    {
      if (set_kata_giji_bzd (yomi_sno, yomi_eno, endvect, endvect1, &maxbzd, 0) < 0)
        return (-1);
    }
  cnt_bzd (maxbzd, &dbn_cnt, &sbn_cnt);
  if ((*dsd_dbn = get_dsd_dbn (dbn_cnt)) <= (struct DSD_DBN *) 0)
    {
      init_work_areas ();
      return (-1);
    }
  if ((dsd_sbn = get_dsd_sbn (sbn_cnt)) <= (struct DSD_SBN *) 0)
    {
      init_work_areas ();
      return (-1);
    }

  dcdbn_set (*dsd_dbn, &dsd_sbn, maxbzd);
  return (1);
}

/************************************************************/
/*      ñʸ�����(��ʸ��) routine                          */
/************************************************************/
int
tan_syo (int yomi_sno,		/* ����ʸ���� start index */
	int yomi_eno,		/* ����ʸ���� end index (end char �μ�) */
	int beginvect,		/* ��ü�٥���(-1:ʸ����Ƭ��-2:�ʤ�Ǥ�)�ʻ�No. */
#ifndef NO_FZK
	w_char *fzkchar,
#endif /* NO_FZK */
	int endvect,		/* ��ü�٥��� */
	int endvect1,		/* ��ü�٥��� */
	struct DSD_SBN **dsd_sbn)  /* ���꾮ʸ����󥨥ꥢ pointer */
{
  int sbn_cnt;
  struct SYO_BNSETSU *rsbnptr;  /* ������оݤȤʤ�Ρ��ɤΥȥåץݥ��� */
  struct SYO_BNSETSU *brsbnptr, *wksbnptr;
  struct SYO_BNSETSU *maxsbn;   /* ������оݤȤʤäƤ���ʸ���
                                   �����ɾ���ͤ���ĥΡ��ɤؤΥݥ��� */
  int divid;
  extern int _status;

  if (c_env->fzk_fid == -1)
    {
      wnn_errorno = WNN_FZK_FILE_NO_LOAD;
      return (-1);
    }
  ft = (struct FT *) files[c_env->fzk_fid].area;

  rsbnptr = 0;
  _status = 0;
  if (chk_yomi_endvect (yomi_sno, yomi_eno, endvect, endvect1) < 0)
    return (-1);
  if (sbn_kai (yomi_sno, yomi_eno, endvect, endvect1, &rsbnptr, 1, (struct SYO_BNSETSU *) 0) < 0)
    {
      init_work_areas ();
      return (-1);              /* ERROR */
    }

  /* �����ɾ���ͤ���ĥΡ��ɤ򸫤Ĥ��� */
  for (brsbnptr = rsbnptr, maxsbn = 0; brsbnptr != 0; brsbnptr = wksbnptr)
    {
      wksbnptr = brsbnptr->lnk_br;
      if (brsbnptr->j_c == yomi_eno - 1)
        {                       /* ʸ��Ĺ�� */
          if (maxsbn == 0)
            {
              maxsbn = brsbnptr;
              divid = get_status (brsbnptr->kangovect, beginvect,
#ifndef NO_FZK
                                  fzkchar,
#endif /* NO_FZK */
                                  &(brsbnptr->status));
              brsbnptr->v_jc = DIVID_HYOUKA (brsbnptr->v_jc, divid);
            }
          else
            {
              divid = get_status (brsbnptr->kangovect, beginvect,
#ifndef NO_FZK
                                  fzkchar,
#endif /* NO_FZK */
                                  &(brsbnptr->status));
              brsbnptr->v_jc = DIVID_HYOUKA (brsbnptr->v_jc, divid);
              if (beginvect != WNN_ALL_HINSI && ((brsbnptr->status == WNN_SENTOU && maxsbn->status != WNN_SENTOU) || (brsbnptr->status == WNN_CONNECT && maxsbn->status != WNN_CONNECT)))
                {
                  freesbn (maxsbn);
                  maxsbn = brsbnptr;
                }
              else if (maxsbn->v_jc < brsbnptr->v_jc)
                {
                  freesbn (maxsbn);
                  maxsbn = brsbnptr;
                }
              else
                {
                  freesbn (brsbnptr);
                }
            }
        }
      else
        {
          freesbn (brsbnptr);
        }
    }

  if (maxsbn == 0)
    {
      if (set_kata_giji_sbn (yomi_sno, yomi_eno, endvect, endvect1, &maxsbn) < 0)
        return (-1);
    }
  if (maxsbn->jentptr == 0 && maxsbn->status == WNN_NOT_CONNECT)
    maxsbn->status = WNN_GIJI;
  sbn_cnt = cnt_syo (maxsbn);
  if ((*dsd_sbn = get_dsd_sbn (sbn_cnt)) <= (struct DSD_SBN *) 0)
    {
      init_work_areas ();
      return (-1);
    }
  dcdsbn_set (*dsd_sbn, maxsbn);
  freesbn (maxsbn);
  return (1);
}

/**********************************************/
/* ���ꤷ��ʸ��ξ���򥻥åȤ���             */
/**********************************************/
static struct DSD_DBN *
dcdbn_set (struct DSD_DBN *dsd_dbn,
	struct DSD_SBN **dsd_sbn,
	struct BZD *bzd)
{
  struct DSD_DBN *nextp;
#ifdef  CONVERT_from_TOP
  struct DSD_DBN *dsd_dbn_head;
  struct BZD *s_bzd;   /* Buffer son's bzd     */
  dsd_dbn_head = dsd_dbn;
  while (bzd != 0)
    {
      nextp = dsd_dbn++;
      nextp->bun_m = bzd->bend_m;
      nextp->bun_jc = bzd->j_c;
      nextp->sbncnt = bzd->sbn_cnt;
      nextp->v_jc = bzd->v_jc;

      if (bzd->sbn->status == 0)
        bzd->sbn->status = WNN_SENTOU;

      nextp->sbn = *dsd_sbn;
      *dsd_sbn = dcdsbn_set (*dsd_sbn, bzd->sbn);

      s_bzd = bzd->lnk_son;
      freebzd (bzd);
      bzd = s_bzd;
    }
  return (dsd_dbn_head);
#else /* CONVERT_from_TOP */
  if (bzd == 0)
    return (dsd_dbn);
  nextp = dcdbn_set (dsd_dbn, dsd_sbn, bzd->lnk_son);
  nextp->bun_m = bzd->bend_m;
  nextp->bun_jc = bzd->j_c;
  nextp->sbncnt = bzd->sbn_cnt;
  nextp->v_jc = bzd->v_jc;

  if (bzd->sbn->status == 0)
    bzd->sbn->status = WNN_SENTOU;

  nextp->sbn = *dsd_sbn;
  *dsd_sbn = dcdsbn_set (*dsd_sbn, bzd->sbn);
  freebzd (bzd);
  return (++nextp);
#endif /* CONVERT_from_TOP */
}

static struct DSD_SBN *
dcdsbn_set (struct DSD_SBN *dsd_sbn,
	struct SYO_BNSETSU *sbn)
{
  if (sbn == 0)
    return (dsd_sbn);
#ifdef  CONVERT_from_TOP
  dsd_sbn = dcdsbn_set (dsd_sbn, sbn->parent);
#endif /* CONVERT_from_TOP */
  dsd_sbn->bun_m = sbn->bend_m;
  dsd_sbn->bun_jc = sbn->j_c;
  dsd_sbn->i_jc = sbn->i_jc;
  dsd_sbn->jentptr = sbn->jentptr;
  dsd_sbn->t_jc = sbn->t_jc;
  dsd_sbn->hinsi = sbn->hinsi_fk;
  dsd_sbn->kangovect = sbn->kangovect;
  dsd_sbn->v_jc = sbn->v_jc;
  dsd_sbn->status = sbn->status;
  dsd_sbn->status_bkwd = sbn->status_bkwd;
  dsd_sbn++;
#ifndef CONVERT_from_TOP
  dsd_sbn = dcdsbn_set (dsd_sbn, sbn->parent);
#endif /* CONVERT_from_TOP */
  return (dsd_sbn);
}

/* 1 ��ʸ����ξ�ʸ��ο� */
static int
cnt_syo (struct SYO_BNSETSU *sbn)
{
  int cnt;
  cnt = 0;
  while (sbn)
    {
      cnt++;
      sbn = sbn->parent;
    }
  return (cnt);
}

/* 1 ��ʸ��ο� */
static void
cnt_bzd (struct BZD *bzd,
	int *dbn_cnt,
	int *sbn_cnt)
{
  *dbn_cnt = 0;
  *sbn_cnt = 0;
  while (bzd)
    {
      *sbn_cnt += cnt_syo (bzd->sbn);
      (*dbn_cnt)++;
      bzd = bzd->lnk_son;
    }
}

static int
chk_yomi_endvect (
	int yomi_sno,		/* ����ʸ���� start index */
	int yomi_eno,		/* ����ʸ���� end index (end char �μ�) */
	int endvect,		/* ��ü�٥��� */
	int endvect1)		/* ��ü�٥��� */
{
  if (yomi_sno == yomi_eno || (fzk_ckvt (endvect) == NO && fzk_ckvt (endvect1) == NO))
    {
      wnn_errorno = WNN_NO_KOUHO;
      log_err ("chk_yomi_endvect: cannot make tan-bunsetu kouho.");
      return (-1);
    }
  return (0);
}

static int
set_kata_giji_sbn (
	int yomi_sno,		/* ����ʸ���� start index */
	int yomi_eno,		/* ����ʸ���� end index (end char �μ�) */
	int endvect,		/* ��ü�٥��� */
	int endvect1,		/* ��ü�٥��� */
	struct SYO_BNSETSU **sbn)
{
  struct ICHBNP *ichbnpbp;      /* ICHBNP �Υ����� */
  int fzkcnt;
  int tempi;
  int connect_flg = NO;

  if (chk_yomi_endvect (yomi_sno, yomi_eno, endvect, endvect1) < 0)
    return (-1);

  fzkcnt = fzk_kai (&bun[yomi_sno], &bun[yomi_eno], endvect, endvect1, &ichbnpbp);
  if (fzkcnt <= 0)
    {
      log_err ("tan_syo(): cannot make tan-bunsetu kouho.");
      init_work_areas ();
      return (-1);              /* ERROR */
    }
  for (tempi = fzkcnt - 1; (int) tempi >= (int) 0; tempi--)
    {
      if (kan_ck_vector (giji_no, getfzkoh (ichbnpbp, tempi)->vector) == WNN_CONNECT_BK)
        {
          connect_flg = YES;
          break;
        }
    }
  if (tempi < 0)
    {
      for (tempi = fzkcnt - 1; (int) tempi >= (int) 0; tempi--)
        {
          if (kan_ck_vector (giji_no, getfzkoh1 (ichbnpbp, tempi)->vector) == WNN_CONNECT_BK)
            {
              break;
            }
        }
      if (tempi < 0)
        {
          freeibsp (ichbnpbp);
          wnn_errorno = WNN_NO_KOUHO;
          log_err ("tan_syo(): cannot make tanbunsetu kouho.");
          return (-1);
        }
    }

  if ((*sbn = getsbnsp ()) == 0)
    return (-1);

  (*sbn)->j_c = yomi_eno - 1;
  (*sbn)->i_jc = getfzkoh (ichbnpbp, tempi)->offset + yomi_sno;
  (*sbn)->bend_m = yomi_sno;
  (*sbn)->v_jc = 0;
  (*sbn)->jentptr = 0;
  (*sbn)->t_jc = WNN_KATAKANA;
  (*sbn)->kangovect = ft->kango_hinsi_area[giji_no];
  (*sbn)->hinsi_fk = giji_no;
  (*sbn)->status = WNN_GIJI;
  (*sbn)->status_bkwd = connect_flg;
  freeibsp (ichbnpbp);
  return (1);
}

static int
set_kata_giji_bzd (
	int yomi_sno,		/* ����ʸ���� start index */
	int yomi_eno,		/* ����ʸ���� end index (end char �μ�) */
	int endvect,		/* ��ü�٥��� */
	int endvect1,		/* ��ü�٥��� */
	struct BZD **bzd,
	int buncnt)
{
  if ((*bzd = getbzdsp ()) == 0)
    return (-1);
  if (set_kata_giji_sbn (yomi_sno, yomi_eno, endvect, endvect1, &(*bzd)->sbn) < 0)
    return (-1);
  (*bzd)->j_c = yomi_eno - 1;
  (*bzd)->bend_m = yomi_sno;
  (*bzd)->v_jc = 0;
  (*bzd)->sbn_cnt = 1;
  (*bzd)->kbcnt = buncnt + 1;
  return (1);
}