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

/*=====================================*/
    /*    MODULE DE DETERMINATION DE LA    */
    /*  DOUBLE LIQUIDATION ET DES DONNEES  */
    /*     DES COLLECTIVITES N ET 2003     */
    /*        programme th-7KTAC.C         */
    /*=====================================*/


#define LA_SIGNATURE

#if defined (ILIAD)
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif

typedef struct {
    char taux[03];
    long quotite ;
} s_abatt_unit_cole;

s_cons *p_cons;

int controle_ktac(s_e8 * p_e8,s_s8 * p_s8);
int A_Son_Propre_Regime(s_cole * p_cole);
int A_Son_Propre_Regime_Taux(s_cole * p_cole);
int Abatt1_Inferieurs_Abatt2( s_cole * p_cole1, s_cole  * p_cole2);
int Controle_Abattements(s_cole * p_cole, char dep[], char csdi[], short antax);
int Conversion_Taux_Abt(char * p_Taux_Abt_Chaine);
int Abatt1_Unit_Inf_Abatt2_Unit(s_abatt_unit_cole * p_abatt_unit1,s_abatt_unit_cole * p_abatt_unit2);
void Abt_colb_Mix( long * pabmos, char * p_codef, char * p_codeg, s_cole * p_Abt_Cne_2003, s_cole * p_Abt_Col_N, s_colb * p_Abt_Mix);
void Abt_colb_Init(s_colb * p_Abt_Init);
void Abt_colb_Charge(long * p_abmos, char * p_codef, char * p_codeg, s_cole * p_Abt_cole_Source, s_colb * p_Abt_colb);
void Abt_colb_Recopie(s_cole * p_Abt_cole_Cible, s_colb * p_Abt_colb_Source, s_colb * p_Abt_colb_Cible);
void Abt_colb_Mix_Als_Mos(long * pabmos, char * p_codef, char * p_codeg, s_cole * p_Abt_Cne_2003, s_cole * p_Abt_Col_N, s_colb * p_Abt_Mix);

/*============================================================================
   M A I N
  ============================================================================*/

