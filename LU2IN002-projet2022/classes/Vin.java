//SOUAIBY Christina
//SAMAHA Elio

public abstract class Vin extends Ressource{
  // Constructeurs
  public Vin(String nom){
    super(nom,0);
  }

  // Methodes
  public void vendre(){
    if (this.getQuantite()>0){
      int x = (int)(Math.random()*201);
      this.setQuantite(this.getQuantite() - x); 
      System.out.println(x + " bouteilles de " + ((this instanceof VinChardonnay)?"Chardonnay":"Muscat") + " ont été vendues");
    }
    else System.out.println("La récolte de "+((this instanceof VinChardonnay)?"Chardonnay":"Muscat")+" n'a pas été productive aujourd'hui. Pas de vente :(");
  }
}