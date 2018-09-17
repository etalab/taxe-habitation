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

/* Fichier de definition des structures                                       */

#if !defined( th_7KSTS )
    #define   th_7KSTS
    #include  <math.h>
    #include  <stdlib.h>
    #include  <string.h>
    #include  <float.h>
#if defined   ILIAD
    #include  <th-7ktos.h>
#else
    #include  "th-7ktos"
#endif



/*----------------------------------------------------------------------------
   définition des rangs pour la signature
  ----------------------------------------------------------------------------*/

#define RKTOS   1
#define RKSTS   2
#define RKFOS   3
#define RKARC   4
#define RKSFC   5
#define RKSEC   6
#define RKTAC   7
#define RKVLC   8
#define RKDNC   9
#define RKBNC  10
#define RKCOC  11
#define RKRAC  12
#define RKCVC  13
#define RKTVC  14
#define RKCAC  15
#define RKREC  16
#define RKDEC  17



/*----------------------------------------------------------------------------
   définition des identifiants externes pour la signature
  ----------------------------------------------------------------------------*/

#define MILLESIME "7"
#define KTOS_D    "B"
#define KSTS_D    "E"
#define KFOS_D    "B"
#define KARC_D    "C"
#define KSFC_V    "B"
#define KSEC_V    "B"
#define KTAC_V    "C"
#define KVLC_V    "B"
#define KDNC_D    "B"
#define KBNC_V    "D"
#define KCOC_V    "B"
#define KRAC_V    "B"
#define KCVC_V    "B"
#define KTVC_V    "B"
#define KCAC_V    "D"
#define KREC_V    "B"
#define KDEC_D    "B"



/*----------------------------------------------------------------------------
   voici la signature
  ----------------------------------------------------------------------------*/
#define SIGNATURE \
MILLESIME \
KTOS_D \
KSTS_D \
KFOS_D \
KARC_D \
KSFC_V \
KSEC_V \
KTAC_V \
KVLC_V \
KDNC_D \
KBNC_V \
KCOC_V \
KRAC_V \
KCVC_V \
KTVC_V \
KCAC_V \
KREC_V \
KDEC_D



/*============================================================================
   s_rev : structure des seuils annuels de revenu en fonction du nombre
   de parts pour les abattements (Art 1417-II) et les RFR(Art 1414-A.I)
  ============================================================================*/

typedef struct {
    long    revenu_unepart        ; /* max de revenu fiscal de reference
                                       pour une part de quotient familial     */
    long    rev_prem_demipart     ; /* montant de revenu fiscal de
                                       reference autorise pour la premiere
                                       demi-part supplementaire               */
    long    rev_deuxieme_demipart ; /* montant de revenu fiscal de
                                       reference autorise pour la deuxieme
                                       demi-part supplementaire               */
    long    rev_troisieme_demipart; /* montant de revenu fiscal de
                                       reference autorise pour la
                                       troisieme demi-part supplementaire     */
    long    rev_quatrieme_demipart; /* montant de revenu fiscal de
                                       reference autorise pour la
                                       quatrieme demi-part supplementaire     */
    long    rev_autre_demipart    ; /* montant de revenu fiscal de
                                       reference autorise a partir de la
                                       troisieme demi-part supplementaire     */
} s_rev;

/*-------- fin de  s_rev : structure des seuils annuels de revenu en
           fonction du nombre de parts pour les plafonnements et RFR  --------*/

/*============================================================================
   s_rev_w : structure des familles de seuils annuels de revenu pour
   code seuil 1417-I Bis (droit W) a partir de 2016
  ============================================================================*/

typedef struct {
    long    rev_w_2parts         ; /* Seuil exonération droit acquis
                                      W art 1417-I bis jusqu'à 2 parts */
    long    rev_w_autrepart      ; /* Seuil exonération droit acquis
                                      W demie part supplémentaire      */
} s_rev_w;

/*--------- fin de  s_rev_w : structure des seuils annuels de
            revenu en fonction du nombre de parts */


/*============================================================================
   s_typseuil : structure des familles de seuils annuels de revenu en
   fonction du nombre de parts par type d'allegement
  ============================================================================*/

