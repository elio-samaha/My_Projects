//SOUAIBY Christina
//SAMAHA Elio

public abstract class Vigne extends Ressource implements Evolution{
  // Attributs
  private static String[] saisons= {"Printemps", "Ete", "Automne", "Hiver"};
  protected static int saison=0; // 0 : printemps
  // Constructeurs
  public Vigne(String nom){
    super(nom,200);
  }
  // Methodes
  public static void passageSaisons(){ 
    saison=(saison+1)%4;
  }

  public void croissance(){
   if (saison==3){
      this.setQuantite(this.getQuantite()-25);
    }
    else{
       this.setQuantite(this.getQuantite()+10);
    }
  }
  // Accesseurs
  public static int getSaison(){
    return saison;
  }
  public static String[] get4Saisons(){
    return saisons;
  }
}