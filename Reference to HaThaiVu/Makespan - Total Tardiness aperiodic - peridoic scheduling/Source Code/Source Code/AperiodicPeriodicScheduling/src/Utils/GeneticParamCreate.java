package Utils;

/**
 * @author VuHT
 * Description: Create input parameters for genetic algorithm
 */
public class GeneticParamCreate {
	public int[] populationSizes;
	public int[] runtimes;
	public double[] mutateRates;
	public double[] crossoverRates;
	
	// Constructor
	public GeneticParamCreate(int[] populationSizes, double[] mutateRates, double[] crossoverRates,int[] runtimes){
		this.populationSizes = new int[populationSizes.length];
		for(int i = 0; i < populationSizes.length; i++){
			this.populationSizes[i] = populationSizes[i];
		}
		
		this.mutateRates = new double[mutateRates.length];
		for(int i = 0; i < mutateRates.length; i++){
			this.mutateRates[i] = mutateRates[i];
		}
		
		this.crossoverRates = new double[crossoverRates.length];
		for(int i = 0; i < crossoverRates.length; i++){
			this.crossoverRates[i] = crossoverRates[i];
		}
		
		this.runtimes = new int[runtimes.length];
		for(int i = 0; i < runtimes.length; i++){
			this.runtimes[i] = runtimes[i];
		}
	}
}