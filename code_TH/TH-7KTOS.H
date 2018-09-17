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

#define ktos 'B'
/*============================================================
           s_liberreur; structure lib erreur calculette th
============================================================*/
typedef struct {
     char    libelle[68];        /*zone libelle erreur calculette th   */
                 }
           s_liberreur  ;
/*- fin de liberreur    :structure lib erreur calculette th  --*/
/*============================================================
           s_erreur;    structure d'erreur  calculette th
============================================================*/
typedef struct {
     long    code;               /*code retour calculette th           */
     s_liberreur     liberreur_;      /*structure lib erreur calculette th  */
                 }
           s_erreur     ;
/*- fin de erreur       :structure d'erreur  calculette th   --*/
/*============================================================
           s_signature; structure signature  calculette th
============================================================*/
typedef struct {
     char    signature[32];      /*zone signature      calculette th   */
                 }
           s_signature  ;
/*- fin de signature    :structure signature  calculette th  --*/
/*============================================================
           s_e1;        entree local a actualiser
============================================================*/
typedef struct {
     char    csdi[04];           /*code dsf de gestion                 */
     char    dep[03];            /*code departement                    */
     char    cne[04];            /*code commune de l'aft               */
     short   antax;              /*annee campagne taxation (millesime) */
     char    aff;                /*code affectation d'un local         */
     long    vlb7l;              /*vl base 70 local en comp pour c     */
     long    vlb7c;              /*vl base 70 locaux communs en c      */
     char    cxloz[03];          /*nature local meuble exonere zrr     */
     short   txzrr;              /*taux d'exoneration local meuble zrr */
                 }
           s_e1         ;
/*- fin de e1           :entree local a actualiser           --*/
/*============================================================
           s_s1;        sortie local actualise et revalorise
============================================================*/
typedef struct {
     short   anref;              /*annee ref calculette th (millesime) */
     char    versr;              /*lettre version d'un programme       */
     long    vlbll;              /*vl act/revalorisee du local en c    */
     long    vlblc;              /*vl act/reval des locaux communs en c*/
     long    vlblt;              /*vl act/reval. tot local en c imposee*/
     long    vlxm1;              /*vl exoneree zrr m1 du local en c    */
     long    vlxm2;              /*vl exoneree zrr m2 du local en c    */
     long    vlxm3;              /*vl exoneree zrr m3 du local en c    */
     s_signature * signature;    /*pointeur sur signature              */
     s_liberreur * libelle;      /*pointeur sur erreur                 */
                 }
           s_s1         ;
/*- fin de s1           :sortie local actualise et revalorise--*/
/*============================================================
           s_cols;      structure sortie collectivite cols
============================================================*/
typedef struct {
     long    abgen;              /*abatt general base  par collectivite*/
     long    abpac;              /*abatt total pac par collectivite    */
     long    abp12;              /*abatt pour pac1-2 par collectivite  */
     long    abspe;              /*abatt special base par collectivite */
     long    abhan;              /*abatt special handicape collectivite*/
     long    vlntt;              /*valeur locative nette  collectivite */
     long    vlnex;              /*vl nette exoneree par collectivite  */
     long    bnimp;              /*base nette d'imposition par collecti*/
     char    absps;              /*code abat. special a la base simule */
                 }
           s_cols       ;