int th_7ktac ( s_e8 * p_e8, s_s8 * p_s8)
{
    extern s_signature la_signature ;
    int rets;       /* code retour signature */
    char versr='C'; /* version du programme */
    int retour ;    /* code retour controle */
    int retp=0;     /* code retour final */
    int regq;       /* retour fonction regime EPCI */
    int regi;       /* retour fonction regime EPCI 2003 */
    int abattqi;    /* retour fonction abattement EPCI - EPCI 2003 */
    int abattqo;    /* retour fonction abattement EPCI - commune 2003 */
    int abattco;    /* retour fonction abattement commune - commune 2003 */
    int retcons=0;  /*code retour cherche_const  */
    /* long sv_vmd89; */
    long Zero = 0;
    char blanc = ' ';
    p_s8->signature = &la_signature ;
    p_s8->versr = versr;
    p_s8->anref = 2017;


    /* controle signature */

    rets=controle_signature(RKTAC,p_s8->versr,&(p_s8->libelle));

    if (rets!=0)
    {
        return(rets);
    }

    /* ajout des espaces pour les abattements forfaitaires */
    ajout_espace_taux_abattement(&(p_e8->cole_c));
    ajout_espace_taux_abattement(&(p_e8->cole_q));
    ajout_espace_taux_abattement(&(p_e8->cole_d));
    ajout_espace_taux_abattement(&(p_e8->cole_o));
    ajout_espace_taux_abattement(&(p_e8->cole_i));
    ajout_espace_taux_abattement(&(p_e8->cole_p));
    ajout_espace_taux_abattement(&(p_e8->cole_f));
    ajout_espace_taux_abattement(&(p_e8->cole_j));
    ajout_espace_taux_abattement(&(p_e8->cole_r));
    ajout_espace_taux_abattement(&(p_e8->cole_m));

    retcons=(short)cherche_const(p_e8->antax,&p_cons);
        cherche_Erreur(retcons,&(p_s8->libelle)) ;
        if (retcons == 1)          /* annee invalide */
        {
            return (1);
        }

    /* controle  des  donnees en entree  */

    retour = controle_ktac(p_e8,p_s8);

    if (retour != 0)
    {
        cherche_Erreur(retour,&(p_s8->libelle));
        return (retour);
    }

    /* determination de l'indicateur intercommunalité  */

    regq = A_Son_Propre_Regime_Taux(&(p_e8->cole_r));
    regi = A_Son_Propre_Regime(&(p_e8->cole_m));
    abattqi = Abatt1_Inferieurs_Abatt2(&(p_e8->cole_r),&(p_e8->cole_m));
    abattqo = Abatt1_Inferieurs_Abatt2(&(p_e8->cole_r),&(p_e8->cole_j));

    if (((p_e8->cnqg3 == 'O') && (regq == 1) && (regi == 1) && (abattqi == 1))
     || ((p_e8->cnqg3 == 'O') && (regq == 1) && (regi == 0) && (abattqo == 1)))
    {
        p_s8->cudl = 'O';
    }
    else
    {
        p_s8->cudl = 'N';
    }

    /* determination de l'indicateur commune  */
    abattco = Abatt1_Inferieurs_Abatt2(&(p_e8->cole_f),&(p_e8->cole_j));

    if ((p_e8->cneg3 == 'O') && (abattco == 1))
    {
        p_s8->cndl = 'O';
    }
    else
    {
        p_s8->cndl = 'N';
    }

    /* determination des abattements annee N */

    /* Données commune */
    Abt_colb_Charge (&(p_e8->abmos),&(p_e8->codef),&(p_e8->codeg),
                     &(p_e8->cole_c),&(p_s8->colb_c));

    /* Données commune avant ajustement */
    Abt_colb_Charge (&(p_e8->abmos),&(p_e8->codef),&(p_e8->codeg),
                     &(p_e8->cole_f),&(p_s8->colb_f));

    /* Données syndicat */
    if (p_e8->tisyn != 0)
    {
        /* commune avant ajustement */
        Abt_colb_Recopie (&(p_e8->cole_f), &(p_s8->colb_f), &(p_s8->colb_s));
    }
    else
    {
        Abt_colb_Init (&(p_s8->colb_s));
    }

    /* Données intercommunalité */
    if (p_e8->cocnq == 'C')
    {
        if (A_Son_Propre_Regime_Taux(&(p_e8->cole_q)))
        {
            Abt_colb_Charge (&Zero, &blanc, &blanc, &(p_e8->cole_q),
                             &(p_s8->colb_q));
        }
        else
        {
        /* commune avant ajustement */
            Abt_colb_Recopie (&(p_e8->cole_q), &(p_s8->colb_f),
                              &(p_s8->colb_q));
            /* Si l'interco a des quotités ajustées
            de la VLM interco à partir de la VLM communale*/
            if(A_Son_Propre_Regime(&(p_e8->cole_q)))
            {
                p_s8->colb_q.abbas = p_e8->cole_q.abbas;
                p_s8->colb_q.apac1 = p_e8->cole_q.apac1;
                p_s8->colb_q.apac3 = p_e8->cole_q.apac3;
                p_s8->colb_q.abspe = p_e8->cole_q.abspe;
                p_s8->colb_q.abhan = p_e8->cole_q.abhan;
            }

        }
    }
    else
    {
        Abt_colb_Init (&(p_s8->colb_q));
    }

    /* Données intercommunalité avant ajustement */
    if (p_e8->cocnq == 'C')
    {
        if (A_Son_Propre_Regime(&(p_e8->cole_r)))
        {
            Abt_colb_Charge (&Zero, &blanc, &blanc, &(p_e8->cole_r),
                             &(p_s8->colb_r));
        }
        else
        {
            Abt_colb_Recopie (&(p_e8->cole_r), &(p_s8->colb_f),
                              &(p_s8->colb_r));

        }
    }
    else
    {
        Abt_colb_Init (&(p_s8->colb_r));
    }

    /* Données departement */
    if (A_Son_Propre_Regime(&(p_e8->cole_d)))
    {
        Abt_colb_Charge (&Zero,&blanc,&blanc, &(p_e8->cole_d), &(p_s8->colb_d));
    }
    else
    {
        Abt_colb_Recopie (&(p_e8->cole_d), &(p_s8->colb_c), &(p_s8->colb_d));
    }

    /* Données TSE */
    if (p_e8->titsn != 0)
    {
        Abt_colb_Recopie (&(p_e8->cole_f), &(p_s8->colb_f), &(p_s8->colb_n));
    }
    else
    {
        Abt_colb_Init (&(p_s8->colb_n));
    }

    /* Données TSE Autre */
    if (p_e8->titgp != 0)
    {
        Abt_colb_Recopie (&(p_e8->cole_f), &(p_s8->colb_f), &(p_s8->colb_g));
    }
    else
    {
        Abt_colb_Init (&(p_s8->colb_g));
    }
    /* Données GEMAPI */
        if (p_e8->timpe > 0)
        {
            Abt_colb_Recopie (&(p_e8->cole_c), &(p_s8->colb_c), &(p_s8->colb_e));
        }
        else
        {
            Abt_colb_Init (&(p_s8->colb_e));
        }

    /* determination des abattements fictifs 2003 */

    if ((p_s8->cndl == 'N') && (p_s8->cudl == 'N')  )
    {
        Abt_colb_Init (&(p_s8->colb_o));
        Abt_colb_Init (&(p_s8->colb_j));
        Abt_colb_Init (&(p_s8->colb_y));
        Abt_colb_Init (&(p_s8->colb_i));
        Abt_colb_Init (&(p_s8->colb_m));
        Abt_colb_Init (&(p_s8->colb_p));
        Abt_colb_Init (&(p_s8->colb_v));
        Abt_colb_Init (&(p_s8->colb_b));
        Abt_colb_Init (&(p_s8->colb_a));
    }
    else
    {
        /* Données commune 2003 */
        if (p_e8->cneg3 == 'O' )
        {
            if (p_s8->cndl  == 'O' &&
                p_e8->codef == ' ' &&
                p_e8->codeg == ' ' &&
                p_e8->cnam3 == 'O')
            {
                 Abt_colb_Mix_Als_Mos(&(p_e8->abmos),&(p_e8->codef),
                                      &(p_e8->codeg), &(p_e8->cole_o),
                                      &(p_e8->cole_c), &(p_s8->colb_o));
            }
            else
            {
                Abt_colb_Charge (&(p_e8->abmos), &(p_e8->codef), &(p_e8->codeg),
                                 &(p_e8->cole_o), &(p_s8->colb_o));
                p_s8->colb_o.abhan=p_e8->cole_c.abhan;
                strncpy(p_s8->colb_o.txhan,p_e8->cole_c.txhan, 3);
            }
        }
        else
        {
            Abt_colb_Recopie (&(p_e8->cole_c), &(p_s8->colb_c),
                              &(p_s8->colb_o));
        }

        /* Données commune avant ajustement 2003 */
        if (p_e8->cneg3 == 'O' )
        {
            if (p_s8->cndl  == 'O' &&
                p_e8->codef == ' ' &&
                p_e8->codeg == ' ' &&
                p_e8->cnam3 == 'O')
            {
                 Abt_colb_Mix_Als_Mos(&(p_e8->abmos),&(p_e8->codef),
                                      &(p_e8->codeg), &(p_e8->cole_j),
                                      &(p_e8->cole_f), &(p_s8->colb_j));
            }
            else
            {
                Abt_colb_Charge (&(p_e8->abmos), &(p_e8->codef), &(p_e8->codeg),
                                 &(p_e8->cole_j), &(p_s8->colb_j));
                p_s8->colb_j.abhan=p_e8->cole_f.abhan;
                strncpy(p_s8->colb_j.txhan,p_e8->cole_f.txhan, 3);
            }
        }
        else
        {
            Abt_colb_Recopie (&(p_e8->cole_f), &(p_s8->colb_f),
                              &(p_s8->colb_j));
        }

        /* Données syndicat 2003 */
         Abt_colb_Recopie (&(p_e8->cole_o), &(p_s8->colb_s), &(p_s8->colb_y));

        /* Données intercommunalité 2003 */
        if (p_e8->cocnq != 'C')
        {
            Abt_colb_Init (&(p_s8->colb_i));
        }
        else
        {
            if (p_e8->cnqg3 == 'N')
            {
                Abt_colb_Recopie (&(p_e8->cole_q), &(p_s8->colb_q),
                                  &(p_s8->colb_i));
            }
            else
            {
                if (A_Son_Propre_Regime(&(p_e8->cole_q)))
                {
                    if (A_Son_Propre_Regime(&(p_e8->cole_i)))
                    {
                        Abt_colb_Charge (&Zero, &blanc, &blanc, &(p_e8->cole_i),
                                         &(p_s8->colb_i));
                        p_s8->colb_i.abhan=p_e8->cole_q.abhan;
                        strncpy(p_s8->colb_i.txhan,p_e8->cole_q.txhan, 3);
                    }
                    else
                    {
                        Abt_colb_Mix ( &(p_e8->abmos), &(p_e8->codef),
                                       &(p_e8->codeg), &(p_e8->cole_o),
                                       &(p_e8->cole_q), &(p_s8->colb_i));
                    }
                }
                else
                {
                    Abt_colb_Recopie (&(p_e8->cole_q), &(p_s8->colb_f),
                                      &(p_s8->colb_i));
                }
            }
        }

        /* Données intercommunalité avant ajustement 2003 */
        if (p_e8->cocnq != 'C')
        {
            Abt_colb_Init (&(p_s8->colb_m));
        }
        else
        {
            if (p_e8->cnqg3 == 'N')
            {
                Abt_colb_Recopie (&(p_e8->cole_r), &(p_s8->colb_r),
                                  &(p_s8->colb_m));
            }
            else
            {
                if (A_Son_Propre_Regime(&(p_e8->cole_r)))
                {
                    if (A_Son_Propre_Regime(&(p_e8->cole_m)))
                    {
                        Abt_colb_Charge (&Zero, &blanc, &blanc, &(p_e8->cole_m),
                                         &(p_s8->colb_m));
                        p_s8->colb_m.abhan=p_e8->cole_r.abhan;
                        strncpy(p_s8->colb_m.txhan,p_e8->cole_r.txhan, 3);
                    }
                    else
                    {
                        Abt_colb_Mix ( &(p_e8->abmos), &(p_e8->codef),
                                       &(p_e8->codeg), &(p_e8->cole_j),
                                       &(p_e8->cole_r), &(p_s8->colb_m));
                    }
                }
                else
                {
                    Abt_colb_Recopie (&(p_e8->cole_r), &(p_s8->colb_f),
                                      &(p_s8->colb_m));
                }
            }
        }

        /* Données departement 2003 */
        if (A_Son_Propre_Regime(&(p_e8->cole_d)))
        {
            if (A_Son_Propre_Regime(&(p_e8->cole_p)))
            {
                Abt_colb_Charge (&Zero,&blanc,&blanc, &(p_e8->cole_p),
                                 &(p_s8->colb_p));
                p_s8->colb_p.abhan=p_e8->cole_d.abhan;
                strncpy(p_s8->colb_p.txhan,p_e8->cole_d.txhan, 3);
            }
            else
            {
                Abt_colb_Mix (&(p_e8->abmos),&(p_e8->codef),&(p_e8->codeg),
                              &(p_e8->cole_j),&(p_e8->cole_d),&(p_s8->colb_p));
            }
        }
        else
        {
            Abt_colb_Recopie (&(p_e8->cole_d), &(p_s8->colb_c),
                              &(p_s8->colb_p));
        }

        /* Données TSE 2003 */
        Abt_colb_Recopie (&(p_e8->cole_f), &(p_s8->colb_n), &(p_s8->colb_v));

        /* TSE Autre 2003*/
        if (p_e8->titgp != 0)
        {
            Abt_colb_Recopie (&(p_e8->cole_f), &(p_s8->colb_g), &(p_s8->colb_b));
        }
        else
        {
            Abt_colb_Init (&(p_s8->colb_b));
        }
        /* Données GEMAPI 2003 */
        if (p_e8->timpe > 0)
               {
                   Abt_colb_Recopie (&(p_e8->cole_c), &(p_s8->colb_e), &(p_s8->colb_a));
               }
               else if (p_e8->timpe == 0)
               {
                   Abt_colb_Init (&(p_s8->colb_a));
               }
    }
    return (retp);
}

