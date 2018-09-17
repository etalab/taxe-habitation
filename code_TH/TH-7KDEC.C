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

/*===========================================*/
    /* MODULE DE CALCUL DEG/PLAF DES DEPENDANCES */
    /*===========================================*/

#define LA_SIGNATURE
#if defined (ILIAD)
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif


int controle_kdec( s_e5 * p_e5);
long determination_abat (char*csdi, short npaff, s_cons *p_cons);

void init_dns(s_dns *);

/*============================================================================
   M A I N
  ============================================================================*/

int th_7kdec(s_e5 * p_e5, s_s5 * p_s5)
{
    int ret_cons;  /* zone retour de recherche des constantes */
    int ret;  /* zone retour strcmp */
    int rets;
    extern s_signature la_signature ;
    long redthnet=0;
    long thnet=0;
    long plafcouavreduc=0;
    float RapportRFR=0;
    long majpc=0;           /*cotisation TH provisoire */
    long abattement=0;      /*abattemenent 1414A*/

    static s_cons * p_cons=NULL;

    static s_dns dns_glob;
    static s_dne dne_glob;

    s_dne *p_dne;
    s_dns *p_ds5;
    s_dns *p_dns;
    p_dne=&dne_glob;
    p_dns=&dns_glob;
    p_ds5=&(p_s5->dns_5);

    p_ds5->signature = &la_signature ;

    /* IDENTIFICATION DU MODULE */
    /* ------------------------ */
    p_s5->anref = 2017;    /* emission année de référence */
    p_s5->versr = 'B';     /* lettre de version  */
    p_s5->signe = ' ';     /* signe delta nap */
    p_s5->napdt = 0;       /* delta nap */


    /* TRAITEMENT */
    /* ---------- */
    rets=controle_signature(RKDEC,p_s5->versr,&(p_ds5->libelle));
    if (rets!=0)
    {
        return(rets);
    }

    /* appel de la fonction taux annuel*/
    ret_cons=(short)cherche_const(p_e5->antax,&p_cons);
    if (ret_cons != 0)  /* retour de la fonction*/
    {
        cherche_Erreur(ret_cons,&(p_ds5->libelle));
        return (1); /*l'année de taxation est incorrect*/
    }

    ret=controle_kdec(p_e5);
    if (ret !=0 )
    {
        cherche_Erreur(ret,&(p_ds5->libelle)) ;
        return (ret);
    }

    init_dns(&(p_s5->dns_5));
    p_s5->napdt=0;
    p_s5->signe=' ';

    dne_glob.degex=p_e5->degex;
    dne_glob.revim=0;
    dne_glob.revff=p_e5->revff;
    dne_glob.somrc=(p_e5->somrp + p_e5->sdrpa);
    dne_glob.somrp=(p_e5->somrp + p_e5->sdrpa);
    dne_glob.somr3=0;
    dne_glob.redtt=(p_e5->redtc + p_e5->redat);
    dne_glob.redt3=0;
    dne_glob.vlexo='N';
    dne_glob.antax=p_e5->antax;
    dne_glob.cndl='N';
    dne_glob.cudl='N';
    dne_glob.npaff=p_e5->npaff;
    strncpy(dne_glob.csdi, p_e5->csdi, 4);

    ret=th_7kdnc(p_e5->antax,&dne_glob,&dns_glob);
    if (ret!=0)
    {
        cherche_Erreur(ret,&(p_ds5->libelle));
        return (ret);
    }
    redthnet=max(0, dns_glob.redta - p_e5->redtr);
    thnet=max(0, dns_glob.plart - p_e5->mdega);
    /* Rajout du 14/9 */
    redthnet=min(p_e5->somrp , redthnet);
    /* Fin Rajout du14/9 */

    /* Modification impact H1 sur H2 fiche 44016 */
    if (dns_glob.pmajc > 0)
    {
        dns_glob.degpl = dns_glob.pmajc - dns_glob.majoc;
    }
    /* Fin modification impact H1 sur H2 fiche 44016 */

    plafcouavreduc = max(0, dns_glob.degpl +
                            dns_glob.redta +
                            dns_glob.redsa -
                            p_e5->mdega);

    p_ds5->redta=min(plafcouavreduc, redthnet);
    p_ds5->plart=min((p_e5->somrp - p_ds5->redta) , thnet);

    if (p_ds5->plart < p_cons->seuil_deg)
    {
        p_ds5->redsa=p_ds5->plart;
    }
    else
    {
        p_ds5->redsa=0;
    }

    p_ds5->degpl=max(0, p_ds5->plart - p_ds5->redsa);

    if ( (Determination_Plaf_Total(p_e5->revff))
      && (est_autre_allegement(p_e5->degex)))
    {
        majpc = p_e5->somrp - p_ds5->degpl;
        abattement=determination_abat(p_e5->csdi, p_e5->npaff, p_cons);
        if (abattement!=0)
        {
            RapportRFR = (float)p_e5->revim / (float)abattement;
        }
        else
        {
            RapportRFR = 0;
        }
        p_ds5->majoc = max(0 ,arrondi_nieme_decimale_voisine(majpc * (100-arrondi_nieme_decimale_voisine(100*RapportRFR,0))/100,0));
        p_ds5->pmajc = p_ds5->degpl + p_ds5->majoc;

        if (p_ds5->pmajc < p_cons->seuil_deg)
        {
            p_ds5->redsa = p_ds5->pmajc;
            p_ds5->pmajc = 0;
        }
        p_ds5->degpl = p_ds5->pmajc;
        p_ds5->totth = max(0,p_e5->somrp - p_ds5->pmajc);
    }
    else
    {
        p_ds5->totth = (p_e5->somrp - p_ds5->degpl);
        p_ds5->majoc = 0;
        p_ds5->pmajc = 0;
    }

    if ((p_ds5->totth > 0)&&(p_ds5->totth < p_cons->seuil_nap))
    {
        p_ds5->netth=0;
        if (p_ds5->degpl==0)
        {
            strncpy(p_ds5->codro,"NV", 3);
        }
        else
        {
            strncpy(p_ds5->codro,"PA",3);
        }
    }
    else
    {
        p_ds5->netth=p_ds5->totth;
        if (p_ds5->degpl > 0)
        {
            strncpy(p_ds5->codro,"PA",3);
        }
        else
        {
            strncpy(p_ds5->codro,"  ",3);
        }
    }

    if (p_e5->regul=='R')
    {
        regularisation(p_ds5->netth,p_e5->napin,&(p_s5->napdt),&(p_s5->signe));
    }

    /*     SORTIE DU MODULE     */

    if (p_ds5->degpl < 0)
    {
        ret=5030;
        cherche_Erreur(ret,&(p_ds5->libelle));
        return (ret);
    }

    if (p_ds5->totth < 0)
    {
        ret=5031;
        cherche_Erreur(ret,&(p_ds5->libelle));
        return (ret);
    }

    if (p_ds5->netth < 0)
    {
        ret=5035;
        cherche_Erreur(ret,&(p_ds5->libelle));
        return (ret);
    }

    if (p_ds5->redta < 0)
    {
        ret=(5034);
    }

    if (p_s5->napdt < 0)
    {
        ret=(5042);
    }

    if ((p_s5->signe!='+')&&(p_s5->signe!='-')
    &&  (p_s5->signe!='=')&&(p_s5->signe!=' '))
    {
        ret=(5043);
    }

    if ((p_s5->signe!=' ')&&(p_s5->signe!='=')&&(p_s5->napdt==0))
    {
        ret=(5044);
    }

    if (((p_s5->signe==' ')||(p_s5->signe=='='))&&(p_s5->napdt > 0))
    {
        ret=(5045);
    }

    if (p_ds5->redaa!=0)
    {
        ret=(5046);
    }

    if (p_ds5->redsa < 0)
    {
        ret=(5047);
    }

    if (p_ds5->majoc < 0)
    {
        ret= (5128);
    }
    if (p_ds5->pmajc < 0)
    {
        ret= (5129);
    }

    cherche_Erreur(ret,&(p_ds5->libelle));
    return (ret);
}



