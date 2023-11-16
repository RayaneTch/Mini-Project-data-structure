#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
typedef char nom[30];
typedef struct {
    char NomClient[30];
    int NUM_Ticket;             /*les infos relatifs à chaque client*/
    char Guichet;
}infoClient;
typedef struct maillon {
    infoClient donnee;           /*Création d'une liste chainee et chaque maillon représente un client*/
    struct maillon *suiv;
}Client;
typedef struct {
    Client *tetef,*queue;
}File;
typedef struct {
    File file;
    char nom_guichet;
         /*une file dynamique(sous forme d'une liste) */
}info_guichets;                      /*tete pointe vers l'adresse du premier maillon et la queue pointe vers le dernier element de la liste */

typedef struct {
    int indice;
    char nmGCHT;
} infEclat;

typedef struct
{
  Client *sommet;    // On aura besoin d'une pile pour équilibrer les guichets dans les "Opérations en Vrac"
}PILE ;

/*********************MODULES FILES DYNAMIQUES**********************************/
bool FileVide(File f)
{
  return (f.tetef==NULL);
}
/****************************************************************************/
void insertfile(info_guichets *f,Client *c){
    c->suiv=NULL;
    if (FileVide(f->file)) {
        f->file.tetef=c;
        f->file.queue=c;
    }
    else {
        f->file.queue->suiv=c;
        f->file.queue=c;
    }
}
/****************************************************************************/
void init(info_guichets *f){
    f->file.tetef  = NULL;
    f->file.queue  = NULL;

}
/****************************************************************************/
bool defiler(Client **x,File *f)
{

    if(FileVide(*f)){
        printf("la file est vide impossible de defiler ");
        return false;
    }
    else
    {
        *x=f->tetef;
        f->tetef=f->tetef->suiv;
    return true;
    }
}
/****************************************************************************/
void affiche(File f){
    Client *p;
          p=f.tetef;
          if(f.tetef==NULL)
           printf("Le guichet  est vide \n");   // Rôle : afficher une file
         else
         {
          while(p!=NULL)
           {
            printf("-------------------------------------------------------------------------------------------------------------\n");
            printf("NOM CLIENT : %s || GUICHET : %c || NUM_TICKET : %d",p->donnee.NomClient,p->donnee.Guichet,p->donnee.NUM_Ticket);
            printf("\n");
            p=p->suiv;
           }
         }
}
/**********************************************************************/
bool GuichetVide(char gcht,int nbrguichets,info_guichets tab[])
{
  int i=0;
  bool trouv=false;
  while(!trouv && i<nbrguichets)
  {                                               // Rôle : vérifier si un guichet est vide
      if(tab[i].nom_guichet==gcht)
      {
       trouv=true;
       return (tab[i].file.tetef==NULL);
      }
      i++;
  }
}
/*********************************************************************/
bool Verf_Guichet(char gcht , char GuichetMAX)
{
   return (  'A' <= gcht && gcht < GuichetMAX);   // Rôle : véririer si un guichet existe ou pas
}
/********************************************************************/



               /**********************************AJOUTER DES CLIENTS******************************************/
                                /*-------------------(a)-----------------------*/