/* controle des donnees recues */

int controle_ktac(s_e8 * p_e8, s_s8 * p_s8)
{
    int retabatt=0;
    int retour;
    s_cole * pt;

    /* Traitement annee taxation */

    if ((p_e8->antax < p_s8->anref-3 )||((p_e8->antax > p_s8->anref )))
    {
        return (1);
    }

    /* Commune */

    retabatt=Controle_Abattements(&(p_e8->cole_f), p_e8->dep, p_e8->csdi, p_e8->antax);

    if (retabatt != 0)
    {
        retour= retabatt + 2300;
        return (retour);
    }

    /* Intercommunalité */

    retabatt=Controle_Abattements(&(p_e8->cole_r), p_e8->dep, p_e8->csdi, p_e8->antax);

    if (retabatt != 0)
    {
        retour= retabatt + 2320;
        return (retour);
    }

    /* Departement*/

    retabatt=Controle_Abattements(&(p_e8->cole_d), p_e8->dep, p_e8->csdi, p_e8->antax);

    if (retabatt != 0)
    {
        retour= retabatt + 2340;
        return (retour);
    }

    /* Commune 2003 */

    retabatt=Controle_Abattements(&(p_e8->cole_j), p_e8->dep, p_e8->csdi, p_e8->antax);

    if (retabatt != 0)
    {
        retour= retabatt + 2360;
        return (retour);
    }

    /* Intercommunalité 2003 */

    retabatt=Controle_Abattements(&(p_e8->cole_m), p_e8->dep, p_e8->csdi, p_e8->antax);

    if (retabatt != 0)
    {
        retour= retabatt + 2380;
        return (retour);
    }

    /* Departement 2003 */

    retabatt=Controle_Abattements(&(p_e8->cole_p), p_e8->dep, p_e8->csdi, p_e8->antax);

    if (retabatt != 0)
    {
        retour= retabatt + 2400;
        return (retour);
    }

    if ((strchr("C ",p_e8->cocnq) == NULL)
    ||  (strchr("F ",p_e8->codef) == NULL)
    ||  (strchr("12345 ",p_e8->codeg) == NULL)
    ||  (strchr("NO",p_e8->cneg3) == NULL)
    ||  (strchr("NO",p_e8->cnqg3) == NULL)
    ||  (strchr("NO",p_e8->cnam3) == NULL))
    {
        return (2422);
    }

    if (((p_e8->abmos > 0) && (p_e8->codef != 'F' || p_e8->codeg != ' '))  ||
         (p_e8->abmos == 0 && (p_e8->codef == 'F' || (strchr("12345 ",p_e8->codeg) == NULL))))
    {
        return (2423);
    }

    pt = &p_e8->cole_q;

    if ((p_e8->cocnq == ' ' && p_e8->cnqg3 == 'O')
    ||  (p_e8->cocnq == ' ' && pt->apac1 > 0))
    {
        return (2421);
    }

    /* a n o  2475 GEMAPI */
    if((p_e8->antax) < 2015){
    	if (p_e8->indgem != ' '){

    		return(2475);
    	}
    }

    if((p_e8->antax) == 2015){
    	if ((p_e8->indgem != ' ' )&& (p_e8->indgem != 'C' ) && (p_e8->indgem != 'I' )){

    		return(2475);
    	}
    }else{
        if ((p_e8->indgem != ' ' )&& (p_e8->indgem != 'C' ) && (p_e8->indgem != 'I' ) && (p_e8->indgem != 'M' )){

    		return(2475);
    	}
    }


    /* a n o  2476 GEMAPI */
       if((p_e8->antax) < 2015){
       	if (p_e8->timpe != 0){

                return(2476);
            }
       }

       if((p_e8->antax) == 2015){
       	if ((((p_e8->indgem == 'C' ) || (p_e8->indgem == 'I' )) && (p_e8->timpe == 0 ))
       		|| ((p_e8->indgem == ' ' ) && (p_e8->timpe != 0 ))
       		|| (p_e8->timpe > 1)){

       		return(2476);
       	}
       }

       if ((p_e8->antax) >= 2015){
        if ((p_e8->indgem == ' ') && (p_e8->timpe != 0 )
            || (p_e8->timpe > 1)){

       		return(2476);
        }
       }

    /* Controle de la valorisation du taux d’imposition de la TSE Autre */
    if ((p_e8->titgp != 0 )  && (!dep_TSE_autre(p_e8->dep)))
    {
        return(2425);
    }

    else
    {
        return (0);
    }

}


