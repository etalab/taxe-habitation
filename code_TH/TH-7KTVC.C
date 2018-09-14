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

/*======================================*= */
/*MODULE D ACTUALISATION DES VL (th-7KTVC) */
/*======================================*= */

#define LA_SIGNATURE

#if defined (ILIAD)
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif


    /*    MAIN    */
    /*   ------   */

int th_7ktvc(s_e6 * p_e6, s_s6 * p_s6)
{
    extern s_signature la_signature;

    int ret;
    int retvlc=0;       /*code retour th_nkvlc */
    int retdepv=0;      /*code retour recherche_depv */
    int retcons=0;      /*code retour cherche_const  */
    short dif=0;        /*difference entre les 2 annees*/
    double txtlv;
    s_cons *p_cons;     /*pointeur sur struct des taux */
    s_e1   *p_e1;       /*pointeur sur struct s_e1     */
    s_s1   *p_s1;       /*pointeur sur struct s_s1     */
    s_e9   *p_e9;       /*pointeur sur struct s_e9     */
    s_s1   s1;
    s_e9   e9;
    p_s1=&s1;
    p_e9=&e9;

    /*    identification du module   */
    /*    ------------------------   */

    p_s6->anref=2017;   /*   annee de reference  */
    p_s6->versr='B';    /*   version du module   */



    /*    traitement   */
    /*    ----------   */

    /* controle de la signature */
    p_s6->signature = &la_signature ;

    ret = controle_signature(RKTVC,p_s6->versr, &(p_s6->libelle));
    if (ret != 0 )
    {
        return (ret);
    }

    /*   initialisation des zones entrees   */
    /*   --------------------------------   */

    strncpy(p_e6->e1_l.cxloz,"  ", 3);
    p_e6->e1_l.txzrr = 0;

    p_e9->antax = p_e6->e1_l.antax;   /*annee de taxation*/
    strncpy(p_e9->dep, p_e6->e1_l.dep, 3);
    strncpy(p_e9->cne, p_e6->e1_l.cne, 4);
    strncpy(p_e9->cxcn1, "  ", 3);
    strncpy(p_e9->cxcn2, "  ", 3);
    strncpy(p_e9->cxcn3, "  ", 3);

    /*   initialisation des zones sorties   */
    /*   --------------------------------   */

    p_s1->vlbll=0;
    p_s1->vlblc=0;
    p_s1->vlblt=0;
    p_s6->txtlv='0';
    p_s6->vlbll=0;
    p_s6->vlblc=0;
    p_s6->vlblt=0;

    if (p_e6->atlvc>p_e6->e1_l.antax)
    {
        ret=4104;
        cherche_Erreur(ret,&(p_s6->libelle));
        return(4104);
    }
    dif=p_e6->e1_l.antax-p_e6->atlvl;
    if (dif<1)
    {
        ret=4105;
        cherche_Erreur(ret,&(p_s6->libelle));
        return(4105);
    }

    p_e6->e1_l.aff='H';
    /* appel de la fonction recherche_depv. */

    retdepv=recherche_dep_TLV(p_e6->e1_l.dep);



    if (retdepv==0)        /*  retour de la fonction  */
    {
        ret=4106;
        cherche_Erreur(ret,&(p_s6->libelle));
        return (4106);       /*  departement non concerne  */
    }
    /*=====================================================*/
    /*           Appel de la fonction th_nkvlc             */
    /*=====================================================*/

    p_e1=&(p_e6->e1_l);
    retvlc=th_7kvlc(p_e1, p_e9, p_s1);  /* appel de la fonction th_nkvlc */

    if (retvlc!=0) /*  retour du module th_nkvlc  */
    {
        ret=retvlc;
        cherche_Erreur(ret,&(p_s6->libelle));
        return(ret);  /*  anomalie  */
    }

   /*=====================================================*/
    /*                  Traitement de la TLV               */
    /*=====================================================*/

    p_s6->vlbll=p_s1->vlbll;
    p_s6->vlblc=p_s1->vlblc;
    p_s6->vlblt=p_s1->vlblt;

    retcons=cherche_const(p_e6->e1_l.antax, &p_cons); /*appel fonction taux TLV*/
    if (retcons==1)
    {
        ret=retcons;
        cherche_Erreur(ret,&(p_s6->libelle));
        return(ret);      /*annee non valide*/
    }

    if (p_e6->e1_l.antax==p_e6->atlvc) /* Commune dans le champ TLV pour la 1ère fois */
    {
        txtlv=p_cons->taux_TLV_1 * 1000;
    }
    /* Commune dans le champ TLV depuis au moins un an */

        if (p_e6->e1_l.antax>=p_e6->atlvc + 1)
        {
            if (p_e6->atlvl==p_e6->e1_l.antax - 1)
            {
                txtlv=p_cons->taux_TLV_1 * 1000;
            }
            if (p_e6->atlvl<p_e6->e1_l.antax - 1)
            {
                txtlv=p_cons->taux_TLV_2 * 1000;
            }
        }
    p_s6->txtlv=arrondi_euro_voisin(txtlv);

    return (0);  /* retour correct */
}
    /*   fin de traitement   */
    /*   -----------------   */
