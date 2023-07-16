//SOUAIBY Christina
//SAMAHA Elio

public class VinChardonnay extends Vin{
  // Attributs
  private static final VinChardonnay INSTANCE= new VinChardonnay();
  // Constructeurs
  private VinChardonnay(){
    super("Vin Chardonnay");
  }
  // Accesseurs
  public static final VinChardonnay getInstance(){
    return INSTANCE;
  }
}