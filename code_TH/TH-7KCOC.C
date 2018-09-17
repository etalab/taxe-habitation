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

/*====================================*/
    /*  MODULE DE CALCUL DES COTISATIONS  */
    /*        programme th-7KCOC.C        */
    /*====================================*/

#define LA_SIGNATURE

#if defined (ILIAD)
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif

    /*==============================*/
    /*   PROTOTYPAGE DES FONCTIONS  */
    /*==============================*/

long base_principaux(long bntot, char tax);
                                    /* calcul des bases principales           */

void cotisation(long bntot,long bnp,double taux,long *p_cott,long *p_cotp);
                                    /* calcul des cotisations                 */

void prelevement (char tyimp,long baspc,long vlbsc, char cnat,s_cons *pc,
                  long *p_prelt, long *p_prelp, long *p_pre02,
                  long *p_pre12, long *p_pre17);
                                                   /* calcul des prelevements */

void prelevement_add_ths (long totcott, s_cons *pc, long *p_preths);

void frais_gest_ar(long totcotcq,     long cotits,      long totcotng, long cotite,
                       double taux_FG_cq, double taux_FG_s, double taux_FG_ng, double taux_FG_e,
                       double *fraisGEST_cq, long *fraisGEST_s, long *fraisGEST_ng, double *fraisGEST_e);

                           /* calcul des prelevements additionnel THS */

    /*======*/
    /* MAIN */
    /*======*/

