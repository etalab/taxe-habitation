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

/* Fichier d'implementation des fonctions communes */
#if defined ILIAD
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif

/* #include  <stdlib.h> */

/* identifiant interne du KARC */
#define KARC 'C'

#define MAXANNEE 4
#define MAXDEP 97 /* nombre de departements*/
#define MAXDEP2 8 /* nombre de communes du departement 21 */
#define MAXDIR 108 /* nombre de directions */
#define MAXDEPTLV 28   /*nombre de departements 'vacants' à partir de 2013*/
#define MAXDEPV2011 16   /*nombre de departements 'vacants' avant 2013*/

/*-------------------- Ne rien inserer avant cette ligne  --------------------*/

int limit_plaf = 15;

/*============================================================================
   Contrôle de la signature
   la presence d'un commentaire dans la fonction permet de récupérer la valeur
   de la signature dans la compilation sous GCOS
  ============================================================================*/

int controle_signature(int  rang_module , char ident_module, s_liberreur * * p_liberreur)
{
    int retour;

    if (/*definition de la Signature */ SIGNATURE
       [rang_module] != ident_module )
    {
        retour= 3000 + rang_module ;
        cherche_Erreur(retour, p_liberreur);
        return retour;
    }
    else
    {
        if (SIGNATURE [RKTOS] != ktos )
        {
            retour= 3000 + RKTOS ;
            cherche_Erreur(retour, p_liberreur);
            return retour;
        }
        else
        {
            if ( SIGNATURE  [RKFOS] != KFOS )
            {
                retour= 3000 + RKFOS ;
                cherche_Erreur(retour, p_liberreur);
                return retour;
            }
            else
            {
                if (SIGNATURE [RKARC] != KARC )
                {
                    retour= 3000 + RKARC ;
                    cherche_Erreur(retour, p_liberreur);
                    return retour;
                }
                else
                {
                    cherche_Erreur(0, p_liberreur);
                    return 0;
                }
            }
        }
    }
}


/*============================================================================
   Recherche des constantes
  ============================================================================*/

