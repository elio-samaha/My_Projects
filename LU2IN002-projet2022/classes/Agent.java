//SOUAIBY Christina
//SAMAHA Elio
import java.util.ArrayList;

public abstract class Agent {
  // Attributs
  protected int x, y;
  protected boolean estActif;
  public final int NBACTIFSMAX = 12;
  protected static int nbActifs = 0;
  protected static int quantiteRecoltes = 0;
  protected static ArrayList<Agent> listeActifs = new ArrayList<>(); 
  
  // Constructeurs
  public Agent(int x, int y, Terrain t) {
    this.x = x;
    this.y = y;
    estActif = (t.sontValides(x, y) && (inListeActifs(x,y)==null) && nbActifs < NBACTIFSMAX) ;
    if (estActif) listeActifs.add(this);
    nbActifs = (estActif) ? (nbActifs + 1) : (nbActifs);
  }

  public Agent(Terrain t) {
    this((int) (Math.random() * t.nbLignes), (int) (Math.random() * t.nbColonnes), t);
  }

  // Methodes
  public double distance(int x, int y) {
    return Math.sqrt(Math.pow(x - this.x, 2) + Math.pow(y - this.y, 2));
  }
  
  public void seDeplacer(int xnew, int ynew) {
    if (inListeActifs(xnew,ynew)==null){
      String agent=(this instanceof Homme)?(((Homme)this).getNom()):("Machine "+(((MachineAVendanger)this).getId()));
      System.out.println(agent+" se déplace en ("+xnew+","+ynew+") car la ressource en ("+x+","+y+") est épuisée.");
      x = xnew;
      y = ynew;
    }
  }

  public void trouverPlusProcheRessource(Terrain t){
    double dmin = 100;
    int a = x;
    int b = y;
    for (int i=0; i<t.nbLignes; i++){
      for (int j=0; j<t.nbColonnes; j++){
        if (t.caseEstVide(i,j) || (i==x && j==y) || (inListeActifs(x, y)!=null && inListeActifs(x, y)!=this) ) 
          continue;
        else{
          if (distance(i,j)<dmin){
            dmin=distance(i,j);
            a=i;
            b=j;
          }
        }
      }
    }
    seDeplacer(a,b);
  }
  
  public Agent inListeActifs(int x, int y){
    if (listeActifs!=null){
      for (Agent a : listeActifs){
        if ((x == a.x) && (y == a.y) && a!=this){
          return a;
        }
      }
    }
    return null;
  }
  
  public String toString() {
    return " | Position (" + x + "," + y + ") | " + ((estActif) ? "Actif" : "Inactif");
  }
  
  public abstract void recolter(Ressource r, Terrain t);

  public void transformer(Vin r){
    int bouteille = 200 * (quantiteRecoltes/125);
    quantiteRecoltes = quantiteRecoltes%125;
    r.setQuantite(r.getQuantite() + bouteille);
  }

  public void entrerTerrain(){
    if (!estActif){
      if (nbActifs < NBACTIFSMAX && (inListeActifs(x , y)==null) ) {
        estActif = true;
        if (this instanceof Homme) {
          Homme temp = (Homme)this;
          System.out.println(temp.getNom()+" est rentré(e) sur le terrain");
        }
        if (this instanceof MachineAVendanger) System.out.println("Machine "+((MachineAVendanger)this).getId() + " est rentré(e) sur le terrain");
        listeActifs.add(this);
        nbActifs++;
      }
    }
  }

  public abstract void sortirTerrain();
  
  // Accesseurs
  public static int getNbActif() {
    return nbActifs;
  }

  public int getX() {
    return x;
  }

  public int getY() {
    return y;
  }

  public boolean getEstActif() {
    return estActif;
  }
  public static ArrayList<Agent> getListeActifs(){
    return listeActifs;
  }
}