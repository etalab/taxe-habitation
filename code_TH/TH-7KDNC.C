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

/*==================================*/
    /* MODULE DE CALCUL DES COTISATIONS */
    /*       programme th-7KDNC.C       */
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


int NaP_Deg_Tot(s_cons *p_cons,          /*net a payer avec degrevement total */
                 s_dne *p_dne,
                 s_dns *p_dns);

int NaP_Exo( s_dne *p_dne,
             s_dns *p_dns);

int NaP_Plafonnement_1414A_Double(s_cons *p_cons,/* plafonnement 1414a double */
                                  s_dne *p_dneN,
                                  s_dns *p_dnsN);


int NaP_Plafonnement_1414A_Total(s_cons *p_cons,/* plafonnement 1414a total */
                                  s_dne *p_dneN,
                                  s_dns *p_dnsN);


long determination_abat (char*csdi, short npaff, s_cons *p_cons);

void  init_sortie_kdnc(s_dns * p_dns);

    /*======*/
    /* MAIN */
    /*======*/

int th_7kdnc(short antax ,
             s_dne * p_dne ,   /*pointeur sur structure entree*/
             s_dns * p_dns)    /*pointeur sur structure sortie*/
{
    char versr = 'B';
    extern s_signature la_signature ;
    int rets;                 /*code retour signature*/
    int ret_cons;            /* code retour de cherche_const */
    int ret;                 /* code retour sous-fonctions   */
    int retour;
    s_cons *p_cons;          /* pointeur sur struct des constantes */
    p_dns->signature = &la_signature ;


    /* controle de la signature */
    /* ------------------------ */
    rets=controle_signature(RKDNC,versr,&(p_dns->libelle));
    if (rets!=0)
    {
        return(rets);
    }
    /* initialisation des zones de sortie */
    /* ---------------------------------- */
        init_sortie_kdnc(p_dns);

    /* recherche des constantes de l'annee */
    /* ----------------------------------- */
    ret_cons= cherche_const(antax, &(p_cons));
    if (ret_cons != 0)
    {
        cherche_Erreur(ret_cons,&(p_dns->libelle));
        return (1);            /* annee invalide */
    }
    /* Controle des valeurs d entree */
    /* ----------------------------- */

    /* somme a recouvrer de la THP */
    if (p_dne->somrp < 0)
    {
        retour = 1002;
        cherche_Erreur(retour,&(p_dns->libelle));
        return (1002);
    }
    /* somme a recouvrer */
    if (p_dne->somrc < 0)
    {
        retour = 1003;
        cherche_Erreur(retour,&(p_dns->libelle));
        return (1003);
    }
    /* somme a recouvrer < somme a recouvrer de la THP */
    if (p_dne->somrc < p_dne->somrp)
    {
        retour = 1004;
        cherche_Erreur(retour,&(p_dns->libelle));
        return (1004);
    }
    /* revenu imposable */
    if (p_dne->revim < 0)
    {
        retour = 1005;
        cherche_Erreur(retour,&(p_dns->libelle));
        return (1005);
    }
    /* presence d'une vl exoneree non nulle */
    if ((p_dne->vlexo !='O') && (p_dne->vlexo !='N'))
    {
        retour = 1009;
        cherche_Erreur(retour,&(p_dns->libelle));
        return (1009);
    }
    /* revenu imposable du foyer fiscal */
    if (p_dne->revff < 0)
    {
        retour = 1010;
        cherche_Erreur(retour,&(p_dns->libelle));
        return (1010);
    }

    /* reduction theorique negative */
    if (p_dne->redtt < 0)
    {
        retour = 1013;
        cherche_Erreur(retour,&(p_dns->libelle));
        return (1013);
    }



    /*==============================*/
    /* T  R  A  I  T  E  M  E  N  T */
    /*==============================*/

     /* EXONERATION */
     /* ----------- */

    if (est_exoneration(p_dne->degex))
    {
        if (p_dne->somrp > 0)
        {
            retour = 1006;
            cherche_Erreur(retour,&(p_dns->libelle));
            return (1006);
        }
        else
        {
            if ((p_dne->vlexo == 'O')||(p_dne->degex=='D'))
            {
                ret=NaP_Exo(p_dne,p_dns);
                if (ret!=0)
                {
                    return ret;
                }
            }
            else
            {
                ret=NaP_Ni_Deg_Ni_Exo(p_cons,p_dne,p_dns);
                return ret;
            }
        }
    }


     /* DEGREVEMENT */
     /* ----------- */
    else
    {
        if (est_degrevement(p_dne->degex))
        {
            if (est_degrevement_total(p_dne->degex))
            {
                ret=NaP_Deg_Tot(p_cons,p_dne,p_dns);
                if (ret!=0)
                {
                    return ret;
                }
            }
            else
            {
                retour = 1007;
                cherche_Erreur(retour,&(p_dns->libelle));
                return (1007);
            }
        }

    /* PLAFONNEMENT TOTAL a partir de 2009 */
    /* ----------------------------------- */
        else
        {
            if ((Determination_Plaf_Total(p_dne->revff))
                  && (est_autre_allegement(p_dne->degex)))
            {
                ret=NaP_Plafonnement_1414A_Total(p_cons,p_dne,p_dns);
                if (ret!=0)
                {
                    return ret;
                }
            }

    /* PLAFONNEMENT PARTIEL */
    /* -------------------- */

            else
            {
                if (est_autre_allegement(p_dne->degex))
                {
                    if (p_dne->degex == 'H')
                    {
                        ret=NaP_Plafonnement_1414A_Double(p_cons,p_dne,p_dns);
                        if (ret!=0)
                        {
                            return ret;
                        }
                    }
                    else
                    {
                        retour = 1008;
                        cherche_Erreur(retour,&(p_dns->libelle));
                        return (1008);
                    }
                }
    /* AUCUN ALLEGEMENT */
    /* ---------------- */
                else
                {
                switch (p_dne->degex)
                    {
                        case ' ':
                            ret=NaP_Ni_Deg_Ni_Exo(p_cons,p_dne,p_dns);
                            if (ret!=0)
                            {
                                return ret;
                            }
                            break;
                        default:
                            retour = 1008;
                            cherche_Erreur(retour,&(p_dns->libelle));
                            return (1008);
                    }
                }
            }
        }
    }

   /*================*/
   /* FIN TRAITEMENT */
   /*================*/

    return (0);
}



    /* ===================================================== */
    /* fonction de calcul net a payer avec degrevement total */
    /* ===================================================== */

