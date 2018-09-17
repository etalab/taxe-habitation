/* 
#*************************************************************************************************************************
#
#DGFIP [2018]
#
#Ce logiciel a été initialement développé par la Direction Générale des
#Finances Publiques pour permettre le calcul de la taxe d'habitation 2017.
#
#Ce logiciel est régi par la licence CeCILL 2.1 soumise au droit français
#et respectant les principes de diffusion des logiciels libres. Vous pouvez
#utiliser, modifier et/ou redistribuer ce programme sous les conditions de
#la licence CeCILL 2.1 telle que diffusée par le CEA, le CNRS et l'INRIA  sur
#le site "http://www.cecill.info".
#
#Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
#connaissance de la licence CeCILL 2.1 et que vous en avez accepté les termes.
#
#**************************************************************************************************************************
*/

/*============================================================================*/
/* module de calcul des bases nettes d'imposition par collectivite et de      */
/* simulation de l'abattement special à la base                               */
/*============================================================================*/

#define LA_SIGNATURE

#if defined (ILIAD)
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif

    /*                       prototypage des fonctions                        */

void init_cols(s_cols *);
void base_nette(s_colb *, s_bne1 *,
                s_cols *, short, short, short, short, long*);
void hlm(s_cols *);
void abat_gen(long, long, long, long *, long *);
void cal_preal( long, long *, long *, long *, long *);
void detf_abat_pac(long, long, long,
                   long *, long *);
void detg_abat_pac(long, long, long, long, long,
                   short, long *, long *);
void abat_pac(short , short , short, short, long *, long *, long *, long *, long *);
void abat_spe(long, long, short, short, long, long *, long *, char *);
void abat_han(long *, char, long *, long *);
void cumul_vl_exo(char, long, long *, long *, long *, long *);
void base_nettethlv(s_bne1 *,s_cols *);

                         /*=========*/
                         /*   main  */
                         /*=========*/

