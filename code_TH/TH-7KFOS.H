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

/* ======================================================
=     FICHIER DE DEFINITION DES EN-TETE DE FONCTION     =
========================================================*/
#if defined(ILIAD)
    #include <th-7ksts.h>

#else
    #include "th-7ksts"

#endif

/* #include <stdlib.h> */
#if !defined(KFOS)
#define KFOS  'B'
/* **************************************************
*     NE RIEN INSERER AVANT CET EN-TETE             *
***************************************************** */

#if !defined(max)
#define max(A, B)  ((A) > (B) ? (A) : (B))
#define min(A, B)  ((A) > (B) ? (B) : (A))
#endif

int controle_signature(int  rang_module , char ident_module, s_liberreur * * p_liberreur);

double arrondi_nieme_decimale_voisine(double a_arrondir, short nbdec);

long arrondi_euro_inf(double a_arrondir);

long arrondi_euro_voisin(double a_arrondir);

long arrondi_dizaine_inf(double a_arrondir);

long maximum(long valeur1,long valeur2);

int cherche_const(short annee,s_cons * * p_cons);

double recherche_tdep(char[], char[], char);

int th_7kbnc (s_bne1 * p_bne1, s_bne2 * p_bne2, s_bns * p_bns);

int th_7kcoc (s_coe1 * p_coe1, s_coe2 * p_coe2, s_cos * p_cos);

int th_7kdnc (short antax , s_dne * p_dne, s_dns * p_dns);

int th_7kdec(s_e5 * p_e5, s_s5 * p_s5);

int th_7kvlc(s_e1 * p_e1, s_e9 * p_e9, s_s1 * p_s1);

int th_7ksec(s_ese * p_e, s_sse * p_s);

int th_7krec(s_ree * p_e, s_res * p_s);

int th_7ktac(s_e8 * p_e8, s_s8 * p_s8);

int th_7krac(s_rae * p_rae, s_ras * p_ras);

int est_degrevement(char degex);

int est_degrevement_total(char degex     /* code degrevement */);

int est_autre_allegement(char degex     /* code degrevement */);

int est_exoneration(char degex);

int cherche_Erreur(int erreur,s_liberreur * * p_liberreur);
int recherche_csdi(char[]);
int NaP_Ni_Deg_Ni_Exo(s_cons *p_cons,        /*net a payer sans degrevement ni exoneration */
                       s_dne *p_dne,
                       s_dns *p_dns);

void frais(long totcott, long totcotp, double taux_FDR, double taux_AR, long *fraisDR, long *fraisAR, long *fraisDRP);

void determination_parts(short,short*,short*,short*,short*,short*,short*,short*,short*,
                         short*,short*);

short determination_parts_w(short);

void regularisation(long,long,long*,char*);

char determination_droits(long,char,short,short,short,short,short,
                          short, short, short, short, short, s_typseuil *pc);

char determination_droits_w(long,short, s_typseuil *pc);

int dep_TSE_autre(char * dep);

int Determination_Plaf_Total(long revff);

void ajout_espace_taux_abattement(s_cole * p_cole);
int recherche_dep_TLV(char[]);

/*===================== Ne Rien ecrire sous cette ligne ================*/
#endif