typedef struct {
    s_rev     rev_tousdroits      ; /* donnees seuil non imposition           */
    s_rev_w   rev_w               ; /* seuils relatifs au droit exoneration W */
    s_rev     rev_plaf            ; /* donnees seuil de plafonnement          */
    s_rev     rev_abat            ; /* donnees abattement sur rfr             */
} s_typseuil;

/*--------- fin de  s_typseuil : structure des seuils annuels de
            revenu en fonction du nombre de parts                     --------*/


/*============================================================================
   s_cons : structure des constantes annuelles
  ============================================================================*/

typedef struct {
    short   antax                 ; /* annee de taxation a laquelle se
                                       referent les differents seuils         */
    short   antax_moins1          ;
    short   antax_moins2          ;
    short   antax_moins3          ;
    short   dat_deg60             ; /* derniere annee de naissance ouvrant
                                       droit au degrevement plus de 60 ans    */
    short   dat_deg65             ; /* derniere annee de naissance ouvrant
                                       droit au degrevement plus de 65 ans
                                       droit acquis                           */

    /***** Coefficient revalorisation *****/
    double  reval_metro           ; /* taux de revalorisation metropole
                                       amalgame                               */
    double  reval_dom             ; /* taux de revalorisation DOM amalgame    */
    double  reval_mayotte         ; /* taux de revalorisation Mayotte amalgame*/

    /***** Donnees de degrevement TH  *****/
    long    seuil_deg             ; /* minimum de degrevement                 */
    long    seuil_nap             ; /* minimum de net a payer en dessous
                                       c'est une non-valeur                   */
    long    seuil_reduc           ; /* minimum de reduction                   */
    double  coeff_plaf_1414A      ; /* taux du plafonnement 1414 A
                                       applicable au revenu imposable         */
    long  plaf_quotite_gen      ; /* seuil quotité abattement maximum       */
    long  plaf_quotite_may      ; /* seuil quotité abattement Mayotte       */

    /***** Donnees de prelevement TH  *****/
    long    seuil_prel            ; /* limite a partir de laquelle
                                       s'applique le prelevement              */
    double  taux_prelp            ; /* taux du prelevement sur principaux     */
    double  taux_prels1           ; /* taux du prelevement sur secondaires    */
    long    seuil_prels           ; /* limite a partir de laquelle
                                       s'applique le deuxieme taux de
                                       prelevement sur secondaires            */
    double  taux_prels2           ; /* taux de prelevement sur secondaires
                                       luxueux                                */
    double  taux_preths            ; /* taux du prélèvement additionnel THS   */
    double  taux_majths           ; /* Taux majoration THS                    */
    double  taux_TLV_1            ; /* taux de TLV après seulement 2 ans
                                       de vacance                             */
    double  taux_TLV_2            ; /* taux de TLV après 3 ans de vacance     */

    /*****    Déclaration des coefficients de FAR et FGEST ****/
    /*****        (frais de gestion TH differencie par collectivite ****/
    double  coeff_FAR_THP_THE_c   ; /* coefficient frais assiette recouvrement (FAR)
                                    THP/THE commune et interco                      */
    double  coeff_FAR_THP_THE_s   ; /* coefficient FAR THP/THE du syndicat          */
    double  coeff_FAR_THP_THE_t   ; /* coefficient FAR THP/THE de la TSE et
                                    TSE Autre                                       */
    double  coeff_FAR_THP_THE_g   ; /* coefficient frais d'assiette et de recouvrement
    								THP/THE GEMAPI                                  */
    double  coeff_FAR_THS_c       ; /* coefficient FAR THS de la commune et
                                    l'interco et Majoration THS                                      */
    double  coeff_FAR_THS_s       ; /* coefficient FAR THS du syndicat              */
    double  coeff_FAR_THS_t       ; /* coefficient DAR THS de la TSE et TSE Autre   */
    double  coeff_FAR_THS_g       ; /* coefficient FAR THS GEMAPI                   */
    double  coeff_FGEST_THP_THE_c ; /* coefficient frais de gestion(FGEST)
                                    THP/THE de la commune et de l'interco.          */
    double  coeff_FGEST_THP_THE_s ; /* coefficient FGEST THP/THE du syndicat        */
    double  coeff_FGEST_THP_THE_t ; /* coefficient FGEST THP/THE de la TSE et
                                    TSE Autre                                       */
    double  coeff_FGEST_THP_THE_g ; /* coefficient FGEST THP/THE GEMAPI             */
    double  coeff_FGEST_THS_c     ; /* coefficient FGEST THS de la commune et de
                                    l'interco et majoration THS                                      */
    double  coeff_FGEST_THS_s     ; /* coefficient FGEST THS du syndicat            */
    double  coeff_FGEST_THS_t     ; /* coefficient FGEST THS de la TSE et TSE Autre */
    double  coeff_FGEST_THS_g     ; /* coefficient FGEST THS GEMAPI                 */
    double  coeff_frais_ass_THLV  ; /* frais assiette recouvrement THLV       */
    double  coeff_frais_THLV      ; /* frais de rôle THLV                     */
    double  coeff_frais_TLV       ; /* frais pour la TLV                      */
    double  coeff_frais_ass_TLV   ; /* frais assiette et de recouvrement TLV  */
    long    seuil_rs              ; /* minimum d'etablissement d'un role
                                       supplementaire                         */

    /* ****    Donnees redevance TV    **** */
    short   frais_redev_metro     ; /* frais_redev_metro                      */
    short   frais_redev_dom       ; /* frais_redev_dom                        */
    short   cotis_redev_metro     ; /* cotis_redev_metro                      */
    short   cotis_redev_dom       ; /* cotis_redev_dom                        */

    s_typseuil   typseuil_metro    ; /* donnees seuil metropole                */
    s_typseuil   typseuil_dom_124  ; /* donnees seuil Guadeloupe, Martinique, Réunion */
    s_typseuil   typseuil_973      ; /* donnees seuil Guyane                   */
    s_typseuil   typseuil_976      ; /* donnees seuil Mayotte                  */
} s_cons;