/* controles des abattements */

int Controle_Abattements (s_cole * p_cole, char dep[], char csdi[], short antax)
{
    if (p_cole->vlmoy < 0)
    {
        return (1);
    }

    if (p_cole->abspe < 0)
    {
        return (2);
    }

    if (p_cole->abbas < 0)
    {
        return (3);
    }

    if (p_cole->apac1 < 0)
    {
        return (4);
    }

    if (p_cole->apac3 < 0)
    {
        return (5);
    }

    if (p_cole->vlmoy > 15000)
    {
        return (6);
    }

    if (strcmp(csdi,"976") == 0){

		if (p_cole->abspe > p_cons->plaf_quotite_may)
		{
			return (7);
		}

		if (p_cole->abbas > p_cons->plaf_quotite_may)
		{
			return (8);
		}

		if (p_cole->apac1 > p_cons->plaf_quotite_may)
		{
			return (9);
		}

		if (p_cole->apac3 > p_cons->plaf_quotite_may)
		{
			return (10);
		}
    }else {
		if (p_cole->abspe > p_cons->plaf_quotite_gen)
		{
			return (7);
		}

		if (p_cole->abbas > p_cons->plaf_quotite_gen)
		{
			return (8);
		}

		if (p_cole->apac1 > p_cons->plaf_quotite_gen)
		{
			return (9);
		}

		if (p_cole->apac3 > p_cons->plaf_quotite_gen)
		{
			return (10);
		}
    }

    if (p_cole->apac3 == 0 && p_cole->apac1 > 0)
    {
        return (11);
    }

    if (p_cole->apac1 == 0 && p_cole->apac3 > 0)
    {
        return (12);
    }


	/* controle taux abattement général à la base */
	if ( strcmp( dep, "97" ) != 0 ) {
		if ( !( strcmp(p_cole->txbas,"00") == 0
			|| strcmp(p_cole->txbas,"  ") == 0
			|| strcmp(p_cole->txbas,"F ") == 0
			|| (atoi(p_cole->txbas) >= 1 && atoi(p_cole->txbas) <= 15 ) ) )
			{
				return (13);
			}
	} else {
		if ( !( strcmp(p_cole->txbas,"  ") == 0
			|| strcmp(p_cole->txbas,"00") == 0
			|| atoi(p_cole->txbas) == 40
			|| atoi(p_cole->txbas) == 50  ))
			{
			return (13);
			}
	}

	/* controle du taux d'abattement APAC12 */
	if ( strcmp( dep, "97" ) != 0 ) {
		if ( !(strcmp(p_cole->tpac1,"F ") == 0
			|| strcmp(p_cole->tpac1,"  ") == 0
			|| strcmp(p_cole->tpac1,"00") == 0
			|| (atoi(p_cole->tpac1) >= 10 && atoi(p_cole->tpac1) <= 20 ) ) )
		{
			return (13);
		}
	} else {
		if ( !( strcmp(p_cole->tpac1,"  ") == 0
			|| strcmp(p_cole->tpac1,"00") == 0
			|| atoi(p_cole->tpac1) == 5
			|| atoi(p_cole->tpac1) == 10 ))
		{
			return (13);
		}
	}

	/* controle du taux d'abattement APAC3+ */
	if ( strcmp( dep, "97" ) != 0 ) {
		if ( !( strcmp(p_cole->tpac3,"F ") == 0
			|| strcmp(p_cole->tpac3,"  ") == 0
			||strcmp(p_cole->tpac3,"00") == 0
			|| ( atoi(p_cole->tpac3) >=  15 && atoi(p_cole->tpac3) <= 25 ) ) )
		{
			return (13);
		}
	} else {
		if ( !( strcmp(p_cole->tpac3,"  ") == 0
			|| strcmp(p_cole->tpac3,"00") == 0
			|| atoi(p_cole->tpac3) == 5
			|| atoi(p_cole->tpac3) == 10 ))
		{
			return (13);
		}
	}

	/* controle taux abattement général ASB */
	if ( strcmp( dep, "97" ) != 0 ) {
			if ( !( strcmp(p_cole->txspe,"00") == 0
			|| strcmp(p_cole->txspe,"  ") == 0
			|| ( atoi(p_cole->txspe) >= 1 && atoi(p_cole->txspe) <= 15 ) ) )
		{
			return (13);
		}
	} else {
		if ( !( strcmp(p_cole->txspe,"  ") == 0
			|| strcmp(p_cole->txspe,"00") == 0 ))
		{
			return (13);
		}
	}

	/* controle taux abattement général ASH */
	if (antax < 2016) {
		if ( !( strcmp(p_cole->txhan,"00") == 0
			|| strcmp(p_cole->txhan,"  ") == 0
			|| atoi(p_cole->txhan) == 10 ) )
		{
			return (13);
		}
	} else {
		if ( !( strcmp(p_cole->txhan,"00") == 0
			|| strcmp(p_cole->txhan,"  ") == 0
			|| ( atoi(p_cole->txhan) >= 10 && atoi(p_cole->txhan) <= 20 ) ) )
		{
			return (13);
		}
	}

    if (p_cole->abhan < 0)
    {
        return (15);
    }

    if (strcmp(csdi,"976") == 0){
		if (p_cole->abhan > p_cons->plaf_quotite_may)
		{
			return (16);
		}
		else
		{
			return (0);
		}
    }
    else
    {
		if (p_cole->abhan > p_cons->plaf_quotite_gen)
		{
			return (16);
		}
		else
		{
			return (0);
		}
    }
}

