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

/*=================================
              MODULE GENERAL DE CALCUL DE LA th-7KCAC
             =================================== */
#define LA_SIGNATURE

#if defined (ILIAD)
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif


/* La fonction th_7kcac calcule pour une cote les differentes composantes
de la taxation :
    - les bases nettes d'imposition
    - les cotisations par collectivites et les divers frais et prelevements
    - les allegements net a payer et code role resultants

Pour ILIAD, le fichier de definition des structures th-nksts.h devra etre
inclus avant l'appel de la fonction.
Ce fichier reserve
    - une donnee e2 de structure s_e2 pour les entrees de la cote
    - une donnee s2 de structure s_s2 pour les sorties de la cote
    - une donnee e3 de structure s_e3 pour les entrees des donnees
          collectivite (taux, abattements, vl moyenne).



    Avant l'appel de la fonction, tous les champs de la donnee e3
devront avoir ete servis. La fonction ne modifie pas les donnees entree.
Aussi est-il possible de ne modifier le contenu des donnees de e3 que
lorsque l'on change de collectivite. Les champs non utilises doivent etre
reinitialises ( a 0, blanc ou vide)avant l'appel.

    Avant l'appel de la fonction il faudra avoir servi de maniere exhaustive
les differents champs de la variable d'entree e2. La fonction ne modifie pas
les zones d'entree. Tous les champs, y compris ceux non utilises
devront recevoir une valeur adequate.
L'affectation se fait par exemple par : e2.tax='P'

    Apres l'appel de la fonction, si le code retour est egal a 0, les
donnees sortie se recuperent par une simple affectation :
code_role=s2.codro
Si le code retour est different de 0 le contenu des donnees sortie est
imprevisible, a l'exception du libelle d'erreur qui est normalement renseigne.

*/


void  initialisation_sortie(s_s2 * p_s2);
void  initialisation_sortie_col1(s_cols * p_cols);


int controles(s_e2 * p_e2, s_e3 * p_e3, s_s2 * p_s2);


/*============================================================================
   Main
  ============================================================================*/