int cherche_const(short annee,s_cons * * p_cons)
{
     static s_cons tabcons[MAXANNEE] = {

        /*====================================================================
           Debut de 2017
          ====================================================================*/
        { /*  antax */       2017 ,
        /*  antax_moins1 */  2016 ,
        /*  antax_moins2 */  2015 ,
        /*  antax_moins3 */  2014 ,
        /*  dat_deg60    */  1956 ,
        /*  dat_deg65_tv */  1939 ,

        /************ Coefficient revalorisation*********/
        /*  reval_metro  */  3.092 ,
        /*  reval_dom    */  2.811 ,
        /*  reval_mayotte*/  1.023 ,

        /************* Donnees de degrevement TH *****/
        /*  seuil_deg    */       8 ,
        /*  seuil_nap    */      12 ,
        /*  seuil_reduc  */      15 ,
        /*  coeff_plaf_1414A */   0.0344 ,
        /*  plaf_quotite_gen */   5000 ,
        /*  plaf_quotite_may */   7000 ,

        /********* Donnees de prelevement TH THLV TLV *******/
        /*  seuil_prel   */     4573  ,
        /*  taux_prelp   */     0.002 ,
        /*  taux_prels1  */     0.012 ,
        /*  seuil_prels  */     7622  ,
        /*  taux_prels2  */     0.017 ,
        /*  taux_preths  */     0.015 ,
        /*  taux_majths  */     0.00  ,
        /*  taux_TLV_1   */     0.125 ,
        /*  taux_TLV_2   */     0.250 ,

		/*****    Déclaration des coefficients de FAR et FGEST ****/
		/*  coeff_FAR_THP_THE_c		*/ 0.01 ,
		/*  coeff_FAR_THP_THE_s		*/ 0.044 ,
		/*  coeff_FAR_THP_THE_t		*/ 0.054 ,
		/*  coeff_FAR_THP_THE_g		*/ 0.01 ,
		/*  coeff_FAR_THS_c			*/ 0.01 ,
		/*  coeff_FAR_THS_s			*/ 0.044 ,
		/*  coeff_FAR_THS_t			*/ 0.054 ,
		/*  coeff_FAR_THS_g			*/ 0.01 ,
		/*  coeff_FGEST_THP_THE_c	*/ 0.01 ,
		/*  coeff_FGEST_THP_THE_s	*/ 0.08 ,
		/*  coeff_FGEST_THP_THE_t	*/ 0.09 ,
		/*  coeff_FGEST_THP_THE_g   */ 0.03 ,
		/*  coeff_FGEST_THS_c		*/ 0.03 ,
		/*  coeff_FGEST_THS_s		*/ 0.08 ,
		/*  coeff_FGEST_THS_t		*/ 0.09 ,
		/*  coeff_FGEST_THS_g		*/ 0.03 ,
        /*  coeff_frais_ass_THLV 	*/ 0.044 ,
        /*  coeff_frais_THLV 		*/ 0.08 ,
        /*  coeff_frais_TLV 		*/ 0.09 ,
        /*  coeff_frais_ass_TLV 	*/ 0.054 ,
        /*  seuil_rs     			*/ 76 ,


        /********* Donnees redevance TV *******/
        /*  frais_redev_metro */  1 ,
        /*  frais_redev_dom */    1 ,
        /*  cotis_redev_metro */  137 ,
        /*  cotis_redev_dom */    87 ,


        /************* seuils de revenu TH *******/

        { /*************       METROPOLE      *******/

        { /************* seuils ni art 1417-I *******/
        /*  revenu_unepart */             10708 ,
        /*  revenu_prem_demipart */        2859 ,
        /*  revenu_deuxieme_demipart */    2859 ,
        /*  revenu_troisieme_demipart */   2859 ,
        /*  revenu_quatrieme_demipart */   2859 ,
        /*  revenu_autre_demipart */       2859 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 13567 ,
        /*  revenu_part_supplem_droit_W */  2859 , },

        { /************* seuils plafonnement  A art 1417-II *******/
        /*  revenu_unepart */              25180 ,
        /*  revenu_prem_demipart */         5883 ,
        /*  revenu_deuxieme_demipart */     4631 ,
        /*  revenu_troisieme_demipart */    4631 ,
        /*  revenu_quatrieme_demipart */    4631 ,
        /*  revenu_autre_demipart */        4631, },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              5461 ,
        /*  abattement_prem_demipart */        1580 ,
        /*  abattement_deuxieme_demipart */    1580 ,
        /*  abattement_troisieme_demipart */   1580 ,
        /*  abattement_quatrieme_demipart */   1580 ,
        /*  abattement_autre_demipart */       2793 , }
        },

        { /************* ANTILLES et REUNION  *******/

        { /************* seuils ni *******/
        /*  revenu_unepart */             12671 ,
        /*  revenu_prem_demipart */        3027 ,
        /*  revenu_deuxieme_demipart */    2859 ,
        /*  revenu_troisieme_demipart */   2859 ,
        /*  revenu_quatrieme_demipart */   2859 ,
        /*  revenu_autre_demipart */       2859 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 15698 ,
        /*  revenu_part_supplem_droit_W */  2859 , },

        { /************* seuils plafonnement  A art 1417-II *******/
        /*  revenu_unepart */             30431,
        /*  revenu_prem_demipart */        6455 ,
        /*  revenu_deuxieme_demipart */    6155 ,
        /*  revenu_troisieme_demipart */   4631 ,
        /*  revenu_quatrieme_demipart */   4631 ,
        /*  revenu_autre_demipart */       4631 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              6557 ,
        /*  abattement_prem_demipart */        1580 ,
        /*  abattement_deuxieme_demipart */    1580 ,
        /*  abattement_troisieme_demipart */   2793 ,
        /*  abattement_quatrieme_demipart */   2793 ,
        /*  abattement_autre_demipart */       2793 , }
        },

        { /*************       GUYANE         *******/

        { /************* seuils ni *******/
        /*  revenu_unepart */             13248 ,
        /*  revenu_prem_demipart */        3647 ,
        /*  revenu_deuxieme_demipart */    2859 ,
        /*  revenu_troisieme_demipart */   2859 ,
        /*  revenu_quatrieme_demipart */   2859 ,
        /*  revenu_autre_demipart */       2859 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 16895 ,
        /*  revenu_part_supplem_droit_W */  2859 , },

        { /************* seuils plafonnement  A art 1417-II *******/
        /*  revenu_unepart */             33349 ,
        /*  revenu_prem_demipart */        6455 ,
        /*  revenu_deuxieme_demipart */    6455 ,
        /*  revenu_troisieme_demipart */   5496 ,
        /*  revenu_quatrieme_demipart */   4631 ,
        /*  revenu_autre_demipart */       4631 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              7281 ,
        /*  abattement_prem_demipart */        1213 ,
        /*  abattement_deuxieme_demipart */    1213 ,
        /*  abattement_troisieme_demipart */   2909 ,
        /*  abattement_quatrieme_demipart */   2909 ,
        /*  abattement_autre_demipart */       2909 , }
        },

        { /*************       MAYOTTE        *******/

        { /************* seuils ni *******/
        /*  revenu_unepart */             19853 ,
        /*  revenu_prem_demipart */        5463 ,
        /*  revenu_deuxieme_demipart */    4283 ,
        /*  revenu_troisieme_demipart */   4283 ,
        /*  revenu_quatrieme_demipart */   4283 ,
        /*  revenu_autre_demipart */       4283 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 16895 ,
        /*  revenu_part_supplem_droit_W */  2859 , },

        { /************* seuils plafonnement  A art 1417-II *******/
        /*  revenu_unepart */             36648 ,
        /*  revenu_prem_demipart */        7094 ,
        /*  revenu_deuxieme_demipart */    7094 ,
        /*  revenu_troisieme_demipart */   6040 ,
        /*  revenu_quatrieme_demipart */   5088 ,
        /*  revenu_autre_demipart */       5088 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              8002 ,
        /*  abattement_prem_demipart */        1333 ,
        /*  abattement_deuxieme_demipart */    1333 ,
        /*  abattement_troisieme_demipart */   3197 ,
        /*  abattement_quatrieme_demipart */   3197 ,
        /*  abattement_autre_demipart */       3197 , }
        }
        },
        /*--------------------------- FIN DE 2017  ---------------------------*/


        /*====================================================================
           Debut de 2016
          ====================================================================*/
        { /*  antax */       2016 ,
        /*  antax_moins1 */  2015 ,
        /*  antax_moins2 */  2014 ,
        /*  antax_moins3 */  2013 ,
        /*  dat_deg60    */  1955 ,
        /*  dat_deg65_tv */  1939 ,

        /************ Coefficient revalorisation*********/
        /*  reval_metro  */  3.080 ,
        /*  reval_dom    */  2.800 ,
        /*  reval_mayotte*/  1.019 ,

        /************* Donnees de degrevement TH *****/
        /*  seuil_deg    */       8 ,
        /*  seuil_nap    */      12 ,
        /*  seuil_reduc  */      15 ,
        /*  coeff_plaf_1414A */   0.0344 ,
        /*  plaf_quotite_gen */   5000 ,
        /*  plaf_quotite_may */   7000 ,

        /********* Donnees de prelevement TH THLV TLV *******/
        /*  seuil_prel   */     4573  ,
        /*  taux_prelp   */     0.002 ,
        /*  taux_prels1  */     0.012 ,
        /*  seuil_prels  */     7622  ,
        /*  taux_prels2  */     0.017 ,
        /*  taux_preths  */     0.015 ,
        /*  taux_majths  */     0.20  ,
        /*  taux_TLV_1   */     0.125 ,
        /*  taux_TLV_2   */     0.250 ,

		/*****    Déclaration des coefficients de FAR et FGEST ****/
		/*  coeff_FAR_THP_THE_c		*/ 0.01 ,
		/*  coeff_FAR_THP_THE_s		*/ 0.044 ,
		/*  coeff_FAR_THP_THE_t		*/ 0.054 ,
		/*  coeff_FAR_THP_THE_g		*/ 0.01 ,
		/*  coeff_FAR_THS_c			*/ 0.01 ,
		/*  coeff_FAR_THS_s			*/ 0.044 ,
		/*  coeff_FAR_THS_t			*/ 0.054 ,
		/*  coeff_FAR_THS_g			*/ 0.01 ,
		/*  coeff_FGEST_THP_THE_c	*/ 0.01 ,
		/*  coeff_FGEST_THP_THE_s	*/ 0.08 ,
		/*  coeff_FGEST_THP_THE_t	*/ 0.09 ,
		/*  coeff_FGEST_THP_THE_g   */ 0.03 ,
		/*  coeff_FGEST_THS_c		*/ 0.03 ,
		/*  coeff_FGEST_THS_s		*/ 0.08 ,
		/*  coeff_FGEST_THS_t		*/ 0.09 ,
		/*  coeff_FGEST_THS_g		*/ 0.03 ,
        /*  coeff_frais_ass_THLV 	*/ 0.044 ,
        /*  coeff_frais_THLV 		*/ 0.08 ,
        /*  coeff_frais_TLV 		*/ 0.09 ,
        /*  coeff_frais_ass_TLV 	*/ 0.054 ,
        /*  seuil_rs     			*/ 76 ,


        /********* Donnees redevance TV *******/
        /*  frais_redev_metro */  1 ,
        /*  frais_redev_dom */    1 ,
        /*  cotis_redev_metro */  136 ,
        /*  cotis_redev_dom */    86 ,


        /************* seuils de revenu TH *******/

        { /*************       METROPOLE      *******/

        { /************* seuils ni art 1417-I *******/
        /*  revenu_unepart */               10697 ,
        /*  revenu_prem_demipart */          2856 ,
        /*  revenu_deuxieme_demipart */      2856 ,
        /*  revenu_troisieme_demipart */     2856 ,
        /*  revenu_quatrieme_demipart */     2856 ,
        /*  revenu_autre_demipart */         2856 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 13553 ,
        /*  revenu_part_supplem_droit_W */   2856 , },

        { /************* seuils plafonnement A art 1417-II *******/
        /*  revenu_unepart */               25155 ,
        /*  revenu_prem_demipart */          5877 ,
        /*  revenu_deuxieme_demipart */      4626 ,
        /*  revenu_troisieme_demipart */     4626 ,
        /*  revenu_quatrieme_demipart */     4626 ,
        /*  revenu_autre_demipart */         4626 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */            5456 ,
        /*  abattement_prem_demipart */      1578 ,
        /*  abattement_deuxieme_demipart */  1578 ,
        /*  abattement_troisieme_demipart */ 1578 ,
        /*  abattement_quatrieme_demipart */ 1578 ,
        /*  abattement_autre_demipart */     2790 , },
        },

        { /************* ANTILLES et REUNION *******/

        { /************* seuils ni art 1417-I *******/
        /*  revenu_unepart */              12658 ,
        /*  revenu_prem_demipart */         3024 ,
        /*  revenu_deuxieme_demipart */     2856 ,
        /*  revenu_troisieme_demipart */    2856 ,
        /*  revenu_quatrieme_demipart */    2856 ,
        /*  revenu_autre_demipart */        2856 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 15682 ,
        /*  revenu_part_supplem_droit_W */   2856 , },

        { /************* seuils plafonnement A art 1417-II *******/
        /*  revenu_unepart */             30401 ,
        /*  revenu_prem_demipart */        6449 ,
        /*  revenu_deuxieme_demipart */    6149 ,
        /*  revenu_troisieme_demipart */   4626 ,
        /*  revenu_quatrieme_demipart */   4626 ,
        /*  revenu_autre_demipart */       4626 , },

        { /************* abattements sur RFR art 1414-A.I  *******/
        /*  abattement_unepart */              6550 ,
        /*  abattement_prem_demipart */        1578 ,
        /*  abattement_deuxieme_demipart */    1578 ,
        /*  abattement_troisieme_demipart */   2790 ,
        /*  abattement_quatrieme_demipart */   2790 ,
        /*  abattement_autre_demipart */       2790 , }
        },

        {/*************       GUYANE         *******/

        { /************* seuils ni art 1417-I *******/
        /*  revenu_unepart */              13235 ,
        /*  revenu_prem_demipart */         3643 ,
        /*  revenu_deuxieme_demipart */     2856 ,
        /*  revenu_troisieme_demipart */    2856 ,
        /*  revenu_quatrieme_demipart */    2856 ,
        /*  revenu_autre_demipart */        2856 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 16878 ,
        /*  revenu_part_supplem_droit_W */   2856 , },

        { /************* seuils plafonnement A art 1417-II *******/
        /*  revenu_unepart */             33316 ,
        /*  revenu_prem_demipart */        6449 ,
        /*  revenu_deuxieme_demipart */    6449 ,
        /*  revenu_troisieme_demipart */   5491 ,
        /*  revenu_quatrieme_demipart */   4626 ,
        /*  revenu_autre_demipart */       4626 , },

        { /************* abattements sur RFR art 1414-A.I  *******/
        /*  abattement_unepart */              7274 ,
        /*  abattement_prem_demipart */        1212 ,
        /*  abattement_deuxieme_demipart */    1212 ,
        /*  abattement_troisieme_demipart */   2906 ,
        /*  abattement_quatrieme_demipart */   2906 ,
        /*  abattement_autre_demipart */       2906 , }
        },

        { /*************       MAYOTTE        *******/

        { /************* seuils ni art 1417-I *******/
        /*  revenu_unepart */              13235 ,
        /*  revenu_prem_demipart */         3643 ,
        /*  revenu_deuxieme_demipart */     2856 ,
        /*  revenu_troisieme_demipart */    2856 ,
        /*  revenu_quatrieme_demipart */    2856 ,
        /*  revenu_autre_demipart */        2856 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 16878 ,
        /*  revenu_part_supplem_droit_W */   2856 , },

        { /************* seuils plafonnement A art 1417-II *******/
        /*  revenu_unepart */             33316 ,
        /*  revenu_prem_demipart */        6449 ,
        /*  revenu_deuxieme_demipart */    6449 ,
        /*  revenu_troisieme_demipart */   5491 ,
        /*  revenu_quatrieme_demipart */   4626 ,
        /*  revenu_autre_demipart */       4626 , },

        { /************* abattements sur RFR art 1414-A.I  *******/
        /*  abattement_unepart */              7274 ,
        /*  abattement_prem_demipart */        1212 ,
        /*  abattement_deuxieme_demipart */    1212 ,
        /*  abattement_troisieme_demipart */   2906 ,
        /*  abattement_quatrieme_demipart */   2906 ,
        /*  abattement_autre_demipart */       2906 , }
        }
        },
        /*--------------------------- FIN DE 2016  ---------------------------*/


        /*====================================================================
           Debut de 2015
          ====================================================================*/
        { /*  antax */       2015 ,
        /*  antax_moins1 */  2014 ,
        /*  antax_moins2 */  2013 ,
        /*  antax_moins3 */  2012,
        /*  dat_deg60    */  1954 ,
        /*  dat_deg65_tv */  1939 ,

        /************ Coefficient revalorisation*********/
        /*  reval_metro  */  3.049 ,
        /*  reval_dom    */  2.772 ,
        /*  reval_mayotte*/  1.009 ,

        /************* Donnees de degrevement TH *****/
        /*  seuil_deg    */       8 ,
        /*  seuil_nap    */      12 ,
        /*  seuil_reduc  */      15 ,
        /*  coeff_plaf_1414A */   0.0344 ,
        /*  plaf_quotite_gen */   5000 ,
        /*  plaf_quotite_may */   7000 ,

        /***** Donnees de prelevement TH THLV TLV et majoration TH*******/
        /*  seuil_prel   */     4573  ,
        /*  taux_prelp   */     0.002 ,
        /*  taux_prels1  */     0.012 ,
        /*  seuil_prels  */     7622  ,
        /*  taux_prels2  */     0.017 ,
        /*  taux_preths  */     0.015 ,
        /*  taux_majths  */     0.20  ,
        /*  taux_TLV_1   */     0.125 ,
        /*  taux_TLV_2   */     0.250 ,

		/*****    Déclaration des coefficients de FAR et FGEST ****/
		/*  coeff_FAR_THP_THE_c		*/ 0.01 ,
		/*  coeff_FAR_THP_THE_s		*/ 0.044 ,
		/*  coeff_FAR_THP_THE_t		*/ 0.054 ,
		/*  coeff_FAR_THP_THE_g		*/ 0.01 ,
		/*  coeff_FAR_THS_c			*/ 0.01 ,
		/*  coeff_FAR_THS_s			*/ 0.044 ,
		/*  coeff_FAR_THS_t			*/ 0.054 ,
		/*  coeff_FAR_THS_g			*/ 0.01 ,
		/*  coeff_FGEST_THP_THE_c	*/ 0.01 ,
		/*  coeff_FGEST_THP_THE_s	*/ 0.08 ,
		/*  coeff_FGEST_THP_THE_t	*/ 0.09 ,
		/*  coeff_FGEST_THP_THE_g	*/ 0.03 ,
		/*  coeff_FGEST_THS_c		*/ 0.03 ,
		/*  coeff_FGEST_THS_s		*/ 0.08 ,
		/*  coeff_FGEST_THS_t		*/ 0.09 ,
		/*  coeff_FGEST_THS_g		*/ 0.03 ,
        /*  coeff_frais_ass_THLV 	*/ 0.044 ,
        /*  coeff_frais_THLV 		*/ 0.08 ,
        /*  coeff_frais_TLV 		*/ 0.09 ,
        /*  coeff_frais_ass_TLV 	*/ 0.054 ,
        /*  seuil_rs     			*/ 76 ,

        /********* Donnees redevance TV *******/
        /*  frais_redev_metro */  1 ,
        /*  frais_redev_dom */    1 ,
        /*  cotis_redev_metro */  135 ,
        /*  cotis_redev_dom */    85 ,


        /************* seuils de revenu TH *******/

        { /*************       METROPOLE      *******/

        { /************* seuils ni art 1417-I *******/
        /*  revenu_unepart */             10686 ,
        /*  revenu_prem_demipart */        2853 ,
        /*  revenu_deuxieme_demipart */    2853 ,
        /*  revenu_troisieme_demipart */   2853 ,
        /*  revenu_quatrieme_demipart */   2853 ,
        /*  revenu_autre_demipart */       2853 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 0 ,
        /*  revenu_part_supplem_droit_W */  0 , },

        { /************* seuils plafonnement A art 1417-II *******/
        /*  revenu_unepart */              25130 ,
        /*  revenu_prem_demipart */         5871 ,
        /*  revenu_deuxieme_demipart */     4621 ,
        /*  revenu_troisieme_demipart */    4621 ,
        /*  revenu_quatrieme_demipart */    4621 ,
        /*  revenu_autre_demipart */        4621 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              5451 ,
        /*  abattement_prem_demipart */        1576 ,
        /*  abattement_deuxieme_demipart */    1576 ,
        /*  abattement_troisieme_demipart */   1576 ,
        /*  abattement_quatrieme_demipart */   1576 ,
        /*  abattement_autre_demipart */       2787 , }
        },

        { /************* ANTILLES et REUNION *******/

        { /************* seuils ni *******/
        /*  revenu_unepart */              12645 ,
        /*  revenu_prem_demipart */         3021 ,
        /*  revenu_deuxieme_demipart */     2853 ,
        /*  revenu_troisieme_demipart */    2853 ,
        /*  revenu_quatrieme_demipart */    2853 ,
        /*  revenu_autre_demipart */        2853 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 0 ,
        /*  revenu_part_supplem_droit_W */  0 , },

        { /************* seuils plafonnement  A art 1417-II *******/
        /*  revenu_unepart */             30371 ,
        /*  revenu_prem_demipart */        6443 ,
        /*  revenu_deuxieme_demipart */    6143 ,
        /*  revenu_troisieme_demipart */   4621 ,
        /*  revenu_quatrieme_demipart */   4621 ,
        /*  revenu_autre_demipart */       4621 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              6543 ,
        /*  abattement_prem_demipart */        1576 ,
        /*  abattement_deuxieme_demipart */    1576 ,
        /*  abattement_troisieme_demipart */   2787 ,
        /*  abattement_quatrieme_demipart */   2787 ,
        /*  abattement_autre_demipart */       2787 , }
        },

        { /*************       GUYANE         *******/

        { /************* seuils ni *******/
        /*  revenu_unepart */              13222 ,
        /*  revenu_prem_demipart */         3639 ,
        /*  revenu_deuxieme_demipart */     2853 ,
        /*  revenu_troisieme_demipart */    2853 ,
        /*  revenu_quatrieme_demipart */    2853 ,
        /*  revenu_autre_demipart */        2853 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 0 ,
        /*  revenu_part_supplem_droit_W */  0 , },

        { /************* seuils plafonnement  A art 1417-II *******/
        /*  revenu_unepart */             33283 ,
        /*  revenu_prem_demipart */        6443 ,
        /*  revenu_deuxieme_demipart */    6443 ,
        /*  revenu_troisieme_demipart */   5486 ,
        /*  revenu_quatrieme_demipart */   4621 ,
        /*  revenu_autre_demipart */       4621 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              7267 ,
        /*  abattement_prem_demipart */        1211 ,
        /*  abattement_deuxieme_demipart */    1211 ,
        /*  abattement_troisieme_demipart */   2903 ,
        /*  abattement_quatrieme_demipart */   2903 ,
        /*  abattement_autre_demipart */       2903 , }
        },

        { /*************       MAYOTTE        *******/

        { /************* seuils ni *******/
        /*  revenu_unepart */              13222 ,
        /*  revenu_prem_demipart */         3639 ,
        /*  revenu_deuxieme_demipart */     2853 ,
        /*  revenu_troisieme_demipart */    2853 ,
        /*  revenu_quatrieme_demipart */    2853 ,
        /*  revenu_autre_demipart */        2853 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 0 ,
        /*  revenu_part_supplem_droit_W */  0 , },

        { /************* seuils plafonnement  A art 1417-II *******/
        /*  revenu_unepart */             33283 ,
        /*  revenu_prem_demipart */        6443 ,
        /*  revenu_deuxieme_demipart */    6443 ,
        /*  revenu_troisieme_demipart */   5486 ,
        /*  revenu_quatrieme_demipart */   4621 ,
        /*  revenu_autre_demipart */       4621 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              7267 ,
        /*  abattement_prem_demipart */        1211 ,
        /*  abattement_deuxieme_demipart */    1211 ,
        /*  abattement_troisieme_demipart */   2903 ,
        /*  abattement_quatrieme_demipart */   2903 ,
        /*  abattement_autre_demipart */       2903 , }
        }
        },
        /*--------------------------- FIN DE 2015  ---------------------------*/

        /*====================================================================
           Debut de 2014
          ====================================================================*/
        { /*  antax */       2014 ,
        /*  antax_moins1 */  2013 ,
        /*  antax_moins2 */  2012 ,
        /*  antax_moins3 */  2011 ,
        /*  dat_deg60    */  1953 ,
        /*  dat_deg65_tv */  1939 ,

        /************ Coefficient revalorisation*********/
        /*  reval_metro              */  3.022 ,
        /*  reval_dom (hors Mayotte) */  2.747 ,
        /*  reval_mayotte            */  1 ,

        /************* Donnees de degrevement TH *****/
        /*  seuil_deg    */       8 ,
        /*  seuil_nap    */      12 ,
        /*  seuil_reduc  */      15 ,
        /*  coeff_plaf_1414A */   0.0344 ,
        /*  plaf_quotite_gen */   5000 ,
        /*  plaf_quotite_may */   7000 ,

        /********* Donnees de prelevement TH THLV TLV *******/
        /*  seuil_prel   */     4573  ,
        /*  taux_prelp   */     0.002 ,
        /*  taux_prels1  */     0.012 ,
        /*  seuil_prels  */     7622  ,
        /*  taux_prels2  */     0.017 ,
        /*  taux_preths  */     0.015 ,
        /*  taux_majths  */     0.00  ,
        /*  taux_TLV_1   */     0.125 ,
        /*  taux_TLV_2   */     0.250 ,

		/*****    Déclaration des coefficients de FAR et FGEST ****/
		/*  coeff_FAR_THP_THE_c		*/ 0.01 ,
		/*  coeff_FAR_THP_THE_s		*/ 0.044 ,
		/*  coeff_FAR_THP_THE_t		*/ 0.054 ,
		/*  coeff_FAR_THP_THE_g		*/ 0.00 ,
		/*  coeff_FAR_THS_c			*/ 0.01 ,
		/*  coeff_FAR_THS_s			*/ 0.044 ,
		/*  coeff_FAR_THS_t			*/ 0.054 ,
		/*  coeff_FAR_THS_g			*/ 0.00 ,
		/*  coeff_FGEST_THP_THE_c	*/ 0.01 ,
		/*  coeff_FGEST_THP_THE_s	*/ 0.08 ,
		/*  coeff_FGEST_THP_THE_t	*/ 0.09 ,
		/*  coeff_FGEST_THP_THE_g   */ 0.00 ,
		/*  coeff_FGEST_THS_c		*/ 0.03 ,
		/*  coeff_FGEST_THS_s		*/ 0.08 ,
		/*  coeff_FGEST_THS_t		*/ 0.09 ,
		/*  coeff_FGEST_THS_g		*/ 0.00 ,
        /*  coeff_frais_ass_THLV 	*/ 0.044 ,
        /*  coeff_frais_THLV 		*/ 0.08 ,
        /*  coeff_frais_TLV 		*/ 0.09 ,
        /*  coeff_frais_ass_TLV 	*/ 0.054 ,
        /*  seuil_rs     			*/ 76 ,



        /********* Donnees redevance TV *******/
        /*  frais_redev_metro */  1 ,
        /*  frais_redev_dom */    1 ,
        /*  cotis_redev_metro */  132 ,
        /*  cotis_redev_dom */    84 ,


        /************* seuils de revenu TH *******/

        { /*************       METROPOLE      *******/

        { /************* seuils ni art 1417-I *******/
        /*  revenu_unepart */             10633 ,
        /*  revenu_prem_demipart */        2839 ,
        /*  revenu_deuxieme_demipart */    2839 ,
        /*  revenu_troisieme_demipart */   2839 ,
        /*  revenu_quatrieme_demipart */   2839 ,
        /*  revenu_autre_demipart */       2839 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 0 ,
        /*  revenu_part_supplem_droit_W */  0 , },

        { /************* seuils plafonnement  A art 1417-II *******/
        /*  revenu_unepart */              25005 ,
        /*  revenu_prem_demipart */         5842 ,
        /*  revenu_deuxieme_demipart */     4598 ,
        /*  revenu_troisieme_demipart */    4598 ,
        /*  revenu_quatrieme_demipart */    4598 ,
        /*  revenu_autre_demipart */        4598 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              5424 ,
        /*  abattement_prem_demipart */        1568 ,
        /*  abattement_deuxieme_demipart */    1568 ,
        /*  abattement_troisieme_demipart */   1568 ,
        /*  abattement_quatrieme_demipart */   1568 ,
        /*  abattement_autre_demipart */       2773 ,}
        },

        { /************* ANTILLES et REUNION *******/

        { /************* seuils ni *******/
        /*  revenu_unepart */              12582 ,
        /*  revenu_prem_demipart */         3006 ,
        /*  revenu_deuxieme_demipart */     2839 ,
        /*  revenu_troisieme_demipart */    2839 ,
        /*  revenu_quatrieme_demipart */    2839 ,
        /*  revenu_autre_demipart */        2839 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 0 ,
        /*  revenu_part_supplem_droit_W */  0 , },

        { /************* seuils plafonnement  A art 1417-II *******/
        /*  revenu_unepart */             30220 ,
        /*  revenu_prem_demipart */        6411 ,
        /*  revenu_deuxieme_demipart */    6112 ,
        /*  revenu_troisieme_demipart */   4598 ,
        /*  revenu_quatrieme_demipart */   4598 ,
        /*  revenu_autre_demipart */       4598 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              6510 ,
        /*  abattement_prem_demipart */        1568 ,
        /*  abattement_deuxieme_demipart */    1568 ,
        /*  abattement_troisieme_demipart */   2773 ,
        /*  abattement_quatrieme_demipart */   2773 ,
        /*  abattement_autre_demipart */       2773 , }
        },

        { /*************       GUYANE         *******/

        { /************* seuils ni *******/
        /*  revenu_unepart */              13156 ,
        /*  revenu_prem_demipart */         3621 ,
        /*  revenu_deuxieme_demipart */     2839 ,
        /*  revenu_troisieme_demipart */    2839 ,
        /*  revenu_quatrieme_demipart */    2839 ,
        /*  revenu_autre_demipart */        2839 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 0 ,
        /*  revenu_part_supplem_droit_W */  0 , },

        { /************* seuils plafonnement  A art 1417-II *******/
        /*  revenu_unepart */             33117 ,
        /*  revenu_prem_demipart */        6411 ,
        /*  revenu_deuxieme_demipart */    6411 ,
        /*  revenu_troisieme_demipart */   5459 ,
        /*  revenu_quatrieme_demipart */   4598 ,
        /*  revenu_autre_demipart */       4598 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              7231 ,
        /*  abattement_prem_demipart */        1205 ,
        /*  abattement_deuxieme_demipart */    1205 ,
        /*  abattement_troisieme_demipart */   2889 ,
        /*  abattement_quatrieme_demipart */   2889 ,
        /*  abattement_autre_demipart */       2889 , }
        },

        { /*************       MAYOTTE        *******/

        { /************* seuils ni *******/
        /*  revenu_unepart */              13156 ,
        /*  revenu_prem_demipart */         3621 ,
        /*  revenu_deuxieme_demipart */     2839 ,
        /*  revenu_troisieme_demipart */    2839 ,
        /*  revenu_quatrieme_demipart */    2839 ,
        /*  revenu_autre_demipart */        2839 , },
        { /***** exoneration droit acquis W - art 1417-I bis *****/
        /*  revenu_jusqu'a_2part_droit_W */ 0 ,
        /*  revenu_part_supplem_droit_W */  0 , },

        { /************* seuils plafonnement  A art 1417-II *******/
        /*  revenu_unepart */             33117 ,
        /*  revenu_prem_demipart */        6411 ,
        /*  revenu_deuxieme_demipart */    6411 ,
        /*  revenu_troisieme_demipart */   5459 ,
        /*  revenu_quatrieme_demipart */   4598 ,
        /*  revenu_autre_demipart */       4598 , },

        { /************* abattements sur RFR art 1414-A.I *******/
        /*  abattement_unepart */              7231 ,
        /*  abattement_prem_demipart */        1205 ,
        /*  abattement_deuxieme_demipart */    1205 ,
        /*  abattement_troisieme_demipart */   2889 ,
        /*  abattement_quatrieme_demipart */   2889 ,
        /*  abattement_autre_demipart */       2889 , }
        }

        },
        /*--------------------------- FIN DE 2014  ---------------------------*/


    };
    /*                     Fin de la table des constantes                     */


    short numann=0;

