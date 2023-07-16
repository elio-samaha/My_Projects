//SOUAIBY Christina
//SAMAHA Elio

public class MachineAVendanger extends Agent{
  // Attributs
  private static int count = 0;
  private int id;
  
  // Constructeurs
  public MachineAVendanger(int x, int y, Terrain t){
    super(x,y,t);
    count++;
    id=count;
  }
  public MachineAVendanger(Terrain t){
    super(t);
    count++;
    id=count;
  }
  public MachineAVendanger(MachineAVendanger m, Terrain t){//copie
    this(m.x,m.y, t);
    count++;
    id=count;
  }
  
  // Methodes
  public void sortirTerrain(){
    if (estActif){
      if (nbActifs>=NBACTIFSMAX || (inListeActifs(x, y)!=null && inListeActifs(x, y)!=this)){
        estActif=false;
        nbActifs--;
        listeActifs.remove(this);
        if (nbActifs >= NBACTIFSMAX) System.out.println("Trop d'agents sur le terrain.");
        if (inListeActifs(x, y)!=null && inListeActifs(x, y)!=this) System.out.println("Il y'a déjà un agent à la place de Machine "+id+".");
        System.out.println("Machine "+id+" est sorti(e) du terrain");
      }
    }
  }
  public String toString(){
    return "Moi machine "+ id + ". Moi pas fatigué."+super.toString();
  }
  public void recolter(Ressource r, Terrain t){
    if (r == null) return;
    quantiteRecoltes+=(int)(0.3*r.getQuantite()+1);
    r.setQuantite((int)(0.7*r.getQuantite()-1));
    System.out.println("Machine "+id+ " a récolté "+ (int) (0.2 * r.getQuantite()+1) +" kg de raisins "+( (r instanceof VigneChardonnay)?"Chardonnay":"Muscat") ); 
    if (r.getQuantite()<=0) r=t.videCase(x,y);
  }
  public int getId(){
    return id;
  }
}