/*-------- fin de  s_cons : structure des constantes annuelles        --------*/

/*============================================================================
   s_dne : structure des donnees cote entree pour degrevement
  ============================================================================*/

typedef struct {
    short   antax                 ; /* annee de taxation                      */
    char    degex                 ; /* code degrevement                       */
    long    revim                 ; /* revenu imposable                       */
    long    revff                 ; /* revenu imposable du foyer fiscal
                                       apres deduction des abattements        */
    long    somrc                 ; /* somme a recouvrer                      */
    long    somrp                 ; /* somme a recouvrer de la THP            */
    long    somr3                 ; /* somme a recouvrer de la liquidation
                                       fictive                                */
    long    redtt                 ; /* reduction theorique                    */
    long    redt3                 ; /* reduction theorique de la
                                       liquidation fictive                    */
    char    vlexo                 ; /* presence d'une vl exoneree non
                                       nulle O sinon N                        */
    char    cndl                  ; /* double liquidation cause variation
                                       abatt commune                          */
    char    cudl                  ; /* double liquidation cause variation
                                       abatt cu                               */
    char    csdi[04]              ; /* direction (code dsf)                   */
    short   npaff                 ; /*nombre de part                          */
} s_dne;

/*-------- fin de  s_dne:donnees cote entree pour degrevement         --------*/


/*============================================================================
   s_pfe : structure des donnees  entree pour plafonnement fictif
  ============================================================================*/

typedef struct {
    long    revff                 ; /* revenu imposable du foyer fiscal
                                       apres deduction des abattements        */
    long    somrc                 ; /* somme a recouvrer                      */
    long    redtt                 ; /* reduction theorique                    */
} s_pfe;

/*-------- fin de  s_pfe:donnees  entree pour plafonnement fictif     --------*/


/*============================================================================
   s_pfs : structure des donnees sortie pour plafonnement fictif
  ============================================================================*/

typedef struct {
     long    redta                ; /*reduc plafonnement taux appliquee c     */
     long    degpl                ; /*montant degrevement ou plafonnement     */
} s_pfs;

/*-------- fin de  s_pfs:donnees sortie pour plafonnement fictif      --------*/


/*============================================================================
   s_bne1 :  structure entree du module de calcul des bases nettes
  ============================================================================*/