/* on parcourt la table tabcons, jusqu'a ce que l'on trouve
egalite entre l'annee rentree et un poste de la table ,
ou que la variable de parcours
soit plus grande que le nombre de postes de la table*/

    for(numann=0;(numann < MAXANNEE) && (tabcons[numann].antax != annee);
        numann++);
    if (numann == MAXANNEE)
    {
        return 1;
    }
    else
    {
        *p_cons= &(tabcons[numann]);
        return 0;
    }
}

/*-------------------- Fin de la recherche des constantes --------------------*/


/*          RECHERCHE DU TAUX DEPARTEMENTAL  AFFECTATION AUTRE QUE A          */
/*         ----------------------------------------------------------         */

double recherche_tdep(char di1[], char cne1[], char aff)
{
    int i; /* indice utilise dans les for */
    char * p_tab;
    char * p_dir;
    char * p_di1;
    char * p_cne1;
    char * p_cne2;
    char * p_cne3;
    double res_coef;
    static char di2[]="21";  /*  initialisation char caractere */
    static char di3[]="54";
    char * p_di2;
    char * p_di3;
    static char cne3[]="602";

    /*   structure des taux par departement   */
    static struct coef {
        char direction [3];
        double coefficient;
    }c1[MAXDEP]={
        {"01", 1.60},  {"02", 1.75},  {"03", 1.60},  {"04", 1.65},
        {"05", 1.67},  {"06", 1.55},  {"07", 1.59},  {"08", 1.72},
        {"09", 1.60},  {"10", 1.60},  {"11", 1.55},  {"12", 1.73},
        {"13", 1.60},  {"14", 1.66},  {"15", 1.60},  {"16", 1.68},
        {"17", 1.69},  {"18", 1.57},  {"19", 1.67},  {"2A", 1.58},
        {"2B", 1.58},  {"21", 1.72},  {"22", 1.48},  {"23", 1.63},
        {"24", 1.62},  {"25", 1.70},  {"26", 1.70},  {"27", 1.75},
        {"28", 1.57},  {"29", 1.70},  {"30", 1.50},  {"31", 1.49},
        {"32", 1.41},  {"33", 1.65},  {"34", 1.49},  {"35", 1.57},
        {"36", 1.60},  {"37", 1.57},  {"38", 1.63},  {"39", 1.65},
        {"40", 1.70},  {"41", 1.59},  {"42", 1.68},  {"43", 1.65},
        {"44", 1.65},  {"45", 1.64},  {"46", 1.73},  {"47", 1.57},
        {"48", 1.50},  {"49", 1.63},  {"50", 1.70},  {"51", 1.62},
        {"52", 1.59},  {"53", 1.59},  {"54", 1.63},  {"55", 1.70},
        {"56", 1.57},  {"57", 1.76},  {"58", 1.74},  {"59", 1.69},
        {"60", 1.78},  {"61", 1.70},  {"62", 1.77},  {"63", 1.54},
        {"64", 1.70},  {"65", 1.69},  {"66", 1.62},  {"67", 1.67},
        {"68", 1.69},  {"69", 1.63},  {"70", 1.66},  {"71", 1.67},
        {"72", 1.63},  {"73", 1.63},  {"74", 1.60},  {"75", 1.85},
        {"76", 1.73},  {"77", 1.67},  {"78", 1.73},  {"79", 1.59},
        {"80", 1.75},  {"81", 1.54},  {"82", 1.52},  {"83", 1.62},
        {"84", 1.60},  {"85", 1.52},  {"86", 1.55},  {"87", 1.67},
        {"88", 1.67},  {"89", 1.65},  {"90", 1.74},  {"91", 1.71},
        {"92", 1.77},  {"93", 1.68},  {"94", 1.70},  {"95", 1.70},
        {"97", 1.00}

    }, *p_st,
      c1_a[MAXDEP]={
        {"01", 1.99},  {"02", 2.00},  {"03", 1.90},  {"04", 1.74},
        {"05", 2.01},  {"06", 2.03},  {"07", 1.85},  {"08", 2.25},
        {"09", 1.87},  {"10", 1.93},  {"11", 1.73},  {"12", 1.80},
        {"13", 2.00},  {"14", 2.19},  {"15", 1.90},  {"16", 1.83},
        {"17", 1.87},  {"18", 1.99},  {"19", 2.07},  {"2A", 1.83},
        {"2B", 1.77},  {"21", 2.06},  {"22", 1.93},  {"23", 1.81},
        {"24", 1.93},  {"25", 2.28},  {"26", 2.10},  {"27", 2.20},
        {"28", 2.09},  {"29", 1.95},  {"30", 1.90},  {"31", 1.88},
        {"32", 1.75},  {"33", 2.04},  {"34", 1.88},  {"35", 1.87},
        {"36", 1.80},  {"37", 1.92},  {"38", 2.00},  {"39", 1.96},
        {"40", 1.87},  {"41", 1.94},  {"42", 2.10},  {"43", 1.79},
        {"44", 2.06},  {"45", 2.09},  {"46", 1.94},  {"47", 1.84},
        {"48", 1.80},  {"49", 2.13},  {"50", 1.85},  {"51", 2.15},
        {"52", 2.40},  {"53", 1.80},  {"54", 1.96},  {"55", 1.94},
        {"56", 2.03},  {"57", 1.91},  {"58", 1.98},  {"59", 2.02},
        {"60", 2.00},  {"61", 2.18},  {"62", 2.05},  {"63", 1.91},
        {"64", 1.85},  {"65", 2.00},  {"66", 1.90},  {"67", 2.10},
        {"68", 2.08},  {"69", 2.04},  {"70", 2.20},  {"71", 2.15},
        {"72", 2.13},  {"73", 2.04},  {"74", 1.96},  {"75", 2.23},
        {"76", 2.20},  {"77", 2.23},  {"78", 1.93},  {"79", 1.79},
        {"80", 1.90},  {"81", 1.88},  {"82", 1.83},  {"83", 2.06},
        {"84", 2.04},  {"85", 1.91},  {"86", 1.78},  {"87", 2.03},
        {"88", 1.97},  {"89", 2.08},  {"90", 2.10},  {"91", 1.98},
        {"92", 2.13},  {"93", 2.08},  {"94", 2.05},  {"95", 2.25},
        {"97", 1.00}

    }, *p_sta,*p_rech ;   /*   pointeur sur structure  */





    static struct cote_dor {     /*  structure : list des cnes du dep 21 */
    char commune [4];
    }c2[MAXDEP2]={
        {"166"},{"171"},{"231"},{"278"},
        {"355"},{"515"},{"540"},{"617"}
    }, *p_cotedor;

    int retour;
    p_st=c1;    /*  initialisation du pointeur sur structure  */
    p_sta=c1_a;
    p_di1=di1;   /*  initialisation du pointeur sur char  */
    p_di2=di2;
    p_di3=di3;
    p_cne1=cne1;
    p_cne3=cne3;
    res_coef=0;
    p_cotedor=c2;

    /* verification du code affectation */
    if (strchr("HMFSA ",aff)==NULL)
    {
        return(4401);
    }

    /* boucle de recherche de la direction */
    /* ----------------------------------- */
    if (aff == 'A')
    {
        p_rech=p_sta; /* Recherche sur le deuxième taux */
    }
    else
    {
        p_rech=p_st; /* Recherche sur le premier taux */
    }

    for(i=0;i<MAXDEP;i++)
    {
        p_dir=di1;  /* init. pointeur  sur tab de char */
        p_tab=p_rech->direction; /* init. ptr pour dir de struct */

        retour=strcmp(p_dir, p_tab); /* passage des adresses */
        if (retour==0)
        {
            res_coef=p_rech->coefficient; /* mise en reserve du coef*/
        }
        p_rech = p_rech+1;  /* incrementation du pointeur sur struct */
    }

    /*   departement non trouve  */
    /*   ----------------------  */

    if (res_coef==0)
    {
        return(9);  /*  retour si dep non trouve  */
    }
    else
    {

    /*  departement trouve */
    /*  ------------------ */

        retour=strcmp(p_di1 , p_di3);   /*comparaison avec dir 540*/
        if (retour==0)                  /* departement 540        */
        {
            retour=strcmp(p_cne1 , p_cne3); /*comparaison avec cne 602*/
            if (retour==0)                /* commune 602            */
            {
                if (aff == 'A')
                {
                    res_coef=1.94;
                }
                else
                {
                    res_coef=1.70;
                }
            }
            return(res_coef);
        }
        else
        {
            retour=strcmp(p_di1 , p_di2); /* comparaison chaine de caracteres */

            /*   trait-ktac autre que dep 21  */
            /*   ---------------------------  */

            if  (retour!=0) /* test de non egalite */
            {
                return(res_coef); /* retour coef si dep # 21 */
            }

            /*   traitement dep 21 */
            /*   ----------------- */
            else
            {
                for(i=0;i<MAXDEP2;i++) /* recherche communes de la cote d or */
                {
                    p_cne1=cne1;  /* init. pointeur  sur tab de char */
                    p_cne2=p_cotedor->commune; /* init. ptr pour cne
                                                    de struct */
                    retour=strcmp(p_cne1, p_cne2); /* passage des adresses */
                    if (retour==0)
                    {
                        if (aff == 'A')
                        {
                            res_coef=2.06;
                        }
                        else
                        {
                            res_coef=1.57; /* retour du coefficient
                                             si commune trouvee */
                        }
                    }
                    p_cotedor = p_cotedor+1;  /* incrementation du
                                                pointeur sur struct */
                }
                return res_coef;
            }
        }
    }
}


