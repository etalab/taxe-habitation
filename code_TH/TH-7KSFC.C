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

#define LA_SIGNATURE

#if defined (ILIAD)
    #include <th-7ksts.h>
    #include <th-7kfos.h>
#else
    #include "th-7ksts"
    #include "th-7kfos"
#endif


/* La fonction th_7ksfc permet de determiner les droits a degrevements/
   exoneration et le revenu fiscal de reference a prendre en compte pour le
   calcul du plafonnement de taxe d'habitation a 3,5% du Revenu Fiscal de
   Reference, en fonction des elements de personnalisation et des revenus
   (informations IR).                                                         */


/*============================================================================
                     PROTOTYPAGE DES FONCTIONS
  ============================================================================*/
int controle_ksfc(s_e4 * p_e4 , s_s4 * p_s4);
void init_s4(s_reds *);
char restitution_seuil(s_typseuil * pc, short nbr_part, long revenue);

long determination_revff(long srevf,
                         short demipart1,
                         short demipart2,
                         short demipart3,
                         short demipart4,
                         short autredemipart,
                         short quartpart1,
                         short quartpart2,
                         short quartpart3,
                         short quartpart4,
                         short autrequartpart,
                         s_rev rev_abat);



/*============================================================================
   M A I N
  ============================================================================*/