bool ExistClient(nom nomclient,info_guichets GuichetAcceuil,int nbrclients)
{
    bool trouv=false;
    Client *p;
    p=GuichetAcceuil.file.tetef;
    while(!trouv && (p!=NULL))
    {
        if(strcmp(p->donnee.NomClient,nomclient)==0)
            trouv = true;
        p==p->suiv;
    }
    return trouv;
}
void Ajout_Clients(int nbr_clients,int t,info_guichets tab[],info_guichets *GCHTACC,int *numticket,char GuichetMAX){
    Client *p,*q;                      /*t c'est la taille */
    char gcht_choisi;
    int i,j;
    for (j=0;j<nbr_clients;j++){
           p=malloc(sizeof(Client));    // L'utilisation de deux pointeur pour ajouter dans le guichet d'acceuil et le guichet spécifié dans le tableau contenant tous les guichets
           q=malloc(sizeof(Client));
           printf("|-----veuillez donner le guichet que le client (%d) va recevoir-------------|\n",j);
           printf("--> ");
           scanf(" %c",&gcht_choisi);
           gcht_choisi=toupper(gcht_choisi);
           while(!Verf_Guichet(gcht_choisi,GuichetMAX))
                     {
                             printf("Le guichet choisi ne figure pas dans la liste des guichets existants , veuillez reassayer ");
                             scanf(" %c",&gcht_choisi);
                             gcht_choisi=toupper(gcht_choisi);
                     }
           q->donnee.Guichet=p->donnee.Guichet=toupper(gcht_choisi);  //p et q pcq je vais ajouter dans le guichet d'acceuil et le guichet propre
           q->donnee.NUM_Ticket=p->donnee.NUM_Ticket=*numticket;  //  au client
           printf("veuillez donner le nom du client : ");
                        // La lecture d'une variable de type 'chaine de caractères' avec éventuellement des espaces
          fflush(stdin);
          gets(p->donnee.NomClient);
          strcpy(q->donnee.NomClient,p->donnee.NomClient);
          insertfile(GCHTACC,q);
    for (i=0;i<t;i++){
                 if((tab[i].nom_guichet)==gcht_choisi)
                      insertfile(&tab[i],p);
      }
       (*numticket)++;  // incrémentation du numéro du ticket
   }
}
                             /***************************Reception des clients*******************************/

void Afficher_Clients(char guichet,int taille,info_guichets tab[]){
    int i;
    for (i=0;i<taille;i++){
        if(guichet==tab[i].nom_guichet){
          if(GuichetVide(guichet,taille,tab))              // Rôle : afficher la file d'attente d'un guichet
          {
            printf("\n-----------------********---- Guichet Vide ----********-----------------------\n ");
          }
          else
           affiche(tab[i].file);
        }
        }
    }
/***************************************************************************************/
 Client* Ajout_au_debut(char ngcht,info_guichets tab[],int taille, Client *q)
 {
    int i;
    q->donnee.Guichet=ngcht;
     for(i=0;i<taille;i++)
     {
       if(tab[i].nom_guichet==ngcht)
       {
            if(GuichetVide(ngcht,taille,tab))     // cette fonction est utilisé dans le cas du choix '2' redirection d'un client vers un autre guichet
              {
                  tab[i].file.tetef=tab[i].file.queue=q;
                  q->suiv=NULL;
              }
             else {
                    q->suiv=tab[i].file.tetef;
                    tab[i].file.tetef=q;
                 }
       }
    }
    return q;
 }

/*************************************************************************************************/
 void Supp_Client(info_guichets *GuichetAcceuil,nom nclient){
    Client *p,*q,*tmp;
    p=(*GuichetAcceuil).file.tetef;
  if(GuichetAcceuil->file.tetef==NULL)
     printf("Tous les clients sont passés , le guichet d'acceuil est vide ");
  else {
    if(strcmp(p->donnee.NomClient,nclient)==0)
      {
        defiler(&tmp,&(*GuichetAcceuil).file);      // si le client à supprimer se trouve à la tête du guichet d'acceuil
        free(tmp);
      }
      else {
             while ((p!=NULL) && (strcmp(p->donnee.NomClient,nclient)!=0))
            {
                 q=p;
                 p=p->suiv;                       // chercher le client dans le guichet d'acceuil
            }
           q->suiv=p->suiv;
           free(p);                      // libérer le maillon contenant le client à supprimer
           }
       }
}