int th_7kcac(s_e3 * p_e3, s_e2 * p_e2, s_s2 * p_s2)
{
    extern s_signature la_signature;
    int ret_cons;  /* zone retour de recherche des constantes */
    int ret;  /* zone retour strcmp */
    static s_cons * p_const1=NULL;

    static s_bne1 bne1, * p_bne1=&bne1;
    static s_bne2 bne2, * p_bne2=&bne2;
    static s_bns  bns , * p_bns =&bns;
    static s_coe1 coe1, * p_coe1=&coe1;
    static s_coe2 coe2;
    static s_cos  cos , * p_cos =&cos;
    static s_dne dne;
    static s_dns dns;
    static s_ree ree, * p_ree=&ree;
    static s_res res, * p_res=&res;
    static s_e8 e8, * p_e8=&e8;
    static s_s8 s8, * p_s8=&s8;
    static s_rae rae, * p_rae=&rae;
    static s_ras ras, * p_ras=&ras;
    long napdt;
    long svsmrc;
    long svrdtt = 0;
    long svsmrp;
    char signe;


    /* IDENTIFICATION DU MODULE */
    /* ------------------------ */
    p_s2->anref = 2017;    /* emission année de référence */
    p_s2->versr = 'D';     /* lettre de version  */


    /* controle de la signature */
    /* ------------------------ */

    p_s2->signature = &la_signature ;
    ret = controle_signature(RKCAC,p_s2->versr, &(p_s2->libelle));
    if (ret != 0 )
    {
        return (ret);
    }

    /* ajout des espaces pour les abattements forfaitaires */
    ajout_espace_taux_abattement(&(p_e3->cole_c)); /* structure commune                                        */
    ajout_espace_taux_abattement(&(p_e3->cole_q)); /* structure intercommunalité                               */
    ajout_espace_taux_abattement(&(p_e3->cole_d)); /* structure département                                    */
    ajout_espace_taux_abattement(&(p_e3->cole_o)); /* structure commune fictive 2003                           */
    ajout_espace_taux_abattement(&(p_e3->cole_i)); /* structure intercommunalité fictive 2003                  */
    ajout_espace_taux_abattement(&(p_e3->cole_p)); /* structure departement fictif 2003                        */
    ajout_espace_taux_abattement(&(p_e3->cole_f)); /* structure commune avant ajustement                       */
    ajout_espace_taux_abattement(&(p_e3->cole_j)); /* structure commune avant ajustement fictive 2003          */
    ajout_espace_taux_abattement(&(p_e3->cole_r)); /* structure intercommunalite avant ajustement              */
    ajout_espace_taux_abattement(&(p_e3->cole_m)); /* structure intercommunalite avant ajustement fictive 2003 */

    /* Commune de St Martin*/
    /* Modif AMN le 05/09/2008 fiche 34047 forcage a 0 */
    if (!strcmp(p_e3->cne,"127") && !strcmp(p_e3->dep,"97"))
    {
        p_e3->timpd=0;
        p_s2->cols_d.bnimp=0;
    }

    /* Suppression de la part departementale*/
    /* taux d'imposition departemental est valorise a zero depuis 2011 */
    p_e3->timpd=0;

    /* Controle code CSDI/DSF */

    if(!recherche_csdi(p_e2->csdi))
    {
        cherche_Erreur(5,&(p_s2->libelle));
        return 5;
    }

    /* Controle de l'indicateur de synthese THLV */

    if ( p_e2->tyimp=='T' ) {

            if ( !(p_e3->indthlv =='C' || p_e3->indthlv == 'I' ) ) {
                cherche_Erreur(62,&(p_s2->libelle));
                return 62;
            }
    }

    /* T  R  A  I  T  E  M  E  N  T */
    /* ============================ */
    /*        ==============        */

    initialisation_sortie(p_s2);

    ret_cons=(short)cherche_const(p_e3->antax,&p_const1); /* appel de la fonc.taux ann*/
    cherche_Erreur(ret_cons,&(p_s2->libelle)) ;
    if (ret_cons != 0)  /* retour de la fonction*/
    {
        return (ret_cons);
    }

    ret=controles(p_e2,p_e3,p_s2);
    if (ret !=0)
    {
        cherche_Erreur(ret,&(p_s2->libelle));
        return ret;
    }

    /* Determination des VL brutes (imposee et non imposee */
    /* dans le cadre de l'abattement K3                    */

    if (p_e2->tyimp == 'T')

    /* Traitement de la THLV */
    {
        p_s2->vlbni = 0;
        p_s2->vlbri = p_e2->vlbpc;
    }

    else

    {

    /* Traitement de la TH */

        if ((p_e2->tax == 'P') || (p_e2->tax == 'E')){   /* Cas des articles THP ou THE */

            if ((p_e2->abatk == 'K') && (p_e2->rgsork == '3'))
            {
                double test = p_e2->vlbpc ;
                p_s2->vlbni = arrondi_euro_voisin((test * 2) / 3);
                p_s2->vlbri = (p_e2->vlbpc - p_s2->vlbni);
            }
            else
            {
                p_s2->vlbni = 0;
                p_s2->vlbri = p_e2->vlbpc;
            }
        }
        else if (p_e2->tax == 'S')
            {
                p_s2->vlbni = 0;
                p_s2->vlbri = p_e2->vlbsc;
            }
    }

    /* APPEL DU MODULE KTAC */
    /* ==================== */
    /*       =======        */
    /*         ==           */

    {
        p_e8->antax = p_e3->antax;
        strncpy(p_e8->dep, p_e3->dep,3);
        p_e8->cocnq = p_e3->cocnq;
        p_e8->tisyn = p_e3->tisyn;
        p_e8->titsn = p_e3->titsn;
        p_e8->titgp = p_e3->titgp;
        p_e8->abmos = p_e3->abmos;
        p_e8->codef = p_e3->codef;
        p_e8->codeg = p_e3->codeg;
        p_e8->vmd89 = p_e3->vmd89;
        p_e8->cneg3 = p_e3->cneg3;
        p_e8->cnqg3 = p_e3->cnqg3;
        p_e8->cnam3 = p_e3->cnam3;
        p_e8->timpe = p_e3->timpe;
        p_e8->indgem = p_e3->indgem;
        p_e8->cole_c = p_e3->cole_c;
        p_e8->cole_q = p_e3->cole_q;
        p_e8->cole_d = p_e3->cole_d;
        p_e8->cole_o = p_e3->cole_o;
        p_e8->cole_i = p_e3->cole_i;
        p_e8->cole_p = p_e3->cole_p;
        p_e8->cole_j = p_e3->cole_j;
        p_e8->cole_f = p_e3->cole_f;
        p_e8->cole_r = p_e3->cole_r;
        p_e8->cole_m = p_e3->cole_m;
    }

    ret = th_7ktac( p_e8,p_s8);
    if (ret != 0)
    {
        cherche_Erreur(ret,&(p_s2->libelle)) ;
        return (ret);
    }

    {
        p_s2->cndl = p_s8->cndl;
        p_s2->cudl = p_s8->cudl;
    }


    /*     APPEL DU MODULE KBNC   */

    p_bne1->antax  = p_e2->antax;
    p_bne1->tyimp  = p_e2->tyimp;
    p_bne1->vlbpc  = p_e2->vlbpc;
    p_bne1->vlbsc  = p_e2->vlbsc;
    p_bne1->vlbni  = p_s2->vlbni;
    p_bne1->vlbri  = p_s2->vlbri;
    p_bne1->tax    = p_e2->tax;
    p_bne1->nbpac  = p_e2->nbpac;
    p_bne1->nbera  = p_e2->nbera;
    p_bne1->degex  = p_e2->degex;
    p_bne1->codni  = p_e2->codni;
    p_bne1->codsh  = p_e2->codsh;
    p_bne1->aff    = p_e2->aff;
    p_bne1->permo  = p_e2->permo;
    p_bne1->grrev  = p_e2->grrev;
    p_bne2->colb_c = p_s8->colb_c;
    p_bne2->colb_s = p_s8->colb_s;
    p_bne2->tisyn  = p_e3->tisyn;
    p_bne2->colb_q = p_s8->colb_q;
    p_bne2->cocnq  = p_e3->cocnq;
    p_bne2->colb_d = p_s8->colb_d;
    p_bne2->titsn  = p_e3->titsn;
    p_bne2->titgp  = p_e3->titgp;
    p_bne2->timpe  = p_e3->timpe;
    p_bne2->colb_n = p_s8->colb_n;
    p_bne2->colb_g = p_s8->colb_g;
    p_bne2->colb_f = p_s8->colb_f;
    p_bne2->colb_r = p_s8->colb_r;
    p_bne2->colb_e = p_s8->colb_e;
    p_bne2->indthlv = p_e3->indthlv;

    strncpy (p_bne2->dep, p_e3->dep, 3);

    ret = th_7kbnc( p_bne1, p_bne2, p_bns);

    if (ret!=0)
    {
        cherche_Erreur(ret,&(p_s2->libelle)) ;
        return (ret);
    }

    p_s2->cols_c=p_bns->cols_c;
    p_s2->cols_s=p_bns->cols_s;
    p_s2->cols_q=p_bns->cols_q;
    p_s2->cols_d=p_bns->cols_d;
    p_s2->cols_n=p_bns->cols_n;
    p_s2->cols_g=p_bns->cols_g;
    p_s2->cols_f=p_bns->cols_f;
    p_s2->cols_r=p_bns->cols_r;
    p_s2->cols_e=p_bns->cols_e;

    /* CHARGEMENT */

    if (p_s2->cols_c.vlnex + p_s2->cols_q.vlnex + p_s2->cols_s.vlnex > 0 )
    {
        dne.vlexo = 'O';
    }
    else
    {
        dne.vlexo = 'N';
    }

    /*   APPEL DU MODULE -KCOC   */
    /*   ---------------------   */

    p_coe1->antax  = p_e2->antax;
    p_coe1->tyimp  = p_e2->tyimp;
    p_coe1->vlbsc  = p_e2->vlbsc;
    p_coe1->degex  = p_e2->degex;
    p_coe1->cnat   = p_e2->cnat;
    p_coe1->tax    = p_e2->tax;
    p_coe1->cols_c = p_bns->cols_c;
    p_coe1->cols_s = p_bns->cols_s;
    p_coe1->cols_q = p_bns->cols_q;
    p_coe1->cols_d = p_bns->cols_d;
    p_coe1->cols_n = p_bns->cols_n;
    p_coe1->cols_g = p_bns->cols_g;
    p_coe1->cols_e = p_bns->cols_e;
    strncpy(p_coe1->dep, p_e3->dep, 3);
    strncpy(p_coe1->cne, p_e3->cne, 4);
    coe2.timpc     = p_e3->timpc;
    coe2.tisyn     = p_e3->tisyn;
    coe2.titsn     = p_e3->titsn;
    coe2.titgp     = p_e3->titgp;
    coe2.tiagn     = p_e3->tiagn;
    coe2.timpq     = p_e3->timpq;
    coe2.timpd     = p_e3->timpd;
    coe2.timpe     = p_e3->timpe;
    coe2.timths    = p_e3->timths;
    p_coe1->indthlv = p_e3->indthlv;
    p_coe1->indmths = p_e3->indmths;
    p_coe1->indaths = p_e2->indaths;
    p_coe1->indgem = p_e3->indgem;
    p_coe1->aff = p_e2->aff;

    ret = th_7kcoc ( p_coe1,&coe2, p_cos);

    if (ret!=0)
    {
        cherche_Erreur(ret,&(p_s2->libelle)) ;
        return (ret);
    }

    svsmrc = p_cos->somrc;
    svsmrp = p_cos->somrp;

    p_s2->cotid  = p_cos->cotid;
    p_s2->cotic  = p_cos->cotic;
    p_s2->coticm = p_cos->coticm;
    p_s2->cotiq  = p_cos->cotiq;
    p_s2->cotis  = p_cos->cotis;
    p_s2->cotin  = p_cos->cotin;
    p_s2->cotigp = p_cos->cotigp;
    p_s2->cotie  = p_cos->cotie;
    p_s2->cotia  = p_cos->cotia;
    p_s2->frait  = p_cos->frait;
    p_s2->frai4  = p_cos->frai4;
    p_s2->prelt  = p_cos->prelt;
    p_s2->somrc  = p_cos->somrc;
    p_s2->somrp  = p_cos->somrp;
    p_s2->pre02  = p_cos->pre02;
    p_s2->pre12  = p_cos->pre12;
    p_s2->pre17  = p_cos->pre17;
    p_s2->preths = p_cos->preths;
    p_s2->fgest_cq  = p_cos->fgest_cq;
    p_s2->fgest_s   = p_cos->fgest_s;
    p_s2->fgest_ng  = p_cos->fgest_ng;
    p_s2->far_cq    = p_cos->far_cq;
    p_s2->far_s     = p_cos->far_s;
    p_s2->far_ng    = p_cos->far_ng;
    p_s2->majths    = p_cos->majths;
    p_s2->far_e     = p_cos->far_e;
    p_s2->fgest_e   = p_cos->fgest_e;

    /*   APPEL DU MODULE KREC   */

    if  (p_e2->tyimp == 'H')
    {
        p_ree->antax = p_e2->antax;
        p_ree->tax   = p_e2->tax;
        p_ree->aff   = p_e2->aff;
        p_ree->permo = p_e2->permo;
        p_ree->grrev = p_e2->grrev;
        p_ree->timpc = p_e3->timpc;
        p_ree->tisyn = p_e3->tisyn;
        p_ree->titsn = p_e3->titsn;
        p_ree->tiagn = p_e3->tiagn;
        p_ree->timpq = p_e3->timpq;
        p_ree->timpd = p_e3->timpd;
        p_ree->timpe = p_e3->timpe;
        p_ree->titgp = p_e3->titgp;
        p_ree->tig00 = p_e3->tig00;
        p_ree->vlbsc = p_e2->vlbsc;
        p_ree->cocnq = p_e3->cocnq;
        p_ree->cols_c = p_s2->cols_c;
        p_ree->cols_q = p_s2->cols_q;

    /* Initialisation du la sortie du module krec*/
    /* Modif ADE le 08/09/2008 fiche 34047 */
        p_res->redtt = 0;
    /* Saint Martin */
    /* Modif AMN le 05/09/2008 fiche 34047 forcage a 0 a partir de 2008 */
        if (!strcmp(p_e3->cne,"127") && !strcmp(p_e3->dep,"97"))
        {
            p_s2->cols_d.bnimp=0;
        }
        p_ree->cols_d = p_s2->cols_d;

    /* Modif AMN le 05/09/2008 fiche 34047 */
        if ((p_ree->tax == 'P') || (p_ree->tax == 'E'))
        {
            ret=th_7krec(&ree,&res);
            if(ret!=0)
            {
                cherche_Erreur(ret,&(p_s2->libelle));
                return(ret);
            }
        }
        svrdtt = p_res->redtt;
        p_s2->redtt = p_res->redtt;
    }

    #if !defined(ILIAD)
    if ((p_e2->tax =='E') && (est_autre_allegement(p_e2->degex)))
    {
        p_s2->degpl = 0;
        p_s2->totth = 0;
        p_s2->netap = 0;
        p_s2->redaa = 0;
        p_s2->redsa = 0;
        p_s2->redta = 0;
        p_s2->pmajc = 0;
        p_s2->majoc= 0;
        strncpy(p_s2->codro,"  ",3);
    }
    else
    #endif
    {
        if (((p_s8->cndl == 'O') ||
             (p_s8->cudl == 'O'))
                       &&
             (p_e2->tyimp == 'H'))
        {
    /*------------------------------------------*/
    /*  L I Q U I D A T I O N    F I C T I V E  */
    /*------------------------------------------*/


    /*  APPEL DU MODULE  KBNC  */
    /*  ---------------------  */

            p_bne2->colb_c = p_s8->colb_o;
            p_bne2->colb_s = p_s8->colb_y;
            p_bne2->tisyn  = p_e3->tisyn ;
            p_bne2->colb_q = p_s8->colb_i;
            p_bne2->cocnq  = p_e3->cocnq ;
            p_bne2->colb_d = p_s8->colb_p;
            p_bne2->titsn  = p_e3->titsn ;
            p_bne2->colb_n = p_s8->colb_v;
            p_bne2->colb_g = p_s8->colb_b;
            p_bne2->titgp  = p_e3->titgp;
            p_bne2->colb_e = p_s8->colb_a;
            p_bne2->timpe  = p_e3->timpe;

            ret = th_7kbnc( p_bne1, p_bne2, p_bns);

            if (ret!=0)
            {
                cherche_Erreur(ret,&(p_s2->libelle)) ;
                return (ret);
            }

            {
                p_s2->cols_o = p_bns->cols_c;
                p_s2->cols_y = p_bns->cols_s;
                p_s2->cols_i = p_bns->cols_q;
                p_s2->cols_p = p_bns->cols_d;
                p_s2->cols_v = p_bns->cols_n;
                p_s2->cols_b = p_bns->cols_g;
                p_s2->cols_a = p_bns->cols_e;
            }

    /*   APPEL DU MODULE KCOC   */
    /*   --------------------    */
            {
                p_coe1->cols_c = p_bns->cols_c;
                p_coe1->cols_s = p_bns->cols_s;
                p_coe1->cols_q = p_bns->cols_q;
                p_coe1->cols_d = p_bns->cols_d;
                p_coe1->cols_n = p_bns->cols_n;
                p_coe1->cols_g = p_bns->cols_g;
                p_coe1->cols_e = p_bns->cols_e;
            }

            ret =  th_7kcoc ( p_coe1,&coe2, p_cos);

            if (ret!=0)
            {
                cherche_Erreur(ret,&(p_s2->libelle)) ;
                return (ret);
            }

            p_s2->somr3 = p_cos->somrc;


    /*   A P P E L   D U  M O D U L E    K R E C  */
    /*   ---------------------------------------  */


            p_ree->cols_c = p_s2->cols_o;
            p_ree->cols_q = p_s2->cols_i;
            p_ree->cols_d = p_s2->cols_d;

            ret=th_7krec(&ree,&res);
            if(ret!=0)
            {
                cherche_Erreur(ret,&(p_s2->libelle));
                return(ret);
            }

            {
                p_s2->redt3 = p_res->redtt;
            }
        }


    /*-----------------------------------------------------*/
    /*    F I N   L I Q U I D A T I O N   F I C T I V E    */
    /*-----------------------------------------------------*/


    /*   APPEL DU MODULE KDNC   */

        dne.degex = p_e2->degex;
        dne.revim = p_e2->revim;
        dne.revff = p_e2->revff;
        dne.somrc = svsmrc;
        dne.somrp = svsmrp;
        dne.antax = p_e2->antax;
        dne.redtt = svrdtt;
        dne.somr3 = p_s2->somr3;
        dne.redt3 = p_s2->redt3;
        dne.cndl  = p_s2->cndl;
        dne.cudl  = p_s2->cudl;
        dne.npaff = p_e2->npaff;
        strncpy(dne.csdi,p_e2->csdi, 4);

        ret = th_7kdnc( p_e2->antax,  &dne, &dns);

        if (ret != 0)
        {
            cherche_Erreur(ret,&(p_s2->libelle)) ;
            return (ret);
        }

        {
            p_s2->degpl = dns.degpl;
            p_s2->totth = dns.totth;
            p_s2->netth = dns.netth;
            strncpy(p_s2->codro,dns.codro,3);
            p_s2->redta = dns.redta;
            p_s2->redaa = dns.redaa;
            p_s2->redsa = dns.redsa;
            p_s2->dgpl3 = dns.dgpl3;
            p_s2->plart = dns.plart;
            p_s2->majoc = dns.majoc;
            p_s2->pmajc = dns.pmajc;
        }
    }

    p_s2->netap = p_s2->netth + p_s2->nettv;

    /*    APPEL DU MODULE KRAC    */
    /*    --------------------    */

    if (p_e2->tyimp == 'H')
    {
        {
            p_rae->antax = p_e2->antax;
            strncpy(p_rae->dep,p_e3->dep, 3);
            p_rae->champ = p_e2->champ;
            p_rae->qtvrt = p_e2->qtvrt;
            p_rae->degtv = p_e2->degtv;
            p_rae->rgsor = p_e2->rgsor;
        }

        ret = th_7krac (p_rae ,p_ras);
        if (ret != 0)
        {
            cherche_Erreur(ret,&(p_s2->libelle));
            return (ret);
        }

        {
            p_s2->cottv = p_ras->cottv;
            p_s2->fratv = p_ras->fratv;
            p_s2->somtv = p_ras->somtv;
            p_s2->mdgtv = p_ras->mdgtv;
            p_s2->nettv = p_ras->nettv;
            strncpy(p_s2->roltv,p_ras->roltv, 4);
            p_s2->netap = p_s2->netth + p_s2->nettv;
        }
    }

    /* appel fonction regularisation de role */

    if (p_e2->regul=='R')
    {
        regularisation(p_s2->netap ,
                       p_e2->napin,
                       &napdt , &signe);
        p_s2->napdt=napdt;
        p_s2->signe=signe;
    }

    if (p_e2->regul != 'R')
    {
        p_s2->signe = ' ';
        p_s2->napdt = 0;
    }

    /*    SORTIE DU MODULE    */

    return (ret);
}