int NaP_Deg_Tot(s_cons *p_cons,      /* pointeur sur constantes de l'annee */
                s_dne *p_dne,        /* pointeur sur donnee entree */
                s_dns *p_dns)        /* pointeur sur donnee sortie */
{
    int ret;                 /* code retour sous-fonctions   */
    if (p_dne->somrp < 0)
    {
        return (1202);
    }
    if (p_dne->somrc < 0)
    {
        return (1203);
    }
    if (p_dne->somrc < p_dne->somrp)
    {
        return (1204);
    }
    if (p_dne->somrc == 0)
    {
        p_dns->redta = 0;
        p_dns->redaa = 0;
        p_dns->redsa = 0;
        p_dns->dgpl3 = 0;
        p_dns->plart = 0;
        p_dns->degpl = 0;
        p_dns->totth = p_dne->somrc;
        p_dns->netth = 0;
        strncpy(p_dns->codro,"  ",3);
    }
    else
    {
        if (p_dne->somrp == 0)
        {
            ret=NaP_Ni_Deg_Ni_Exo(p_cons,p_dne,p_dns);
            if (ret!=0)
            {
                return ret;
            }
        }
        else
        {
            p_dns->redta = 0;
            p_dns->redaa = 0;
            p_dns->redsa = 0;
            p_dns->dgpl3 = 0;
            p_dns->plart = 0;
            p_dns->degpl = p_dne->somrp;
            p_dns->totth = p_dne->somrc - p_dne->somrp;
            if ((p_dns->totth > 0)
             && (p_dns->totth < p_cons->seuil_nap))
            {
                p_dns->netth = 0;
                strncpy(p_dns->codro,"DV",3);
            }
            else
            {
                p_dns->netth = p_dne->somrc - p_dne->somrp;
                strncpy(p_dns->codro,"DT",3);
            }
        }
    }

    return (0);
}
/*-----------------------Fin de la fonction NaP_Deg_Tot-----------------------*/

    /* =============================================== */
    /* fonction de calcul net a payer avec exoneration */
    /* =============================================== */

