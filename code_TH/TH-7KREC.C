/* 
#*************************************************************************************************************************
#
#Copyright or © or Copr.[DGFIP][2018]
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

/*============================*/
/* MODULE D ACTUALISATION DES */
/*      VALEURS LOCATIVES     */
/*============================*/
#define LA_SIGNATURE

#if defined (ILIAD)
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif

    /* PROTOTYPAGE DES FONCTIONS */
    /* ------------------------- */

    /* Néant */

    /*    MAIN    */
    /*   ------   */
int th_7krec(s_ree * p_e, s_res * p_s)
{
    extern s_signature la_signature;
    int ret = 0;
    double taux_tign;      /*zone resultat calcul tign */
    double reduction;    /*zone calcul reduction theorique */
    double taux_cumul;
    short nbdec = 4;

    double val_min1;
    double val_min2;
    p_s->anref = 2017;


    /* controle de la signature */
    /* ------------------------ */

    p_s->versr = 'B';
    p_s->signature = &la_signature;
    ret = controle_signature(RKREC,p_s->versr, &(p_s->libelle));
    if (ret != 0)
    {
        return (ret);
    }

    /*Controle de l'année de taxation*/
    /*----------------------*/

    if ((p_e->antax < p_s->anref-3 )||((p_e->antax > p_s->anref )))
    {
        ret = 1;
        return (ret);
    }

    /*    traitement    */
    /*    ----------    */


    /*   initialisation des zones sorties   */
    /*   --------------------------------   */


    if ((p_e->tax != 'P')&&(p_e->tax != 'S')&&(p_e->tax != 'E'))
        {
        ret=2202;
        return(ret);
        }
    /* taux global tig00 non compris entre 1% et 100% */
    if ((p_e->tig00<0.01) || (p_e->tig00>1))
        {
        ret=2203;
        return(ret);
        }
    if (p_e->cols_c.bnimp < p_e->vlbsc)
        {
        ret=2204;
        return(ret);
        }
     if ((p_e->cols_q.bnimp < p_e->vlbsc) &&
          (p_e->cocnq == 'C'))
        {
        ret=2205;
        return(ret);
        }
     if (p_e->cols_d.bnimp < p_e->vlbsc)
        {
        ret=2206;
        return(ret);
        }

     taux_cumul=(p_e->timpc + p_e->tisyn + p_e->titsn + p_e->tiagn + p_e->timpq + p_e->timpd + p_e->titgp + p_e->timpe);
     taux_tign=arrondi_nieme_decimale_voisine(taux_cumul,nbdec);
     taux_tign=taux_tign + DBL_EPSILON;

     if (((p_e->permo == '5') || (p_e->permo == '6')) && ((p_e->grrev == '2') || (p_e->grrev == ' ')) &&
         ((p_e->aff == 'F') || (p_e->aff == 'H') || (p_e->aff == 'M')))
        {
     taux_cumul=(p_e->timpc + p_e->tisyn + p_e->tiagn + p_e->timpq + p_e->timpd);
     taux_tign=arrondi_nieme_decimale_voisine(taux_cumul,nbdec);
     taux_tign=taux_tign + DBL_EPSILON;
        }


     if (taux_tign < p_e->tig00)
        {
        p_s->redtt = 0;
        ret = 0;

        return(ret);
        }

     val_min1=min(p_e->cols_c.bnimp,p_e->cols_q.bnimp);
    if (val_min1 == 0)
        {
        val_min1=max(p_e->cols_c.bnimp,p_e->cols_q.bnimp);
        }

    val_min2=min(val_min1,p_e->cols_d.bnimp);
    if (val_min2 == 0)
        {
        val_min2=max(val_min1,p_e->cols_d.bnimp);
        }

    reduction = ((val_min2 - p_e->vlbsc) * (taux_tign - p_e->tig00));



     p_s->redtt=arrondi_euro_voisin(reduction);

     return(0);

}                    /*   fin de traitement */
                     /*   ----------------- */