/*****************************************************************************************/
int Recev_client(char ch1,info_guichets tab[],int taille,info_guichets *GuichetAcceuil,char GuichetMAX,int *clients){     // il est important de mettre le guichet d'acceuil
   int i,chx,j;                                                                 // comment type pointeur dans les paramètres
   Client *q,*t,*p;                                                // car on le modifie après la suppression
   char ngcht;
   for(i=0;i<taille;i++){
       if(tab[i].nom_guichet==ch1)
       {
        printf("----Bonjour  %s ----------\n",tab[i].file.tetef->donnee.NomClient);
        Supp_Client(&(*GuichetAcceuil),tab[i].file.tetef->donnee.NomClient);
        puts("----(1)-----------------Traiter la requete du client-------------------");
        puts("----(2)----------------Redirection vers un autre guichet-------------------");
        printf("Votre decision : ");
        scanf("%d",&chx);
        switch (chx)
        {
        case 1 : {           printf("---------------*******--OK, requete traitee-*******---------------\n ");
        for(i=0;i<taille;i++)
        {
          if(tab[i].nom_guichet == ch1)
          {                                         // la requête du client étant traitée , il sera supprimé du guichet d'acceuil et de son guichet
          defiler(&p,&tab[i].file);
          free(p);
          }
        }
        return (*clients--);      // Décrémentation du nombre de clients du fait de la supperssion
            break;
        }
        case 2 : {
             printf("veuillez choisir le guichet de redirection : ");
             scanf(" %c",&ngcht);
             ngcht=toupper(ngcht);  // si l'utilisateur donne une lettre miniscule
                     while(!Verf_Guichet(ngcht,GuichetMAX))          // Vérifier si le guichet saisi figure dans liste des guichets qui existent
                     {
                             printf("Le guichet choisi ne figure pas dans la liste des guichets existants , veuillez reassayer \n");
                             printf("-->");
                             scanf(" %c",&ngcht);
                             ngcht=toupper(ngcht);
                     }
             defiler(&q,&tab[i].file);
             q->donnee.Guichet=ngcht;
             t= Ajout_au_debut(ngcht,tab,taille,q);
              for(j=0;j<taille;j++){
               if(tab[j].nom_guichet==ngcht)             // affichage de la nouvelle file d'attente du guichet choisi
               {
                printf("\n voici la nouvelle file d'attente du guichet %c \n",ngcht);
                affiche(tab[j].file);
               }
              }
              return *clients;
                  //la suppression du client après la redirection doit se faire après l'ajout à l'autre guichet
             break;                         // car on va libérer le maillon client au cours du défilement
           }


        default:
           return *clients;
            break;
        }
       }
   }
}
/*********************************************************************************************/
void ChngNomGcht(char nvGCHT,Client **ptr)
{
    (*ptr)->donnee.Guichet=nvGCHT;
            // Rôle : changement du nom du guichet d'un client après un éventuel transfert
}
    /*********************************** *ECLATEMENT *****************************************/