/*============================================================================
   CONTROLES GENERAUX
  ============================================================================*/

int controle_kdec(s_e5 * p_e5)
{
float part;

    /* Controle code CSDI/DSF */
    if (!recherche_csdi(p_e5->csdi))
    {
        return (5);
    }

    /*  appel a la fonction est_autre_allegement  */
    if (est_autre_allegement(p_e5->degex))
    {
    }
    else
    {
        return (5002);
    }

    /*  verif SOMRP  */

    if (p_e5->somrp < 0)
    {
        return (5004);
    }

    /*Controle du nombre de part IR   */
    if (p_e5->npaff<0)
    {
        return (5015);
    }

    part=(((float)p_e5->npaff*4)/10);

    if (part!=arrondi_dizaine_inf(part))
    {
        return (5014);
    }

    /*  verif revff  */

    if (p_e5->revff < 0)
    {
        return (5025);
    }

    /*  verif sdrpa */
    if (p_e5->sdrpa < 0)
    {
        return (5026);
    }

    /*  verif mdega */

    if (p_e5->mdega < 0)
    {
        return (5027);
    }

    /*  verif mdega < sdrpa */

    if (p_e5->sdrpa < p_e5->mdega)
    {
        return (5028);
    }

    /*  verif redtc */

    if (p_e5->redtc < 0)
    {
        return (5036);
    }

    /*  verif redat */

    if (p_e5->redat < 0)
    {
        return (5038);
    }

    if (p_e5->redar < 0)
    {
        return (5033);
    }

    if ((p_e5->regul!='R')&&(p_e5->regul!=' '))
    {
        return (5039);
    }

    if (p_e5->napin < 0)
    {
        return (5040);
    }

    if ((p_e5->regul==' ')&&(p_e5->napin > 0))
    {
        return (5041);
    }

    /* tous les controles sont corrects */
    return (0);
}

/*------------------------ fin des controles generaux ---------------------*/

/*---------------------------------------------------------------------------*/
/* fonction d'initialisation des zones sortie                                */
/*---------------------------------------------------------------------------*/

void init_dns(s_dns *p)
{
    p->redta=0;
    p->redaa=0;
    p->redsa=0;
    p->plart=0;
    p->dgpl3=0;
    p->degpl=0;
    p->totth=0;
    p->netth=0;
    p->pmajc=0;
    p->majoc=0;
    strncpy(p->codro,"  ",3);
}
