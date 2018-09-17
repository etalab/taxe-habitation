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


/* La fonction th_7ksec permet de determiner les droits a degrevements et
   exoneration , et la condition de cohabitation en fonction des elements de
   personnalisation (informations DPH) et des revenus (informations IR).
*/

int controle_ksec( s_ese * p_e, s_sse * p_s);

void init_s(s_reds *);

/*============================================================================
   M A I N
  ============================================================================*/
int th_7ksec(s_ese * p_e, s_sse * p_s)
{
    extern s_signature la_signature ;
    int ret_cons;  /* zone retour de recherche des constantes */
    int ret;  /* zone retour strcmp */
    int retp;   /*zone retour programme th_nksec  */
    int rets;   /*zone retour signature  */

    short part1;
    short part2;
    short partcum;
    long rev1;
    long rev2;
    long revcum;
    char front;
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
    char resul1;
    char resul2;
    char resul3;
    char resul1_w;
    char resul2_w;
    char resul3_w;
    static char dep97[]="97";
    static char dir973[]="973";
    static char dir976[]="976";

    s_cons * p_cons;
    s_typseuil * pc;
    front=' ';
    retp=0;
    resul1=' ';
    resul2=' ';
    resul3=' ';
    resul1_w=' ';
    resul2_w=' ';
    resul3_w=' ';
    p_s->versr='B';
    p_s->signature = &la_signature ;

    p_s->anref=2017;

    part1=p_e->rede_1.nbpar;
    part2=p_e->rede_2.nbpar;
    partcum=part1 + part2;
    rev1=0;
    rev2=0;
    revcum=0;

    /* TRAITEMENT */
    /* ---------- */

    /*     controle de la signature          */

    rets=controle_signature(RKSEC,p_s->versr,&(p_s->libelle));

    if (rets!=0)
    {
         return(rets);
    }

    /* appel de la fonction taux annuel */
    ret_cons=(short)cherche_const(p_e->antax,&p_cons);
    cherche_Erreur(ret_cons,&(p_s->libelle)) ;
    if (ret_cons != 0)  /* retour de la fonction*/
    {
        return (ret_cons);
    }


    ret=controle_ksec(p_e,p_s);
    if ( ret !=0 )
    {
        cherche_Erreur(ret,&(p_s->libelle)) ;
        return ret;
    }

    /*    initialisation des zones sorties    */

    init_s(&(p_s->reds_1));
    init_s(&(p_s->reds_2));
    init_s(&(p_s->reds_c));


    /*  determination du departement pour  */
    /* chargement des pointeurs sur typseuil et typseuil_w*/

    pc=&(p_cons->typseuil_metro);    /*trait metropole*/

    ret=strncmp(p_e->csdi,dep97,2);
    if (ret==0)            /*trait dom*/
    {
        pc=&(p_cons->typseuil_dom_124);
    }

    ret=strcmp(p_e->csdi,dir973);
    if (ret==0)            /*trait guyane*/
    {
        pc=&(p_cons->typseuil_973);
    }

    ret=strcmp(p_e->csdi,dir976);
    if (ret==0)            /*trait Mayotte*/
    {
        pc=&(p_cons->typseuil_976);
    }
    /*    stockage des revenus    */

    rev1=p_e->rede_1.revfe;
    rev2=p_e->rede_2.revfe;
    revcum=rev1 + rev2;

    /*     appel determination 1er contribuable  */

    if(part1!=0)
    {
        determination_parts(part1,&demipart1,&demipart2,&demipart3,&demipart4,
                            &autredemipart,&quartpart1,&quartpart2,&quartpart3,
                            &quartpart4,&autrequartpart);
        resul1=determination_droits(rev1,p_e->rede_1.front,demipart1,
                                    demipart2,demipart3,demipart4,autredemipart,
                                    quartpart1,quartpart2,quartpart3,quartpart4,
                                    autrequartpart,pc);
        if (p_e->antax > 2015)
        {
            resul1_w=determination_droits_w(rev1,part1,pc);
        }
        else
        {
            resul1_w = ' ';
        }
    }
    if((p_e->cnua=='S')||(p_e->cnua=='R'))
    {
        resul3=resul1;
        resul3_w=resul1_w;
    }

    /*     appel determination 2me contribuable  */

    if(p_e->cnua=='C')
    {
        if(part2!=0)
        {
            determination_parts(part2,&demipart1,&demipart2,&demipart3,
                                &demipart4,&autredemipart,&quartpart1,
                                &quartpart2,&quartpart3,&quartpart4,
                                &autrequartpart);
            resul2=determination_droits(rev2,p_e->rede_2.front,demipart1,
                                        demipart2,demipart3,demipart4,
                                        autredemipart,quartpart1,quartpart2,
                                        quartpart3,quartpart4,autrequartpart,
                                        pc);
            if (p_e->antax > 2015)
            {
                resul2_w=determination_droits_w(rev2,part2,pc);
            }
            else
            {
                resul2_w = ' ';
            }

        }

        if(p_e->rede_1.front=='K')
        {
            front=p_e->rede_1.front;
            if(rev1==0)
            {
                revcum=0;
            }
        }
        if(p_e->rede_2.front=='K')
        {
            front=p_e->rede_2.front;
            if(rev2==0)
            {
                revcum=0;
            }
        }


    /*    appel determination cumul    */

        if((resul1!=' ')&&(resul2!=' '))
        {
            if((resul1=='0')&&(resul2=='0'))
            {
                resul3='0';
            }
            else
            {
                determination_parts(partcum,&demipart1,&demipart2,&demipart3,
                                    &demipart4,&autredemipart,&quartpart1,
                                    &quartpart2,&quartpart3,&quartpart4,
                                    &autrequartpart);
                resul3=determination_droits(revcum,front,demipart1,
                                            demipart2,demipart3,demipart4,
                                            autredemipart,quartpart1,quartpart2,
                                            quartpart3,quartpart4,
                                            autrequartpart,pc);

            }
        }
        else
        {
            if(resul1==' ')
            {
                if((resul2=='0')||(resul2=='2'))
                {
                    resul3='2';
                }
                if((resul2=='3')||(resul2==' '))
                {
                    resul3=' ';
                }
            }
            else
            {
                if((resul1=='0')||(resul1=='2'))
                {
                    resul3='2';
                }
                if((resul1=='3')||(resul1==' '))
                {
                    resul3=' ';
                }
            }
        }

        if((resul1_w!=' ')&&(resul2_w!=' '))
        {
            if((resul1_w=='0')&&(resul2_w=='0'))
            {
                resul3_w='0';
            }
            else
            {
                determination_parts(partcum,&demipart1,&demipart2,&demipart3,
                                    &demipart4,&autredemipart,&quartpart1,
                                    &quartpart2,&quartpart3,&quartpart4,
                                    &autrequartpart);
                resul3_w=determination_droits_w(revcum,partcum,pc);
            }
        }
        else
        {
            resul3_w = ' ';
        }
        if ((resul3=='0')&&((resul1 != '0')||(resul2 != '0')))
        {
            resul3='2';
        }
    }
    p_s->reds_1.seuil=resul1;
    p_s->reds_2.seuil=resul2;
    p_s->reds_c.seuil=resul3;
    p_s->reds_1.seuilb=resul1_w;
    p_s->reds_2.seuilb=resul2_w;
    p_s->reds_c.seuilb=resul3_w;
    p_s->reds_1.revim=rev1;
    p_s->reds_2.revim=rev2;
    p_s->reds_c.revim=rev1 + rev2;

    /*    sortie du module    */


    return (retp);
}


    /*      fonction init_s4   */