/*- fin de cols         :structure sortie collectivite cols  --*/
/*============================================================
           s_e2;        entree donnees individuelles th
============================================================*/
typedef struct {
     short   antax;              /*annee campagne taxation (millesime) */
     char    tyimp;              /*type d'impot                        */
     long    vlbpc;              /*vl actualisee cote thp en comp      */
     long    vlbsc;              /*vl actualisee cote ths en comp      */
     char    aff;                /*code affectation d'un local         */
     char    permo;              /*code type de personne morale        */
     char    grrev;              /*code groupe revision                */
     char    indaths;            /*Assujetissement majo THS (O ou blc) */
     char    tax;                /*code taxation du local              */
     short   nbpac;              /*nombre total pac th hors era        */
     short   nbera;              /*nombre enfants en residence alternee*/
     char    degex;              /*code degrevement art role th        */
     char    rgsor;              /*rang de sortie exonération droit W  */
     char    abatk;              /*code abattement th (K ou blanc)     */
     char    rgsork;             /*rang de sortie abattement (blc ou 3)*/
     char    champ;              /*code champ / hors champ redevance tv*/
     char    qtvrt;              /*reponse quest tv foyer fiscal retenu*/
     char    degtv;              /*droit a deg redevance article th    */
     char    codni;              /*code non imposable ir pour la th    */
     char    codsh;              /*code abattement handicape applique  */
     long    revim;              /*revenu imposable ir retenu pour th  */
     long    revff;              /*revenu imposable ir foyer apres abat*/
     char    cnat;               /*code nature fip                     */
     char    regul;              /*code de regularisation du role th   */
     long    napin;              /*montant du net a payer initial      */
     char    csdi[04];           /*code dsf de gestion                 */
     short   npaff;              /*nb parts ir foyer fiscal calculette */
                 }
           s_e2         ;
/*- fin de e2           :entree donnees individuelles th     --*/
/*============================================================
           s_s2;        sortie contribuable calculette th
============================================================*/
typedef struct {
     short   anref;              /*annee ref calculette th (millesime) */
     char    versr;              /*lettre version d'un programme       */
     long    vlbri;              /*VL brute imposee (minoree abat K3)  */
     long    vlbni;              /*VL brute non imposee (abattement K3 */
     long    cotid;              /*cotisation departementale           */
     long    cotic;              /*cotisation communale                */
     long    cotiq;              /*cotisation intercommunale           */
     long    cotis;              /*cotisation syndicale                */
     long    cotin;              /*cotisation TSE                      */
     long    cotia;              /*cotisation agglomeration nouvelle   */
     long    cotigp;             /*cotisation TSE Autre                */
     long    cotie;     		 /*Cotisation GEMAPI                   */
     long    redta;              /*reduc plafonnement taux appliquee c */
     long    redaa;              /*reduc plafonnement abatt appliquee c*/
     long    redsa;              /*reduc plafonnement seuil appliquee c*/
     long    frait;              /*total des frais de role             */
     long    frai4;              /*total des frais de role a 4,4 %     */
     long    prelt;              /*montant total des prelevements      */
     long    majths;			 /*Montant majoration THS              */
     long    coticm;			 /*Cotisation communale majorée        */
     long    somrc;              /*total des sommes a recouvrer        */
     long    degpl;              /*montant degrevement ou plafonnement */
     long    totth;              /*montant du total th                 */
     long    netth;              /*montant du net a payer th           */
     long    somrp;              /*montant somme a recouvrer thp ou the*/
     long    redtt;              /*reduction plafonnement theorique c  */
     long    pre02;              /*montant du prelevement a 0,2 %      */
     long    pre12;              /*montant du prelevement  a 1,2 %     */
     long    pre17;              /*montant du prelevement a 1,7 %      */
     char    codro[03];          /*code role th                        */
     long    cottv;              /*cotisation redevance tv             */
     long    fratv;              /*frais de role redevance tv          */
     long    somtv;              /*somme a recouvrer redevance tv      */
     long    mdgtv;              /*mont. degrevement redevance  tv     */
     long    nettv;              /*montant du net a payer tv           */
     char    roltv[04];          /*code role tv                        */
     long    netap;              /*montant du net a payer th+tv        */
     long    plart;              /*plafonnement apres reduction taux   */
     char    signe;              /*signe variation nap regularisation  */
     long    napdt;              /*valeur absolue delta net a payer    */
     char    cndl;               /*indic double liquidation th 2003    */
     char    cudl;               /*indic double liquidation th 2003    */
     long    somr3;              /*total des sommes a recouvrer 2003   */
     long    dgpl3;              /*montant plafonnement fictif 2003    */
     long    redt3;              /*reduc plafonnement theorique c 2003 */
     long    majoc;              /*montant majoration plafonnement prov*/
     long    pmajc;              /*montant plafonnement ap majoration  */
     long    preths;             /*prlvt additionnel resid secondaires */
     long    fgest_cq;           /*montant des fgest commune-interco   */
     long    fgest_s;            /*montant des fgest syndicat          */
     long    fgest_ng;           /*montant des fgest tse-tse gp        */
     long    fgest_e;			 /*Montant des FGEST GEMAPI            */
     long    far_cq;             /*montant des far commune-interco     */
     long    far_s;              /*montant des far syndicat            */
     long    far_ng;             /*montant des far tse-tse gp          */
     long    far_e;			     /*Montant des far GEMAPI              */
     s_cols          cols_c;          /*structure sortie commune                            */
     s_cols          cols_q;          /*structure sortie intercommunalite                   */
     s_cols          cols_d;          /*structure sortie departement                        */
     s_cols          cols_n;          /*structure sortie TSE                                */
     s_cols          cols_o;          /*structure sortie commune fictive 2003               */
     s_cols          cols_y;          /*structure sortie syndicat fictive 2003              */
     s_cols          cols_i;          /*structure sortie intercommunalite fictive 2003      */
     s_cols          cols_p;          /*structure sortie departement fictive 2003           */
     s_cols          cols_v;          /*structure sortie TSE fictive 2003                   */
     s_cols          cols_s;          /*structure sortie syndicat                           */
     s_cols          cols_f;          /*structure sortie commune avant ajustement           */
     s_cols          cols_g;          /*structure sortie TSE Autre                          */
     s_cols          cols_b;          /*structure sortie TSE Autre fictive 2003             */
     s_cols          cols_r;          /*structure sortie intercommunalite avant ajustement  */
     s_cols          cols_e;          /*structure sortie GEMAPI           */
     s_cols          cols_a;          /*structure sortie GEMAPI fictive 2003                */
     s_signature * signature;    /*pointeur sur signature              */
     s_liberreur * libelle;      /*pointeur sur erreur                 */
                 }
           s_s2         ;