int th_7kbnc(s_bne1 * p_bne1, s_bne2 * p_bne2, s_bns * p_bns)
{
    extern s_signature la_signature;
    int ret;    /*zone retour programme th_nkbnc */
    int rets;   /*zone retour signature */

    short nb12;
    short nbsup;
    short nbpac;
    short nbera;
    short nbera12;
    short nberasup;
    long mtvl;

    p_bns->versr='D';
    p_bns->signature=&la_signature;

    ret=0;
    rets=0;
    nb12=0;
    nbsup=0;
    nbera12=0;
    nberasup=0;
    nbpac=p_bne1->nbpac;
    nbera=p_bne1->nbera;

    /*                        controle de la signature                        */

    rets=controle_signature(RKBNC,p_bns->versr,&(p_bns->libelle));

    if (rets!=0)
    {
        cherche_Erreur(rets,&(p_bns->libelle));
        return(rets);
    }

    /*                    initialisation des zones sorties                    */

    init_cols(&(p_bns->cols_c));
    init_cols(&(p_bns->cols_s));
    init_cols(&(p_bns->cols_q));
    init_cols(&(p_bns->cols_d));
    init_cols(&(p_bns->cols_n));
    init_cols(&(p_bns->cols_g));
    init_cols(&(p_bns->cols_e));
    init_cols(&(p_bns->cols_f));
    init_cols(&(p_bns->cols_r));


    /*                            traitement THLV                             */
    if(p_bne1->tyimp=='T')
    {
		/*                           base nette commune                           */

		if( p_bne2->indthlv == 'C' ) {
			base_nettethlv(p_bne1,&(p_bns->cols_c));
			if(p_bne2->tisyn != 0) {/* base nette  syndicat */
				base_nettethlv(p_bne1,&(p_bns->cols_s)); }
		}

		/* base nette  intercommunalité */
		if ( p_bne2->indthlv == 'I' ) {
			base_nettethlv(p_bne1,&(p_bns->cols_q));
		}

        /* Base nette GEMAPI qui peut s'adosser sur Commune/syndicat ou Interco en THLV */

        if (p_bne1->antax > 2015) {
            if ( p_bne2->timpe > 0 ) {
                base_nettethlv(p_bne1,&(p_bns->cols_e));

            if (((p_bne1->permo=='5')||(p_bne1->permo=='6'))&&
            ((p_bne1->grrev=='2')||(p_bne1->grrev==' '))&&
            ((p_bne1->aff=='F')||(p_bne1->aff=='H')||(p_bne1->aff=='M'))) {

                /* exo gemapi */
                hlm(&(p_bns->cols_e));
                }
            }
        }
        return 0;
    }
    /*                             traitement  TH                             */

    /*                determination du nombre de pac par rang                 */

    if (nbera<2)
    {
        nbera12=nbera;
        nberasup=0;

        if (nbpac<(2-nbera12))
        {
            nb12=nbpac;
            nbsup=0;
        }
        else
        {
            nb12=2-nbera12;
            nbsup=nbpac - nb12;
        }
    }
    else
    {
        nbera12=2;
        nberasup=nbera-(short)2;
        nb12=0;
        nbsup=nbpac;
    }

    /*                appel de la fonction base nette commune                 */

    base_nette(&(p_bne2->colb_c),
                 p_bne1,
               &(p_bns->cols_c),
               nb12, nbsup, nbera12, nberasup, &mtvl);

    /*                appel de la fonction base nette syndicat                */

    if (p_bne2->tisyn!=0)
    {
        base_nette(&(p_bne2->colb_s),
                     p_bne1,
                   &(p_bns->cols_s),
                   nb12, nbsup, nbera12, nberasup, &mtvl);
    }

    /*           appel de la fonction base nette communaute urbaine           */

    if (p_bne2->cocnq=='C')
    {
        base_nette(&(p_bne2->colb_q),
                     p_bne1,
                   &(p_bns->cols_q),
                   nb12, nbsup, nbera12, nberasup, &mtvl);
    }

    /*              appel de la fonction base nette departement               */

   base_nette(&(p_bne2->colb_d),
                p_bne1,
              &(p_bns->cols_d),
              nb12, nbsup, nbera12, nberasup, &mtvl);


    /*              appel de la fonction base nette tse                       */

    if (p_bne2->titsn!=0)
    {
        base_nette(&(p_bne2->colb_n),
                     p_bne1,
                   &(p_bns->cols_n),
                   nb12, nbsup, nbera12, nberasup, &mtvl);
    }

        /*  appel de la fonction base nette TSE Autre  */

    if (p_bne2->titgp != 0){
        base_nette(&(p_bne2->colb_g),
                     p_bne1,
                   &(p_bns->cols_g),
                   nb12, nbsup, nbera12, nberasup, &mtvl);
    }
    /*  appel de la fonction base nette GEMAPI  */

    if (p_bne2->timpe > 0){
        base_nette(&(p_bne2->colb_e),
                     p_bne1,
                   &(p_bns->cols_e),
                   nb12, nbsup, nbera12, nberasup, &mtvl);
    }

        /* appel de la fonction base nette commune avant ajustement  */
    base_nette(&(p_bne2->colb_f),
                 p_bne1,
               &(p_bns->cols_f),
               nb12, nbsup, nbera12, nberasup, &mtvl);

        /* appel de la fonction base nette intercommunalité avant ajustement  */
    if (p_bne2->cocnq=='C')
    {
        base_nette(&(p_bne2->colb_r),
                     p_bne1,
                   &(p_bns->cols_r),
                   nb12, nbsup, nbera12, nberasup, &mtvl);
    }

    if (((p_bne1->permo=='5')||(p_bne1->permo=='6'))&&
        ((p_bne1->grrev=='2')||(p_bne1->grrev==' '))&&
        ((p_bne1->aff=='F')||(p_bne1->aff=='H')||(p_bne1->aff=='M')))
    {
        hlm(&(p_bns->cols_n));

        /* La valeur locative nette exonérée finale est égale à la somme de la valeur locative nette exonérée
        calculée suite à la règle détaillée MR-KBN-01 et de la base nette d'imposition (cols_g.bnimp). */
        hlm(&(p_bns->cols_g));

        /* exo gemapi */

        hlm(&(p_bns->cols_e));


    }

    return(ret);
}

    /*                           fonction init_cols                           */

void init_cols(s_cols *p)
{
    p->abgen=0;
    p->abpac=0;
    p->abp12=0;
    p->abspe=0;
    p->abhan=0;
    p->absps=' ';
    p->vlntt=0;
    p->vlnex=0;
    p->bnimp=0;
}

    /*                              fonction hlm                              */

void hlm(s_cols *p)
{
    p->vlnex=p->vlnex + p->bnimp;
    p->bnimp=0;
}


    /*                          fonction base nette                           */