typedef struct {
    short   antax                 ; /* annee de taxation                      */
    char    tyimp                 ; /* type d'impot H TH ou T THLV            */
    long    vlbpc                 ; /* Valeur locative brute des locaux
                                       principaux, actualisee et
                                       revalorisee                            */
    long    vlbsc                 ; /* Valeur locative brute des locaux
                                       secondaires, actualisee et
                                       revalorisee                            */
    long    vlbri                 ; /* VL brute imposee (minoree abat K3)     */
    long    vlbni                 ; /* VL brute non imposee (abattement K3)   */
    char    tax                   ; /* Code taxation P, S, E ou blanc
                                       (THLV)                                 */
    short   nbpac                 ; /* Nombre de PAC hors ERA Total cumule
                                       en cas de taxation conjointe           */
    short   nbera                 ; /* Nombre d'enfants en residence
                                       alternee (ERA) total cumule en cas
                                       de taxation conjointe                  */
    char    degex                 ; /* Code degrevement ou exoneration
                                       F, M, U, I, A, D, V, S, E, P, R, L
                                       ou blanc; code droit a degrevement
                                       ou a exoneration tenant compte de
                                       la condition de cohabitation et de
                                       la situation a l'IR                    */
    char    codni                 ; /* Code NI a l'IR A ou blanc              */
    char    aff                   ; /*code affectation d'un local             */
    char    permo                 ; /* code type de personne morale           */
    char    grrev                 ; /* code groupe revision                   */
    char    codsh                 ; /* code abattement handicape              */
} s_bne1;

/*-------- Fin de la structure entree d'une cote s_bne1               --------*/


/*============================================================================
   s_bne2 : structure entree des donnees collectives du calcul des bases
   nettes
  ============================================================================*/

typedef struct {

    char    dep[3]                ; /* code departement                       */
    s_colb  colb_c                ; /* donnees communales                     */
    s_colb  colb_s                ; /* donnees syndicales                     */
    double  tisyn                 ; /* Taux d'imposition syndical indique
                                       l'application du taux syndical pour
                                       cette commune                          */
    s_colb  colb_q                ; /* donnees groupement de communes         */
    char    cocnq                 ; /* code 1260 mt commune appartenant cu    */
    s_colb  colb_d                ; /* donnees departement                    */
    s_colb  colb_n                ; /* donnees TSE                            */
    double  titsn                 ; /* Somme des Taux d'imposition TSE
                                       indique application TSE pour la commune*/
    s_colb  colb_g                ; /* donnees TSE Autre                      */
    s_colb  colb_e                ; /* donnees GEMAPI                         */
    double  titgp                 ; /* Taux d'imposition de la TSE Autre      */
    double  timpe                 ; /* Taux d'imposition GEMAPI      */
    s_colb  colb_f                ; /* Commune avant ajustement               */
    s_colb  colb_r                ; /* Intercommunalité avant ajustement      */
    char    indthlv               ; /* indic. de synthese THLV                */
} s_bne2;

/*-------- fin de la structure entree des donnees collectives s_bne2  --------*/


/*============================================================================
   s_bns :  structure sortie s2 du calcul des bases nettes
  ============================================================================*/

typedef struct {
    char    versr                 ; /*lettre version d'un programme           */
    s_cols  cols_c                ; /*donnees communales de la cote           */
    s_cols  cols_s                ; /*donnees syndicales de la cote           */
    s_cols  cols_q                ; /*donnees groupement de communes de la
                                      cote                                    */
    s_cols  cols_d                ; /*donnees departementales de la cote      */
    s_cols  cols_n                ; /*donnees TSE de la cote                  */
    s_cols  cols_f                ; /*donnees commune avant ajustement        */
    s_cols  cols_g                ; /*donnees TSE Autre                       */
    s_cols  cols_r                ; /*donnees intercommunalité                */
    s_cols  cols_e                ; /*donnees GEMAPI                          */
    s_signature * signature       ; /*pointeur sur signature                  */
    s_liberreur * libelle         ; /*pointeur sur erreur                     */
} s_bns ;

/*-------- fin de la structure sortie bns du calcul des bases nettes  --------*/


/*============================================================================
   s_lfe : structure entree des donnees liquidation fictive
  ============================================================================*/