/* comparaison des abattements */

int Abatt1_Inferieurs_Abatt2 ( s_cole * p_cole1, s_cole  * p_cole2)
{
    s_abatt_unit_cole col1_abspe, col1_abbas, col1_apac1, col1_apac3;
    s_abatt_unit_cole col2_abspe, col2_abbas, col2_apac1, col2_apac3;

    strncpy(col1_abspe.taux , p_cole1->txspe, 3);
    col1_abspe.quotite = p_cole1->abspe;
    strncpy(col1_abbas.taux , p_cole1->txbas, 3);
    col1_abbas.quotite = p_cole1->abbas;
    strncpy(col1_apac1.taux , p_cole1->tpac1, 3);
    col1_apac1.quotite = p_cole1->apac1;
    strncpy(col1_apac3.taux , p_cole1->tpac3, 3);
    col1_apac3.quotite = p_cole1->apac3;
    strncpy(col2_abspe.taux , p_cole2->txspe, 3);
    col2_abspe.quotite = p_cole2->abspe;
    strncpy(col2_abbas.taux , p_cole2->txbas, 3);
    col2_abbas.quotite = p_cole2->abbas;
    strncpy(col2_apac1.taux , p_cole2->tpac1, 3);
    col2_apac1.quotite = p_cole2->apac1;
    strncpy(col2_apac3.taux , p_cole2->tpac3, 3);
    col2_apac3.quotite = p_cole2->apac3;

    if (Abatt1_Unit_Inf_Abatt2_Unit(&(col1_abspe),&(col2_abspe))
    ||  Abatt1_Unit_Inf_Abatt2_Unit(&(col1_abbas),&(col2_abbas))
    ||  Abatt1_Unit_Inf_Abatt2_Unit(&(col1_apac1),&(col2_apac1))
    ||  Abatt1_Unit_Inf_Abatt2_Unit(&(col1_apac3),&(col2_apac3)))
    {
        return (1);
    }
    else
    {
        return(0);
    }
}