/*- fin de s2           :sortie contribuable calculette th   --*/
/*============================================================
           s_cole;      structure entree collectivite cole
============================================================*/
typedef struct {
     char    txbas[03];          /*taux abattement general             */
     char    tpac1[03];          /*taux abattement pac 1 ou 2          */
     char    tpac3[03];          /*taux abattement pac 3 ou +          */
     char    txspe[03];          /*taux abattement special base        */
     char    txhan[03];          /*taux abattement special handicape   */
     long    vlmoy;              /*vl moyenne collectivite en comp     */
     long    abspe;              /*quotite abattement special en comp  */
     long    abbas;              /*quotite abattement general en comp  */
     long    apac1;              /*quotite abattement pac 1 / 2 en comp*/
     long    apac3;              /*quotite abattement pac 3 / + en comp*/
     long    abhan;              /*quotite abattement handicape en comp*/
                 }
           s_cole       ;
/*- fin de cole         :structure entree collectivite cole  --*/
/*============================================================
           s_e3;        entree donnees collectives th
============================================================*/
typedef struct {
     short   antax;              /*annee campagne taxation (millesime) */
     char    dep[03];            /*code departement                    */
     char    cne[04];            /*code commune de l'aft               */
     s_cole          cole_c;          /*structure entree commune                                         */
     s_cole          cole_q;          /*structure entree intercommunalite                                */
     s_cole          cole_d;          /*structure entree departement                                     */
     s_cole          cole_o;          /*structure entree commune fictive 2003                            */
     s_cole          cole_i;          /*structure entree intercommunalite fictive 2003                   */
     s_cole          cole_p;          /*structure entree departement fictive 2003                        */
     s_cole          cole_f;          /*structure entree commune avant ajustement                        */
     s_cole          cole_j;          /*structure entree commune avant ajustement fictive 2003           */
     s_cole          cole_r;          /*structure entree intercommunalite avant ajustement               */
     s_cole          cole_m;          /*structure entree intercommunalite avant ajustement fictive 2003  */
     char    cocnq;              /*code 1260 mt commune appartenant cu */
     long    abmos;              /*quotite minimale alsace-moselle c   */
     char    codef;              /*code 1260 al-mos rap norm           */
     char    codeg;              /*code 1260 al-mos rang rap           */
     long    vmd89;              /*valeur locative moyenne dom 1989 c  */
     char    cneg3;              /*indic. commune identique en 2003    */
     char    cnqg3;              /*ind. commune appart. meme cu en 2003*/
     char    cnam3;              /*indic. commune regime a-m en 2003   */
     double  timpc;              /*taux imp commune/ifp ...            */
     double  tisyn;              /*taux imp syndicat en c              */
     double  titsn;              /*somme taux imp tse en c             */
     double  tiagn;              /*taux imp agn en c                   */
     double  tig00;              /*taux imposition global 2000         */
     double  timpq;              /*taux imp groupement en c            */
     double  timpd;              /*taux imp dep annee n                */
     double  titgp;              /*taux d'imposition TSE Autre         */
     double  timpe;				 /*taux d'imposition commune/ifp GEMAPI*/
     char    indthlv;            /*code origine de la thlv             */
     char    indgem;             /*Indicateur délibération GEMAPI      */
     char    indmths;            /*Indicateur majoration THS           */
     double  timths;			 /*taux d'imposition majoration THS    */
                 }
           s_e3         ;