int NaP_Exo(s_dne *p_dne,    /* pointeur sur donnee entree */
            s_dns *p_dns)    /* pointeur sur donnee sortie */
{
    if (p_dne->somrp != 0)
    {
        return (1302);
    }
    if (p_dne->somrc != 0)
    {
        return (1303);
    }
    if (p_dne->somrc < p_dne->somrp)
    {
        return (1304);
    }

    p_dns->redta = 0;
    p_dns->redaa = 0;
    p_dns->redsa = 0;
    p_dns->dgpl3 = 0;
    p_dns->plart = 0;
    p_dns->degpl = 0;
    p_dns->totth = p_dne->somrc;
    p_dns->netth = p_dns->totth;
    strncpy(p_dns->codro,"EX",3);

    return (0);
}
/*-----------------------Fin de la fonction NaP_Exo---------------------*/

    /* ===================================================== */
    /* fonction de plafonnement 1414a fictif */
    /* ===================================================== */

int Plafonnement_1414A_Fictif(s_cons *p_cons, /* pointeur sur constantes de l'annee */
                              s_pfe *p_pfe, /* pointeur sur donnee entree */
                              s_pfs *p_pfs) /* pointeur sur donnee sortie */
{
    long plafonnement=0;
    long reduc_appliquee=0;

    if (p_pfe->somrc < 0)
    {
        return (2111);
    }
    if (p_pfe->revff < 0)
    {
        return (2112);
    }
    plafonnement = max(0 , p_pfe->somrc - arrondi_euro_voisin(p_pfe->revff * p_cons->coeff_plaf_1414A));
    if (p_pfe->redtt >= p_cons->seuil_reduc)
    {
        reduc_appliquee = min(p_pfe->redtt , plafonnement);
        plafonnement = plafonnement - reduc_appliquee;
    }
    else
    {
        reduc_appliquee = 0;
    }

    p_pfs->redta = reduc_appliquee;
    p_pfs->degpl = plafonnement;

    return (0);
}
/*------------------Fin de la fonction Plafonnement_1414A_Fictif--------------*/


    /* ===================================================== */
    /* fonction de  Net a payer plafonnement 1414a Double    */
    /* ===================================================== */

int NaP_Plafonnement_1414A_Double(s_cons *p_cons, /* pointeur sur constantes de l'annee */
                                  s_dne *p_dneN, /* pointeur sur donnee entree */
                                  s_dns *p_dnsN) /* pointeur sur donnee sortie */
{
    int ret;                 /* code retour sous-fonctions   */
    long reduction_abattement=0;
    long Plaf_Apres_Reduc_Abatt=0;
    long Reduc_Seuil=0;
    long Plafonnement_Retenu=0;
    long Total_TH=0;
    s_pfe pfe2003;          /* donnees de struct dns */
    s_pfe pfeN;
    s_pfs pfs2003;
    s_pfs pfsN;

    if (p_dneN->somrc < 0)
    {
        return (2122);
    }

    if (p_dneN->somr3 < 0)
    {
        return (2123);
    }

    if (p_dneN->revff < 0)
    {
        return (2124);
    }

    if (p_dneN->revim < 0)
    {
        return (2125);
    }

    if (p_dneN->redtt < 0)
    {
        return (2126);
    }

    if (p_dneN->redt3 < 0)
    {
        return (2127);
    }

    if ((p_dneN->cndl == 'N') && (p_dneN->cudl == 'N'))
    {
        pfe2003.revff = p_dneN->revff;
        pfe2003.somrc = p_dneN->somrc;
        pfe2003.redtt = p_dneN->redtt;
    }
    else
    {
        pfe2003.revff = p_dneN->revff;
        pfe2003.somrc = p_dneN->somr3;
        pfe2003.redtt = p_dneN->redt3;
    }
    ret = Plafonnement_1414A_Fictif(p_cons,&pfe2003,&pfs2003);

    if (ret!=0)
    {
        return ret;
    }

    pfeN.revff = p_dneN->revff;
    pfeN.somrc = p_dneN->somrc;
    pfeN.redtt = p_dneN->redtt;

    ret = Plafonnement_1414A_Fictif(p_cons,&pfeN,&pfsN);

    if (ret!=0)
    {
        return ret;
    }

    reduction_abattement = max(0 , pfsN.degpl - pfs2003.degpl);
    Plaf_Apres_Reduc_Abatt = max(0 ,pfsN.degpl - reduction_abattement);

    if (Plaf_Apres_Reduc_Abatt < p_cons->seuil_deg)
    {
        Reduc_Seuil = Plaf_Apres_Reduc_Abatt;
    }
    else
    {
        Reduc_Seuil = 0;
    }

    Plafonnement_Retenu = Plaf_Apres_Reduc_Abatt - Reduc_Seuil;
    Total_TH = p_dneN->somrc - Plafonnement_Retenu;
    if ((p_dneN->cndl == 'N') && (p_dneN->cndl == 'N') && (p_dneN->cndl == 'N'))
    {
        pfs2003.degpl = 0;
    }

    p_dnsN->redta = pfsN.redta;
    p_dnsN->redaa = reduction_abattement;
    p_dnsN->redsa = Reduc_Seuil;
    p_dnsN->plart = pfsN.degpl;
    p_dnsN->degpl = Plafonnement_Retenu;
    p_dnsN->dgpl3 = pfs2003.degpl;
    p_dnsN->totth = Total_TH;

    if (Plafonnement_Retenu == 0)
    {
        ret=NaP_Ni_Deg_Ni_Exo(p_cons,p_dneN,p_dnsN);
        if (ret!=0)
        {
            return ret;
        }
        p_dnsN->redta = pfsN.redta;
        p_dnsN->redaa = reduction_abattement;
        p_dnsN->redsa = Reduc_Seuil;
        p_dnsN->plart = pfsN.degpl;
        p_dnsN->degpl = Plafonnement_Retenu;
        p_dnsN->dgpl3 = pfs2003.degpl;

    }
    else
    {
        p_dnsN->redta = pfsN.redta;
        p_dnsN->redaa = reduction_abattement;
        p_dnsN->redsa = Reduc_Seuil;
        p_dnsN->plart = pfsN.degpl;
        p_dnsN->degpl = Plafonnement_Retenu;
        p_dnsN->dgpl3 = pfs2003.degpl;
        p_dnsN->totth = p_dneN->somrc - Plafonnement_Retenu;
        strncpy(p_dnsN->codro,"PA",3);
        if (p_dnsN->totth < p_cons->seuil_nap)
        {
            p_dnsN->netth = 0;
        }
        else
        {
            p_dnsN->netth = p_dnsN->totth;
        }
    }
    return (0);
}