/*============================================================================
   Recherche de la Direction
  ============================================================================*/

int recherche_csdi(char di3[])
{
    int i;    /* indice utilise dans les for */
    char *p_tab2;
    char *p_csdi;

    /*  structure des directions  */

    static struct dir {
        char csdi [4];
    }c3[MAXDIR]={
        {"010"}, {"020"}, {"030"}, {"040"}, {"050"}, {"060"}, {"070"}, {"080"},
        {"090"}, {"100"}, {"110"}, {"120"}, {"131"}, {"132"}, {"140"}, {"150"},
        {"160"}, {"170"}, {"180"}, {"190"}, {"2A0"}, {"2B0"}, {"210"}, {"220"},
        {"230"}, {"240"}, {"250"}, {"260"}, {"270"}, {"280"}, {"290"}, {"300"},
        {"310"}, {"320"}, {"330"}, {"340"}, {"350"}, {"360"}, {"370"}, {"380"},
        {"390"}, {"400"}, {"410"}, {"420"}, {"430"}, {"440"}, {"450"}, {"460"},
        {"470"}, {"480"}, {"490"}, {"500"}, {"510"}, {"520"}, {"530"}, {"540"},
        {"550"}, {"560"}, {"570"}, {"580"}, {"591"}, {"592"}, {"600"}, {"610"},
        {"620"}, {"630"}, {"640"}, {"650"}, {"660"}, {"670"}, {"680"}, {"690"},
        {"700"}, {"710"}, {"720"}, {"730"}, {"740"}, {"754"}, {"755"}, {"756"},
        {"757"}, {"758"}, {"760"}, {"770"}, {"780"}, {"790"}, {"800"}, {"810"},
        {"820"}, {"830"}, {"840"}, {"850"}, {"860"}, {"870"}, {"880"}, {"890"},
        {"900"}, {"910"}, {"921"}, {"922"}, {"930"}, {"940"}, {"950"}, {"971"},
        {"972"}, {"973"}, {"974"}, {"976"}
    }, *p_d;    /* pointeur sur structure  */

    int retour;
    p_d=c3;        /* initialisation du pointeur sur structure   */


    /*                  boucle de recherche de la direction                   */
    /*------------------------------------------------------------------------*/

    for(i=0;i<MAXDIR;i++)
    {
        p_csdi=di3;                     /*init pointeur sur tab de char*/
        p_tab2=p_d->csdi;               /*init pointeur pour dir de struct*/
        retour=strcmp(p_csdi,p_tab2);  /*passage des adresses*/
        if(retour==0)
        {
            return 1;   /* direction trouvee */
        }
        p_d=p_d+1;     /* incrementation du pointeur sur struct */
    }
    return 0;        /* direction non trouvee */
}

