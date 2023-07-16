//SOUAIBY Christina
//SAMAHA Elio

public class Homme extends Agent {
  // Attributs
  public final int DUREEMAX = 8;
  private String nom;
  private boolean estVoleur;
  protected int dureeTravail;

  // Constructeurs
  public Homme(int x, int y, String nom, boolean estVoleur, Terrain t) {
    super(x, y, t);
    this.estVoleur = estVoleur;
    this.nom = nom;
    dureeTravail = 0;
  }
  public Homme(int x, int y, String nom, Terrain t) {
    super(x, y, t);
    this.estVoleur = Math.random() < 0.4;
    this.nom = nom;
    dureeTravail = 0;
  }

  public Homme(String nom, Terrain t) {
    super(t);
    this.estVoleur = Math.random() < 0.4;
    this.nom = nom;
    dureeTravail = 0;
  }

  // Methodes
  public void updateDureeTravail() {
    dureeTravail+=2;
  }
  
  public void sortirTerrain() {
    if (estActif){
      if (dureeTravail >= DUREEMAX || nbActifs >= NBACTIFSMAX || (inListeActifs(x, y)!=null && inListeActifs(x, y)!=this)  ) {
        nbActifs--;
        estActif = false;
        listeActifs.remove(this);
        if (nbActifs >= NBACTIFSMAX) System.out.println("Trop d'agents sur le terrain.");
        if (inListeActifs(x, y)!=null && inListeActifs(x, y)!=this) System.out.println("Il y'a déjà un agent à la place de "+nom+".");
        if (dureeTravail >= DUREEMAX) System.out.println(nom+" a travaillé jusqu'à présent "+dureeTravail+" heures. Il/Elle est fatigué(e).");
        System.out.println(nom+" est sorti(e) du terrain");
        dureeTravail=0;
      }
    }
  }

  public void recolter(Ressource r, Terrain t) {
    if (r == null) return;
    if (estVoleur) {
      System.out.println("AU VOLEUR ! La récolte de " + ( (r instanceof VigneChardonnay)?"Chardonnay":"Muscat" ) + " en (" + r.getX() + "," + r.getY()
          + ") a été volée par un Homme malhonnête, 50% a été perdu. Le coupable est : " + nom );
      r.setQuantite((int) (0.5 * r.getQuantite()-1));
    } else {
      System.out.println(nom+ " a récolté "+ (int) (0.2 * r.getQuantite()+1) +" kg de raisins "+( (r instanceof VigneChardonnay)?"Chardonnay":"Muscat")) ; 
      quantiteRecoltes += (int) (0.2 * r.getQuantite()+1);
      r.setQuantite((int) (0.8 * r.getQuantite()-1));
    }
    if (r.getQuantite()<=0) r=t.videCase(x,y);
  }

  public String toString() {
    return "Je m'appelle " + nom + ", j'ai travaillé jusqu'à présent " + dureeTravail + " heures de " + DUREEMAX + " heures." + super.toString();
  }

  // Accesseurs
  public boolean getEstVoleur() {
    return estVoleur;
  }
  public String getNom(){
    return nom;
  }
}