/* vote regime quotité d'abattements */

int A_Son_Propre_Regime (s_cole * p_cole)
{
    if (p_cole->abspe == 0 && p_cole->abbas == 0
    &&  p_cole->apac1 == 0 && p_cole->apac3 == 0)
    {
        return (0);
    }
    else
    {
        return (1);
    }
}

/* vote regime taux d'abattements */

int A_Son_Propre_Regime_Taux(s_cole * p_cole)
{
    if (  ((strcmp(p_cole->txspe,"  ") == 0) || (strcmp(p_cole->txspe,"F ") == 0) || (strcmp(p_cole->txspe,"00") == 0))
        &&((strcmp(p_cole->txbas,"  ") == 0) || (strcmp(p_cole->txbas,"F ") == 0) || (strcmp(p_cole->txbas,"00") == 0))
        &&((strcmp(p_cole->tpac1,"  ") == 0) || (strcmp(p_cole->tpac1,"F ") == 0) || (strcmp(p_cole->tpac1,"00") == 0))
        &&((strcmp(p_cole->tpac3,"  ") == 0) || (strcmp(p_cole->tpac3,"F ") == 0) || (strcmp(p_cole->tpac3,"00") == 0)))
    {
        return (0);
    }
    else
    {
        return (1);
    }
}

/* conversion des taux (chaine)  en integer */

int Conversion_Taux_Abt (char  p_Taux_Abt_Chaine[])
{
    if (strcmp(p_Taux_Abt_Chaine,"F ") == 0)
    {
        return (-1);
    }
    else if ((strcmp(p_Taux_Abt_Chaine,"00") == 0
    ||       (strcmp(p_Taux_Abt_Chaine,"  ") == 0)))
    {
        return (0);
    }
    else if ( atoi(p_Taux_Abt_Chaine) )
    {
        return atoi(p_Taux_Abt_Chaine);
    }
    else
    {
        return (-2);
    }
}