/*------------------------ Fin de recherche direction ------------------------*/


/*============================================================================
   Fonctions d'Arrondi
  ============================================================================*/


double arrondi_nieme_decimale_voisine(double a_arrondir, short nbdec)
{
    return floor(0.50 +(1+DBL_EPSILON) *
    a_arrondir * pow(10.0 , (double)nbdec)
    )
    / pow(10.0 , (double)nbdec);
}


/*---------------- Fin de arrondi a la nieme decimale voisine ----------------*/


long arrondi_euro_inf(double a_arrondir)
{
    return (long)floor(( 1 + 16*DBL_EPSILON ) * a_arrondir);

}
/*-------------------- Fin de arrondi a l'euro inferieur  --------------------*/

long arrondi_euro_voisin(double a_arrondir)
{
    return arrondi_euro_inf(a_arrondir + 0.5000);
}

long arrondi_dizaine_inf(double a_arrondir)
{
    return  10 * arrondi_euro_inf(a_arrondir / 10);
}

/*----------------------- Fin des fonctions d'arrondi  -----------------------*/

/*============================================================================
   Fonction maximum
  ============================================================================*/
long maximum(long valeur1,long valeur2)
{
    if (valeur1 > valeur2)
    {
        return valeur1;
    }
    else
    {
        return valeur2;
    };
}
/*--------------------------- Fin fonction maximum ---------------------------*/


/*============================================================================
   Fonction est_exoneration
  ============================================================================*/