typedef struct {
    short   antax                 ; /* annee de taxation                      */
    char    dep[3]                ; /* code departement                       */
    char    cocnq                 ; /* code 1260 mt commune appartenant cu    */
    char    cneg3                 ; /* code même commune en 2003              */
    char    cnqg3                 ; /* code commune appartenant à même CU
                                       en 2003                                */
    double  tisyn                 ; /* Taux d'imposition syndical indique
                                       l'application du taux syndical pour
                                       cette commune                          */
    double  titsn                 ; /* Somme des Taux d'imposition TSE quand
                                       application de la TSE pour la commune  */
    s_cole  cole_q                ; /* donnees groupement de communes         */
    s_cole  cole_d                ; /* donnees departement                    */
    s_cols  cols_c                ; /* donnees communales année en cours      */
    s_cols  cols_q                ; /* donnees groupement de communes
                                       année en cours                         */
    s_cols  cols_d                ; /* donnees département année en cours     */
    s_cols  cols_n                ; /* donnees TSE                            */
    s_cols  cols_o                ; /* donnees communales 2003                */
    s_cols  cols_i                ; /* donnees groupement de communes 2003    */
    s_cols  cols_p                ; /* donnees departement 2003               */
} s_lfe;

/*-------- fin de la structure entree des donnees liquidation
           fictive collectives s_lfe                                  --------*/


/*============================================================================
   s_lfs :  structure sortie lfs des bases nettes de la double liquidation
  ============================================================================*/

typedef struct {
    s_cols  cols_c                ; /* donnees communales de la cote          */
    s_cols  cols_s                ; /* donnees syndicales de la cote          */
    s_cols  cols_q                ; /* donnees groupement de communes de
                                       la cote                                */
    s_cols  cols_d                ; /* donnees departementales de la cote     */
    s_cols  cols_n                ; /* donnees TSE de la cote                 */
} s_lfs ;

/*-------- fin de la structure sortie lfs des bases nettes de la
           double liquidation                                         --------*/


/*============================================================================
   s_coe1 :  structure entree d'une cote pour le calcul des cotisations
  ============================================================================*/

typedef struct {
    short   antax                 ; /* annee de taxation                      */
    char    dep[3]                ; /* code departement                       */
    char    cne[4]                ; /*code commune de l'aft                   */
    char    tyimp                 ; /* type d'impot H TH ou T THLV            */
    long    vlbsc                 ; /* Valeur locative brute des locaux
                                       secondaires, actualisee et
                                       revalorisee                            */
    char    degex                 ; /* Code degrevement ou exoneration
                                       F, I, A, D, V, S, E, L
                                       ou blanc code droit a degrevement
                                       ou a exoneration tenant compte de
                                       la condition de cohabitation et de
                                       la situation a l'IR */
    char    cnat                  ; /* Code nature FIP du redevable
                                       utilisé pour determiner le taux de
                                       prelevement                            */
    char    tax                   ; /* Code taxation P, S, E ou blanc
                                       (THLV)                                 */
    char    indthlv               ; /* indic. de synthese THLV                */
    char    indmths               ; /* indic. de majoration THS               */
    char    indgem                ;  /*Indicateur délibération GEMAPI         */
    char    indaths               ;  /*Indicateur assujetissement majo THS    */
    char    aff                   ; /* Code affectation du local              */
    s_cols  cols_c                ; /* donnees communales de la cote          */
    s_cols  cols_s                ; /* donnees syndicales de la cote          */
    s_cols  cols_q                ; /* donnees groupement de communes de
                                       la cote                                */
    s_cols  cols_d                ; /* donnees departementales de la cote     */
    s_cols  cols_n                ; /* donnees TSE de la cote                 */
    s_cols  cols_g                ; /* donnees TSE Autre                      */
    s_cols  cols_e                ; /* donnees GEMAPI                         */
} s_coe1;

/*-------- fin de la structure entree d'une cote s_coe1               --------*/


/*============================================================================
   s_coe2 : structure entree des donnees collectives
  ============================================================================*/