void base_nette(s_colb *p1, s_bne1 *p2, s_cols *p3, short nb12,
                short nbsup, short nbera12, short nberasup, long *p_mtvl)
{
    long vlbp10;
    long vlmc10;
    long vlmc15;
    long vlmc20;
    long vlmc25;
    short numg;

    *p_mtvl=0;

    vlbp10=0;
    vlmc10=0;
    vlmc15=0;
    vlmc20=0;
    vlmc25=0;
    numg=0;

    if (p2->tax=='P')
    {
    /*                   appel de la fonction abat_gen                        */

            abat_gen(p2->vlbpc,
                 p2->vlbri,
                 p1->abbas,
               &(p3->abgen),
                 p_mtvl);

    /*                   appel de la fonction cal_preal                       */

        switch (p1->codeg)
        {
            case '1':numg=1;
            break;
            case '2':numg=2;
            break;
            case '3':numg=3;
            break;
            case '4':numg=4;
            break;
            case '5':numg=5;
        }

        if ((p1->codef=='F')||((numg>0)&&(numg<6)))
        {
        vlbp10=arrondi_euro_voisin(p2->vlbpc / 10);

            if (vlbp10<p1->abmos)
            {
                vlbp10=p1->abmos;
            }
            cal_preal( p1->vlmoy,&vlmc10, &vlmc15, &vlmc20, &vlmc25);
        }

    /*                 appel de la fonction detf_abat_pac                     */

        if (p1->codef=='F')
        {
            detf_abat_pac(vlbp10, vlmc10, vlmc15,
                          &(p1->apac1), &(p1->apac3));
        }

    /*                   appel de la fonction detg_abat_pac                   */

        if ((numg>0)&&(numg<6))
        {
            detg_abat_pac(vlbp10, vlmc10, vlmc15, vlmc20, vlmc25,
                          numg,
                          &(p1->apac1), &(p1->apac3));
        }

    /*                   appel de la fonction abat_pac                        */

        abat_pac(nb12,
                 nbera12,
                 nbsup,
                 nberasup,
                 &(p1->apac1),
                 &(p1->apac3),
                 p_mtvl,
                 &(p3->abpac),
                 &(p3->abp12));

    /*                     appel de la fonction abat_spe                      */

        if (p2->codni=='A')
        {
            abat_spe(p1->abspe,
                     p1->vlmoy,
                     p2->nbpac,
                     p2->nbera,
                     p2->vlbpc,
                     p_mtvl,
                     &(p3->abspe),
                     &(p3->absps));
        }
        else
        {
            p3->absps='N';
        }

    /*                     appel de la fonction abat_han                      */

        abat_han(&(p1->abhan),
                   p2->codsh,
                   p_mtvl,
                 &(p3->abhan));

    /*                   appel de la fonction cumul_vl_exo                    */


        cumul_vl_exo(p2->degex,
                     p2->vlbsc,
                     p_mtvl,
                   &(p3->vlntt),
                   &(p3->vlnex),
                   &(p3->bnimp));
    }
    if (p2->tax!='P')
    {
        p3->vlnex=0;
        if (est_exoneration(p2->degex))
        {
            p3->vlnex=p2->vlbpc;
        }

        if (p2->tax == 'E')
        {
            p3->vlntt = p2->vlbri;
        }
        if (p2->tax == 'S')
        {
            p3->vlntt = p2->vlbsc;
        }

        p3->bnimp=p3->vlntt - p3->vlnex;
    }
}

    /*                           fonction abat_gen                            */

void abat_gen(long vlbpc, long vlbri, long abbas, long * p_abgen, long * p_mtvl)
{
    if ((abbas!=0)&&(abbas<vlbri))
    {
        *p_mtvl=vlbri - abbas;
        *p_abgen=abbas;
    }
    if ((abbas!=0)&&(abbas>=vlbri))
    {
        *p_mtvl=0;
        *p_abgen=vlbri;
    }
    if (abbas==0)
    {
        *p_mtvl=vlbri;
        *p_abgen=abbas;
    }
}

    /*                           fonction cal_preal                           */

void cal_preal(long vlmoy, long *p_vlmc10, long *p_vlmc15,
               long *p_vlmc20, long *p_vlmc25)
{
    *p_vlmc10=arrondi_euro_voisin(vlmoy / 10);
    *p_vlmc15=vlmoy * 15;
    *p_vlmc15=arrondi_euro_voisin(*p_vlmc15 / 100);
    *p_vlmc20=arrondi_euro_voisin(vlmoy / 5);
    *p_vlmc25=arrondi_euro_voisin(vlmoy / 4);
}

    /*                         fonction detf_abat_pac                         */

void detf_abat_pac(long vlbp10, long vlmc10, long vlmc15,
                   long *p_apac1, long *p_apac3)
{
    if (vlbp10<vlmc10)
    {
        *p_apac1=vlmc10;
    }
    else
    {
        *p_apac1=vlbp10;
    }
    if (vlbp10<vlmc15)
    {
        *p_apac3=vlmc15;
    }
    else
    {
        *p_apac3=vlbp10;
    }
}

    /*                         fonction detg_abat_pac                         */

