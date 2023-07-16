//SOUAIBY Christina
//SAMAHA Elio

public class TestSimulation{
  public static void main(String[] args)throws InterruptedException{
    int m = (int)(Math.random()*50);
    int n = (int)(Math.random()*10);
    int i;
    Simulation sim;
    try{
      sim = new Simulation(m , n);
      sim.commencer();
      for (i = 1 ; i <= Simulation.ITERATIONS ; i++)
        sim.simuler(i);
    }
    catch (ListeVideException e){
      System.out.println(e.getMessage());
    }
    
  }
}