typedef struct {
    double  timpc                 ; /* Taux d'imposition communal             */
    double  tisyn                 ; /* Taux d'imposition syndical             */
    double  titsn                 ; /* Taux d'imposition TSE quand
                                       application TSE pour cette commune     */
    double  tiagn                 ; /* Taux d'imposition agglomeration
                                       nouvelle n'existe nulle part
                                       actuellement mais legal.               */
    double  timpq                 ; /* Taux d'imposition groupement de
                                       communes                               */
    double  timpd                 ; /* Taux d'imposition departement          */
    double  titgp                 ; /* Taux d'imposition de la TSE Autre      */
    double  timpe                 ; /* Taux d'imposition GEMAPI               */
    double  timths                ; /* Taux d'imposition majoration THS       */
} s_coe2;

/*-------- fin de la structure entree des donnees collectives s_coe2  --------*/


/*============================================================================
   s_cos :  structure sortie cos du calcul des cotisations
  ============================================================================*/

typedef struct {
    char    versr                 ; /* lettre version d'un programme          */
    long    cotid                 ; /* Cotisation departementale              */
    long    coticm                ; /* Cotisation communale majorée           */
    long    cotic                 ; /* Cotisation communale                   */
    long    cotiq                 ; /* Cotisation groupement de communes      */
    long    cotis                 ; /* Cotisation syndicale                   */
    long    cotin                 ; /* Cotisation taxe speciale
                                       d'equipement nouvelles                 */
    long    cotia                 ; /* Cotisation agglomeration nouvelle      */
    long    cotigp                ; /* Cotisation TSE Autre                  */
    long    cotie                 ; /* Cotisation GEMAPI                      */
    long    frait                 ; /* Total des frais de role                */
    long    frai4                 ; /* Partie des frais de rôle a 4,4 %       */
    long    prelt                 ; /* Prelevement total                      */
    long    preths                ; /* prélèvement additionnel résidences secondaires */
    long    somrc                 ; /* Somme a recouvrer, somme des
                                       montants precedents sauf frai4         */
    long    somrp                 ; /* Somme a recouvrer THP                  */
    long    pre02                 ; /* Prelevement a 0,2 %                    */
    long    pre12                 ; /* Prelevement a 1,2 %                    */
    long    pre17                 ; /* Prelevement a 1,7 %                    */
    long    fgest_cq              ; /* Frais de gestion commune & interco     */
    long    fgest_s               ; /* Frais de gestion syndicat              */
    long    fgest_ng              ; /* Frais de gestion TSE-TSE Autre         */
    long    fgest_e               ; /* Frais de gestion GEMAPI                */
    long    far_cq                ; /* Frais d'assiette commune & interco     */
    long    far_s                 ; /* Frais d'assiette syndicat              */
    long    far_ng                ; /* Frais d'assiette TSE-TSE Autre         */
    long    far_e                 ; /* Frais d'assiette GEMAPI                */
    long    majths				  ; /* majoration THS				          */
    s_signature * signature       ; /* pointeur sur signature                 */
    s_liberreur * libelle         ; /* pointeur sur erreur                    */

} s_cos;

/*-------- fin de la structure sortie cos du calcul general           --------*/


/*============================================================================
   s_ree :  structure entree ree du calcul de la reduction de plafonnement
  ============================================================================*/

typedef struct {
    short   antax                 ; /* annee de taxation                      */
    char    tax                   ; /* Code taxation P, S, E ou blanc
                                       (THLV)                                 */
    char    aff                   ; /*code affectation d'un local             */
    char    permo                 ; /* code type de personne morale           */
    char    grrev                 ; /* code groupe revision                   */
    double  timpc                 ; /* Taux d'imposition communal             */
    double  tisyn                 ; /* Taux d'imposition syndical             */
    double  titsn                 ; /* Taux d'imposition TSE indique
                                       application TSE pour cette commune     */
    double  tiagn                 ; /* Taux d'imposition agglomeration
                                       nouvelle n'existe nulle part
                                       actuellement mais legal.               */
    double  timpq                 ; /* Taux d'imposition groupement de
                                       communes                               */
    double  titgp                 ; /* Taux d'imposition TSE Autre            */
    double  timpd                 ; /* Taux d'imposition departement          */
    double  timpe                 ; /* Taux d'imposition GEMAPI               */
    double  tig00                 ; /* Taux global 2000                       */
    long    vlbsc                 ; /* Valeur locative brute des locaux
                                       secondaires, actualisee et
                                       revalorisee                            */
    char    cocnq                 ; /* Code appartenance d'une commune à
                                       une CU                                 */
    s_cols  cols_c                ; /* donnees communales de la cote          */
    s_cols  cols_q                ; /* donnees groupement de communes de
                                       la cote                                */
    s_cols  cols_d                ; /* donnees departementales de la cote     */
} s_ree;