/*-----------------------Fin de la fonction Net a payer plafonnement 1414a Double----------*/

   /* ===================================================== */
    /* fonction de  Net a payer plafonnement 1414a Total    */
    /* ==================================================== */

int NaP_Plafonnement_1414A_Total(s_cons *p_cons, /* pointeur sur constantes de l'annee */
                                  s_dne *p_dneN, /* pointeur sur donnee entree */
                                  s_dns *p_dnsN) /* pointeur sur donnee sortie */
{
    int ret;                 /* code retour sous-fonctions   */
    long abattement=0;
    long reduction_abattement=0;
    long Plaf_Apres_Reduc_Abatt=0;
    long Reduc_Seuil=0;
    long Total_TH=0;
    float RapportRFR=0;
    long majpc=0;           /*cotisation TH provisoire */
    s_pfe pfe2003;          /* donnees de struct dns */
    s_pfe pfeN;
    s_pfs pfs2003;
    s_pfs pfsN;

    if (p_dneN->somrc < 0)
    {
        return (2122);
    }

    if (p_dneN->somr3 < 0)
    {
        return (2123);
    }

    if (p_dneN->revff < 0)
    {
        return (2124);
    }

    if (p_dneN->revim < 0)
    {
        return (2125);
    }

    if (p_dneN->redtt < 0)
    {
        return (2126);
    }

    if (p_dneN->redt3 < 0)
    {
        return (2127);
    }

    if ((p_dneN->cndl == 'N') && (p_dneN->cudl == 'N'))
    {
        pfe2003.revff = p_dneN->revff;
        pfe2003.somrc = p_dneN->somrc;
        pfe2003.redtt = p_dneN->redtt;
    }
    else
    {
        pfe2003.revff = p_dneN->revff;
        pfe2003.somrc = p_dneN->somr3;
        pfe2003.redtt = p_dneN->redt3;
    }
    ret = Plafonnement_1414A_Fictif(p_cons,&pfe2003,&pfs2003);

    if (ret!=0)
    {
        return ret;
    }

    pfeN.revff = p_dneN->revff;
    pfeN.somrc = p_dneN->somrc;
    pfeN.redtt = p_dneN->redtt;

    ret = Plafonnement_1414A_Fictif(p_cons,&pfeN,&pfsN);

    if (ret!=0)
    {
        return ret;
    }
    reduction_abattement = max(0 , pfsN.degpl - pfs2003.degpl);
    Plaf_Apres_Reduc_Abatt = max(0 ,pfsN.degpl - reduction_abattement);

    abattement=determination_abat(p_dneN->csdi, p_dneN->npaff, p_cons);

    majpc = p_dneN->somrc - Plaf_Apres_Reduc_Abatt;

    if (majpc != (reduction_abattement + pfsN.redta))
    {
        Total_TH = p_dneN->somrc;
        p_dnsN->pmajc = 0;
        p_dnsN->majoc = 0;
    }
    else
    {
        if (abattement!=0)
        {
            RapportRFR = (float)p_dneN->revim / (float)abattement;
        }
        else
        {
            RapportRFR = 0;
        }
        p_dnsN->majoc = max(0 ,arrondi_nieme_decimale_voisine(majpc * (100-arrondi_nieme_decimale_voisine(100*RapportRFR,0))/100,0));

        p_dnsN->pmajc = Plaf_Apres_Reduc_Abatt + p_dnsN->majoc;

        if (p_dnsN->pmajc < p_cons->seuil_deg)
        {
            Total_TH = p_dneN->somrc;
            Reduc_Seuil = p_dnsN->pmajc;
            p_dnsN->pmajc = 0;
        }
        else
        {
            Total_TH = max(0 ,  p_dneN->somrc - p_dnsN->pmajc);
            Reduc_Seuil = 0;
        }
    }


    p_dnsN->redta = pfsN.redta;
    p_dnsN->redaa = reduction_abattement;
    p_dnsN->redsa = Reduc_Seuil;
    p_dnsN->plart = pfsN.degpl;
    p_dnsN->degpl = p_dnsN->pmajc;
    p_dnsN->dgpl3 = pfs2003.degpl;
    p_dnsN->totth = Total_TH;


    if (p_dneN->somrc == 0)
    {
        p_dnsN->redta = 0;
        p_dnsN->redaa = 0;
        p_dnsN->redsa = 0;
        p_dnsN->dgpl3 = 0;
        p_dnsN->plart = 0;
        p_dnsN->degpl = 0;
        p_dnsN->totth = p_dneN->somrc;
        p_dnsN->netth = 0;
        strncpy(p_dnsN->codro,"  ", 3);
    }
    else
    {
        if (p_dnsN->totth  < p_cons->seuil_nap)
        {
            if (p_dnsN->pmajc == 0)
            {
                strncpy(p_dnsN->codro,"NV",3);
            }
            else
            {
                strncpy(p_dnsN->codro,"PA",3);
            }
        }
        else
        {
            if (p_dnsN->pmajc == 0)
            {
                strncpy(p_dnsN->codro,"  ",3);
            }
            else
            {
                strncpy(p_dnsN->codro,"PA",3);
            }
        }
    }

    if ((p_dneN->cndl == 'N') && (p_dneN->cndl == 'N') && (p_dneN->cndl == 'N'))
    {
        pfs2003.degpl = 0;
    }

    if (p_dnsN->totth  < p_cons->seuil_nap)
    {
        p_dnsN->netth = 0;
    }
    else
    {
        p_dnsN->netth = p_dnsN->totth ;
    }

        /* Controle des valeurs en sortie */

    if (p_dnsN->majoc < 0)
    {
        return (2128);
    }
    if (p_dnsN->pmajc < 0)
    {
        return (2129);
    }

    return (0);

}

/*-----------------------Fin de la fonction Net a payer plafonnement 1414a Total----------*/

/*----------------------------------------------------------------------------*/
/*                    Initialisation de la zone de sortie                     */
/*----------------------------------------------------------------------------*/

void  init_sortie_kdnc(s_dns * p_dns)
{
    p_dns->redta = 0;
    p_dns->redaa = 0;
    p_dns->redsa = 0;
    p_dns->plart = 0;
    p_dns->degpl = 0;
    p_dns->dgpl3 = 0;
    p_dns->totth = 0;
    p_dns->netth = 0;
    p_dns->majoc = 0;
    p_dns->pmajc = 0;
   strncpy(p_dns->codro, "  ", 3);
}

