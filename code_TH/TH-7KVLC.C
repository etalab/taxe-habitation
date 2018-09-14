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

/*============================================================================*/
/*                  MODULE D ACTUALISATION DES VL (th-7KVLC)                  */
/*============================================================================*/
#define LA_SIGNATURE

#if defined (ILIAD)
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif




    /*   PROTOTYPAGE DES FONCTIONS   */
    /*   -------------------------   */


long calcul_vl(long, double, double); /*calcul vl actualisee*/

    /*    MAIN    */
    /*   ------   */
int th_7kvlc( s_e1 * p_e1, s_e9 * p_e9,  s_s1 * p_s1)
{
    extern s_signature la_signature ;
    int ret;
    int retm;
    int retdep=0;
    int retde=0;
    int retcne=0;
    int retan=0;
    int retcons=0;
    int rets;   /*zone retour signature  */
    s_cons * p_const1; /* pointeur sur struct du taux annuel */
    double taux;    /*   retour du taux departemental */
    double taux_an; /*   retour du taux annuel */
    double val_1; /*valeur locative exoneree zrr */
    static char dep2[]="97";
    char * p_dep2;
    char * p_dep1;
    int ret_cons;
    p_dep2=dep2;   /* initial. pointeur sur char */
    p_dep1=p_e1->dep; /* initial pointeur sur char */
    p_s1->vlblt = 0;
    p_s1->vlxm1 = 0;
    p_s1->vlxm2 = 0;
    p_s1->vlxm3 = 0;

    /* Fin initialisation */


    /*    identification du module    */
    /*    ------------------------    */

    p_s1->anref=2017;   /*   annee de reference  */
    p_s1->versr='B';    /*   version du module   */
    p_s1->signature = &la_signature ;



    /*    traitement    */
    /*    ----------    */

    /* controle de la signature */

    rets=controle_signature(RKVLC,p_s1->versr,&(p_s1->libelle));

    if (rets!=0)
    {
        return(rets);
    }
    /* Controle des variables en entree */

    if (p_e1->antax != p_e9->antax)
    {
        retan = 4402;
        cherche_Erreur(retan,&(p_s1->libelle));
        return(4402);
    }

    if (strcmp(p_e1->dep,p_e9->dep)!= 0)
    {
        retde = 4404;
        cherche_Erreur(retde,&(p_s1->libelle));
        return (4404);
    }

    if (strcmp(p_e1->cne,p_e9->cne)!= 0)
    {
        retcne = 4403;
        cherche_Erreur(retcne,&(p_s1->libelle));
        return (4403);
    }
    /*   Fin controle   */

    taux=recherche_tdep (p_e1->dep, p_e1->cne,p_e1->aff); /* appel de la fonction taux dep. */

    if (taux==9)        /*  retour de la fonction  */
    {
        retdep = 2;
        cherche_Erreur(retdep,&(p_s1->libelle));
        return (2);       /*  departement absent  */
    }
    else
    {
        if (taux==4401)
        {
            retdep = 4401;
            cherche_Erreur(retdep,&(p_s1->libelle));
            return (4401);
        }
        else
        {
            ret_cons=cherche_const(p_e1->antax,&p_const1); /* appel de la fonction taux annuel */
            if (ret_cons==1) /* retour de la fonction */
            {
                retcons = 1;
                cherche_Erreur(retcons,&(p_s1->libelle));
                return(1);  /*  annee  non valide   */
            }
            ret=strcmp(p_dep1,p_dep2); /* test si DOM */
            if (ret==0)
            {
                retm=(strcmp(p_e1->csdi,"976")); /* test Direction = Mayotte */
                if (retm == 0)
                {
                    taux_an=p_const1->reval_mayotte; /* taux revalorisation Mayotte */
                }
                else
                {
                    taux_an=p_const1->reval_dom; /* taux revalorisation DOM */
                }

            }
            else
            {
                taux_an=p_const1->reval_metro; /* taux revalorisation METRO */
            }
    /* appel fonction calcul vl du local*/
            p_s1->vlbll = calcul_vl( p_e1->vlb7l, taux, taux_an);

    /* appel fonction calcul vl des loc. communs */
            p_s1->vlblc = calcul_vl( p_e1->vlb7c, taux, taux_an);

    /* cumul des vl actualisees */
            p_s1->vlblt = p_s1->vlbll+p_s1->vlblc;

    /* calcul des valeurs locatives exonerees ZRR */
            if ((!strcmp(p_e9->cxcn1,"M1")) && (!strcmp(p_e1->cxloz,"M1")))
            {
                val_1 = (double) p_s1->vlblt * p_e1->txzrr / 100;
                p_s1->vlxm1 = arrondi_euro_voisin(val_1);
            }
            if ((!strcmp(p_e9->cxcn2, "M2")) && (!strcmp(p_e1->cxloz, "M2")))
            {
                val_1 = (double) p_s1->vlblt * p_e1->txzrr / 100;
                p_s1->vlxm2 = arrondi_euro_voisin(val_1);
            }

            /* A partir de 2017, le M3 n'existe plus */

            if (p_e1->antax < 2017)
            {
                if ((!strcmp(p_e9->cxcn3, "M3")) && (!strcmp(p_e1->cxloz, "M3")))
                {
                    val_1 = (double) p_s1->vlblt * p_e1->txzrr / 100;
                    p_s1->vlxm3 = arrondi_euro_voisin(val_1);
                }
            }
            else
            {
                p_s1->vlxm3 = 0;
            }
            p_s1->vlblt = p_s1->vlblt - p_s1->vlxm1 - p_s1->vlxm2 - p_s1->vlxm3;
            return (0);  /* calcul effectue correctement */
        }
    }
}
    /*   fin de traitement   */
    /*   -----------------   */

    /*     CALCUL DES VL   */
    /*     -------------   */

long calcul_vl( long vl,  double taux_d, double taux_a)
{
    double fvl;
    long vl_cal;
    fvl = (double)vl * taux_d; /* vl multipliee par taux departemental */
    vl_cal =arrondi_euro_voisin(fvl); /* arrondi */

    fvl = (double)vl_cal * taux_a; /* vl multipliee par taux annuel */
    vl_cal=arrondi_euro_voisin(fvl); /* arrondi */
    return(vl_cal); /* retour de la vl calculee */
}