int th_7ksfc(s_e4 * p_e4, s_s4 * p_s4)
{
    extern s_signature la_signature;

    int ret_cons;  /* zone retour de recherche des constantes */
    int ret;
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
    long srevf;

    s_ese         ese;
    s_sse         sse;
    s_cons * p_cons;
    s_typseuil * pc;

    p_s4->versr='B';
    p_s4->anref=2017;

    /*============*/
    /* TRAITEMENT */
    /*============*/

    /* controle de la signature */
    p_s4->signature = &la_signature ;

    ret = controle_signature(RKSFC,p_s4->versr, &(p_s4->libelle));
    if (ret != 0 )
    {
        return (ret);
    }

    /* 1 appel de la fonction taux annuel */

    ret_cons = (short)cherche_const(p_e4->antax,&p_cons);
    cherche_Erreur(ret_cons,&(p_s4->libelle)) ;
    if (ret_cons != 0)               /* retour de la fonction */
    {
        return (ret_cons);
    }

    /* 2 appel de la fonction controle du th_ksfc */
    ret = controle_ksfc(p_e4 , p_s4);
    if ( ret !=0 )
    {
        cherche_Erreur(ret,&(p_s4->libelle)) ;
        return ret;
    }

    /* 3 initialisation des zones sorties   */

    init_s4(&(p_s4->reds_1));
    init_s4(&(p_s4->reds_2));
    init_s4(&(p_s4->reds_c));
    init_s4(&(p_s4->reds_f));


    /* 4 determination du departement pour chargement du pointeur sur typseuil
     */
    if ( strncmp(p_e4->csdi,dep97,2) == 0 )
    {
        pc = &(p_cons->typseuil_dom_124);              /* trait dom */
        if ( strcmp(p_e4->csdi,dir973) == 0 )
        {
            pc = &(p_cons->typseuil_973);         /* trait guyane */
        }
        else
        if ( strcmp(p_e4->csdi,dir976) == 0 )
        {
            pc = &(p_cons->typseuil_976);         /* trait Mayotte */
        }
    }
    else
    {
        pc = &(p_cons->typseuil_metro);                /* trait metropole */
    }

    /* 5    appel du module th_7ksec  */
    ese.antax = p_e4->antax;       /* chargement des zones entree du th_7ksec */
    strncpy(ese.csdi , p_e4->csdi, 4);
    ese.cnua = p_e4->cnua;
    ese.rede_1.revfe = p_e4->rede_1.revfe;
    ese.rede_1.nbpar = p_e4->rede_1.nbpar;
    ese.rede_1.front = p_e4->rede_1.front;
    ese.rede_1.annir = p_e4->rede_1.annir;
    ese.rede_2.revfe = p_e4->rede_2.revfe;
    ese.rede_2.nbpar = p_e4->rede_2.nbpar;
    ese.rede_2.front = p_e4->rede_2.front;
    ese.rede_2.annir = p_e4->rede_2.annir;

    ret = th_7ksec(&ese,&sse);         /* appel du th_7ksec */

    if (ret != 0)                      /* code retour du th_7ksec */
    {
        cherche_Erreur(ret,&(sse.libelle));
        p_s4->libelle=sse.libelle;
        return (ret);
    }

    /* recuperation des zones sortie du th_7ksec */
    p_s4->reds_1.seuil = sse.reds_1.seuil;
    p_s4->reds_1.seuilb= sse.reds_1.seuilb;
    p_s4->reds_1.revim = sse.reds_1.revim;
    p_s4->reds_2.seuil = sse.reds_2.seuil;
    p_s4->reds_2.seuilb= sse.reds_2.seuilb;
    p_s4->reds_2.revim = sse.reds_2.revim;
    p_s4->reds_c.seuil = sse.reds_c.seuil;
    p_s4->reds_c.seuilb= sse.reds_c.seuilb;
    p_s4->reds_c.revim = sse.reds_c.revim;
    p_s4->signature    = sse.signature;


    /* 6    seuil de synthese du foyer fiscal  */
    if (p_e4->cnua == 'R')
    {
        p_s4->reds_f.seuil = ' ';
        p_s4->reds_f.seuilb= ' ';
        p_s4->revff = 0;
        p_s4->reds_f.revim = 0;
        p_s4->npaff = 0;
        srevf = 0;
        return 0;
    }
    else
    {
    /* calcul du nb de parts total et du revenu total */
        p_s4->npaff = p_e4->rede_r.nbpar;
        srevf = p_e4->rede_r.revfe;
        if ((p_e4->rede_1.front != 'K' ) || (p_s4->reds_1.revim != 0 ))
        {
            p_s4->npaff = p_s4->npaff + p_e4->rede_1.nbpar;
            srevf = srevf + p_s4->reds_1.revim;
        }
        if ((p_e4->rede_2.front != 'K' ) || (p_s4->reds_2.revim != 0 ))
        {
            p_s4->npaff = p_s4->npaff + p_e4->rede_2.nbpar;
            srevf = srevf + p_s4->reds_2.revim;
        }
        p_s4->reds_f.revim = srevf;
    }

    if (p_s4->npaff != 0) /* il existe un declarant (eventuellement rattache) */
    {
        determination_parts(p_s4->npaff,&demipart1,&demipart2,&demipart3,&demipart4,&autredemipart,
                            &quartpart1,&quartpart2,&quartpart3,&quartpart4,&autrequartpart);

        p_s4->revff=determination_revff(srevf,demipart1,demipart2,demipart3,demipart4,autredemipart,
                                        quartpart1,quartpart2,quartpart3,quartpart4,autrequartpart,
                                        pc->rev_abat);

        p_s4->reds_f.seuil = determination_droits(srevf,' ',demipart1,demipart2,demipart3,demipart4,autredemipart,
                                                  quartpart1,quartpart2,quartpart3,quartpart4,autrequartpart,
                                                  pc);
        if (p_e4->antax > 2015)
        {
            p_s4->reds_f.seuilb = determination_droits_w(srevf,p_s4->npaff,pc);
        }
        else
        {
            p_s4->reds_f.seuilb = ' ';
        }


    /* non frontalier et declarant */
        if ((p_e4->rede_r.front != 'K') && (p_s4->reds_c.seuil != ' ')
        && ((p_e4->rede_1.front != 'K') || (p_e4->rede_1.revfe != 0))
        && ((p_e4->rede_2.front != 'K') || (p_e4->rede_2.revfe != 0)))
        {
            if ((p_s4->reds_f.seuil == '0') || (p_s4->reds_f.seuil == '2'))
            {
                p_s4->reds_f.seuil = 'A';
            }
        }
    /* frontalier ou occupants en titre non declarants */
        if ((p_e4->rede_r.front == 'K') || (p_s4->reds_c.seuil == ' ')
        || ((p_e4->rede_1.front == 'K') && (p_e4->rede_1.revfe == 0))
        || ((p_e4->rede_2.front == 'K') && (p_e4->rede_2.revfe == 0)))
        {
            p_s4->reds_f.seuil = '3';
        }
    }
    else                             /* non declarant */
    {
        p_s4->reds_f.seuil = ' ';
        p_s4->revff = 0;
        p_s4->reds_f.revim = 0;
    }

    if (p_s4->npaff != 0) /* il existe un declarant (eventuellement rattache) */
    {
            determination_parts(p_s4->npaff,
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
    }

    p_s4->reds_1.serfrc = restitution_seuil(pc, p_e4->rede_1.nbpar, p_s4->reds_1.revim);
    p_s4->reds_2.serfrc = restitution_seuil(pc, p_e4->rede_2.nbpar, p_s4->reds_2.revim);
    p_s4->reds_c.serfrc = restitution_seuil(pc, p_e4->rede_1.nbpar
                                               +p_e4->rede_2.nbpar, p_s4->reds_c.revim);
    p_s4->reds_f.serfrc = restitution_seuil(pc, p_s4->npaff       , p_s4->reds_f.revim);

    /* 7 sortie du module */
    return 0;
}

/*============================================================================*/

    /*----------------------*/
    /*   fonction init_s4   */
    /*----------------------*/

void init_s4(s_reds * p)
{
    p->seuil  = ' ';
    p->seuilb = ' ';
    p->serfrc = '0';
}


    /*------------------------------*/
    /* fonction determination_revff */
    /*------------------------------*/

long determination_revff(long srevf, short demipart1,short demipart2,
                         short demipart3, short demipart4,short autredemipart,
                         short quartpart1, short quartpart2, short quartpart3,
                         short quartpart4, short autrequartpart, s_rev rev_abat)
{
    long tot_abat_rev;

    tot_abat_rev=(rev_abat.revenu_unepart +
                 (demipart1 * (rev_abat.rev_prem_demipart))      +
                 (demipart2 * (rev_abat.rev_deuxieme_demipart))  +
                 (demipart3 * (rev_abat.rev_troisieme_demipart)) +
                 (demipart4 * (rev_abat.rev_quatrieme_demipart)) +
                 (autredemipart * (rev_abat.rev_autre_demipart)) +
                 (quartpart1 * arrondi_euro_voisin((rev_abat.rev_prem_demipart)      * 0.5)) +
                 (quartpart2 * arrondi_euro_voisin((rev_abat.rev_deuxieme_demipart)  * 0.5)) +
                 (quartpart3 * arrondi_euro_voisin((rev_abat.rev_troisieme_demipart) * 0.5)) +
                 (quartpart4 * arrondi_euro_voisin((rev_abat.rev_quatrieme_demipart) * 0.5)) +
                 (autrequartpart * arrondi_euro_voisin((rev_abat.rev_autre_demipart) * 0.5)));
    if (srevf <= tot_abat_rev)
    {
        return 0;
    }
    else
    {
        return (srevf - tot_abat_rev);
    }
}

    /*---------------------*/
    /*  controles generaux */
    /*---------------------*/

int controle_ksfc(s_e4 * p_e4 , s_s4 * p_s4)
{
    int part;

    if (recherche_csdi(p_e4->csdi) == 0 )         /* si direction non trouvee */
    {
        cherche_Erreur(4302,&(p_s4->libelle)) ;
        return (4302);
    }

    /* nature d'affectation */
    if ((p_e4->cnua != 'S') && (p_e4->cnua != 'C') && (p_e4->cnua != 'R'))
    {
        cherche_Erreur(4303,&(p_s4->libelle)) ;
        return (4303);
    }

    /* code frontalier */
    if ((p_e4->rede_1.front != ' ') && (p_e4->rede_1.front != 'K'))
    {
        cherche_Erreur(4304,&(p_s4->libelle)) ;
        return (4304);
    }

    if ((p_e4->rede_2.front != ' ') && (p_e4->rede_2.front != 'K'))
    {
        cherche_Erreur(4305,&(p_s4->libelle)) ;
        return (4305);
    }

    if ((p_e4->rede_r.front != ' ') && (p_e4->rede_r.front != 'K'))
    {
        cherche_Erreur(4306,&(p_s4->libelle)) ;
        return (4306);
    }

    if ((p_e4->cnua == 'R') && (p_e4->rede_2.nbpar > 0))   /* nombre de parts */
    {
        cherche_Erreur(4307,&(p_s4->libelle)) ;
        return (4307);
    }

    if ((p_e4->cnua == 'R') && (p_e4->rede_1.nbpar < 100))
    {
        cherche_Erreur(4308,&(p_s4->libelle)) ;
        return (4308);
    }

    if ((p_e4->cnua == 'S') && (p_e4->rede_2.nbpar > 0))
    {
        cherche_Erreur(4309,&(p_s4->libelle)) ;
        return (4309);
    }

    part=(int)(p_e4->rede_1.nbpar*4);
    if (part != arrondi_dizaine_inf(arrondi_dizaine_inf(part) * 0.1) * 10)
    {
        cherche_Erreur(4310,&(p_s4->libelle)) ;
        return (4310);
    }

    part=(int)(p_e4->rede_2.nbpar*4);
    if (part != arrondi_dizaine_inf(arrondi_dizaine_inf(part) * 0.1) * 10)
    {
        cherche_Erreur(4311,&(p_s4->libelle)) ;
        return (4311);
    }

    part=(int)(p_e4->rede_r.nbpar*4);
    if (part != arrondi_dizaine_inf(arrondi_dizaine_inf(part) * 0.1) * 10)
    {
        cherche_Erreur(4312,&(p_s4->libelle)) ;
        return (4312);
    }

    if ((p_e4->rede_1.front == 'K') && (p_e4->rede_1.nbpar < 100))
    {
        cherche_Erreur(4313,&(p_s4->libelle)) ;
        return (4313);
    }

    if ((p_e4->rede_2.front =='K') && (p_e4->rede_2.nbpar < 100))
    {
        cherche_Erreur(4314,&(p_s4->libelle)) ;
        return (4314);
    }

    if ((p_e4->rede_r.front == 'K') && (p_e4->rede_r.nbpar < 100))
    {
        cherche_Erreur(4315,&(p_s4->libelle)) ;
        return (4315);
    }

    if (p_e4->rede_1.nbpar < 0)
    {
        cherche_Erreur(4316,&(p_s4->libelle)) ;
        return (4316);
    }

    if (p_e4->rede_2.nbpar < 0)
    {
        cherche_Erreur(4317,&(p_s4->libelle)) ;
        return (4317);
    }

    if (p_e4->rede_r.nbpar < 0)
    {
        cherche_Erreur(4318,&(p_s4->libelle)) ;
        return (4318);
    }

    if (p_e4->rede_1.revfe < 0)                           /* revenu imposable */
    {
        cherche_Erreur(4319,&(p_s4->libelle)) ;
        return (4319);
    }

    if (p_e4->rede_2.revfe < 0)
    {
        cherche_Erreur(4320,&(p_s4->libelle)) ;
        return (4320);
    }

    if (p_e4->rede_r.revfe < 0)
    {
        cherche_Erreur(4321,&(p_s4->libelle)) ;
        return (4321);
    }

    if ((p_e4->rede_r.nbpar ==0) && ((p_e4->rede_r.front == 'K')
    ||  (p_e4->rede_r.revfe > 0)))
    {
        cherche_Erreur(4322,&(p_s4->libelle)) ;
        return (4322);
    }

    if ((p_e4->rede_r.nbpar == 100) && ((p_e4->rede_r.front == 'K')
     && (p_e4->rede_r.revfe > 0)))
    {
        cherche_Erreur(4323,&(p_s4->libelle)) ;
        return (4323);
    }

    if ((p_e4->rede_r.annir < (p_e4->antax - 1)) && (p_e4->rede_r.nbpar > 0))
    {
        cherche_Erreur(4326,&(p_s4->libelle)) ;
        return (4326);
    }

    if (p_e4->rede_1.annir > p_e4->antax - 1)
    {
        cherche_Erreur(4327,&(p_s4->libelle)) ;
        return (4327);
    }

    if (p_e4->rede_2.annir > p_e4->antax - 1)
    {
        cherche_Erreur(4328,&(p_s4->libelle)) ;
        return (4328);
    }

    if (p_e4->rede_r.annir > p_e4->antax - 1)
    {
        cherche_Erreur(4329,&(p_s4->libelle)) ;
        return (4329);
    }
    return 0;                            /* tous les controles sont corrects */
}

char restitution_seuil(s_typseuil * pc, short nbr_part, long revenu)
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
    long seuil_1414AI;
    long seuil_1417I;
    long seuil_1417II;
    s_rev rev_1414;
    s_rev rev_1417I;
    s_rev rev_1417II;

    rev_1414 = pc->rev_abat;
    rev_1417I = pc->rev_tousdroits;
    rev_1417II = pc->rev_plaf;

    if (revenu==0)
    {
        return '0';
    }
    else
    {
        determination_parts(nbr_part,
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

        seuil_1414AI = ((rev_1414.revenu_unepart                  )
        + ((long) demipart1      * rev_1414.rev_prem_demipart     )
        + ((long) demipart2      * rev_1414.rev_deuxieme_demipart )
        + ((long) demipart3      * rev_1414.rev_troisieme_demipart)
        + ((long) demipart4      * rev_1414.rev_quatrieme_demipart)
        + ((long) autredemipart  * rev_1414.rev_autre_demipart    )
        + ((long) quartpart1     * arrondi_euro_voisin((rev_1414.rev_prem_demipart)      * 0.5))
        + ((long) quartpart2     * arrondi_euro_voisin((rev_1414.rev_deuxieme_demipart)  * 0.5))
        + ((long) quartpart3     * arrondi_euro_voisin((rev_1414.rev_troisieme_demipart) * 0.5))
        + ((long) quartpart4     * arrondi_euro_voisin((rev_1414.rev_quatrieme_demipart) * 0.5))
        + ((long) autrequartpart * arrondi_euro_voisin((rev_1414.rev_autre_demipart)     * 0.5)));

        if (revenu <= seuil_1414AI)
        {
            return '1';
        }
        else
        {
            seuil_1417I = ((rev_1417I.revenu_unepart                  )
            + ((long) demipart1      * rev_1417I.rev_prem_demipart     )
            + ((long) demipart2      * rev_1417I.rev_deuxieme_demipart )
            + ((long) demipart3      * rev_1417I.rev_troisieme_demipart)
            + ((long) demipart4      * rev_1417I.rev_quatrieme_demipart)
            + ((long) autredemipart  * rev_1417I.rev_autre_demipart    )
            + ((long) quartpart1     * arrondi_euro_voisin((rev_1417I.rev_prem_demipart)      * 0.5))
            + ((long) quartpart2     * arrondi_euro_voisin((rev_1417I.rev_deuxieme_demipart)  * 0.5))
            + ((long) quartpart3     * arrondi_euro_voisin((rev_1417I.rev_troisieme_demipart) * 0.5))
            + ((long) quartpart4     * arrondi_euro_voisin((rev_1417I.rev_quatrieme_demipart) * 0.5))
            + ((long) autrequartpart * arrondi_euro_voisin((rev_1417I.rev_autre_demipart)     * 0.5)));

            if (revenu <= seuil_1417I)
            {
                return '2';
            }
            else
            {
                seuil_1417II = ((rev_1417II.revenu_unepart                  )
                + ((long) demipart1      * rev_1417II.rev_prem_demipart     )
                + ((long) demipart2      * rev_1417II.rev_deuxieme_demipart )
                + ((long) demipart3      * rev_1417II.rev_troisieme_demipart)
                + ((long) demipart4      * rev_1417II.rev_quatrieme_demipart)
                + ((long) autredemipart  * rev_1417II.rev_autre_demipart    )
                + ((long) quartpart1     * arrondi_euro_voisin((rev_1417II.rev_prem_demipart)      * 0.5))
                + ((long) quartpart2     * arrondi_euro_voisin((rev_1417II.rev_deuxieme_demipart)  * 0.5))
                + ((long) quartpart3     * arrondi_euro_voisin((rev_1417II.rev_troisieme_demipart) * 0.5))
                + ((long) quartpart4     * arrondi_euro_voisin((rev_1417II.rev_quatrieme_demipart) * 0.5))
                + ((long) autrequartpart * arrondi_euro_voisin((rev_1417II.rev_autre_demipart)     * 0.5)));

                if (revenu <= seuil_1417II)
                {
                    return '3';
                }
                else
                {
                    return '4';
                }
            }
        }
    }
}