void detg_abat_pac(long vlbp10, long vlmc10, long vlmc15,
                   long vlmc20, long vlmc25,
                   short numg,
                   long *p_apac1, long *p_apac3)
{
    if (vlmc10>=vlbp10)
    {
        *p_apac1=vlmc10;
        *p_apac3=vlmc15;
    }
    if ((vlmc10<vlbp10)&&(vlbp10<=vlmc15))
    {
        *p_apac1=vlbp10;
        *p_apac3=vlmc15;
    }
    if (vlbp10>vlmc25)
    {
        *p_apac1=(vlbp10 - ((vlbp10 - vlmc20) * numg) / 5);
        *p_apac3=(vlbp10 - ((vlbp10 - vlmc25) * numg) / 5);

    }
    if ((vlbp10>vlmc20)&&(vlbp10<=vlmc25))
    {
        *p_apac1=(vlbp10 - ((vlbp10 - vlmc20) * numg) / 5);
        *p_apac3=vlbp10;
    }
    if ((vlbp10>vlmc15)&&(vlbp10<=vlmc20))
    {
        *p_apac1=vlbp10;
        *p_apac3=vlbp10;
    }
}

    /*                           fonction abat_pac                            */

void abat_pac(short nb12, short nbera12, short nbsup, short nberasup,
              long *p_apac1, long *p_apac3,
              long *p_mtvl, long *p_abpac, long *p_abp12)
{
    long valaba;
    long aera1;
    long aera3;

    valaba=0;
    aera1=0;
    aera3=0;

    aera1=arrondi_euro_voisin(((double)*p_apac1)/ 2);
    aera3=arrondi_euro_voisin(((double)*p_apac3)/ 2);

    valaba=(nb12 * *p_apac1) + (nbsup * *p_apac3) + (nbera12 * aera1) + (nberasup * aera3);
    if (*p_mtvl>valaba)
    {
        *p_mtvl=*p_mtvl - valaba;
        *p_abpac=valaba;
        *p_abp12=(nb12 * *p_apac1) + (nbera12 * aera1);
    }
    else
    {
        *p_abpac=*p_mtvl;
        if (*p_mtvl-((nb12 * *p_apac1) + (nbera12 * aera1))<0)
        {
            *p_abp12=*p_mtvl;
        }
        else
        {
            *p_abp12=(nb12 * *p_apac1) + (nbera12 * aera1);
        }
        *p_mtvl=0;
    }
}

    /*                           fonction abat_spe                            */

void abat_spe(long abspe, long vlmoy, short nbpac, short nbera, long vlbpc,
              long *p_mtvl, long *p_abspe, char *p_absps)
{
    long sasp;
    double nb13;
    double vl130;

    sasp=0;
    nb13=0;
    vl130=0;

    nb13=nbpac + ((double)nbera / 2) + (short)13;
    vl130=vlmoy * nb13;
    sasp=arrondi_euro_voisin(((double)vl130) / 10);

    if (abspe>0)
    {
        if (vlbpc<sasp)
        {
            *p_absps = 'O';
            if ((*p_mtvl - abspe)>0)
            {
                *p_mtvl=*p_mtvl - abspe;
                *p_abspe=abspe;
            }
            else
            {
                *p_abspe=*p_mtvl;
                *p_mtvl=0;
            }
        }
        else
        {
           *p_absps = 'N';
        }
    }
}

    /*                           fonction abat_han                            */

void abat_han(long *pe_abhan, char codsh, long *p_mtvl, long *ps_abhan)
{
    long mtvl = *p_mtvl;

    if (codsh=='1' || codsh=='2' || codsh=='3' || codsh=='4' || codsh=='5')
    {
        if ((*pe_abhan!= 0) && (*pe_abhan < *p_mtvl))
        {
            mtvl = *p_mtvl - *pe_abhan;
            *ps_abhan = *pe_abhan;
        }

        if ((*pe_abhan!= 0) && (*pe_abhan >= *p_mtvl))
        {
            *ps_abhan = *p_mtvl;
            mtvl = 0;
        }

        if (*pe_abhan== 0)
        {
            *ps_abhan = *pe_abhan;
        }

        *p_mtvl = mtvl;
    }
}

    /*                         fonction cumul_vl_exo                          */

void cumul_vl_exo(char degex, long vlbsc, long *p_mtvl,
                  long *p_vlntt, long *p_vlnex, long *p_bnimp)
{
    int retexo;
    retexo=0;

    retexo=est_exoneration(degex);
    if (retexo==1)
    {
        *p_vlnex=*p_mtvl;
        *p_vlntt=vlbsc + *p_mtvl;
        *p_bnimp=vlbsc;
    }
    else
    {
        *p_vlntt=*p_mtvl + vlbsc;
        *p_vlnex=0;
        *p_bnimp=*p_mtvl + vlbsc;
    }
}

    /*                        fonction base_nettethlv                         */

void base_nettethlv( s_bne1 *p2, s_cols *p3)
{
    p3->vlntt=p2->vlbpc;
    p3->bnimp=p2->vlbpc;
}