int th_7kcoc(s_coe1 * p_coe1 ,      /* pointeur sur structure entree          */
             s_coe2 * p_coe2 ,      /* pointeur sur structure entree          */
             s_cos  * p_cos)        /* pointeur sur structure sortie          */
{
    extern  s_signature la_signature;

    long    baspc        = 0   ;    /* bases nettes des principaux            */
    long    basps        = 0   ;
    long    baspn        = 0   ;
    long    baspq        = 0   ;
    long    baspd        = 0   ;
    long    baspg        = 0   ;
    long    baspe        = 0   ;
    long    cotitc       = 0   ;    /* cotisations totales                    */
    long    cotits       = 0   ;
    long    cotitq       = 0   ;
    long    cotitd       = 0   ;
    long    cotitn       = 0   ;
    long    cotitg       = 0   ;
    long    cotite       = 0   ;
    long    cotipc       = 0   ;    /* cotisations des principaux             */
    long    cotips       = 0   ;
    long    cotipq       = 0   ;
    long    cotipd       = 0   ;
    long    cotipn       = 0   ;
    long    cotipg       = 0   ;
    long    cotipe       = 0   ;
    long    totcott      = 0   ;    /* total des cotisations complètes        */
    long    totcotp      = 0   ;    /* total des cotisations des principaux   */
    long    totcotcq     = 0   ;    /* total des cotisations commune,interco  &
                                       et majoration THS */
    long    totcotng     = 0   ;    /* total des cotisations TSE et TSE Autre */
    int     ret_cons           ;    /* code retour de cherche_const           */
    int     ret                ;
    s_cons  *p_cons            ;    /* pointeur sur struct des constantes     */
    long    fraisDR      = 0   ;    /* frais de role                          */
    long    fraisAR      = 0   ;    /* frais d'assiette et de recouvremetn    */
    long    fraisDRP     = 0   ;    /* frais de role à titre principal        */
    double  fgest_cq     = 0   ;    /* frais de gestion commune , interco.
    								   & majoration THS                       */
    long    fgest_s      = 0   ;    /* frais de gestion syndicat              */
    long    fgest_ng     = 0   ;    /* frais de gestion TSE TSE Autre         */
    double  fgest_e      = 0   ;    /* frais de gestion GEMAPI         */
    double  far_cq       = 0   ;    /* frais d'assiette commune interco.      */
    long    far_s        = 0   ;    /* frais d'assiette syndicat              */
    long    far_ng       = 0   ;    /* frais d'assiette TSE TSE Autre         */
    double  far_e        = 0   ;    /* frais d'assiette GEMAPI        */
    long    fraip        = 0   ;
    long    prelt        = 0   ;    /* prelevement                            */
    long    prelp        = 0   ;
    long    preths       = 0   ;    /* prelevement                            */
    long    pre02        = 0   ;
    long    pre12        = 0   ;
    long    pre17        = 0   ;
            p_cos->versr = 'B' ;

    /* controle de la signature */
    p_cos->signature = &la_signature ;

    ret = controle_signature(RKCOC,p_cos->versr, &(p_cos->libelle));
    if (ret != 0 )
    {
        return (ret);
    }

     /* 1 */
    /* base nette des locaux principaux au niveau commune */

    baspc = base_principaux(p_coe1->cols_c.bnimp,p_coe1->tax);
    if (baspc < 0)
    {
        cherche_Erreur(901,&(p_cos->libelle));
        return (901);
    }

    /* base nette des locaux principaux au niveau syndicat */
    if (p_coe2->tisyn > 0)
    {
        basps = base_principaux(p_coe1->cols_s.bnimp,p_coe1->tax);
        if (basps < 0)
        {
            cherche_Erreur(905,&(p_cos->libelle));
            return (905);
        }
    }

    /* base nette des locaux principaux au niveau T.S.E.*/
    if (p_coe2->titsn > 0)
    {
        baspn = base_principaux(p_coe1->cols_n.bnimp,p_coe1->tax);
        if (baspn < 0)
        {
            cherche_Erreur(906,&(p_cos->libelle));
            return (906);
        }
    }

    /* base nette des locaux principaux au niveau C.U. */
    if (p_coe2->timpq > 0)
    {
        baspq = base_principaux(p_coe1->cols_q.bnimp,p_coe1->tax);
        if (baspq < 0)
        {
            cherche_Erreur(903,&(p_cos->libelle));
            return (903);
        }
    }

    /* base nette des locaux principaux au niveau departement */
    baspd = base_principaux(p_coe1->cols_d.bnimp,p_coe1->tax);
    if (baspd < 0)
    {
        cherche_Erreur(904,&(p_cos->libelle));
        return (904);
    }

    /* TSE Autre */
    if (p_coe2->titgp > 0)
    {
        baspg = base_principaux(p_coe1->cols_g.bnimp,p_coe1->tax);
        if (baspg < 0)
        {
            cherche_Erreur(907,&(p_cos->libelle));
            return (907);
        }
    }

    /* base  GEMAPI */
    if (p_coe2->timpe > 0)
        {
         baspe = base_principaux(p_coe1->cols_e.bnimp,p_coe1->tax);

        if (baspe < 0)
        {
            cherche_Erreur(908,&(p_cos->libelle));
            return (908);
        }
     }
    /* 2 */
    /* cotisations au niveau commune */
    cotisation(p_coe1->cols_c.bnimp, baspc, p_coe2->timpc, &cotitc,&cotipc);

    /* cotisations au niveau syndicat */
    cotisation(p_coe1->cols_s.bnimp, basps, p_coe2->tisyn, &cotits,&cotips);

    /* cotisations au niveau C.U. */
    cotisation(p_coe1->cols_q.bnimp, baspq, p_coe2->timpq, &cotitq,&cotipq);

    /* cotisations au niveau departement */
    cotisation(p_coe1->cols_d.bnimp, baspd, p_coe2->timpd, &cotitd,&cotipd);

    /* cotisations au niveau T.S.E. */
    cotisation(p_coe1->cols_n.bnimp, baspn, p_coe2->titsn, &cotitn,&cotipn);

    /* cotisations au niveau T.S.E. Autre */
    cotisation(p_coe1->cols_g.bnimp, baspg, p_coe2->titgp, &cotitg,&cotipg);

    /* cotisations au niveau GEMAPI */
    cotisation(p_coe1->cols_e.bnimp, baspe, p_coe2->timpe, &cotite,&cotipe);

    /* chargement des cotisations totales dans la structure de sortie */
    p_cos->cotid  = cotitd;
    p_cos->cotic  = cotitc;
    p_cos->cotiq  = cotitq;
    p_cos->cotis  = cotits;
    p_cos->cotin  = cotitn;
    p_cos->cotigp = cotitg;
    p_cos->cotie  = cotite;
    p_cos->cotia  = 0;

    /* cumul des cotisations */
    totcott    = cotitc + cotits + cotitq + cotitd + cotitn + cotitg + cotite;
    totcotp    = cotipc + cotips + cotipq + cotipd + cotipn + cotipg + cotipe;
    totcotcq   = cotitc + cotitq;
    totcotng   = cotitn + cotitg;

    /* Appel et contrôle des const tabcons */
    ret_cons= cherche_const(p_coe1->antax, &(p_cons));
    if (ret_cons != 0)
    {
        cherche_Erreur(1,&(p_cos->libelle)) ;
        return (1);               /* Erreur interne th_nkcoc : annee invalide*/
    }

    /* Détermination de la majoration THS */
    if (p_coe1->tax == 'S')
        {
            if (p_coe1->antax < 2016)
            {
                if (((p_coe1->aff == 'H') || (p_coe1->aff == 'M')) && (p_coe1->cnat != '5'))
                {
                    if (p_coe1->indmths == 'O') {

                        p_cos->majths = arrondi_euro_voisin(p_cos->cotic * p_cons->taux_majths);

                    }else{
                        p_cos->majths = 0 ;
                    }
                }else{
                     p_cos->majths = 0 ;
                     }
            }else if ((p_coe1->aff == 'H') && (p_coe1->indmths == 'O')) {
                     if ((p_coe1->cnat != '5')  || (p_coe1->indaths == 'O'))
                        {
                            if (p_coe1->antax == 2016) {
                                p_cos->majths = arrondi_euro_voisin(p_cos->cotic * p_cons->taux_majths);
                              }
                              else { /* A partir de 2017 taux donne par la zone TIMTHS*/
                                p_cos->majths = arrondi_euro_voisin(p_cos->cotic * p_coe2->timths);
                              }
                        }else{
                            p_cos->majths = 0 ;
                            }
                    }else{
                         p_cos->majths = 0 ;
                         }
        }else{
             p_cos->majths = 0 ;
             }

    /* Détermination cotisation majorée */

    p_cos->coticm = p_cos->cotic + p_cos->majths ;

    /* 3 */
    /* calcul des frais */

    /* Rajout de la Majoration THS */
    totcotcq += p_cos->majths ;

    /* initialisation Frais */
    p_cos->fgest_cq = fgest_cq;
    p_cos->fgest_s  = fgest_s;
    p_cos->fgest_ng = fgest_ng;
    p_cos->far_cq = far_cq;
    p_cos->far_s  = far_s;
    p_cos->far_ng = far_ng;
    p_cos->fgest_e  = fgest_e;
    p_cos->far_e  = far_e;
    p_cos->frai4  = 0;
    p_cos->frait  = 0;

    if (p_coe1->tyimp == 'H')
    {
            if (p_coe1->tax == 'S') {  /* frais THS  */
                frais_gest_ar(totcotcq, cotits, totcotng, cotite, p_cons->coeff_FGEST_THS_c, p_cons->coeff_FGEST_THS_s, p_cons->coeff_FGEST_THS_t,
                		      p_cons->coeff_FGEST_THS_g,  &fgest_cq, &fgest_s, &fgest_ng, &fgest_e);
                p_cos->fgest_e  = fgest_e;
                p_cos->fgest_cq = arrondi_euro_voisin(fgest_cq) + arrondi_euro_voisin(fgest_e);
                p_cos->fgest_s  = fgest_s;
                p_cos->fgest_ng = fgest_ng;
                p_cos->fgest_e  = 0;
                frais_gest_ar(totcotcq, cotits, totcotng, cotite, p_cons->coeff_FAR_THS_c, p_cons->coeff_FAR_THS_s, p_cons->coeff_FAR_THS_t,
                		      p_cons->coeff_FAR_THS_g,  &far_cq , &far_s , &far_ng, &far_e  );
                p_cos->far_e = far_e ;
                p_cos->far_cq = arrondi_euro_voisin(far_cq) + arrondi_euro_voisin(far_e) ;
                p_cos->far_s  = far_s;
                p_cos->far_ng = far_ng;
                p_cos->far_e  = 0;
                p_cos->frait = p_cos->fgest_cq + p_cos->fgest_s + p_cos->fgest_ng;
                p_cos->frai4 = p_cos->far_cq + p_cos->far_s + p_cos->far_ng;
                fraip = 0;

            }
            else {  /* frais THP ou THE  */
                frais_gest_ar(totcotcq, cotits, totcotng, cotite, p_cons->coeff_FGEST_THP_THE_c, p_cons->coeff_FGEST_THP_THE_s, p_cons->coeff_FGEST_THP_THE_t,
                		      p_cons->coeff_FGEST_THP_THE_g,  &fgest_cq, &fgest_s, &fgest_ng, &fgest_e);
                p_cos->fgest_e  = fgest_e;
                p_cos->fgest_cq = arrondi_euro_voisin(fgest_cq) + arrondi_euro_voisin(fgest_e);
                p_cos->fgest_s  = fgest_s;
                p_cos->fgest_ng = fgest_ng;
                p_cos->fgest_e  = 0;
                frais_gest_ar(totcotcq, cotits, totcotng, cotite, p_cons->coeff_FAR_THP_THE_c, p_cons->coeff_FAR_THP_THE_s, p_cons->coeff_FAR_THP_THE_t,
                		      p_cons->coeff_FAR_THP_THE_g,  &far_cq , &far_s , &far_ng , &far_e);
                p_cos->far_e  = far_e;
                p_cos->far_cq = arrondi_euro_voisin(far_cq) + arrondi_euro_voisin(far_e) ;
                p_cos->far_s  = far_s;
                p_cos->far_ng = far_ng;
                p_cos->far_e  = 0;
                fraip = p_cos->fgest_cq + p_cos->fgest_s + p_cos->fgest_ng ;
                p_cos->frait = p_cos->fgest_cq + p_cos->fgest_s + p_cos->fgest_ng ;
                p_cos->frai4 = p_cos->far_cq + p_cos->far_s + p_cos->far_ng;


            }
    }

    if (p_coe1->tyimp == 'T')  /* frais THLV */
    {
        p_cos->frai4  = 0;
        p_cos->frait  = 0;

        /* En cas de THLV avec GEMAPI, les taux de frais sont différents et il faut
        calculer séparément les frais sur Commune/Syndicat (ou Interco) puis GEMAPI */

        totcott -= cotite;
        totcotp -= cotite;
        frais(totcott, totcotp, p_cons->coeff_frais_THLV,p_cons->coeff_frais_ass_THLV, &fraisDR, &fraisAR, &fraisDRP);
        fgest_e = ( arrondi_euro_voisin (cotite * p_cons->coeff_FGEST_THP_THE_g));
        far_e = ( arrondi_euro_voisin (cotite * p_cons->coeff_FAR_THP_THE_g));
        totcott += cotite;
        totcotp += cotite;
        p_cos->fgest_e  = fgest_e;
        p_cos->far_e  = far_e;
        p_cos->fgest_cq = fraisDR;
        p_cos->far_cq  = fraisAR;
        p_cos->frait = ( fraisDR + fgest_e );
        p_cos->frai4 = ( fraisAR + far_e );
        fraip        = ( fraisDRP + fgest_e );
    }

    /* evolution 2008 : mise a zero des frais pour la commune de Saint-Martin */
    if (!strcmp(p_coe1->cne,"127") && !strcmp(p_coe1->dep,"97"))
    {
        p_cos->frait = 0;
        p_cos->frai4 = 0;
        fraip        = 0;
    }

    /* 4 */
    /* calcul du prelevement */
    if (p_coe1->degex == ' ')
    {
        if ( p_coe1->tyimp == 'T' && p_coe1->indthlv == 'I')
        {
            prelevement(p_coe1->tyimp, baspq, p_coe1->vlbsc, p_coe1->cnat, p_cons,
                        &prelt, &prelp, &pre02, &pre12, &pre17);
        }
        else
        {
            prelevement(p_coe1->tyimp, baspc, p_coe1->vlbsc, p_coe1->cnat, p_cons,
                        &prelt, &prelp, &pre02, &pre12, &pre17);
        }
    }
    p_cos->prelt = prelt;
    p_cos->pre02 = pre02;
    p_cos->pre12 = pre12;
    p_cos->pre17 = pre17;

    /* 5 */
    /* calcul de la somme a recouvrer */
    p_cos->somrc = 0;
    p_cos->somrp = 0;
    p_cos->preths = 0;

    if (p_coe1->tax == 'S')
    {
    	prelevement_add_ths (totcotcq, p_cons,&preths);
        p_cos->preths = preths;
    }

    p_cos->somrc = totcott + p_cos->frait + p_cos->prelt + p_cos->preths + p_cos->majths;
    p_cos->somrp = totcotp + fraip + prelp;

    /* 6 */
    /* traitement fin */
    return (0);
}

    /*----------------------------------------------*/
    /* fonction de calcul des bases des principales */
    /*----------------------------------------------*/