/*- fin de e3           :entree donnees collectives th       --*/
/*============================================================
           s_rede;      structure entree redevable rede
============================================================*/
typedef struct {
     long    revfe;              /*revenu imposable ir en franc ou euro*/
     short   nbpar;              /*nombre de parts ir calculette       */
     char    front;              /*code indicateur frontalier          */
     short   annir;              /*annee tax ir redevable th millesime */
                 }
           s_rede       ;
/*- fin de rede         :structure entree redevable rede     --*/
/*============================================================
           s_reds;      structure sortie redevable reds
============================================================*/
typedef struct {
     char    seuil;              /*code seuil th fonction du revenu ir */
     long    revim;              /*revenu imposable ir retenu pour th  */
     char    serfrc;             /*code seuil th 1414a 1417ii          */
     char    seuilb;             /*code seuil th article 1417-I bis    */
                 }
           s_reds       ;
/*- fin de reds         :structure sortie redevable reds     --*/
/*============================================================
           s_e4;        entree donnees seuil degrevement
============================================================*/
typedef struct {
     short   antax;              /*annee campagne taxation (millesime) */
     char    csdi[04];           /*code dsf de gestion                 */
     char    cnua;               /*code nature d'affectation red.th    */
     s_rede          rede_1;          /*structure entree redevable rede     */
     s_rede          rede_2;          /*structure entree redevable rede     */
     s_rede          rede_r;          /*structure entree redevable rede     */
                 }
           s_e4         ;
/*- fin de e4           :entree donnees seuil degrevement    --*/
/*============================================================
           s_s4;        sortie seuil degrevement th
============================================================*/
typedef struct {
     short   anref;              /*annee ref calculette th (millesime) */
     char    versr;              /*lettre version d'un programme       */
     long    revff;              /*revenu imposable ir foyer apres abat*/
     short   npaff;              /*nb parts ir foyer fiscal calculette */
     s_reds          reds_1;          /*structure sortie redevable reds     */
     s_reds          reds_2;          /*structure sortie redevable reds     */
     s_reds          reds_c;          /*structure sortie redevable reds     */
     s_reds          reds_f;          /*structure sortie redevable reds     */
     s_signature * signature;    /*pointeur sur signature              */
     s_liberreur * libelle;      /*pointeur sur erreur                 */
                 }
           s_s4         ;