void init_s(s_reds * p)
{
    p->seuil=' ';
    p->revim=0;
}


/*==============================================================================
   CONTROLES GENERAUX
  ============================================================================*/

int controle_ksec( s_ese * p_e, s_sse * p_s)
{
    short part;
    int retour=0;
    retour=recherche_csdi(p_e->csdi);
    if (retour==0)   /* si direction non trouvee  */
    {
        return (4002);
    }

    /* traitement annee taxation */

    if ((p_e->antax < p_s->anref-3 )||((p_e->antax > p_s->anref )))
    {
        return (1);
    }

    if ((p_e->cnua!='S')&&(p_e->cnua!='C')&&(p_e->cnua!='R'))
    {
        return (4003);
    }

    if ((p_e->rede_1.front!=' ')&&(p_e->rede_1.front!='K'))
    {
        return (4004);
    }

    if ((p_e->rede_2.front!=' ')&&(p_e->rede_2.front!='K'))
    {
        return (4005);
    }

    if ((p_e->cnua=='R')&&(p_e->rede_2.nbpar>0))
    {
        return (4006);
    }

    if ((p_e->cnua=='R')&&(p_e->rede_1.nbpar<100))
    {
        return (4007);
    }

    if ((p_e->cnua=='S')&&(p_e->rede_2.nbpar>0))
    {
        return (4008);
    }

    part=(short)((p_e->rede_1.nbpar*4)/10);

    if (part!=arrondi_dizaine_inf(part))
    {
        return (4009);
    }

    part=(short)((p_e->rede_2.nbpar*4)/10);

    if (part!=arrondi_dizaine_inf(part))
    {
        return (4010);
    }

    if ((p_e->rede_1.front=='K')&&(p_e->rede_1.nbpar<100))
    {
        return (4013);
    }

    if ((p_e->rede_2.front=='K')&&(p_e->rede_2.nbpar< 100))
    {
        return (4014);
    }

    if (p_e->rede_1.nbpar<0)
    {
        return (4015);
    }

    if (p_e->rede_2.nbpar<0)
    {
        return (4016);
    }

    if (p_e->rede_1.revfe<0)
    {
        return (4017);
    }

    if (p_e->rede_2.revfe<0)
    {
        return (4018);
    }

    if (p_e->rede_1.annir>p_e->antax - 1)
    {
        return (4021);
    }
    if (p_e->rede_2.annir>p_e->antax - 1)
    {
        return (4022);
    }

    /*     tous les controles sont corrects       */
    return 0;
}