void Eclatement(info_guichets tab[],int taille,int nbrGchtselec,Client *tete,char GuichetMAX)
{
  int i,j,l=0;
  char GchtSelec;
  infEclat t_eclat[nbrGchtselec];
  Client *k,*q,*t;
  printf("Veuillez choisir les guichets : ");
  for(i=0;i<nbrGchtselec;i++)
  {
    scanf(" %c",&GchtSelec);
    GchtSelec=toupper(GchtSelec);
    while(!Verf_Guichet(GchtSelec,GuichetMAX))
    {
        printf("Le guichet n'existe pas, veuillez reassayer : ");
      scanf(" %c",&GchtSelec);
      GchtSelec=toupper(GchtSelec);
    }
    while(!GuichetVide(GchtSelec,taille,tab))          // le guichet choisi doit être vide
    {
      printf("vous avez choisi un guichet non vide veuillez reassayer : ");
      scanf(" %c",&GchtSelec);
      GchtSelec=toupper(GchtSelec);
    }

     t_eclat[i].nmGCHT=GchtSelec;

   for(j=0;j<taille;j++)
   {
    if(tab[j].nom_guichet==GchtSelec)
    {
     t_eclat[l].indice=j;            // on sauvegarde l'indice de chaque guichet vide choisi dans le tableau "t_eclat" ainsi que le nom du guichet
     l++;
    }
   }
  }
   t = tete;

   q=tete->suiv;
   tete->donnee.Guichet=t_eclat[0].nmGCHT;
   insertfile(&(tab[t_eclat[0].indice]),t);
   i=1;
   while(q!=NULL)
   {
    k=q;
    ChngNomGcht(t_eclat[i % nbrGchtselec].nmGCHT,&k);    // on change le guichet du client qu'on a déplacé du guichet principal au nouveau guichet ("Guichet" dans infoclients)
    q=q->suiv;
    insertfile(&(tab[t_eclat[i % nbrGchtselec].indice]),k);       // le "i % nbrGchtselec " sert à faire une insertion circulaire des clients du guichet principal
                                        // pour garder la trace de l'ancien guichet on va pas changer le guichet du client dans le guichet d'acceuil
    i++;                                                       // dans les nouveaux guichets choisis
   }

}
/******************************************************************/
bool Rech_Gcht(char gcht,char t_gcht[],int taille1)
{
  int i;
  bool trouv=false;
  for(i=0;i<taille1;i++)
  {                             // Rôle : vérifier l'existence du guichet 'gcht' dans le tableau t_gcht
    if(t_gcht[i]==gcht)
     trouv=true;
  }
  return trouv;
}

