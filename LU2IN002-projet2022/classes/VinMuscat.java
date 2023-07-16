//SOUAIBY Christina
//SAMAHA Elio

public class VinMuscat extends Vin{
  // Attributs
  private static final VinMuscat INSTANCE= new VinMuscat();
  // Constructeurs
  private VinMuscat(){
    super("Vin Muscat");
  }
  // Accesseurs
  public static final VinMuscat getInstance(){
    return INSTANCE;
  }
}