/*============================================================================
   Controle generaux
  ============================================================================*/
int controles(s_e2 * p_e2,
              s_e3 * p_e3,
              s_s2 * p_s2)
{
    long quot_abat;  /*  zone traitement quotite  */

    char dep97[]="97";
    char val40[]="40";
    char val50[]="50";

    static char dep21[]="21";  /*initialisation departement 21 */
    double tau1; /* zone retour taux act */
    static char cne1[]="000";  /* initial char sur cne */
    int retour=0;
    float part;

    retour=strcmp(dep21 , p_e3->dep); /* comparaison si dep 21 */
    if (retour !=0)   /* si dep <> 21 on cherche son taux dans table d'actualisation*/
    {
        tau1=recherche_tdep (p_e3->dep, cne1 , 'H');
        if (tau1==2)  /*retour de la fonction */
        {
            return (2);
        }
    }

    /* traitement code taxation */

    if (((p_e2->tax!='P')&&(p_e2->tax!='S')&&(p_e2->tax!='E')) &&
         (p_e2->tyimp == 'H'))
    {
        return (3);
    }

    /* traitement PAC */

    if (p_e2->nbpac > 99)
    {
        return (4);
    }




    /* traitement code NI a l IR */

    if ((p_e2->codni!='A')&&(p_e2->codni!=' '))
    {
        return (6);
    }

    /* traitement codsh */

    if ((p_e2->codsh!='1')&&(p_e2->codsh!='2')&&(p_e2->codsh!='3')&&
        (p_e2->codsh!='4')&&(p_e2->codsh!='5')&&(p_e2->codsh!=' '))

    {
        return (7);
    }


    /*    traitement tyimp et code nature FIP    */

    if ((p_e2->cnat!='3')&&(p_e2->cnat!='4')&&
        (p_e2->cnat!='5')&&(p_e2->cnat!='6'))
    {
        return (8);
    }

    /*Controle du code affectation du local */

    if ((p_e2->aff!='A')&&(p_e2->aff!='F')&&(p_e2->aff!='H')&&
        (p_e2->aff!='M')&&(p_e2->aff!='S')&&(p_e2->aff!=' '))

    {
        return (10);
    }
    /*Controle du nombre de part IR   */

    if (p_e2->npaff<0)
    {
        return (15);
    }

    part=(float)((p_e2->npaff*4)/10);

    if (part!=arrondi_dizaine_inf(part))
    {
        return (14);
    }

    /* nbre pac > 99 */
    if (p_e2->nbera > 99)
    {
        return (16);
    }

    /* code permo diff de 0 1 2 3 4 5 6 7 8 9 ' ' */

    if((p_e2->permo!='0')&&
       (p_e2->permo!='1')&&
       (p_e2->permo!='2')&&
       (p_e2->permo!='3')&&
       (p_e2->permo!='4')&&
       (p_e2->permo!='5')&&
       (p_e2->permo!='6')&&
       (p_e2->permo!='7')&&
       (p_e2->permo!='8')&&
       (p_e2->permo!='9')&&
       (p_e2->permo!=' '))
    {
        return (17);
    }

    /* code grrev diff de  1 2 3 4  ' ' */
    if ((p_e2->grrev!='1')&&(p_e2->grrev!='2')&&
        (p_e2->grrev!='3')&&
        (p_e2->grrev!='4')&&(p_e2->grrev!=' '))
    {
        return (18);
    }

    /* code champ diff de c d e i m s t */
    if ((p_e2->champ!='C')&&(p_e2->champ!='D')&&(p_e2->champ!='E')&&
        (p_e2->champ!='I')&&(p_e2->champ!='M')&&(p_e2->champ!='S')&&
        (p_e2->champ!='R')&&(p_e2->champ!='T')&&(p_e2->tyimp == 'H'))
    {
        return (19);
    }

    /* code qtvrt diff de  0, 1 */
    if ((p_e2->qtvrt!='0')&&(p_e2->qtvrt!='1')&&(p_e2->tyimp == 'H'))
    {
        return (20);
    }


    /* code degtv diff de ejfiadvws ' ' */
    if (p_e2->antax == 2014)
    {
        if ((p_e2->degtv!='E')&&(p_e2->degtv!='J')&&(p_e2->degtv!='F')&&
        (p_e2->degtv!='I')&&(p_e2->degtv!='A')&&(p_e2->degtv!='D')
        &&(p_e2->degtv!='V')&&(p_e2->degtv!='S')&&(p_e2->degtv!=' ')
        &&(p_e2->degtv!='Z')&&(p_e2->degtv!='W'))

            {
            return (21);
            }
    }
    else
    {
       if ((p_e2->degtv!='E')&&(p_e2->degtv!='J')&&(p_e2->degtv!='F')
         &&(p_e2->degtv!='I')&&(p_e2->degtv!='A')&&(p_e2->degtv!='D')
         &&(p_e2->degtv!='V')&&(p_e2->degtv!='S')&&(p_e2->degtv!=' ')
         &&(p_e2->degtv!='Z')&&(p_e2->degtv!='W')&&(p_e2->degtv!='K'))
            {
            return (21);
            }
    }

    /* code degex diff de fgumiadvwsh ' ' */
    if (p_e2->antax == 2014)
    {
        if((p_e2->degex!='F')&&(p_e2->degex!='G')&&(p_e2->degex!='I')&&
        (p_e2->degex!='S')&&(p_e2->degex!='A')&&(p_e2->degex!='D')&&
        (p_e2->degex!='V')&&(p_e2->degex!='H')&&(p_e2->degex!='W')&&
        (p_e2->degex!=' '))
            {
            return (22);
            }
    }
    /* sortie en sifflet */
    else
    {
        if((p_e2->degex!='F')&&(p_e2->degex!='G')&&(p_e2->degex!='I')&&
        (p_e2->degex!='S')&&(p_e2->degex!='A')&&(p_e2->degex!='D')&&
        (p_e2->degex!='V')&&(p_e2->degex!='H')&&(p_e2->degex!='W')&&
        (p_e2->degex!='K')&&(p_e2->degex!=' '))
            {
            return (22);
            }
    }
    /* code TYIMP diff de h et t   */
    if ((p_e2->tyimp!='H')&&(p_e2->tyimp!='T'))
    {
        return (23);
    }

    /* code TYIMP egal a t , nbpac nbera diff 0 */
    if ((p_e2->tyimp=='T')&&((p_e2->nbpac!=0)||(p_e2->nbera!=0)))
    {
        return (25);
    }

    /* code TYIMP egal a t , degex servi */
    if ((p_e2->tyimp=='T')&&(p_e2->degex!=' '))
    {
        return (26);
    }

     /* code TYIMP egal a t , codni servi */
    if ((p_e2->tyimp=='T')&&(p_e2->codni!=' '))
    {
        return (27);
    }

    /* code TYIMP egal a t , champ TV servi */
    if ((p_e2->tyimp=='T')&&(p_e2->champ!=' '))
    {
        return (28);
    }

    /* code TYIMP egal a t , degtv servi */
    if ((p_e2->tyimp=='T')&&(p_e2->degtv!=' '))
    {
        return (29);
    }

    /* code TYIMP egal a t , qtvrt servi */
    if ((p_e2->tyimp=='T')&&(p_e2->qtvrt!=' '))
    {
        return (30);
    }

    /* code TYIMP egal a t , permo servi */
    if ((p_e2->tyimp=='T')&&(p_e2->permo!=' ')&&(p_e2->antax <2016))
    {
        return (31);
    }

    /* code TYIMP egal a t , grrev servi */
    if ((p_e2->tyimp=='T')&&(p_e2->grrev!=' ')&&(p_e2->antax <2016))
    {
        return (32);
    }

    /* code TYIMP egal a t , tax servi */
    if ((p_e2->tyimp=='T')&&(p_e2->tax!=' '))
    {
        return (35);
    }

    /* code TYIMP egal a t , vlbsc diff 0 */
    if ((p_e2->tyimp=='T')&&(p_e2->vlbsc!=0))
    {
        return (36);
    }

    /* code TYIMP egal a t , revff diff 0 */
    if ((p_e2->tyimp=='T')&&(p_e2->revff!=0))
    {
        return (38);
    }

    /* indicateur majoration THS diff de O et N*/
    if((p_e3->antax) >= 2015){

			if ((p_e3->indmths!='N')&& (p_e3->indmths!='O'))
			{
				return (39);
			}

    /* Controle du périmètre du champ d'application de la majoration THS */
       if(((p_e3->indmths) == 'O') && (recherche_dep_TLV(p_e3->dep) == 0)){
   				return (40);
       }
    }
    /* Controle taux d'imposition de la majoration THS */
    if (p_e3->antax < 2017)
    {
        if (p_e3->timths != 0)
        {
            return (41);
        }
    }
    else
    {
        if ((p_e3->indmths == 'O') && (p_e3->timths > 0.60))
            {
            return (41);
            }
    }

    /* Controle indicateur d'assujetissement a la majoration THS */
    if (p_e2->antax > 2015)
        {
        if ((p_e2->indaths != 'O') &&(p_e2->indaths != ' '))
            {
                return (42);
            }
        }

    /* regul different de R */
    if ((p_e2->regul!='R')&&(p_e2->regul!= ' '))
    {
        return(5201);
    }

    /* comparaison  annee de reference */
    if (p_e2->antax != p_e3->antax)
    {
        return (51);
    }

    /* code tax  S ou E et pac # zero */
    if ((p_e2->tax == 'S' || p_e2->tax == 'E') && ((p_e2->nbpac != 0) || (p_e2->nbera != 0)))
    {
        return (52);
    }

    /* code tax S et VL brute non nulle */
    if ((p_e2->tax == 'S')&&(p_e2->vlbpc != 0))
    {
        return (53);
    }

    /* code tax S et code exoneration servi */
    if ((p_e2->tax == 'S')&&(p_e2->degex != ' '))
    {
        return (54);
    }

    /* code tax S et code ni = "A" */
    if ((p_e2->tax == 'S')&&(p_e2->codni == 'A'))
     {
     return (55);
     }


    /* code deg D ,code DEP # 97 */
    retour=strcmp(p_e3->dep,dep97); /* comparaison si dep 97 */
    if (retour!=0) /* traitement metropole */
    {
        if (p_e2->degex=='D')
        {
            return (58);
        }
    }

    /* code tax E et VL brute secondaire non nulle */

    if ((p_e2->tax=='E') &&(p_e2->vlbsc!=0))
    {
        return (59);
    }

    /* 31/01/2012 - DMA */
    /*  code deg A V  , code tax P ,code ni # A , mill.>= 2012 */
    if ((p_e2->degex == 'A' || p_e2->degex == 'V') &&
        (p_e2->tax =='P' && p_e2->codni != 'A'))
    {
        return (60);
    }

    /* code deg G , code nature cnat # 5 */
    if ((p_e2->degex == 'G') && (p_e2->cnat != '5'))
    {
        return (61);
    }

    /* valeurs locatives > zero */
    if ((p_e2->vlbpc != 0) && (p_e2->vlbsc != 0))
    {
        return (64);
    }

    /* champ # c  qtvrt = 1 */
    if ((p_e2->champ != 'C') && (p_e2->qtvrt == '1'))
    {
        return (65);
    }

    /* champ = c  tax = e */
    if ((p_e2->champ == 'C') && (p_e2->tax == 'E'))
    {
        return (66);
    }

    /* champ = c  cnat = 5 */
    if ((p_e2->champ == 'C') && (p_e2->cnat == '5'))
    {
        return (67);
    }

    /* champ = c  tax = p */
    if ((p_e2->champ == 'C') && (p_e2->tax == 'P')&&(p_e2->degex!=' ')&&
        (p_e2->degex!='H')&& (p_e2->degtv!=p_e2->degex))
    {
        return (68);
    }

    /* champ # c  degtv servi */
    if ((p_e2->champ != 'C') && (p_e2->degtv != ' '))
    {
        return (69);
    }

    /* degtv = e j  degth # h z ' ' */
    if (((p_e2->degtv == 'E') || (p_e2->degtv == 'J'))&&
        ((p_e2->degex!='H')&&(p_e2->degex!=' ')))
    {
        return (70);
    }

    /* a n o  71 */
    /* ========= */

    if ((p_e2->degex == 'H') &&
       ((p_s2->somrc)!=(p_s2->degpl)) &&
       ((p_s2->somrc) != (p_s2->redta)) &&
       ((p_s2->somrc) != (p_s2->redsa)))
    {
        return (71);
    }

    /* Controle du rang de sortie en sifflet exoneration TH */

    if (p_e2->antax == 2016){
        if (((p_e2->degex == 'W') && (p_e2->rgsor != '2')) ||
           (( p_e2->degex == 'K') && ((p_e2->rgsor != '1') && (p_e2->rgsor != '2'))) ||
           (((p_e2->degex != 'K') && (p_e2->degex != 'W')) && (p_e2->rgsor != ' ')))
        {
            return(77);
        }
    }else if (p_e2->antax == 2017){
        if (((p_e2->degex == 'W') && (p_e2->rgsor != ' ')) ||
           (( p_e2->degex == 'K') && ((p_e2->rgsor != '1') && (p_e2->rgsor != '2'))) ||
           (((p_e2->degex != 'K') && (p_e2->degex != 'W')) && (p_e2->rgsor != ' ')))
        {
            return(77);
        }
    }else{
        if (p_e2->rgsor != ' ')
        {
            return(77);
        }
    }

    /* Controle du code abattement TH et du rang de sortie quand sortie en sifflet */

    if (p_e2->antax == 2017){
        if (((p_e2->abatk== 'K') && (p_e2->rgsork != '3')) ||
            ((p_e2->abatk== ' ') && (p_e2->rgsork != ' ')))
        {
            return(78);
        }
    }

    /* Codes abattement et exoneration/degrevement sont exclusifs sauf en cas de plafonnement */

    if (p_e2->antax == 2017){
        if(((p_e2->degex=='A')||(p_e2->degex=='F')||(p_e2->degex=='I')||
            (p_e2->degex=='S')||(p_e2->degex=='V')||(p_e2->degex=='W')||
            (p_e2->degex=='K')) && (p_e2->abatk=='K'))
        {
            return(79);
        }
    }

    /* Le code abattement TH est incompatible avec une THS */

    if (p_e2->tax=='S' && p_e2->abatk=='K')
        {
            return(80);
        }

    /* test vl nulle ou > 200000 */
    if ((p_e3->cole_c.vlmoy==0)||(p_e3->cole_c.vlmoy>200000))
    {
        return (101);
    }

    /* quotite abat. pac rang 1 et 2 et plus */

    retour=strcmp(p_e3->dep, dep97);  /* comparaison si dep 97 */
    if (retour !=0) /* trait metropole */
    {
        quot_abat=arrondi_dizaine_inf((p_e3->cole_f.vlmoy * 0.1)-1);
        if ((p_e3->cole_f.apac1 < quot_abat)&& /* trait 1 ou 2 pac */
            (p_e3->codef==' ')&&(p_e3->codeg==' '))
        {
            return (102);
        }
        quot_abat=arrondi_dizaine_inf((p_e3->cole_f.vlmoy * 0.15)-1); /*trait + de 2 pac*/
        if ((p_e3->cole_f.apac3 < quot_abat)&&(p_e3->cole_f.apac3 > 0))
        {
            return (103);
        }
    }
    else /* trait hors metropole */
    {
        quot_abat=arrondi_dizaine_inf((p_e3->cole_f.vlmoy * 0.05)-1); /* trait 1 ou 2 pac */
        if ((p_e3->cole_f.apac1 < quot_abat)&&
            (p_e3->codef==' ')&&(p_e3->codeg==' '))
        {
            return (102);
        }
        quot_abat=arrondi_dizaine_inf((p_e3->cole_f.vlmoy * 0.05)-1); /* trait + de 2 pac */
        if (p_e3->cole_f.apac3 < quot_abat)
        {
            return (103);
        }
    }

    /* quotite abat . cne general a la base */
    retour =strcmp(p_e3->dep,dep97);  /* comparaison si dep 97 */
    if (retour !=0) /* trait metropole */
    {
        if (!( strcmp(p_e3->cole_f.txbas,"F ") == 0))
        {
            quot_abat=arrondi_euro_inf((p_e3->cole_f.vlmoy * 0.15)+1);
            if (p_e3->cole_f.abbas > quot_abat)
            {
                return (104);
            }
        }
    }

    /* quotite abat . cne special a la base */
    quot_abat=arrondi_euro_inf((p_e3->cole_f.vlmoy * 0.15)+1);
    if (p_e3->cole_f.abspe > quot_abat)
    {
        return (105);
    }

    /*  anomalie PAC alsace moselle */

    if (((p_e3->codeg!=' ')||(p_e3->codef!=' '))&&
        ((p_e3->cole_f.apac1>0)||(p_e3->cole_f.apac3 > 0)))
    {
        return (106);
    }

    if ((p_e3->codef == ' ')&&(p_e3->codeg == ' ')&&(p_e3->cole_f.apac1 == 0))
    {
        return (107);
    }

    if ((p_e3->codef == ' ')&&(p_e3->codeg == ' ')&&(p_e3->cole_f.apac3 == 0))
    {
        return (108);
    }

    if (p_e3->codeg != ' ')
    {
        return (109);
    }

    /* indicateur CU # c ou blanc */
    if ((p_e3->cocnq!='C')&&(p_e3->cocnq!=' '))
    {
        return (111);
    }

    /* taux global tig00 non compris entre 1% et 100% */
    if ((p_e3->tig00<0.01) || (p_e3->tig00>1))
    {
        return (112);
    }

    /* alsace moselle codef servi , abmos doit etre positif */
    if ((p_e3->codef!=' ')&&(p_e3->abmos <= 0))
    {
        return (114);
    }

    /* a n o m a l i e  115 */
    if (p_e3->vmd89 > 200000)
    {
        return (115);
    }

    /* a n o m a  l i e  116 */
    retour=strcmp(p_e3->dep,dep97);
    /*Modif AMN le 03/09/2008 fiche 33972 if  ((p_e3->vmd89 == 0)*/
    if ((p_e3->vmd89 == 0) && (retour == 0))
    {
        return (116);
    }
    /* a n o m a  l i e  117 */

    retour=strcmp(p_e3->dep,dep97);
    if ((retour != 0) && (p_e3->vmd89 != 0))
    {
        return (117);
    }

    /* a n o m a  l i e  118 */
    /* Autre ecriture de l'ano 118 */
    /* Modif AMN le 03/09/2008 fiche 33972 if (
        ( strcmp(p_e3->dep,dep97)) */
    if (( !strcmp(p_e3->dep,dep97))&&
       ((strcmp(p_e3->cole_c.txbas , val40))&&
        (strcmp(p_e3->cole_c.txbas , val50))))    {
        return (118);
    };

    /* quotite d'abattements ASH commune */
    if (p_e3->antax < 2016) {
        if (p_e3->cole_f.abhan > 0.1*p_e3->cole_f.vlmoy + 1)
        {
            return (119);
        }
    }
    else{
        if (!(strcmp(p_e3->cole_f.txhan,"  ") == 0) && !(strcmp(p_e3->cole_f.txhan,"00") == 0)){
            if ((p_e3->cole_f.abhan > 0.2*p_e3->cole_f.vlmoy + 1) ||
                (p_e3->cole_f.abhan < 0.1*p_e3->cole_f.vlmoy - 1))
            {
                return (119);
            }
        }
    }

    /* a n o m a  l i e  152 */
    if ((p_e3->cole_j.apac1 == 0)&&(p_e3->cneg3 == 'O')&&(p_e3->cnam3 == 'N'))
    {
        return (152);
    }

    /* a n o m a  l i e  153 */
    if ((p_e3->cole_j.apac3 == 0)&&(p_e3->cneg3 == 'O')&&(p_e3->cnam3 == 'N'))
    {
        return (153);
    }

    /* a n o m a  l i e  154 */
    if ((p_e3->cneg3 != 'O')&&(p_e3->cneg3 != 'N'))
    {
        return (154);
    }

    /* a n o m a  l i e  155 */
    if ((p_e3->cnqg3 != 'O')&&(p_e3->cnqg3 != 'N'))
    {
        return (155);
    }

    /* a n o m a  l i e  156 */
    if ((p_e3->cnam3 != 'O')&&(p_e3->cnam3 != 'N'))
    {
        return (156);
    }

    /*   trait vl moyenne groupement de cnes */
    if (p_e3->cole_q.vlmoy > 200000)
    {
        return (201);
    }

    /*    quotite abat group cne 1 ou 2 pac */
    quot_abat=arrondi_dizaine_inf((p_e3->cole_r.vlmoy * 0.1)-1);
    if (p_e3->cole_r.apac1 < quot_abat)
    {
        if (p_e3->cole_r.apac1 > 0)
        {
            return (202);
        }
    }

    /*    quotite abat group cne + de pac */
    quot_abat=arrondi_dizaine_inf((p_e3->cole_r.vlmoy * 0.15)-1);
    if (p_e3->cole_r.apac3 < quot_abat)
    {
        if (p_e3->cole_r.apac3 > 0)
        {
            return (203);
        }
    }

    /*   quotite abat group cne abat general base  */
    quot_abat=arrondi_euro_inf((p_e3->cole_r.vlmoy * 0.15)+1);
    if (p_e3->cole_r.abbas > quot_abat)
    {
        return (204);
    }

    /*   quotite abat group cne abat special base  */
    quot_abat=arrondi_euro_inf((p_e3->cole_r.vlmoy * 0.15)+1);
    if (p_e3->cole_r.abspe > quot_abat)
    {
        return (205);
    }

    /* quot. abatt. group.comm # zeros */
    if (((p_e3->cole_r.vlmoy != 0) || (p_e3->cole_r.abspe != 0 ) ||
        (p_e3->cole_r.abbas != 0) || (p_e3->cole_r.apac1 != 0) ||
        (p_e3->cole_r.apac3 != 0))&&(p_e3->cocnq == ' '))
    {
        return (206);
    }

    /* taux imp. group cne non nul et CU a blanc */
    if ((p_e3->timpq != 0) && (p_e3->cocnq == ' '))
    {
        return (211);
        }

    /* quotite d'abattements ASH inter-comm */
    if (p_e3->antax < 2016) {
        if (p_e3->cole_r.abhan > 0.1*p_e3->cole_r.vlmoy + 1)
        {
            return (212);
        }
    }
    else{
        if (!(strcmp(p_e3->cole_r.txhan,"  ") == 0) && !(strcmp(p_e3->cole_r.txhan,"00") == 0)){
            if ((p_e3->cole_r.abhan > 0.2*p_e3->cole_r.vlmoy + 1) ||
                (p_e3->cole_r.abhan < 0.1*p_e3->cole_r.vlmoy - 1))
            {
                return (212);
            }
        }
    }

    /* indic cnqg3 = o et cocnq # c */

    if ((p_e3->cnqg3 == 0) && (p_e3->cocnq != 'C'))
    {
        return (251);
    }


    /* trait vl moyenne departementale */

    if (!strcmp(p_e2->csdi,"976"))
    {
        if (p_e3->cole_d.vlmoy > 200000)
        {
            return (301);
        }
    }
    else if ((p_e3->cole_d.vlmoy == 0) || (p_e3->cole_d.vlmoy > 200000))
        {
            return (301);
        }


    /* quotite abat departemental 1 ou 2 pac */
    quot_abat=arrondi_euro_inf((p_e3->cole_d.vlmoy * 0.2)+1);
    if (p_e3->cole_d.apac1 > quot_abat)
    {
        return (302);
    }

    /* quotite abat departemental + 2 de pac */
    quot_abat=arrondi_euro_inf((p_e3->cole_d.vlmoy * 0.25)+1);
    if (p_e3->cole_d.apac3 > quot_abat)
    {
        return (303);
    }

    /* quotite abat departemental abat general base */
    quot_abat=arrondi_euro_inf((p_e3->cole_d.vlmoy * 0.15)+1);
    if (p_e3->cole_d.abbas > quot_abat)
    {
        return (304);
    }

    /* quotite abat departemental abat special base */
    quot_abat=arrondi_euro_inf((p_e3->cole_d.vlmoy * 0.15)+1);
    if (p_e3->cole_d.abspe > quot_abat)
    {
        return (305);
    }

    /* quotite d'abattements ASH departemental */
    if (p_e3->cole_d.abhan > 0.1*p_e3->cole_d.vlmoy + 1)
    {
        return (306);
    }

    /* taux imposition aggl. nouvelle */

    if (p_e3->tiagn != 0)
    {
        return (810);
    }

    /* Déclenchement du message 110 si somme des taux d'imposition anormalement élevée */
    if ((p_e3->timpc + p_e3->timpq + p_e3->tisyn + p_e3->titsn + p_e3->titgp+ p_e3->timpe) > 1)/* c'est a dire 100% */
    {
        return (110);
    }

    /* tous les controles sont corrects */
    return 0;
}
/*------------------------ fin des controles generaux ------------------------*/