/***************************Modules Piles Dynamiques*********************************/
void CreerPile(PILE *pile)
{
  pile->sommet = NULL;
}
/************************************************************************************/
bool PileVide(PILE pile)
{
  return(pile.sommet==NULL);
}
/*************************************************************************************/
void empiler(Client *x,PILE *pile)
{
  Client *q;                    //il est important d'utiliser un pointeur intermédiaire sinon un chainage de plus sera effectué à la fin de l'empilement
  q=malloc(sizeof(*q));
  q=x;
  q->suiv=pile->sommet;
  pile->sommet=q;
}
/***********************************************************************************/
void depiler(Client **x,PILE *pile)
{

    *x=pile->sommet;
    pile->sommet=pile->sommet->suiv;
}
/***********************************************************************************/
bool Verf_Equilibre(info_guichets tab[], int nbrguichets, int nbrclients)
{
  int i, cpt, nb_guichets_extra_clients;
  bool equilibre = true;                   // Rôle : vérifier si on a besoin d'équilibrer les guichets ou pas
  Client *p;

  // Calculer le nombre de clients que chaque guichet devrait avoir
  int nb_clients_par_guichet = nbrclients / nbrguichets;         // si nbr_clients_par_guichet == 0 alors au moin un guichet est vide ...(*)

  // Calculer le nombre de guichets qui doivent avoir un client supplémentaire
  nb_guichets_extra_clients = nbrclients % nbrguichets;

  // Parcourir chaque guichet et vérifier qu'il a le bon nombre de clients
  for (i = 0; i < nbrguichets; i++)
  {
    cpt = 0;
    p = tab[i].file.tetef;
    while (p != NULL) {
      cpt++;
      p = p->suiv;
    }
    if (cpt == nb_clients_par_guichet)   // d'après (*) cette condition sera vérifiée et ça va poser aucun problème si un guichet ou plus est vide
                                         // donc pas la peine de vérifier les guichets entrés ne comportent pas de guichets vides
    {
      // Ce guichet a le bon nombre de clients
    }
       else if (cpt == nb_clients_par_guichet + 1 && nb_guichets_extra_clients > 0)
        {
          // Ce guichet a un client de plus que les autres guichets
           nb_guichets_extra_clients--;
        }
          else
           {
             // Ce guichet n'a pas le bon nombre de clients
             equilibre = false;
             break;
           }
  }

  return equilibre;
}
/**************************************************************************************/
void Change_numTckt(info_guichets GuichetAccueil, info_guichets tab[],int nbr_guichets)
{
  Client *tmp,*tmp1;
  bool trouv;
  int i,nouv_tckt=1;
  tmp=GuichetAccueil.file.tetef;
    while(tmp!=NULL)
    {
                             // on change le numéro de ticket du client dans le guichet d'acceuil selon sa positon qui est sauvegardée dans le tableau
        i=0;       // tab_num_tickets
       trouv = false;
       tmp->donnee.NUM_Ticket = nouv_tckt;
       while(!trouv && (i<nbr_guichets))
       {

            if(tab[i].nom_guichet == tmp->donnee.Guichet)
            {
              tmp1 = tab[i].file.tetef ;            // on change le numéro de ticket du client dans le guichet correspondant grâce aux informations contenu dans tmp
                while((tmp1!=NULL) && !trouv)             // quand on l'a trouvé (càd : le nom du guichet et le nom du client)
                {
                  if(strcmp(tmp1->donnee.NomClient,tmp->donnee.NomClient) == 0 )
                  {
                    tmp1->donnee.NUM_Ticket = nouv_tckt;
                    trouv = true;
                    nouv_tckt++;
                  }
                   tmp1 = tmp1->suiv;
                }

            }
            i++;
       }
           tmp = tmp->suiv;
    }

}
/*****************************************FIN IMPLEMENTATION DES MODULES****************************************/
int main (){
    /*---------------------------------PARTIE DECLARATION ------------------------------------------*/
    char choix_gcht,ch_gcht,GuichetMAX;          //NOTER BIEN : Le rôle des variables déclarées portant de noms imprécis va s'éclaircir dans la suite du programme
    bool trouv,AjouterClients=false,trouv1;    // AjouterClient c'est pour s'assurer que l'utilisateur ajoute d'abord des clients aux guichets
    int n,j=0,cpt=0,clients;           // au début du programme avant de traiter les autres opérations
    Client *tmp,*tmp1,*k,*p;
    int numticket=1;
    int nbrGchtVides=0,nbrGchtselec;
    int choix,nbr_guichets,nbr_clients=0,i,g;
    info_guichets GuichetAcceuil;
    int taille,GchtDestination;
    char ngcht='A';
    int choix1;     // on va l'utiliser dans les opérations en vrac
    PILE pile;                         // le type "PILE" se trouve tout en haut du programme
    printf("|--------------------------------------------------BIENVENUE-----------------------------------------------------------|\n");
    printf("|------------------Veuillez donner le nombre de guichets dans le centre hors celui de l'acceuille----------------------|\n");
    printf("Nombre de guichets : ");
    scanf("%d",&nbr_guichets);
    taille=nbr_guichets;
     info_guichets tab[taille];
     init (&GuichetAcceuil);           // Initialiser la file d'attente du guichet d'acceuil
     printf("----Guichets : ");
    for(i=0;i<nbr_guichets;i++)             // Tous les guichets sont représentés par des lettres majuscules en commençant par 'A'
    {
          printf(" %c | ",ngcht);
          tab[i].nom_guichet=ngcht;        // Afficher les guichets disponibles dans le centre selon le nombre de guichets saisi
          ngcht++;
        init(&tab[i]);            // Initialisation de toutes les files d'attentes de tous les guichets
    }
     GuichetMAX=ngcht;          // Le GuichetMAX servira après à vérifier si l'utilisateur entre un guichet qui existe ou pas, dans l'ajout des clients
    do {
    puts("\n                                    _________________________________________");
    puts("\n                                                     -MENU-                   ");
    puts("                                    _________________________________________");
    puts("|----(1)-------------------------------------Ajouter des clients-------------------------------------------------------|");
    puts("|----(2)------------------------------------Reception des clients -----------------------------------------------------|");
    puts("|----(3)-------Afficher la file d'attente globale avec tous les clients avec son propre guichet------------------------|");
    puts("|----(4)-----------------------Operations en vrac sur les files d'attente----------------------------------------------|");
    puts("|----(5)------------------------------------------Quitter--------------------------------------------------------------|");
    printf("Choix ? : ");
    scanf("%d",&choix);
    switch (choix){
         case 1 : {
           AjouterClients=true;
           printf("\n");
           puts("|--------------------------------------------Vous etes au guichet d acceuil--------------------------------------------|");
           puts("|--------------------------------veuillez donner le nombre de clients a ajouter----------------------------------------|");
           printf("Nombre de clients : ");
           scanf("%d",&nbr_clients);
           clients+=nbr_clients;                    // la variable "clients" servira à garder le dernier nbrclients au cas où l'utilisateur rajoutera d'autres clients car la lecture se fait par "nbrclients"
           Ajout_Clients(nbr_clients,nbr_guichets,tab,&GuichetAcceuil,&numticket,GuichetMAX);
           puts("|------------------------------------------Guichet Acceuil------------------------------------------------------|");
           affiche(GuichetAcceuil.file);
           break;
         }

         case 2 : {
            if(!AjouterClients)
                puts("Veuillez d abord remplir les guichets, traitez la 1ere option ");
        else
        {
             tmp=GuichetAcceuil.file.tetef;
           if(tmp==NULL)
          printf("Pas de clients , operation non permise");
          else
          {
            printf("\n");
            printf("\n");
            puts("|-------------------------------------------------------------------------------------------------------------|");
            puts("|---------------------------------------Reception des clients-------------------------------------------------|");
            puts("|--------------------------------Afficher la liste des clients d'un guichet-----------------------------------|");
            puts("Quel guichet voulez vous afficher : ");
            scanf(" %c",&ch_gcht);
            ch_gcht=toupper(ch_gcht);
             while(!Verf_Guichet(ch_gcht,GuichetMAX))          // Vérifier si le guichet saisi figure dans liste des guichets qui existent
                     {
                             printf("Le guichet choisi ne figure pas dans la liste des guichets existants , veuillez reassayer \n");
                             printf("-->");
                             scanf(" %c",&ch_gcht);
                             ch_gcht=toupper(ch_gcht);
                     }
            Afficher_Clients(ch_gcht,nbr_guichets,tab);
            if(GuichetVide(ch_gcht,nbr_guichets,tab))
                printf("Le guichet est vide , l'operation ne peut pas etre effectuee");
            else
            {

            puts("---------------------------------------Recevoir un client---------------------------------------------------");
            nbr_clients=Recev_client(ch_gcht,tab,taille,&GuichetAcceuil,GuichetMAX,&nbr_clients);
            printf("\n");
            printf("\n");
            puts("|------------------------------------------AFFICHAGE-------------------------------------------|");
            printf("\n|-----------------------Voici la nouvelle file d'attente du guichet %c ------------------------|",ch_gcht);
            Afficher_Clients(ch_gcht,nbr_guichets,tab);
          }
        }
        }
        break;

        }

         case 3 :
     {
             if(!AjouterClients)
                puts("Veuillez d abord remplir les guichets, traitez la 1ere option ");
        else
        {

            puts("|---------------------Affichage de la file d attente globale---------------------------|\n");
            for(i=0;i<nbr_guichets;i++)
            {
                printf("\n------------------------<<  Guichet %c >>---------------------------\n",tab[i].nom_guichet);
                affiche(tab[i].file);
            }
        }
            break;


     }


        case 4 :
 {
          if(!AjouterClients)
            puts("Veuillez d'abord remplir les guichets , traitez la 1ere option ");
        else
   {
        tmp=GuichetAcceuil.file.tetef;
           if(tmp==NULL)
          printf("Pas de clients , operation non permise");
          else
          {
            do {
                 puts("|------------------------------------------Menu------------------------------------------|");
                 puts("|----(1)--------Rediriger une file d'attente sur 'n' autres guichets vides---------------|");
                 puts("|----(2)--------Rediriger deux files d'attente ou plus vers le meme guichet--------------|");
                 puts("|----(3)------------Reequilibrer la taille des files d'attente---------------------------|");
                 puts("|----(4)----------------Reinitialisation des numeros de tickets--------------------------|");
                 puts("|----(5)------------------------Retourner a l'ecran precedent----------------------------|");
    printf("Choix ? ");
    scanf("%d",&choix1);
    switch(choix1)
  {

        case 1 :
     {

        puts("|---------------------Redirection d'une file d'attente d'un guichet sur 'n' autres guichets vides ----------------|");
        trouv=false;
         for(i=0;i<nbr_guichets;i++)
         {
          if(GuichetVide(tab[i].nom_guichet,nbr_guichets,tab))
          {
            trouv=true;
            nbrGchtVides++;
          }
         }
         if(!trouv)
         {
         printf("Pas de guichets vides . ");
         printf("Cette operation ne peut pas etre effectuee , Merci\n");
         }
         else
    {
        printf("Veuillez choisir un guichet :");
        scanf(" %c",&ngcht);
        ngcht=toupper(ngcht);
        while(!Verf_Guichet(ngcht,GuichetMAX))
        {
         puts("Le guichet saisi n'existe pas, reassayez");
         printf("--> ");
         scanf(" %c",ngcht);
         ngcht = toupper(ngcht);
        }
         while(GuichetVide(ngcht,nbr_guichets,tab))
          {
            printf("\n----------------Le guichet choisi est vide , veuillez choisir un autre guichet------------------");
            printf("--> ");
            scanf(" %c",&ngcht);
            ngcht=toupper(ngcht);
          }
         puts("|--------------------------------Voici actuellement les guichets vides----------------------------------|");
         printf("-----> GUICHETS : ");
         for(i=0;i<nbr_guichets;i++)
         {
          if(GuichetVide(tab[i].nom_guichet,nbr_guichets,tab))
          {
            printf("%c |",tab[i].nom_guichet);                          // on affiche les guichets vides disponibles
          }
         }
          puts("\nVeuillez selectionner le nombre et le nom des guichets que vous allez choisir ");
          printf("----Nombre :");
          scanf("%d",&nbrGchtselec);                       // demander à choisir le nombre et les guichets
           while(nbrGchtselec > nbrGchtVides)
           {
            puts("Le nombre de guichets choisis depasse le nombre maximal des guichets vides, veuillez reassayer");
            printf("---Nombre : ");                        // vérifier si le nombre de guichets choisi ne dépasse pas le nombre de guichets vides disponibles
            scanf("%d",&nbrGchtselec);
           }
          for(i=0;i<nbr_guichets;i++)
          {
            if(tab[i].nom_guichet==ngcht)      // on se positionne sur file d'attente du guichet à éclater choisi
             k=tab[i].file.tetef;
          }
            // ce tableau va contenir les  guichets où on a éclaté le guichet choisi
          Eclatement(tab,nbr_guichets,nbrGchtselec,k,GuichetMAX);
          for(i=0;i<nbr_guichets;i++)
          {
              if(tab[i].nom_guichet==ngcht)
              while(!FileVide(tab[i].file))
              {
                  defiler(&p,&tab[i].file);
              }
          }

         // le type "infEclat" se trouve tout en haut du code

        // la tete étant sauvegardée au début , on va la supprimer et le guichet éclaté deviendra alors vide du fait
        // de la redirection de ses clients vers de nouveaux guichets (initialement vides)
         }

          break;
      }
        case 2 :
       {

          puts("-----------------------------Redirection de  'n >= 2 ' files d'attente vers un guichet -----------------------------");
          printf("Combien de files d'attente voulez vous rediriger : ");
          scanf("%d",&n);
          while(n<2)
          {
              printf("Erreur , veuillez choisir un nombre superieur ou egal a 2 : ");
              scanf("%d",&n);
          }
           char t_gcht[n];
          printf("Veuillez choisir les guichets concernés : ");
          for(i=0;i<n;i++)
          {
           printf("--> ");                                 //puisque on est entrain de rediriger des clients donc leur requête n'est pas encore traité d'où on va utiliser
          scanf(" %c",&ch_gcht);  // le guichet d'acceuil afin de respecter aussi l'ordre des clients
          ch_gcht=toupper(ch_gcht);
            while(!Verf_Guichet(ch_gcht,GuichetMAX))
             {
              printf("Ce guichet n'existe pas , reassayer ");
              printf("--> ");
              scanf(" %c",&ch_gcht);
              ch_gcht=toupper(ch_gcht);
                             // on va sauvegarder le nom des guichets dans un tableau de caractères
             }

             while(GuichetVide(ch_gcht,nbr_guichets,tab))
                {
              printf("Le guichet choisi est vide , reassayez ");
              printf("--> ");
              scanf(" %c",&ch_gcht);
              ch_gcht=toupper(ch_gcht);
                              // on va sauvegarder le nom des guichets dans un tableau de caractères
             }
              t_gcht[j]=ch_gcht;
           j++;
          }
          printf("Veuillez donner le guichet ou vous allez les rediriger \n");
          printf("--> ");
          scanf(" %c",&ngcht);
          ngcht=toupper(ngcht);
          while(!Verf_Guichet(ngcht,GuichetMAX))
             {
              printf("Le guichet choisi n'existe pas , reassayez  : ");
              printf("--> ");
              scanf(" %c",&ngcht);
              ngcht=toupper(ngcht);
             }
             trouv1=false;
            i=0;
          while(!trouv1 && (i<nbr_guichets))
          {
            if(tab[i].nom_guichet==ngcht)
            {
                GchtDestination=i;        // sauvegarder le guichet destinataire
                trouv=true;
            }
            i++;
          }
          printf("des : %c ",tab[GchtDestination].nom_guichet);
             for(i=0;i<nbr_guichets;i++)
             {
                 if(Rech_Gcht(tab[i].nom_guichet,t_gcht,n))
                 {
                          // on se positoinne sur l'un des guichets choisis
                     while(!FileVide(tab[i].file))
                     {
                         defiler(&tmp1,&(tab[i].file));
                         ChngNomGcht(ngcht,&tmp1);     // changer l'ancien guichet du client par le nouveau
                         insertfile(&tab[GchtDestination],tmp1);   // insérer dans le guichet destinataire


                     }

                 }
             }

           break;
        }
        case 3 :
     {

          CreerPile(&pile);
          puts("--------------------------------------Equilibrer la taille ---------------------------------------------");
          if(Verf_Equilibre(tab,nbr_guichets,clients))
          printf("Toutes les files d'attente sont equilibrees ");
          else
        {
          for(i=0;i<nbr_guichets;i++)
          {
            while(!FileVide(tab[i].file))
            {
            defiler(&p,&tab[i].file);
            empiler(p,&pile);
            }
          }
          i=0;
          while(!PileVide(pile))
          {
              depiler(&p,&pile);
               p->donnee.Guichet=tab[i % nbr_guichets].nom_guichet;                    // il est important de mettre un passage par adresse pour 'p' sinon le prochain dépilement ne sera pas pris en compte
              insertfile(&tab[i % nbr_guichets],p);           // la même chose pour defiler
              i++;
          }
         }
          break;
      }
        case 4 :
        {

          puts("--------------------Reinitialisation des numeros de tickets effectue avec succes------------------------");
          Change_numTckt(GuichetAcceuil,tab,nbr_guichets);
              break;
        }

    }
   }while(choix1!=5);
   }
   }
          break;
    }


 }
    }while(choix!=5);
    puts("|------------------------------------------FIN DU TRAVAIL----------------------------------------------------|");
    exit(0);

  return 0;


}