/*- fin de s4           :sortie seuil degrevement th         --*/
/*============================================================
           s_site;      structure entree redevable site
============================================================*/
typedef struct {
     char    degex;              /*code degrevement art role th        */
     long    somrp;              /*montant somme a recouvrer thp ou the*/
     long    revim;              /*revenu imposable ir retenu pour th  */
     short   nbpar;              /*nombre de parts ir calculette       */
     short   nbpa2;              /*nb de parts ir calculette concubin  */
     char    seuil;              /*code seuil th fonction du revenu ir */
     char    cohed;              /*code cohabitation a editer          */
     char    cohae;              /*ind. origine maj cohabitation edite */
                 }
           s_site       ;
/*- fin de site         :structure entree redevable site     --*/
/*============================================================
           s_cume;      structure entree redevable cume
============================================================*/
typedef struct {
     char    indp;               /*indicateur presence thp taxee avant */
     long    mdegc;              /*montant cumule des degrevements en c*/
     long    smrpc;              /*cumul somme a recouvrer thp/the en c*/
                 }
           s_cume       ;
/*- fin de cume         :structure entree redevable cume     --*/
/*============================================================
           s_dns;       structure sortie the dns
============================================================*/
typedef struct {
     long    redta;              /*reduc plafonnement taux appliquee c */
     long    redaa;              /*reduc plafonnement abatt appliquee c*/
     long    redsa;              /*reduc plafonnement seuil appliquee c*/
     long    plart;              /*plafonnement apres reduction taux   */
     long    degpl;              /*mont. degrevement ou plaf  the c    */
     long    dgpl3;              /*montant plafonnement fictif 2003    */
     long    totth;              /*mont. du total th        en    c    */
     long    netth;              /*montant du net a payer th           */
     char    codro[03];          /*code role th                        */
     long    majoc;              /*montant majoration plafonnement prov*/
     long    pmajc;              /*montant plafonnement ap majoration  */
     s_signature * signature;    /*pointeur sur signature              */
     s_liberreur * libelle;      /*pointeur sur erreur                 */
                 }
           s_dns        ;
/*- fin de dns          :structure sortie the dns            --*/
/*============================================================
           s_e5;        entree donnee calcul degrevement the
============================================================*/
typedef struct {
     short   antax;              /*annee campagne taxation (millesime) */
     char    degex;              /*code degrevement art role th        */
     long    revim;              /*revenu imposable ir retenu pour th  */
     long    revff;              /*revenu imposable ir foyer apres abat*/
     long    somrp;              /*montant somme a recouvrer thp ou the*/
     long    redtc;              /*reduction plafonnement theo courante*/
     long    sdrpa;              /*somme rec  principal anterieur      */
     long    mdega;              /*somme deg principaux anterieurs     */
     long    mdgta;              /*somme deg princ ant apres reduc taux*/
     long    redat;              /*reduc plafonnements anterieures theo*/
     long    redtr;              /*reduc reelle plaf taux anterieur    */
     long    redar;              /*reduc reelle plaf abat anterieur    */
     long    redsr;              /*reduc reelle plaf seuil anterieur   */
     char    regul;              /*code de regularisation du role th   */
     long    napin;              /*montant du net a payer initial      */
     char    csdi[04];           /*code dsf de gestion                 */
     short   npaff;              /*nb parts ir foyer fiscal calculette */
                 }
           s_e5         ;
/*- fin de e5           :entree donnee calcul degrevement the--*/
/*============================================================
           s_s5;        sortie calcul degrevement the
============================================================*/
typedef struct {
     short   anref;              /*annee ref calculette th (millesime) */
     char    versr;              /*lettre version d'un programme       */
     s_dns           dns_5;      /*structure sortie the dns            */
     char    signe;              /*signe variation nap regularisation  */
     long    napdt;              /*valeur absolue delta net a payer    */
                 }
           s_s5         ;
/*- fin de s5           :sortie calcul degrevement the       --*/
/*============================================================
           s_e6;        entree calcul base et taux tlv
============================================================*/
typedef struct {
     s_e1            e1_l;       /*entree local a actualiser           */
     short   atlvl;              /*annee reference tlv en comp         */
     short   atlvc;              /*annee app. tlv de la commune en comp*/
                 }
           s_e6         ;