/* comparaison des taux d'abattements */

int Abatt1_Unit_Inf_Abatt2_Unit(s_abatt_unit_cole * p_abatt_unit1,
                                s_abatt_unit_cole * p_abatt_unit2)
{
    if(Conversion_Taux_Abt(p_abatt_unit1->taux) != -1
    && Conversion_Taux_Abt(p_abatt_unit2->taux) != -1)
    {
        if (Conversion_Taux_Abt(p_abatt_unit1->taux) < Conversion_Taux_Abt(p_abatt_unit2->taux))
        {
            return (1);
        }
        else
        {
            return (0);
        }
    }
    else
    {
        return (p_abatt_unit1->quotite < p_abatt_unit2->quotite);
    }
}

/* mixage des abattements communaux 2003 et collectivite N */

void Abt_colb_Mix ( long * pabmos, char * p_codef, char * p_codeg, s_cole * p_Abt_Cne_2003, s_cole * p_Abt_Col_N, s_colb * p_Abt_Mix)
{
    /* indicateurs */

    p_Abt_Mix->vlmoy = p_Abt_Col_N->vlmoy;
    p_Abt_Mix->abmos = *pabmos;
    p_Abt_Mix->codef = *p_codef;
    p_Abt_Mix->codeg = *p_codeg;

    /*abattement general */

    if (Conversion_Taux_Abt(p_Abt_Cne_2003->txbas) != -1)
    {
        p_Abt_Mix->abbas = arrondi_euro_voisin(0.01 * Conversion_Taux_Abt(p_Abt_Cne_2003->txbas)* p_Abt_Col_N->vlmoy );
     }
    else
    {
        p_Abt_Mix->abbas = p_Abt_Cne_2003->abbas;
    }
    strncpy(p_Abt_Mix->txbas,p_Abt_Cne_2003->txbas, 3);

    /* abattement special */

    if (Conversion_Taux_Abt(p_Abt_Cne_2003->txspe) != -1)
    {
        p_Abt_Mix->abspe = arrondi_euro_voisin(0.01 * Conversion_Taux_Abt(p_Abt_Cne_2003->txspe) * p_Abt_Col_N->vlmoy);
    }
    else
    {
        p_Abt_Mix->abspe = p_Abt_Cne_2003->abspe;
    }
    strncpy(p_Abt_Mix->txspe,p_Abt_Cne_2003->txspe, 3);

    /* abattement special handicape*/

    p_Abt_Mix->abhan=p_Abt_Col_N->abhan;
    strncpy(p_Abt_Mix->txhan,p_Abt_Col_N->txhan, 3);

    /* pac 1 - 2 */

    if (*p_codef != ' ' || *p_codeg != ' ' )
    {
        p_Abt_Mix->apac1 = arrondi_euro_voisin(0.01 * Conversion_Taux_Abt(p_Abt_Col_N->tpac1) * p_Abt_Col_N->vlmoy);
        strncpy(p_Abt_Mix->tpac1,p_Abt_Col_N->tpac1, 3);
    }
    else
    {
        if (Conversion_Taux_Abt(p_Abt_Cne_2003->tpac1) != -1)
        {
            p_Abt_Mix->apac1 = arrondi_euro_voisin(0.01 * Conversion_Taux_Abt(p_Abt_Cne_2003->tpac1) * p_Abt_Col_N->vlmoy);
        }
        else
        {
            p_Abt_Mix->apac1 = p_Abt_Cne_2003->apac1;
        }
        strncpy(p_Abt_Mix->tpac1,p_Abt_Cne_2003->tpac1, 3);
    }

    /* pac 3 et + */

    if (*p_codef != ' ' || *p_codeg != ' ' )
    {
        p_Abt_Mix->apac3 = arrondi_euro_voisin(0.01 * Conversion_Taux_Abt(p_Abt_Col_N->tpac3) * p_Abt_Col_N->vlmoy);
        strncpy(p_Abt_Mix->tpac3,p_Abt_Col_N->tpac3, 2);
         p_Abt_Mix->codef = ' ';
        p_Abt_Mix->codeg = ' ';
        p_Abt_Mix->abmos = 0;
    }
    else
    {
        if (Conversion_Taux_Abt(p_Abt_Cne_2003->tpac3) != -1)
        {
            p_Abt_Mix->apac3 = arrondi_euro_voisin(0.01 * Conversion_Taux_Abt(p_Abt_Cne_2003->tpac3) * p_Abt_Col_N->vlmoy);
        }
        else
        {
            p_Abt_Mix->apac3 = p_Abt_Cne_2003->apac3;
        }
        strncpy(p_Abt_Mix->tpac3,p_Abt_Cne_2003->tpac3, 3);
    }

    if (*p_codef != ' ' || *p_codeg != ' ' )
    {
        p_Abt_Mix->abmos = 0;
        p_Abt_Mix->codef = ' ';
        p_Abt_Mix->codeg = ' ';
    }
}

