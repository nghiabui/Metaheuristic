package Elements;

import java.util.ArrayList;

/**
 * @author VuHT
 * Description: This is an individual of a population
 */
public class Individual {
	public int[] listAperiodicJob;
	public int makespan;
	public int totalTardiness;
	
	// Constructor
	public Individual(){
		makespan = 0;
		totalTardiness = 0;
	}
	
	// Constructor
	public Individual(int[] listAperiodicJob){
		this.listAperiodicJob = new int[listAperiodicJob.length];
		for(int i = 0; i < listAperiodicJob.length; i++){
			this.listAperiodicJob[i] = listAperiodicJob[i];
		}
		makespan = 0;
		totalTardiness = 0;
	}
	
	// Constructor
	public Individual(ArrayList<Integer> listAperiodicJob){
		this.listAperiodicJob = new int[listAperiodicJob.size()];
		for(int i = 0; i < listAperiodicJob.size(); i++){
			this.listAperiodicJob[i] = (int)listAperiodicJob.get(i);
		}
		makespan = 0;
		totalTardiness = 0;
	}
	
	// Constructor
	public Individual(Individual indiv){
		this.listAperiodicJob = new int[indiv.listAperiodicJob.length];
		for(int i = 0; i < indiv.listAperiodicJob.length; i++){
			this.listAperiodicJob[i] = indiv.listAperiodicJob[i];
		}
		makespan = indiv.makespan;
		totalTardiness = indiv.totalTardiness;
	}
	
	public void printListAperiodicJob(){
		System.out.print("Individual: ");
		for(int i = 0; i < listAperiodicJob.length; i++){
			System.out.print(listAperiodicJob[i] + " ");
		}
		System.out.println();
	}
}