long base_principaux(long bntot, char tax)
{
    if (tax == 'S')
    {
        return(0);
    }
    else
    {
        return (bntot);
    }
}

    /*-----------------------------------------------------*/
    /* fonction de calcul des cotisations par collectivite */
    /*-----------------------------------------------------*/

void cotisation (long bntot,long bnp,double taux,long *p_cott,long *p_cotp)
{
    *p_cott = arrondi_euro_voisin(bntot * taux);
                                    /* cotisation totale */
    *p_cotp = arrondi_euro_voisin(bnp * taux);
                                    /* cotisation des principaux */
}

    /*-----------------------------------*/
    /* fonction de calcul du prelevement */
    /*-----------------------------------*/

void prelevement (char tyimp, long baspc,long vlbsc,char cnat,s_cons *pc,
                  long *p_prelt, long *p_prelp, long *p_pre02, long *p_pre12,
                  long *p_pre17)
{
    if (tyimp == 'H')
    {
        if (baspc > pc->seuil_prel)
        {
            *p_prelt += arrondi_euro_voisin(baspc * pc->taux_prelp);
            *p_prelp += arrondi_euro_voisin(baspc * pc->taux_prelp);
            *p_pre02 += arrondi_euro_voisin(baspc * pc->taux_prelp);
        }
        if ((vlbsc > pc->seuil_prel) && (cnat == '5'))
        {
            *p_prelt += arrondi_euro_voisin(vlbsc * pc->taux_prelp);
            *p_pre02 += arrondi_euro_voisin(vlbsc * pc->taux_prelp);
        }
        if ((vlbsc > pc->seuil_prel) &&
            (vlbsc <= pc->seuil_prels) &&
            (cnat != '5'))
        {
            *p_pre12 = arrondi_euro_voisin(vlbsc * pc->taux_prels1);
            *p_prelt += arrondi_euro_voisin(vlbsc * pc->taux_prels1);
        }
        if ((vlbsc > pc->seuil_prels) && (cnat != '5'))
        {
            *p_pre17 = arrondi_euro_voisin(vlbsc * pc->taux_prels2);
            *p_prelt += arrondi_euro_voisin(vlbsc * pc->taux_prels2);
        }
    }

    if (tyimp == 'T')
    {
        if ((baspc >  pc->seuil_prel) &&
            (baspc <= pc->seuil_prels))
        {
            *p_pre12 = arrondi_euro_voisin(baspc * pc->taux_prels1);
            *p_prelt += arrondi_euro_voisin(baspc * pc->taux_prels1);
        }
        if ((baspc > pc->seuil_prels))
        {
            *p_pre17 = arrondi_euro_voisin(baspc * pc->taux_prels2);
            *p_prelt += arrondi_euro_voisin(baspc * pc->taux_prels2);
        }
    }
}

/*JBI:Le prélèvement additionnel résidence secondaires est valorisé par la multiplication
 du total des cotisations par le taux prélèvement additionnel THS */
void prelevement_add_ths(long totcott ,s_cons *pc, long *p_preths)
{
 *p_preths = arrondi_euro_voisin(totcott * pc->taux_preths);
}

/* --------------------------------------- */
/* fonction de calcul des frais de gestion */
/* --------------------------------------- */

void frais_gest_ar(long totcotcq, long cotits, long totcotng, long cotite,
                   double taux_F_cq, double taux_F_s, double taux_F_ng, double taux_F_e,
                   double *frais_cq, long *frais_s, long *frais_ng, double *frais_e)

{
    *frais_cq = totcotcq * taux_F_cq;
    *frais_s  = arrondi_euro_voisin(cotits * taux_F_s);
    *frais_ng = arrondi_euro_voisin(totcotng * taux_F_ng);
    *frais_e  = cotite * taux_F_e;
}