int est_exoneration(char degex)
{
    if ((degex=='F')||(degex=='D')||(degex=='S')||(degex=='I')||
        (degex=='V')||(degex=='A')||(degex=='W')||(degex=='K'))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/*-------------------- Fin de la fonction est_exoneration --------------------*/


/*============================================================================
   Fonction est_degrevement
  ============================================================================*/
int est_degrevement(char degex     /* code degrevement */)
{
    if (est_degrevement_total(degex))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/*-------------------- Fin de la fonction est_degrevement --------------------*/


/*============================================================================
   Fonction est_autre_allegement
  ============================================================================*/
int est_autre_allegement(char degex     /* code degrevement */)
{
    if (degex=='H')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/*---------------- Fin de la fonction est_degrevement_partiel ----------------*/

/*============================================================================
   Recherche dans la table d'erreurs
  ============================================================================*/
int cherche_Erreur(int erreur,s_liberreur * * p_liberreur)
{
    #define MAXERREUR 336
    static  s_erreur TableErreur[MAXERREUR] = {
        {0  ,{"Les donnees sorties ont pu etre calculees correctement "}},
        {1  ,{"L'annee de taxation est incorrecte"}},
        {2  ,{"Le code departement est invalide"}},
        {3  ,{"Le code taxation TH doit etre egal a P, E ou S"}},
        {4  ,{"Le nombre de personnes a charge est invalide"}},
        {5  ,{"CODE DIRECTION : Le code direction est invalide"}},
        {6  ,{"Le code non imposable a l'IR est invalide"}},
        {7  ,{"Le code ASH est invalide"}},
        {8  ,{"Le code nature FIP est invalide"}},
        {10 ,{"KCAC code affectation invalide <> H,M,F,S,A,BLANC"}},
        {14 ,{"KCAC calcul abattements: nombre de part errone"}},
        {15 ,{"KCAC calcul abattements: nombre de part negatif"}},
        {16 ,{"Le nombre d'ERA est invalide"}},
        {17 ,{"Le code type de personne morale est invalide"}},
        {18 ,{"Le code groupe revision est invalide"}},
        {19 ,{"Le code champ redevance TV est invalide"}},
        {20 ,{"Le code assujetti redevance TV est invalide"}},
        {21 ,{"Le code degrevement redevance TV est invalide"}},
        {22 ,{"Le code degrevement/exoneration TH est invalide"}},
        {23 ,{"Le code type d'impot est invalide"}},
        {25 ,{"Des personnes a charge sont servies en THLV"}},
        {26 ,{"Un code degrevement est servi en THLV"}},
        {27 ,{"Le code ni a l'IR est servi en THLV"}},
        {28 ,{"Le code champ TV est servi en THLV"}},
        {29 ,{"Le code degrevement TV est servi en THLV"}},
        {30 ,{"Le code questionnaire TV est servi en THLV"}},
        {31 ,{"Le code type de personne morale est servi en THLV"}},
        {32 ,{"Le code groupe revision est servi en THLV"}},
        {35 ,{"Le code taxation est servi en THLV"}},
        {36 ,{"La V.L.brute des secondaires est differente de zero en THLV"}},
        {38 ,{"Le revenu du foyer fiscal est different de zero en THLV"}},
        {39 ,{"L'indicateur majoration THS est invalide"}},
        {40 ,{"Le departement est hors champ pour la majoration THS"}},
        {41 ,{"Le taux d'imposition majoration THS est invalide"}},
        {42 ,{"Indicateur assujetissement majoration THS invalide"}},
        {51 ,{"DISCORDANCE:annee de taxation cote et collectivites locales"}},
        {52 ,{"Le code taxation est incompatible avec des personnes a charge"}},
        {53 ,{"Le code taxation est incompatible avec la VL principale"}},
        {54 ,{"Le code taxation est incompatible avec le code exo/degrevement"}},
        {55 ,{"Le code taxation est incompatible avec le code NI/IR (ABS)"}},
        {58 ,{"DISCORDANCE:code exoneration D en Metropole"}},
        {59 ,{"Le code taxation est incompatible avec la VL secondaire"}},
        {60 ,{"DISCORDANCE:code exoneration / degrevement et code NI/IR"}},
        {61 ,{"DISCORDANCE:code exoneration / degrevement et code nature FIP"}},
		{62 ,{"Indicateur THLV invalide"}},
        {64 ,{"Presence a tort de cotes mixtes"}},
        {65 ,{"DISCORDANCE:code champ TV / assujetti TV"}},
        {66 ,{"DISCORDANCE:code champ TV / taxation E"}},
        {67 ,{"DISCORDANCE:code champ TV / code nature FIP"}},
        {68 ,{"DISCORDANCE:code degrevement TV / degrevement TH"}},
        {69 ,{"DISCORDANCE:code champ TV / code degevement TV"}},
        {70 ,{"DISCORDANCE:code degrevement TV droits acquis / degrevement TH"}},
        {71 ,{"Somme plaf et reductions differente de somme des cotisations"}},
        {72 ,{"Incoherence entre code degrevement TV a Z et code DSF"}},
        {77 ,{"Le rang de sortie exonération TH est invalide"}},
        {78 ,{"Le rang de sortie abattement TH est invalide"}},
        {79 ,{"Les codes abattement et exoneration sont servis"}},
        {80 ,{"Le code taxation est incompatible avec le code abattement"}},
        {101 ,{"COMMUNE:La valeur locative moyenne est invalide"}},
        {102 ,{"COMMUNE:Quotite d'abattement pour PAC de rang 1 ou 2 invalide"}},
        {103 ,{"COMMUNE:Quotite d'abattement pour PAC de rang 3 ou + invalide"}},
        {104 ,{"COMMUNE:Quotite d'abattement general a la base invalide"}},
        {105 ,{"COMMUNE:Quotite d'abattement special a la base invalide"}},
        {106 ,{"COMMUNE:Alsace/Moselle:Abattement PAC servi"}},
        {107 ,{"COMMUNE:hors Alsace/Moselle:Abattement PAC rang 1 ou 2 non positif"}},
        {108 ,{"COMMUNE:hors Alsace/Moselle:Abattement PAC rang 3 ou + non positif"}},
        {109 ,{"CASE G interdite desormais en Alsace-Moselle"}},
        {110 ,{"Somme des taux d'impositions de N anormalement elevee"}},
        {111 ,{"COMMUNE:L'indicateur d'appartenance a une CU est invalide"}},
        {112 ,{"COMMUNE:Le taux global 2000 est invalide"}},
        {114 ,{"COMMUNE:Quotite minimale Alsace-Moselle absente"}},
        {115 ,{"COMMUNE:La valeur locative moyenne DOM 1989 est invalide"}},
        {116 ,{"COMMUNE:VL moyenne DOM 1989 absente pour une commune DOM"}},
        {117 ,{"COMMUNE:VL moyenne DOM 1989 presente pour une commune hors DOM"}},
        {118 ,{"COMMUNE:Taux abatt. general a la base invalide pour une commune DOM"}},
        {119 ,{"COMMUNE:La quotite d'abattement ASH invalide"}},
        {152 ,{"COMMUNE:Abattements PAC 12 2003 absents "}},
        {153 ,{"COMMUNE:Abattements PAC 3+ 2003 absents "}},
        {154 ,{"COMMUNE:Indicateur meme commune absent"}},
        {155 ,{"COMMUNE:Indicateur commune appartenant a meme CU absent "}},
        {156 ,{"COMMUNE:Indicateur regime Alsace-Moselle en 2003 absent "}},
        {201 ,{"InterCom:La valeur locative moyenne est invalide"}},
        {202 ,{"InterCom:Quotite d'abattement pour PAC de rang 1 ou 2 invalide"}},
        {203 ,{"InterCom:Quotite d'abattement pour PAC de rang 3 ou + invalide"}},
        {204 ,{"InterCom:La quotite d'abattement general a la base est invalide"}},
        {205 ,{"InterCom:La quotite d'abattement special a la base est invalide"}},
        {206 ,{"InterCom:Presence d'une quotite d'abattement CU sans indicateur CU"}},
        {211 ,{"InterCom:Presence d'un taux d'imposition CU sans indicateur CU"}},
        {212 ,{"InterCom:La quotite d'abattement ASH invalide"}},
        {251 ,{"InterCom:Presence indic meme CU et commune n'appartenant pas a CU"}},
        {301 ,{"DEPARTEMENT:La valeur locative moyenne est invalide"}},
        {302 ,{"DEPARTEMENT:Quotite d'abattement pour PAC rang 1 ou 2 invalide"}},
        {303 ,{"DEPARTEMENT:Quotite d'abattement pour PAC rang 3 ou + invalide"}},
        {304 ,{"DEPARTEMENT:la quotite d'abattement general a la base est invalide"}},
        {305 ,{"DEPARTEMENT:La quotite d'abattement special a la base est invalide"}},
        {306 ,{"DEPARTEMENT:La quotite d'abattement ASH invalide"}},
        {810 ,{"AGGLOMERATION NOUVELLE:Le taux d'imposition est different de zero"}},
        {901 ,{"ERREUR INTERNE th_nkcoc : baspc negative"}},
        {902 ,{"ERREUR INTERNE th_nkcoc : baspt negative"}},
        {903 ,{"ERREUR INTERNE th_nkcoc : baspq negative"}},
        {904 ,{"ERREUR INTERNE th_nkcoc : baspd negative"}},
        {905 ,{"ERREUR INTERNE th_nkcoc : basps negative"}},
        {906 ,{"ERREUR INTERNE th_nkcoc : baspn negative"}},
        {907 ,{"ERREUR INTERNE th_nkcoc : baspg negative"}},
        {908 ,{"ERREUR INTERNE th_nkcoc : baspe negative"}},
        {1001 ,{"ERREUR INTERNE th_nkdnc : annee invalide"}},
        {1002 ,{"ERREUR INTERNE th_nkdnc : somrp negative"}},
        {1003 ,{"ERREUR INTERNE th_nkdnc : somrc negative"}},
        {1004 ,{"ERREUR INTERNE th_nkdnc : somrc < somrp"}},
        {1005 ,{"ERREUR INTERNE th_nkdnc : revim negative"}},
        {1006 ,{"ERREUR INTERNE th_nkdnc : exo et cotisation des principaux positive"}},
        {1007 ,{"ERREUR INTERNE th_nkdnc : code degrevement invalide"}},
        {1008 ,{"ERREUR INTERNE th_nkdnc : code degrevement invalide"}},
        {1009 ,{"ERREUR INTERNE th_nkdnc : code vl exo invalide"}},
        {1010 ,{"ERREUR INTERNE th_nkdnc : revff negatif"}},
        {1013 ,{"ERREUR INTERNE th_nkdnc : redut negative "}},
        {1102 ,{"ERREUR INTERNE NaP_Ni_Deg_Ni_Exo : somrp negative"}},
        {1103 ,{"ERREUR INTERNE NaP_Ni_Deg_Ni_Exo : somrc negative"}},
        {1104 ,{"ERREUR INTERNE NaP_Ni_Deg_Ni_Exo : somrc < somrp"}},
        {1202 ,{"ERREUR INTERNE NaP_Deg_Tot : somrp negative"}},
        {1203 ,{"ERREUR INTERNE NaP_Deg_Tot : somrc negative"}},
        {1204 ,{"ERREUR INTERNE NaP_Deg_Tot : somrc < somrp"}},
        {1302 ,{"ERREUR INTERNE NaP_Exo : somrp negative"}},
        {1303 ,{"ERREUR INTERNE NaP_Exo : somrc negative"}},
        {1304 ,{"ERREUR INTERNE NaP_Exo : somrc < somrp"}},
        {2111 ,{"ERREUR INTERNE Plafonnement_1414A_Fictif : somrc negative"}},
        {2112 ,{"ERREUR INTERNE Plafonnement_1414A_Fictif : revff negative"}},
        {2122 ,{"ERREUR INTERNE NaP_Plafonnement_1414_Double : somrc negative"}},
        {2123 ,{"ERREUR INTERNE NaP_Plafonnement_1414_Double : somr3 negative"}},
        {2124 ,{"ERREUR INTERNE NaP_Plafonnement_1414_Double : revff negative"}},
        {2125 ,{"ERREUR INTERNE NaP_Plafonnement_1414_Double : revim negative"}},
        {2126 ,{"ERREUR INTERNE NaP_Plafonnement_1414_Double : redtt negative"}},
        {2127 ,{"ERREUR INTERNE NaP_Plafonnement_1414_Double : redt3 negative"}},
        {2128 ,{"ERREUR INTERNE NaP_Plafonnement_1414_total : Major. plaf. prov. < 0"}},
        {2129 ,{"ERREUR INTERNE NaP_Plafonnement_1414_total : Plaf. apres major. < 0"}},
        {2202 ,{"ERREUR INTERNE th_nkrec : Code taxation invalide"}},
        {2203 ,{"ERREUR INTERNE th_nkrec : Le taux global 2000 est invalide"}},
        {2204 ,{"ERREUR INTERNE th_nkrec : Base nette communale < vlbsc"}},
        {2205 ,{"ERREUR INTERNE th_nkrec : Base nette CU < vlbsc"}},
        {2206 ,{"ERREUR INTERNE th_nkrec : Base nette departementale < vlbsc"}},
        {2301 ,{"ERREUR INTERNE th_nktac : CNE VL moyenne negative"}},
        {2302 ,{"ERREUR INTERNE th_nktac : CNE abattement special negatif"}},
        {2303 ,{"ERREUR INTERNE th_nktac : CNE abattement general negatif"}},
        {2304 ,{"ERREUR INTERNE th_nktac : CNE abattement PAC1 negatif"}},
        {2305 ,{"ERREUR INTERNE th_nktac : CNE abattement PAC3+ negatif"}},
        {2306 ,{"ERREUR INTERNE th_nktac : CNE VL moyenne > 15000"}},
        {2307 ,{"ERREUR INTERNE th_nktac : CNE abattement special > SEUIL"}},
        {2308 ,{"ERREUR INTERNE th_nktac : CNE abattement general > SEUIL"}},
        {2309 ,{"ERREUR INTERNE th_nktac : CNE abattement PAC1 > SEUIL"}},
        {2310 ,{"ERREUR INTERNE th_nktac : CNE abattement PAC3+ > SEUIL"}},
        {2311 ,{"ERREUR INTERNE th_nktac : CNE abattement apac3 = 0, apac1 > 0"}},
        {2312 ,{"ERREUR INTERNE th_nktac : CNE abattement apac1 = 0 ,apac3 > 0"}},
        {2313 ,{"ERREUR INTERNE th_nktac : CNE taux d'abattement invalide"}},
        {2315 ,{"ERREUR INTERNE th_nktac : CNE abattement special handicape negatif"}},
        {2316 ,{"ERREUR INTERNE th_nktac : CNE abattement special handicape > SEUIL"}},
        {2321 ,{"ERREUR INTERNE th_nktac : EPCI VL moyenne negative"}},
        {2322 ,{"ERREUR INTERNE th_nktac : EPCI abattement special negatif"}},
        {2323 ,{"ERREUR INTERNE th_nktac : EPCI abattement general negatif"}},
        {2324 ,{"ERREUR INTERNE th_nktac : EPCI abattement PAC1 negatif"}},
        {2325 ,{"ERREUR INTERNE th_nktac : EPCI abattement PAC3+ negatif"}},
        {2326 ,{"ERREUR INTERNE th_nktac : EPCI VL moyenne > 15000"}},
        {2327 ,{"ERREUR INTERNE th_nktac : EPCI abattement special > SEUIL"}},
        {2328 ,{"ERREUR INTERNE th_nktac : EPCI abattement general > SEUIL"}},
        {2329 ,{"ERREUR INTERNE th_nktac : EPCI abattement PAC1 > SEUIL"}},
        {2330 ,{"ERREUR INTERNE th_nktac : EPCI abattement PAC3+ > SEUIL"}},
        {2331 ,{"ERREUR INTERNE th_nktac : EPCI abattement apac3 = 0 ,apac1 > 0"}},
        {2332 ,{"ERREUR INTERNE th_nktac : EPCI abattement apac1 = 0 ,apac3 > 0"}},
        {2333 ,{"ERREUR INTERNE th_nktac : EPCI taux d'abattement invalide"}},
        {2335 ,{"ERREUR INTERNE th_nktac : EPCI abattement special handicape negatif"}},
        {2336 ,{"ERREUR INTERNE th_nktac : EPCI abattement special handicape > SEUIL"}},
        {2341 ,{"ERREUR INTERNE th_nktac : DEP VL moyenne negative"}},
        {2342 ,{"ERREUR INTERNE th_nktac : DEP abattement special negatif"}},
        {2343 ,{"ERREUR INTERNE th_nktac : DEP abattement general negatif"}},
        {2344 ,{"ERREUR INTERNE th_nktac : DEP abattement PAC1 negatif"}},
        {2345 ,{"ERREUR INTERNE th_nktac : DEP abattement PAC3+ negatif"}},
        {2346 ,{"ERREUR INTERNE th_nktac : DEP VL moyenne > 15000"}},
        {2347 ,{"ERREUR INTERNE th_nktac : DEP abattement special > SEUIL"}},
        {2348 ,{"ERREUR INTERNE th_nktac : DEP abattement general > SEUIL"}},
        {2349 ,{"ERREUR INTERNE th_nktac : DEP abattement PAC1 > SEUIL"}},
        {2350 ,{"ERREUR INTERNE th_nktac : DEP abattement PAC3+ > SEUIL"}},
        {2351 ,{"ERREUR INTERNE th_nktac : DEP abattement apac3 = 0 ,apac1 > 0"}},
        {2352 ,{"ERREUR INTERNE th_nktac : DEP abattement apac1 = 0 ,apac3 > 0"}},
        {2353 ,{"ERREUR INTERNE th_nktac : DEP taux d'abattement invalide"}},
        {2355 ,{"ERREUR INTERNE th_nktac : DEP abattement special handicape negatif"}},
        {2356 ,{"ERREUR INTERNE th_nktac : DEP abattement special handicape > SEUIL"}},
        {2361 ,{"ERREUR INTERNE th_nktac : CNE 2003 VL moyenne negative"}},
        {2362 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement special negatif"}},
        {2363 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement general negatif"}},
        {2364 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement PAC1 negatif"}},
        {2365 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement PAC3+ negatif"}},
        {2366 ,{"ERREUR INTERNE th_nktac : CNE 2003 VL moyenne > 15000"}},
        {2367 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement special > SEUIL"}},
        {2368 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement general > SEUIL"}},
        {2369 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement PAC1 > SEUIL"}},
        {2370 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement PAC3+ > SEUIL"}},
        {2371 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement apac3 = 0 ,apac1 > 0"}},
        {2372 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement apac1 = 0 ,apac3 > 0"}},
        {2373 ,{"ERREUR INTERNE th_nktac : CNE 2003 taux d'abattement invalide"}},
        {2375 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement ASH negatif"}},
        {2376 ,{"ERREUR INTERNE th_nktac : CNE 2003 abattement ASH > SEUIL"}},
        {2381 ,{"ERREUR INTERNE th_nktac : EPCI 2003 VL moyenne negative"}},
        {2382 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement special negatif"}},
        {2383 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement general negatif"}},
        {2384 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement PAC1 negatif"}},
        {2385 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement PAC3+ negatif"}},
        {2386 ,{"ERREUR INTERNE th_nktac : EPCI 2003 VL moyenne > 15000"}},
        {2387 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement special > SEUIL"}},
        {2388 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement general > SEUIL"}},
        {2389 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement PAC1 > SEUIL"}},
        {2390 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement PAC3+ > SEUIL"}},
        {2391 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement apac3 = 0 ,apac1 > 0"}},
        {2392 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement apac1 = 0 ,apac3 > 0"}},
        {2393 ,{"ERREUR INTERNE th_nktac : EPCI 2003 taux d'abattement invalide"}},
        {2395 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement ASH  negatif"}},
        {2396 ,{"ERREUR INTERNE th_nktac : EPCI 2003 abattement ASH > SEUIL"}},
        {2401 ,{"ERREUR INTERNE th_nktac : DEP 2003 VL moyenne negative"}},
        {2402 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement special negatif"}},
        {2403 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement general negatif"}},
        {2404 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement PAC1 negatif"}},
        {2405 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement PAC3+ negatif"}},
        {2406 ,{"ERREUR INTERNE th_nktac : DEP 2003 VL moyenne > 15000"}},
        {2407 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement special > SEUIL"}},
        {2408 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement general > SEUIL"}},
        {2409 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement PAC1 > SEUIL"}},
        {2410 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement PAC3+ > SEUIL"}},
        {2411 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement apac3 = 0 ,apac1 > 0"}},
        {2412 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement apac1 = 0 ,apac3 > 0"}},
        {2413 ,{"ERREUR INTERNE th_nktac : DEP 2003 taux d'abattement invalide"}},
        {2415 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement ASH negatif"}},
        {2416 ,{"ERREUR INTERNE th_nktac : DEP 2003 abattement ASH > SEUIL"}},
        {2421 ,{"ERREUR INTERNE th_nktac : indicateurs EPCI errones"}},
        {2422 ,{"ERREUR INTERNE th_nktac : indicateurs errones"}},
        {2423 ,{"ERREUR INTERNE th_nktac : Regime Alsace/Moselle errone"}},
        {2425 ,{"ERREUR INTERNE th_nktac : Taux TSE Autre renseigne a tort"}},
        {2475 ,{"L'indicateur de deliberation GEMAPI est invalide"}},
        {2476 ,{"Le taux d'imposition GEMAPI est invalide"}},
        {2501 ,{"ERREUR INTERNE th_nkrac : Annee de taxation invalide"}},
        {2502 ,{"ERREUR INTERNE th_nkrac : Code de departement invalide"}},
        {2503 ,{"ERREUR INTERNE th_nkrac : Code champ redevance invalide"}},
        {2504 ,{"ERREUR INTERNE th_nkrac : Code assujetti TV invalide"}},
        {2505 ,{"ERREUR INTERNE th_nkrac : Code droit a degrevement TV invalide"}},
        {2506 ,{"ERREUR INTERNE th_nkrac : Incoherence entre champ et qtvrt"}},
        {2507 ,{"ERREUR INTERNE th_nkrac : Incoherence entre champ et degtv"}},
        {3001 ,{"ERREUR SIGNATURE CALCULETTE module errone : KTOS"}},
        {3002 ,{"ERREUR SIGNATURE CALCULETTE module errone : KSTS"}},
        {3003 ,{"ERREUR SIGNATURE CALCULETTE module errone : KFOS"}},
        {3004 ,{"ERREUR SIGNATURE CALCULETTE module errone : KARC"}},
        {3005 ,{"ERREUR SIGNATURE CALCULETTE module errone : KSFC"}},
        {3006 ,{"ERREUR SIGNATURE CALCULETTE module errone : KSEC"}},
        {3007 ,{"ERREUR SIGNATURE CALCULETTE module errone : KTAC"}},
        {3008 ,{"ERREUR SIGNATURE CALCULETTE module errone : KVLC"}},
        {3009 ,{"ERREUR SIGNATURE CALCULETTE module errone : KDNC"}},
        {3010 ,{"ERREUR SIGNATURE CALCULETTE module errone : KBNC"}},
        {3011 ,{"ERREUR SIGNATURE CALCULETTE module errone : KCOC"}},
        {3012 ,{"ERREUR SIGNATURE CALCULETTE module errone : KRAC"}},
        {3013 ,{"ERREUR SIGNATURE CALCULETTE module errone : KCVC"}},
        {3014 ,{"ERREUR SIGNATURE CALCULETTE module errone : KTVC"}},
        {3015 ,{"ERREUR SIGNATURE CALCULETTE module errone : KCAC"}},
        {3016 ,{"ERREUR SIGNATURE CALCULETTE module errone : KREC"}},
        {3017 ,{"ERREUR SIGNATURE CALCULETTE module errone : KDEC"}},
        {4002 ,{"CODE DIRECTION : Le code direction est invalide"}},
        {4003 ,{"NATURE D'AFFECTATION : Valeur differente de S C R "}},
        {4004 ,{"CODE FRONTALIER PREMIER CONT : Valeur differente de blanc ou K "}},
        {4005 ,{"CODE FRONTALIER SECOND CONT : Valeur differente de blanc ou K "}},
        {4006 ,{"RATTACHE : Presence d'informations IR du conjoint pour un rattache "}},
        {4007 ,{"RATTACHE : Absence d'informations IR pour un rattache "}},
        {4008 ,{"TAX SIMPLE : Presence d'info IR du conjoint pour une tax simple "}},
        {4009 ,{"PREMIER CONTRIBUABLE : Nombre de parts errone "}},
        {4010 ,{"SECOND CONTRIBUABLE : Nombre de parts errone "}},
        {4013 ,{"PREMIER CONT : presence d'un code frontalier sans situation IR "}},
        {4014 ,{"SECOND CONT : presence d'un code frontalier sans situation IR "}},
        {4015 ,{"PREMIER CONT : presence d'un nombre de parts negatif "}},
        {4016 ,{"SECOND CONT : presence d'un nombre de parts negatif "}},
        {4017 ,{"PREMIER CONT : presence d'un revenu fiscal de reference negatif "}},
        {4018 ,{"SECOND CONT : presence d'un revenu fiscal de reference negatif "}},
        {4021 ,{"PREMIER CONT : annee revenu superieure ou egale a annee tax TH "}},
        {4022 ,{"SECOND CONT :  annee revenu superieure ou egale a annee tax TH "}},
        {4104 ,{"Date commune TLV posterieure a l'annee de taxation"}},
        {4105 ,{"Incompatibilite date local TLV et annee de taxation"}},
        {4106 ,{"Departement hors champ TLV"}},
        {4206 ,{"Erreur interne : code role TLV errone"}},
        {4302 ,{"CODE DIRECTION : Le code direction est invalide"}},
        {4303 ,{"NATURE D'AFFECTATION : Valeur differente de S C R "}},
        {4304 ,{"CODE FRONTALIER PREMIER CONT : Valeur differente de blanc ou K "}},
        {4305 ,{"CODE FRONTALIER SECOND CONT : Valeur differente de blanc ou K "}},
        {4306 ,{"CODE FRONTALIER RATTACHES : Valeur differente de blanc ou K "}},
        {4307 ,{"RATTACHE : Presence d'informations IR du conjoint pour un rattache "}},
        {4308 ,{"RATTACHE : Absence d'informations IR pour un rattache "}},
        {4309 ,{"TAX SIMPLE : Presence d'info IR du conjoint pour une tax simple "}},
        {4310 ,{"PREMIER CONTRIBUABLE : Nombre de parts errone "}},
        {4311 ,{"SECOND CONTRIBUABLE : Nombre de parts errone "}},
        {4312 ,{"RATTACHES : Nombre de parts errone "}},
        {4313 ,{"PREMIER CONT : presence d'un code frontalier sans situation IR "}},
        {4314 ,{"SECOND CONT : presence d'un code frontalier sans situation IR "}},
        {4315 ,{"RATTACHES : presence d'un code frontalier sans situation IR "}},
        {4316 ,{"PREMIER CONT : presence d'un nombre de parts negatif "}},
        {4317 ,{"SECOND CONT : presence d'un nombre de parts negatif "}},
        {4318 ,{"RATTACHES : presence d'un nombre de parts negatif "}},
        {4319 ,{"PREMIER CONT : presence d'un revenu fiscal de reference negatif "}},
        {4320 ,{"SECOND CONT : presence d'un revenu fiscal de reference negatif "}},
        {4321 ,{"RATTACHES : presence d'un revenu fiscal de reference negatif "}},
        {4322 ,{"RATTACHES : presence d'un RFR ou code frontalier sans nb parts "}},
        {4323 ,{"RATTACHES : presence RFR>0 et code frontalier avec un seul rattache"}},
        {4326 ,{"RATTACHES : millesime de l'annee de revenu < antax - 1 "}},
        {4327 ,{"PREMIER CONT : millesime de l'annee de revenu > antax - 1 "}},
        {4328 ,{"SECOND CONT : millesime de l'annee de revenu > antax - 1 "}},
        {4329 ,{"RATTACHES : millesime de l'annee de revenu > antax - 1 "}},
        {4401 ,{"ERREUR KVLC : code affectation invalide <> H,M,F,S,A,BLANC"}},
        {4402 ,{"ERREUR KVLC : DISCORDANCE:annee de taxation local et collectivites"}},
        {4403 ,{"ERREUR KVLC : DISCORDANCE:code commune local et collectivites"}},
        {4404 ,{"ERREUR KVLC : DISCORDANCE:code departement local et collectivites"}},
        {5002 ,{"th_nkdec : Erreur code degrevement "}},
        {5004 ,{"th_nkdec : somrp courant negative "}},
        {5014 ,{"th_nkdec :calcul abattements: nombre de part errone"}},
        {5015 ,{"th_nkdec :calcul abattements: nombre de part negatif"}},
        {5025 ,{"th_nkdec : revenu Foyer fiscal negatif       "}},
        {5026 ,{"th_nkdec : cumul somme a recouvrer negatif   "}},
        {5027 ,{"th_nkdec : cumul degrevement negatif  "}},
        {5028 ,{"th_nkdec : cumul degrevement > cumul somme a recouvrer"}},
        {5030 ,{"th_nkdec : plafonnement calcule negatif "}},
        {5031 ,{"th_nkdec : total th calcule negatif "}},
        {5033 ,{"th_nkdec : somme des reductions reellesde plafonnement negative "}},
        {5034 ,{"th_nkdec : reduction de plafonnement appliquee negative "}},
        {5035 ,{"th_nkdec : net a payer calcule negatif "}},
        {5036 ,{"th_nkdec : reduc plaf theorique negative "}},
        {5038 ,{"th_nkdec : somme des reductions theoriques de plafonnement negative"}},
        {5039 ,{"th_nkdec : code regularisation invalide "}},
        {5040 ,{"th_nkdec : net a payer initial negatif "}},
        {5041 ,{"th_nkdec : code regularisation incompatible avec nap initial "}},
        {5042 ,{"th_nkdec : delta de net a payer negatif "}},
        {5043 ,{"th_nkdec : signe de delta de nap invalide "}},
        {5044 ,{"th_nkdec : signe delta nap significatif et napdt = 0 "}},
        {5045 ,{"th_nkdec : delta nap >0 et signe incoherent "}},
        {5128 ,{"th_nkdec : NaP_Plafonnement_1414_total : Majoration plafonnement < 0"}},
        {5129 ,{"th_nkdec : NaP_Plafonnement_1414_total : Plaft apres majoration < 0"}},
        {5201 ,{"code regularisation de role invalide "}},
    };

    static s_liberreur ErreurInconnue={"Erreur interne au module de calcul TH"};
    short numerreur=0;

    /* on parcourt la table TableErreur, jusqu'a ce que l'on trouve
    egalite entre le code erreur et un poste de la table ,
    ou que la variable de parcours
    soit plus grande que le nombre de postes de la table*/

    for(numerreur=0; (numerreur < MAXERREUR) && (TableErreur[numerreur].code != erreur) ;
    numerreur++);
    if (numerreur == MAXERREUR)
    {
        *p_liberreur=&ErreurInconnue;
        return 1;
    }
    else
    {
        *p_liberreur= &(TableErreur[numerreur].liberreur_);
        return 0;
    };
}
/*---------------- Fin de la recherche dans la table d'erreurs  ----------------*/


/*============================================================================
   Fonctions de  degrevement
  ============================================================================*/

/* -------------------------------------------------------------- */
/* Fonction de calcul net a payer sans degrevement ni exoneration */
/* -------------------------------------------------------------- */

int NaP_Ni_Deg_Ni_Exo(s_cons *p_cons,   /* pointeur sur constantes de l'annee */
                      s_dne *p_dne,     /* pointeur sur donnee entree */
                      s_dns *p_dns)     /* pointeur sur donnee sortie */
{
    if (p_dne->somrp < 0)
    {
        return (1102);
    }
    if (p_dne->somrc < 0)
    {
        return (1103);
    }
    if (p_dne->somrc < p_dne->somrp)
    {
        return (1104);
    }
    if ((p_dne->somrc < p_cons->seuil_nap) && (p_dne->somrc > 0))
    {
        p_dns->redta = 0;
        p_dns->redaa = 0;
        p_dns->redsa = 0;
        p_dns->dgpl3 = 0;
        p_dns->plart = 0;
        p_dns->degpl = 0;
        p_dns->totth = p_dne->somrc;
        p_dns->netth = 0;
        strncpy(p_dns->codro,"NV",3);
    }
    else
    {
        p_dns->redta = 0;
        p_dns->redaa = 0;
        p_dns->redsa = 0;
        p_dns->dgpl3 = 0;
        p_dns->plart = 0;
        p_dns->degpl = 0;
        p_dns->totth = p_dne->somrc;
        p_dns->netth = p_dns->totth;
        strncpy(p_dns->codro,"  ",3);
    }
    return (0);
}


/*============================================================================*/
/*                       Fonction est_degrevement_total                       */
/*============================================================================*/
int est_degrevement_total(char degex     /* code degrevement */)
{
 	if ( degex == 'G' )
    {
        return 1;
    }
    else
    {
       return 0;
    }
}
/*-----------------------Fin de la fonction est_degrevement_total-------------*/


/* ---------------------------- */
/* fonction de calcul des frais */
/* ---------------------------- */

void frais(long totcott, long totcotp, double taux_FDR, double taux_AR,
           long *fraisFDR, long *fraisAR, long *fraisDRP)

{
    *fraisFDR = arrondi_euro_voisin(taux_FDR * totcott);
    *fraisAR  = arrondi_euro_voisin(taux_AR * totcott);
    *fraisDRP = arrondi_euro_voisin(taux_FDR * totcotp);
}

/* ------------------------------------------------------- */
/* fonction determination_parts utilisée dans KSEC et KSFC */
/* ------------------------------------------------------- */


void determination_parts(short part, short *pp1_demipart1,
                         short *pp2_demipart2, short *pp3_demipart3,
                         short *pp4_demipart4, short *pp5_autredemipart,
                         short *pp6_quartpart1, short *pp7_quartpart2,
                         short *pp8_quartpart3, short *pp9_quartpart4,
                         short *pp10_autrequartpart)


{
    *pp1_demipart1=0;
    *pp2_demipart2=0;
    *pp3_demipart3=0;
    *pp4_demipart4=0;
    *pp5_autredemipart=0;
    *pp6_quartpart1=0;
    *pp7_quartpart2=0;
    *pp8_quartpart3=0;
    *pp9_quartpart4=0;
    *pp10_autrequartpart=0;

    if(part > 125)
    {
        *pp1_demipart1=1;
    }

    if(part > 175)
    {
        *pp2_demipart2=1;
    }

    if(part > 225)
    {
        *pp3_demipart3=1;
    }

    if(part > 275)
    {
        *pp4_demipart4=1;
    }

    if(part == 125)
    {
        *pp6_quartpart1=1;
    }

    if(part == 175)
    {
        *pp7_quartpart2=1;
    }

    if(part == 225)
    {
        *pp8_quartpart3=1;
    }

    if(part == 275)
    {
        *pp9_quartpart4=1;
    }

    /* if ((part > 300) && (arrondi_euro_voisin(bidon2) == 1)) */
    if ((part > 300) && (arrondi_euro_voisin((0.2 * part) - (2 * arrondi_euro_inf(0.1 * part))) == 1))
    {
        *pp10_autrequartpart=1;
    }

    if(part > 325)
    {
        *pp5_autredemipart=(short)arrondi_euro_voisin((part - (*pp10_autrequartpart * 25) - 300)*0.02);
    }
}


/* -------------------------------------------------------- */
/* fonction determination_droits utilisée dans KSEC et KSFC */
/* -------------------------------------------------------- */



char determination_droits(long rev, char front, short demipart1,
                          short demipart2, short demipart3, short demipart4,
                          short autredemipart, short quartpart1,
                          short quartpart2, short quartpart3, short quartpart4,
                          short autrequartpart, s_typseuil *pc)

{
    long revseuil;
    s_rev *pr;

    if ((front!='K')||((front=='K')&&(rev!=0)))
    {
        pr=&(pc->rev_tousdroits);
        revseuil=(pr->revenu_unepart +
        (quartpart1 * arrondi_euro_voisin(0.5 * (pr->rev_prem_demipart))) +
        (demipart1*(pr->rev_prem_demipart)) +
        (quartpart2 * arrondi_euro_voisin(0.5 * (pr->rev_deuxieme_demipart))) +
        (demipart2*(pr->rev_deuxieme_demipart)) +
        (quartpart3 * arrondi_euro_voisin(0.5 * (pr->rev_troisieme_demipart))) +
        (demipart3*(pr->rev_troisieme_demipart)) +
        (quartpart4 * arrondi_euro_voisin(0.5 * (pr->rev_quatrieme_demipart))) +
        (demipart4*(pr->rev_quatrieme_demipart)) +
        (autrequartpart * arrondi_euro_voisin(0.5 * (pr->rev_autre_demipart))) +
        (autredemipart*(pr->rev_autre_demipart)));

        if (revseuil>=rev)
        {
           return ('0');
        }
        else
        {
            pr=&(pc->rev_plaf);
            revseuil=(pr->revenu_unepart +
            (quartpart1 * arrondi_euro_voisin(0.5 * (pr->rev_prem_demipart))) +
            (demipart1*(pr->rev_prem_demipart)) +
            (quartpart2 * arrondi_euro_voisin(0.5 * (pr->rev_deuxieme_demipart))) +
            (demipart2*(pr->rev_deuxieme_demipart)) +
            (quartpart3 * arrondi_euro_voisin(0.5 * (pr->rev_troisieme_demipart))) +
            (demipart3*(pr->rev_troisieme_demipart)) +
            (quartpart4 * arrondi_euro_voisin(0.5 * (pr->rev_quatrieme_demipart))) +
            (demipart4*(pr->rev_quatrieme_demipart)) +
            (autrequartpart * arrondi_euro_voisin(0.5 * (pr->rev_autre_demipart))) +
            (autredemipart*(pr->rev_autre_demipart)));
            if (revseuil>=rev)
            {
                return ('2');
            }
            else
            {
                return ('3');
            }
        }
    }
    else
    {
        return ('3');
    }
}


/* ---------------------------------------------------------- */
/* fonction determination_parts W utilisée dans KSEC et KSFC */
/* ---------------------------------------------------------- */

short determination_parts_w(short part)
{
    return part = floor((((part - 200) * 2) + 100) / 100);
}

/* ---------------------------------------------------------- */
/* fonction determination_droits W utilisée dans KSEC et KSFC */
/* ---------------------------------------------------------- */



char determination_droits_w(long rev, short part, s_typseuil *pc)

{
    long revseuilb = 0;

    if (part != 0)
    {
        if (part < 200) {
            revseuilb = pc->rev_w.rev_w_2parts;
        }else{
            part = determination_parts_w(part);
            revseuilb = (pc->rev_w.rev_w_2parts +( part * pc->rev_w.rev_w_autrepart));
            }
        if (rev <= revseuilb)
        {
           return ('0');
        }
        else
        {
           return ('1');
        }

    }
    else
    {
        return ('1');
    }
}
/* ----------------------------- */
/* fonction determination_abat */
/* ----------------------------- */

long determination_abat(char * csdi,short npaff,s_cons *p_cons)
{
    short demipart1;
    short demipart2;
    short demipart3;
    short demipart4;
    short autredemipart;
    short quartpart1;
    short quartpart2;
    short quartpart3;
    short quartpart4;
    short autrequartpart;
    static char dep97[]="97";
    static char dir973[]="973";
    static char dir976[]="976";
    long tot_abat_rev;
    s_typseuil * pc;

/* Determination du departement pour chargement du pointeur sur typseuil
     */
    if ( strncmp(csdi,dep97,2) == 0 )
    {
        pc = &(p_cons->typseuil_dom_124);              /* trait dom */
        if ( strcmp(csdi,dir973) == 0 )
        {
            pc = &(p_cons->typseuil_973);         /* trait guyane */
        }
        else
        if ( strcmp(csdi,dir976) == 0 )
        {
            pc = &(p_cons->typseuil_976);         /* trait Mayotte */
        }
    }
    else
    {
        pc = &(p_cons->typseuil_metro);                /* trait metropole */
    }


    if (npaff != 0) /* il existe un declarant (eventuellement rattache) */
    {
        determination_parts(  npaff,
                             &demipart1,
                             &demipart2,
                             &demipart3,
                             &demipart4,
                             &autredemipart,
                             &quartpart1,
                             &quartpart2,
                             &quartpart3,
                             &quartpart4,
                             &autrequartpart);

    tot_abat_rev=(pc->rev_abat.revenu_unepart +
                 (demipart1 * (pc->rev_abat.rev_prem_demipart))      +
                 (demipart2 * (pc->rev_abat.rev_deuxieme_demipart))  +
                 (demipart3 * (pc->rev_abat.rev_troisieme_demipart)) +
                 (demipart4 * (pc->rev_abat.rev_quatrieme_demipart)) +
                 (autredemipart * (pc->rev_abat.rev_autre_demipart)) +
                 (quartpart1 * arrondi_euro_voisin((pc->rev_abat.rev_prem_demipart)      * 0.5)) +
                 (quartpart2 * arrondi_euro_voisin((pc->rev_abat.rev_deuxieme_demipart)  * 0.5)) +
                 (quartpart3 * arrondi_euro_voisin((pc->rev_abat.rev_troisieme_demipart) * 0.5)) +
                 (quartpart4 * arrondi_euro_voisin((pc->rev_abat.rev_quatrieme_demipart) * 0.5)) +
                 (autrequartpart * arrondi_euro_voisin((pc->rev_abat.rev_autre_demipart) * 0.5)));

  return (tot_abat_rev);
  }
  else
  {
  return (0);
  }

}

/*------------------------------------*/
/* fonction de regularisation de role */
/*------------------------------------*/

void regularisation (long netth, long napin, long *p_napdt, char *p_signe)
{
    if (netth > napin)
    {
        *p_signe = '+';
        *p_napdt = netth - napin;
        return;
    }
    if (netth < napin)
    {
        *p_signe = '-';
        *p_napdt = napin - netth;
        return;
    }

    if (netth == napin)
    {
        *p_signe = '=';
        *p_napdt = 0;
        return;
    }
}

int Determination_Plaf_Total(long revff)
{
    if (revff < limit_plaf)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* La TSE Autre s'applique à l'Ile de France ET à la Martinique depuis 2013 */

int dep_TSE_autre(char * dep)
{
    if(!strcmp(dep,"75")
        ||!strcmp(dep,"77")
        ||!strcmp(dep,"78")
        ||!strcmp(dep,"91")
        ||!strcmp(dep,"92")
        ||!strcmp(dep,"93")
        ||!strcmp(dep,"94")
        ||!strcmp(dep,"95")
        ||!strcmp(dep,"97"))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void ajout_espace_taux_abattement(s_cole * p_cole)
{
    if ( strncmp(p_cole->txbas,"F",1) == 0 )
    {
        strncpy(p_cole->txbas, "F ", 3);
    }

    if ( strncmp(p_cole->tpac1,"F",1) == 0 )
    {
        strncpy(p_cole->tpac1, "F ", 3);
    }

    if ( strncmp(p_cole->tpac3,"F",1) == 0 )
    {
        strncpy(p_cole->tpac3, "F ", 3);
    }

    if ( strncmp(p_cole->txspe,"F",1) == 0 )
    {
        strncpy(p_cole->txspe, "F ", 3);
    }

    if ( strncmp(p_cole->txhan,"F",1) == 0 )
    {
        strncpy(p_cole->txhan, "F ", 3);
    }
}
/*     RECHERCHE DEPARTEMENT CONCERNE A PARTIR DE 2013 */
/*     ----------------------------------------------  */

int recherche_dep_TLV(char depv[])
{
int i;      /*indice utilise dans les for */
char *p_tab;
char *p_depv;
/*   structure des departements 'vacants'   */

static struct dep
{
    char csdep[3];
} c2[MAXDEPTLV] = {
{"01"}, {"06"}, {"13"}, {"17"}, {"2A"}, {"2B"}, {"30"}, {"31"}, {"33"}, {"34"}, {"38"}, {"40"}, {"44"}, {"59"},
{"60"}, {"64"}, {"67"}, {"69"}, {"74"}, {"75"}, {"77"}, {"78"}, {"83"}, {"91"}, {"92"}, {"93"}, {"94"}, {"95"}
}, *p_d;  /*pointeur sur structure */

int retour;
p_d=c2;         /*initialisation du pointeur sur structure  */


/* boucle de recherche du departement 'vacant'  */



for(i=0;i<MAXDEPTLV;i++)
{
    p_depv=depv;
    p_tab=p_d->csdep;
    retour=strcmp(p_depv,p_tab);
    if (retour==0)
    {
        return 1;  /* direction trouvee */
    }
    p_d=p_d+1;
}
return 0;          /* direction non trouvee */
}
