package Elements;

/**
 * @author VuHT
 * Description: This is the result of one algorithm
 */
public class Run {
	public int makespan;
	public String algorithmName;
	public int totalTardiness;
	
	// Constructor
	public Run(int makespan, String algorithmName) {
		this.makespan = makespan;
		this.algorithmName = algorithmName;
		this.totalTardiness = 0;
	}
	
	// Constructor
	public Run(String algorithmName, int totalTardiness){
		this.totalTardiness = totalTardiness;
		this.algorithmName = algorithmName;
		this.makespan = 0;
	}
	
	// Constructor
	public Run(Run run){
		this.makespan = run.makespan;
		this.totalTardiness = run.totalTardiness;
		this.algorithmName = run.algorithmName;
	}
}