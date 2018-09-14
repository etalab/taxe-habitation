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

#define LA_SIGNATURE
#if defined (ILIAD)
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif

/*-------------------------------------*/
/* MODULE DE CALCUL DE LA CONTRIBUTION */
/*        AUDIOVISUELLE PUBLIQUE       */
/*-------------------------------------*/

int controle_krac (s_rae * p_rae) ;

/*============================================================================
   M A I N
  ============================================================================*/
int th_7krac ( s_rae * p_rae, s_ras * p_ras)
{
    extern s_signature la_signature ;
    int rets;                 /*code retour signature*/
    int ret;                  /*code retour contrôle_krac*/
    int ret_const;

    int retp=0;             /* code retour du programme th-krac*/
    long cotis;
    long frais;
    char chaine[2]={' ','\0'};
    s_cons * p_cons;
    char dep97[]="97";
    int retour=0;

    /* TRAITEMENT */
    /* ---------- */
    p_ras->versr = 'B';
    p_ras->signature = &la_signature ;

    /* controle signature */
    rets=controle_signature(RKRAC,p_ras->versr,&(p_ras->libelle));

    if (rets!=0)
    {
        return(rets);
    }

    /* controle des zones d'entree */

    ret=controle_krac(p_rae);
    if (ret != 0)
    {
        cherche_Erreur(ret,&(p_ras->libelle));
        return(ret);
    }
    ret_const=cherche_const(p_rae->antax,&p_cons);
    if (ret_const != 0)
    {
        cherche_Erreur(ret_const,&(p_ras->libelle)); /* mvo*/
        return (ret_const);
    }

    /*recuperation des constantes */
    /*----------------------------*/

    retour=strcmp(p_rae->dep,dep97);
    if (retour!=0)

    /* recuperation des constantes pour la metropole */
    {
        cotis=p_cons->cotis_redev_metro;
        frais=p_cons->frais_redev_metro;
    }
    else    /* recuperation des constantes pour les DOM */
    {
        cotis=p_cons->cotis_redev_dom;
        frais=p_cons->frais_redev_dom;
    }

    chaine[0] = p_rae->qtvrt;
    strncpy(p_ras->roltv,chaine, 4);    /* code role TV */
    chaine[0] = p_rae->degtv;
    strcat(p_ras->roltv,chaine);     /*code role tv */
    if (p_rae->antax < 2016) {
        chaine[0] = ' ';
    }
    else{
        chaine[0] = p_rae->rgsor;
    }
    strcat(p_ras->roltv,chaine);

    if ((p_rae->qtvrt == '0') && (p_rae->champ != 'C'))
    {
        strncpy(p_ras->roltv,"0N ", 4);
    }
    p_ras->cottv=0;
    p_ras->fratv=0;
    p_ras->somtv=0;
    p_ras->mdgtv=0;
    p_ras->nettv=0;
    if ((p_rae->champ=='C')&&(p_rae->qtvrt == '1'))
    {
        p_ras->cottv=cotis;
        p_ras->fratv=frais;
        p_ras->somtv = cotis + frais;
    }

    if (p_rae->degtv == ' ')
    {
        p_ras->nettv = p_ras->somtv;
    }
    else
    {
        p_ras->mdgtv = p_ras->somtv;
    }
    return(retp);
}


    /* controle entree */

int controle_krac(s_rae * p_rae)
{
    if ((strcmp(p_rae->dep,"01")<0) ||
        (strchr("0123456789",p_rae->dep[0])==NULL) ||
       ((strchr("0123456789AB",p_rae->dep[1])==NULL)&&
        (strchr("2",p_rae->dep[0])!=NULL)) ||
       ((strchr("0123456789",p_rae->dep[1])==NULL)&&
        (strchr("2",p_rae->dep[0])==NULL)))
    {
        return (2502);
    }

    if (strchr("CDEIMRST",p_rae->champ)==NULL)
    {
        return (2503);
    }

    if (strchr("01",p_rae->qtvrt) == NULL)
    {
        return (2504);
    }

    if (p_rae->antax == 2014)
    {
        if (strchr("EJFIADVSZW ",p_rae->degtv) == NULL)
        {
            return (2505);
        }
    }
    else
    {
        if (strchr("EJFIADVSZWK ",p_rae->degtv) == NULL)
        {
            return (2505);
        }
    }

    if (p_rae->champ != 'C' && p_rae->qtvrt == '1')
    {
        return (2506);
    }

    if (p_rae->champ != 'C' && p_rae->degtv != ' ')
    {
        return (2507);
    }
    else
    {
        return (0);
    }
}