/*-------- fin de la structure entree ree de la reduction de
           plafonnement                                               --------*/


/*============================================================================
   s_res :  structure sortie res du calcul de la reduction de plafonnement
  ============================================================================*/

typedef struct {
    short   antax                 ; /*annee campagne taxation (millesime)     */
    short   anref                 ; /* annee ref calculette th (millesime)    */
    char versr                    ; /* lettre version d'un programme          */
    long redtt                    ; /* reduction theorique                    */
    s_signature * signature       ; /* pointeur sur signature                 */
    s_liberreur * libelle         ; /* pointeur sur erreur                    */
} s_res;

/*-------- fin de la structure sortie res de la reduction de
           plafonnement                                               --------*/


/*============================================================================
   s_ese;        entree donnees seuil degrevement
  ============================================================================*/

typedef struct {
    short   antax                 ; /*annee campagne taxation (millesime)     */
    char    csdi[04]              ; /*direction (code dsf)                    */
    char    cnua                  ; /*code nature d'affectation red.th        */
    s_rede  rede_1                ; /*structure entree redevable rede         */
    s_rede  rede_2                ; /*structure entree redevable rede         */
} s_ese;

/*-------- fin de ese :entree donnees seuil degrevement               --------*/


/*============================================================================
   s_sse;        sortie seuil degrevement th
  ============================================================================*/

typedef struct {
    short   anref                 ; /* annee ref calculette th (millesime)    */
    char    versr                 ; /* lettre version d'un programme          */
    s_reds  reds_1                ; /* structure sortie redevable reds        */
    s_reds  reds_2                ; /* structure sortie redevable reds        */
    s_reds  reds_c                ; /* structure sortie redevable reds        */
    s_signature * signature       ; /* pointeur sur signature                 */
    s_liberreur * libelle         ; /* pointeur sur erreur                    */
} s_sse;

/*-------- fin de sse : sortie seuil degrevement th                   --------*/


/*============================================================================
   s_rae : structure entree redevance audiovisuelle
  ============================================================================*/

typedef struct {
    short   antax                 ; /* annee campagne taxation (millesime)    */
    char    dep[3]                ; /* code departement                       */
    char    champ                 ; /* code champ redevance                   */
    char    qtvrt                 ; /* code questionnaire redevance retenu
                                       redevance                              */
    char    rgsor                 ; /*rand de sortie exonération droit W      */
    char    degtv                 ; /* code degrevement redevance             */
} s_rae;

/*-------- fin de la structure entree redevance audiovisuelle s_coe2  --------*/


/*============================================================================
   s_ras :  structure sortie redevance audiovisuelle
  ============================================================================*/

typedef struct {
    char    versr                 ; /* lettre version d'un programme          */
    long    cottv                 ; /* Cotisation redevance                   */
    long    fratv                 ; /* Frais redevance                        */
    long    somtv                 ; /* Montant redevance en recouvrement      */
    long    mdgtv                 ; /* Degrevement redevance                  */
    long    nettv                 ; /* Net a payer redevance                  */
    char    roltv[4]              ; /* Code role redevance                    */
    s_signature * signature       ; /* pointeur sur signature                 */
    s_liberreur * libelle         ; /* pointeur sur erreur                    */
} s_ras;

/*-------- fin de la structure sortie redevance audiovisuelle         --------*/


/*============================================================================
   reservation des variables pour ILIAD
  ============================================================================*/

#if defined(ILIAD)
    s_e1 e1;
    s_s1 s1;
    s_e2 e2;
    s_s2 s2;
    s_e3 e3;
    s_e4 e4;
    s_s4 s4;
    s_e5 e5;
    s_s5 s5;
    s_e6 e6;
    s_s6 s6;
    s_e7 e7;
    s_s7 s7;
#endif

#endif

#if !defined(LA_SIGNATURE)
    extern const s_signature la_signature ={ SIGNATURE } ;
    #define LA_SIGNATURE
#endif