/*- fin de e6           :entree calcul base et taux tlv      --*/
/*============================================================
           s_s6;        sortie calcul base et taux tlv
============================================================*/
typedef struct {
     short   anref;              /*annee ref calculette th (millesime) */
     char    versr;              /*lettre version d'un programme       */
     long    vlbll;              /*vl act/revalorisee du local en c    */
     long    vlblc;              /*vl act/reval des locaux communs en c*/
     long    vlblt;              /*vl act/reval. tot local en c imposee*/
     long    txtlv;              /*taux d'imposition tlv en comp       */
     s_signature * signature;    /*pointeur sur signature              */
     s_liberreur * libelle;      /*pointeur sur erreur                 */
                 }
           s_s6         ;
/*- fin de s6           :sortie calcul base et taux tlv      --*/
/*============================================================
           s_e7;        entree calcul de la tlv
============================================================*/
typedef struct {
     short   antax;              /*annee campagne taxation (millesime) */
     long    btlv1;              /*base locaux tlv taxee taux 12,5% à compter de 2013) comp*/
     long    btlv2;              /*base locaux tlv taxee taux 25% à compter de 2013)   comp*/
                 }
           s_e7         ;
/*- fin de e7           :entree calcul de la tlv             --*/
/*============================================================
           s_s7;        sortie calcul de la tlv
============================================================*/
typedef struct {
     short   anref;              /*annee ref calculette th (millesime) */
     char    versr;              /*lettre version d'un programme       */
     long    cotv1;              /*mt cotis tlv taxee a 12,5%          */
     long    cotv2;              /*mt cotis tlv taxee a 25%            */
     long    cotiv;              /*somme des cotisations tlv           */
     long    frait;              /*total des frais de role             */
     long    frai5;              /*total des frais de role a 5,4 %     */
     long    totlv;              /*montant du total tlv                */
     long    netap;              /*montant du net a payer th+tv        */
     char    codro[03];          /*code role th                        */
     s_signature * signature;    /*pointeur sur signature              */
     s_liberreur * libelle;      /*pointeur sur erreur                 */
                 }
           s_s7         ;
/*- fin de s7           :sortie calcul de la tlv             --*/
/*============================================================
           s_colb;      structure sortie collectivite colb
============================================================*/
typedef struct {
     char    txbas[03];          /*taux abattement general             */
     char    tpac1[03];          /*taux abattement pac 1 ou 2          */
     char    tpac3[03];          /*taux abattement pac 3 ou +          */
     char    txspe[03];          /*taux abattement special base        */
     char    txhan[03];          /*taux abattement special handicape   */
     long    vlmoy;              /*vl moyenne collectivite en comp     */
     long    abspe;              /*quotite abattement special en comp  */
     long    abhan;              /*quotite abattement handicape en comp*/
     long    abbas;              /*quotite abattement general en comp  */
     long    apac1;              /*quotite abattement pac 1 / 2 en comp*/
     long    apac3;              /*quotite abattement pac 3 / + en comp*/
     long    abmos;              /*quotite minimale alsace-moselle c   */
     char    codef;              /*code 1260 al-mos rap norm           */
     char    codeg;              /*code 1260 al-mos rang rap           */
                 }
           s_colb       ;