/*----------------------------------------------------------------------------*/
/*                    Initialisation de la zone de sortie                     */
/*----------------------------------------------------------------------------*/

void  initialisation_sortie(s_s2 * p_s2)
{
    p_s2->vlbri = 0;
    p_s2->vlbni = 0;
    p_s2->cotid = 0;
    p_s2->cotic = 0;
    p_s2->coticm = 0;
    p_s2->cotiq = 0;
    p_s2->cotis = 0;
    p_s2->cotin = 0;
    p_s2->cotia = 0;
    p_s2->cotigp = 0;
    p_s2->frait = 0;
    p_s2->frai4 = 0;
    p_s2->prelt = 0;
    p_s2->pre02 = 0;
    p_s2->pre12 = 0;
    p_s2->pre17 = 0;
    p_s2->somrc = 0;
    p_s2->degpl = 0;
    p_s2->totth = 0;
    p_s2->netap = 0;
    p_s2->somrp = 0;
    p_s2->redta = 0;
    p_s2->redaa = 0;
    p_s2->redsa = 0;
    p_s2->netth = 0;
    p_s2->redtt = 0;
    p_s2->cottv = 0;
    p_s2->fratv = 0;
    p_s2->somtv = 0;
    p_s2->mdgtv = 0;
    p_s2->nettv = 0;
    p_s2->netap = 0;
    p_s2->napdt = 0;
    p_s2->somr3 = 0;
    p_s2->dgpl3 = 0;
    p_s2->redt3 = 0;
    p_s2->plart = 0;
    p_s2->cotie = 0;
    p_s2->fgest_cq = 0;
    p_s2->fgest_s  = 0;
    p_s2->fgest_ng = 0;
    p_s2->fgest_e  = 0;
    p_s2->far_cq   = 0;
    p_s2->far_s    = 0;
    p_s2->far_e    = 0;
    p_s2->far_ng   = 0;
    p_s2->cndl = ' ';
    p_s2->cudl = ' ';
    strncpy(p_s2->codro, "  ",3);
    strncpy(p_s2->roltv, "   ",4);
    p_s2->libelle = NULL ;
    initialisation_sortie_col1(&p_s2->cols_c);
    initialisation_sortie_col1(&p_s2->cols_q);
    initialisation_sortie_col1(&p_s2->cols_d);
    initialisation_sortie_col1(&p_s2->cols_y);
    initialisation_sortie_col1(&p_s2->cols_n);
    initialisation_sortie_col1(&p_s2->cols_o);
    initialisation_sortie_col1(&p_s2->cols_i);
    initialisation_sortie_col1(&p_s2->cols_p);
    initialisation_sortie_col1(&p_s2->cols_v);
    initialisation_sortie_col1(&p_s2->cols_b);
    initialisation_sortie_col1(&p_s2->cols_g);
    initialisation_sortie_col1(&p_s2->cols_e);
    initialisation_sortie_col1(&p_s2->cols_a);
}

void   initialisation_sortie_col1(s_cols *  p_c)
{
    p_c->abgen = 0;
    p_c->abpac = 0;
    p_c->abp12 = 0;
    p_c->abspe = 0;
    p_c->abhan = 0;
    p_c->vlntt = 0;
    p_c->vlnex = 0;
    p_c->bnimp = 0;
    p_c->absps = ' ';
}
