//SOUAIBY Christina
//SAMAHA Elio

import java.util.*;

public class Simulation{
  // Attributs
  private Terrain t;
  private ArrayList<Agent> listeAgent;
  private ArrayList<Ressource> listeRessources;
  public final static int ITERATIONS = 16; //4 saisons
  public final static String[] noms = {"Elio", "Christina", "Maissa", "Sarah", "Nadia",
                               "Rayan", "Ina", "Nino", "Dominique", "Siwar", "Lounes", 
                               "Maria", "Paul", "Lea", "Valentin", "Catalina", "Marie"};
  // Constructeur
  public Simulation(int m , int n) throws ListeVideException{
    if (n == 0 || m == 0) throw new ListeVideException();
    // CREATION DU TERRAIN
    t = new Terrain(10,10);
    // CREATION DES RESSOURCES
    for (int i = 0 ; i < m ; i++){
      int ligne = (int)(Math.random()*10);
      int col = (int)(Math.random()*10);      
      if (t.caseEstVide(ligne , col)){
        if ((i % 2) == 0)
          t.setCase(ligne, col, new VigneChardonnay());
        else 
          t.setCase(ligne, col, new VigneMuscat());
      }
    }
    listeRessources = t.lesRessources();
    // CREATION DES AGENTS
    listeAgent = new ArrayList<Agent>() ;
    for (int i = 0 ; i < n ; i++){
      if (Math.random()<0.3)
        listeAgent.add(new MachineAVendanger(t)); 
      else listeAgent.add(new Homme(noms[i%noms.length], t)); 
    }
  
  }

  public void commencer(){
    // AFFICHAGE DE L'ETAT INITIAL DU TERRAIN
    System.out.println("\nBienvenue dans le Vignoble de Christina et Elio ! Bonne Visite :) ");
    t.affiche(16);
    System.out.println("Permettez nous de vous présenter les Vignerons :\n");
    for (Agent a: listeAgent){
      System.out.println(a);
    }
    System.out.println("\nEt maintenant, passons aux choses sérieuses!\n");
  }
  // PROCEDURE
  /* Pour éviter un affichage trop chargé, on se contente de représenter un jour de chaque saison, et de modéliser     une partie des entrées et sorties sur le terrain.
  Chaque jour compte 8heures de travail. Chaque incrementation de i est considérée comme 2heures.
  On considère que la vente se fait chaque jour.
  */
  public void simuler(int i) throws InterruptedException{
    if (i%4==1) System.out.println("\nSaison actuelle: " + (Vigne.get4Saisons())[Vigne.getSaison()] +"\n");
    for (Agent a : listeAgent){ // parcours des agents actifs et inactifs 
        a.entrerTerrain();// on fait entrer un agent en cas de besoin
        if (a.getEstActif()){ //on fait travailler les actifs
          if (t.caseEstVide(a.getX(), a.getY())){
            a.trouverPlusProcheRessource(t);
          }
          a.recolter(t.getCase(a.getX(), a.getY()),t);
          if ( t.getCase(a.getX(), a.getY()) instanceof VigneChardonnay)
            a.transformer(VinChardonnay.getInstance());
          if ( t.getCase(a.getX(), a.getY()) instanceof VigneMuscat)
            a.transformer(VinMuscat.getInstance());
          if (a instanceof Homme && a.getEstActif())
            ((Homme)a).updateDureeTravail();
        }
        a.sortirTerrain();
      }
      if (i%4==0){
        // fin de journee
        (VinChardonnay.getInstance()).vendre();
        (VinMuscat.getInstance()).vendre();
        // Affichage etat actuel
        Thread.sleep(3000);
        t.affiche(16);
        // passage saison
        Vigne.passageSaisons();
        for (Ressource r: listeRessources){
          if (r instanceof Vigne)
            ((Vigne)r).croissance();
        }
      }
  }
    
}