/*- fin de colb         :structure sortie collectivite colb  --*/
/*============================================================
           s_e8;        entree determin double liquid kta
============================================================*/
typedef struct {
     short   antax;              /*annee campagne taxation (millesime) */
     char    dep[03];            /*code departement                    */
     char    csdi[04];           /*code direction de gestion           */
     s_cole          cole_c;          /*structure entree commune                                         */
     s_cole          cole_q;          /*structure entree intercommunalite                                */
     s_cole          cole_d;          /*structure entree departement                                     */
     s_cole          cole_o;          /*structure entree commune fictive 2003                            */
     s_cole          cole_i;          /*structure entree intercommunalite fictive 2003                   */
     s_cole          cole_p;          /*structure entree departement fictive 2003                        */
     s_cole          cole_f;          /*structure entree commune avant ajustement                        */
     s_cole          cole_j;          /*structure entree commune avant ajustement fictive 2003           */
     s_cole          cole_r;          /*structure entree intercommunalite avant ajustement               */
     s_cole          cole_m;          /*structure entree intercommunalite avant ajustement fictive 2003  */
     double  tisyn;              /*taux imp syndicat en c              */
     char    cocnq;              /*code 1260 mt commune appartenant cu */
     double  titsn;              /*somme taux imp tse en c             */
     long    abmos;              /*quotite minimale alsace-moselle c   */
     char    codef;              /*code 1260 al-mos rap norm           */
     char    codeg;              /*code 1260 al-mos rang rap           */
     long    vmd89;              /*valeur locative moyenne dom 1989 c  */
     char    cneg3;              /*indic. commune identique en 2003    */
     char    cnqg3;              /*ind. commune appart. meme cu en 2003*/
     char    cnam3;              /*indic. commune regime a-m en 2003   */
     double  titgp;              /*taux d'imposition TSE Autre         */
     double  timpe;              /*taux d'imposition commune/ifp GEMAPI*/
     char    indgem;             /*Indicateur délibération GEMAPI      */
                 }
           s_e8         ;
/*- fin de e8           :entree determin double liquid kta   --*/
/*============================================================
           s_s8;        sortie determin double liquid kta
============================================================*/
typedef struct {
     short   anref;              /*annee ref calculette th (millesime) */
     char    versr;              /*lettre version d'un programme       */
     s_colb          colb_c;          /*structure sortie commune                                         */
     s_colb          colb_s;          /*structure sortie syndicat                                        */
     s_colb          colb_q;          /*structure sortie intercommunalite                                */
     s_colb          colb_d;          /*structure sortie departement                                     */
     s_colb          colb_n;          /*structure sortie TSE                                             */
     s_colb          colb_o;          /*structure sortie commune fictive 2003                            */
     s_colb          colb_y;          /*structure sortie syndicat fictive 2003                           */
     s_colb          colb_i;          /*structure sortie intercommunalite fictive 2003                   */
     s_colb          colb_p;          /*structure sortie departement fictive 2003                        */
     s_colb          colb_v;          /*structure sortie TSE fictive 2003                                */
     s_colb          colb_f;          /*structure sortie commune avant ajustement                        */
     s_colb          colb_j;          /*structure sortie commune avant ajustement fictive 2003           */
     s_colb          colb_g;          /*structure sortie TSE Autre                                       */
     s_colb          colb_b;          /*structure sortie TSE Autre fictive 2003                          */
     s_colb          colb_r;          /*structure sortie intercommunalite avant ajustement               */
     s_colb          colb_m;          /*structure sortie intercommunalite avant ajustement fictive 2003  */
     s_colb          colb_e;          /*structure sortie GEMAPI           */
     s_colb          colb_a;          /*structure sortie GEMAPI fictive 2003                */
     char    cndl;               /*indic double liquidation th 2003    */
     char    cudl;               /*indic double liquidation th 2003    */
     s_signature * signature;    /*pointeur sur signature              */
     s_liberreur * libelle;      /*pointeur sur erreur                 */
                 }
           s_s8         ;
/*- fin de s8           :sortie determin double liquid kta   --*/
/*============================================================
           s_e9;        entree donnees collectives act reval
============================================================*/
typedef struct {
     short   antax;              /*annee campagne taxation (millesime) */
     char    dep[03];            /*code departement                    */
     char    cne[04];            /*code commune de l'aft               */
     char    cxcn1[03];          /*code 1260 exo locaux meubles m1 gite*/
     char    cxcn2[03];          /*code 1260 exo locaux meubles m2     */
     char    cxcn3[03];          /*code 1260 exo locaux meubles m3     */
                 }
           s_e9         ;
/*- fin de e9           :entree donnees collectives act reval--*/
