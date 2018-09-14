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

/*==================================*/
/* MODULE DE CALCUL DES COTISATIONS */
/*          pour la TLV             */
/*     programme th-7KCVC.C         */
/*==================================*/

#define LA_SIGNATURE

#if defined (ILIAD)
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif

    /*===========================*/
    /* PROTOTYPAGE DES FONCTIONS */
    /*===========================*/


    /*======*/
    /* MAIN */
    /*======*/

int th_7kcvc(s_e7 * p_e7 ,     /*pointeur sur structure entree*/
             s_s7 * p_s7)      /*pointeur sur structure sortie*/
{

    extern s_signature la_signature;

    long fraiDR;        /* frais */
    long fraiAR;
    long fraiDRP;
    int ret_cons;       /* code retour de cherche_const */
    int ret;            /* code retour de Nap_Ni_Deg_Ni_Exo */
    s_cons *p_cons;     /* pointeur sur struct des constantes */
    s_dne dne;          /* pointeur sur struct entrée net à payer */
    s_dne *p_dne;
    s_dns dns;          /* pointeur sur struct sortie net à payer */
    s_dns *p_dns;

    p_dne=&dne;
    p_dns=&dns;

    /* identification du module */
    p_s7->anref = 2017 ;
    p_s7->versr = 'B' ;


    /* controle de la signature */
    p_s7->signature = &la_signature ;

    ret = controle_signature(RKCVC,p_s7->versr, &(p_s7->libelle));
    if (ret != 0 )
    {
        return (ret);
    }

    /* 1 */
    /* recherche des constantes */
    ret_cons=(short)cherche_const(p_e7->antax,&p_cons);
    cherche_Erreur(ret_cons,&(p_s7->libelle)) ;
    if (ret_cons == 1)          /* annee invalide */
    {
        return (1);
    }

    /* 3 */

    /* cotisation à partir de la base à taxer au taux 1 */
    p_s7->cotv1 = arrondi_euro_voisin(p_e7->btlv1 * p_cons->taux_TLV_1) ;

    /* cotisation à partir de la base à taxer au taux 2 */
    p_s7->cotv2 = arrondi_euro_voisin(p_e7->btlv2 * p_cons->taux_TLV_2) ;

    /* somme des cotisations */
    p_s7->cotiv = p_s7->cotv1 + p_s7->cotv2 ;

    /* total des frais de rôle */
    frais(p_s7->cotiv, 0, p_cons->coeff_frais_TLV,p_cons->coeff_frais_ass_TLV, &fraiDR, &fraiAR, &fraiDRP);
    p_s7->frait = fraiDR ;
    p_s7->frai5 = fraiAR ;

    /* net à payer */
    p_dne->degex = ' ' ;
    p_dne->revim = 0 ;
    p_dne->somrp = 0 ;
    p_dne->somrc = p_s7->cotiv + p_s7->frait ;
    p_dne->vlexo = 'N' ;
    ret=NaP_Ni_Deg_Ni_Exo(p_cons,p_dne,p_dns);
    if (ret!=0)
    {
        cherche_Erreur(ret,&(p_s7->libelle)) ;
        return ret;
    }
    p_s7->totlv = p_dns->totth ;
    p_s7->netap = p_dns->netth ;
    strncpy(p_s7->codro, p_dns->codro, 3) ;

    /* 4 */
    /* fin de travail : test du code rôle obtenu */
    if ((strcmp(p_s7->codro , "  ") == 0) || (strcmp(p_s7->codro , "NV") == 0))
    {
        return (0) ;
    }
    else
    {
        cherche_Erreur(4206,&(p_s7->libelle)) ;
        return (4206) ;
    }
}