/* mixage Alsace Moselle */

void Abt_colb_Mix_Als_Mos ( long * pabmos, char * p_codef, char * p_codeg, s_cole * p_Abt_Cne_2003, s_cole * p_Abt_Col_N, s_colb * p_Abt_Mix)
{
    s_colb Abt_Fic;
    s_colb * p_Abt_Fic = &Abt_Fic;
    Abt_colb_Charge (pabmos, p_codef, p_codeg, p_Abt_Cne_2003,p_Abt_Mix);
    Abt_colb_Charge (pabmos, p_codef, p_codeg, p_Abt_Col_N, p_Abt_Fic);
    strncpy(p_Abt_Mix->tpac1,p_Abt_Fic->tpac1, 3);
    p_Abt_Mix->apac1 = p_Abt_Fic->apac1;
    strncpy(p_Abt_Mix->tpac3,p_Abt_Fic->tpac3, 3);
    p_Abt_Mix->abhan=p_Abt_Fic->abhan;
    strncpy(p_Abt_Mix->txhan,p_Abt_Fic->txhan, 3);
    p_Abt_Mix->apac3 = p_Abt_Fic->apac3;
    p_Abt_Mix->abmos = 0;
    p_Abt_Mix->codef = ' ';
    p_Abt_Mix->codeg = ' ';
}

/* initialisation des abattements */

void Abt_colb_Init (s_colb * p_Abt_Init)
{
    p_Abt_Init->vlmoy = 0;
    p_Abt_Init->abspe = 0;
    p_Abt_Init->abbas = 0;
    p_Abt_Init->apac1 = 0;
    p_Abt_Init->apac3 = 0;
    p_Abt_Init->abhan = 0;
    p_Abt_Init->abmos = 0;
    p_Abt_Init->codef = ' ';
    p_Abt_Init->codeg = ' ';
    strncpy(p_Abt_Init->txbas,"00", 3);
    strncpy(p_Abt_Init->txspe,"00", 3);
    strncpy(p_Abt_Init->tpac1,"00", 3);
    strncpy(p_Abt_Init->tpac3,"00", 3);
    strncpy(p_Abt_Init->txhan,"00", 3);
}

void Abt_colb_Charge (long * p_abmos, char * p_codef, char * p_codeg, s_cole * p_Abt_cole_Source, s_colb * p_Abt_colb)
{
    p_Abt_colb->vlmoy = p_Abt_cole_Source->vlmoy;
    p_Abt_colb->abspe = p_Abt_cole_Source->abspe;
    p_Abt_colb->abbas = p_Abt_cole_Source->abbas;
    p_Abt_colb->apac1 = p_Abt_cole_Source->apac1;
    p_Abt_colb->apac3 = p_Abt_cole_Source->apac3;
    p_Abt_colb->abhan = p_Abt_cole_Source->abhan;
    strncpy(p_Abt_colb->txbas , p_Abt_cole_Source->txbas, 3);
    strncpy(p_Abt_colb->txspe , p_Abt_cole_Source->txspe, 3);
    strncpy(p_Abt_colb->tpac1 , p_Abt_cole_Source->tpac1, 3);
    strncpy(p_Abt_colb->tpac3 , p_Abt_cole_Source->tpac3, 3);
    strncpy(p_Abt_colb->txhan , p_Abt_cole_Source->txhan, 3);
    p_Abt_colb->abmos = *p_abmos;
    p_Abt_colb->codef = *p_codef;
    p_Abt_colb->codeg = *p_codeg;
}

/* recopie des abattements d'une collectivité pour N */

void Abt_colb_Recopie (s_cole * p_Abt_cole_Cible, s_colb * p_Abt_colb_Source, s_colb * p_Abt_colb_Cible)
{
    p_Abt_colb_Cible->vlmoy = p_Abt_colb_Source->vlmoy;
    p_Abt_colb_Cible->abspe = p_Abt_colb_Source->abspe;
    p_Abt_colb_Cible->abbas = p_Abt_colb_Source->abbas;
    p_Abt_colb_Cible->apac1 = p_Abt_colb_Source->apac1;
    p_Abt_colb_Cible->apac3 = p_Abt_colb_Source->apac3;
    p_Abt_colb_Cible->abhan = p_Abt_colb_Source->abhan;
    strncpy(p_Abt_colb_Cible->txbas , p_Abt_colb_Source->txbas, 3);
    strncpy(p_Abt_colb_Cible->txspe , p_Abt_colb_Source->txspe, 3);
    strncpy(p_Abt_colb_Cible->tpac1 , p_Abt_colb_Source->tpac1, 3);
    strncpy(p_Abt_colb_Cible->tpac3 , p_Abt_colb_Source->tpac3, 3);
    strncpy(p_Abt_colb_Cible->txhan , p_Abt_colb_Source->txhan, 3);
    p_Abt_colb_Cible->abmos = p_Abt_colb_Source->abmos;
    p_Abt_colb_Cible->codef = p_Abt_colb_Source->codef;
    p_Abt_colb_Cible->codeg = p_Abt_colb_Source->codeg;
}
