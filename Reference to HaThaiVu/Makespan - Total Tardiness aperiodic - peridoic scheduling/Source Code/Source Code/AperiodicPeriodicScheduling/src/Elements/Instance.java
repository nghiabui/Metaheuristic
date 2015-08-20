package Elements;

import java.util.ArrayList;


/**
 * @author VuHT
 * Description: This is one instance of the problem that we need to schedule. It includes set of periodic jobs and
 * 				set of aperiodic jobs.
 */
public class Instance {
	public Job periodicJob;
	public ArrayList<Job> listAperiodicJob;
	
	public int cycle;
	public int makespan;
	public int numberAperiodicJob;
	
	// Scheduling result
	public ArrayList<Record> listRecord;
	
	public int totalTardiness;
	
	//Constructor
	public Instance() {
		periodicJob = new Job();
		listAperiodicJob = new ArrayList<Job>();
		
		cycle = 0;
		makespan = 0;
		numberAperiodicJob = 0;
		
		totalTardiness = 0;
	}
	
	// Constructor
	public Instance(Instance ins){
		periodicJob = new Job(ins.periodicJob);
		
		listAperiodicJob = new ArrayList<Job>();
		for(int i = 0; i < ins.listAperiodicJob.size(); i++){
			listAperiodicJob.add(new Job(ins.listAperiodicJob.get(i)));
		}
		
		cycle = ins.cycle;
		makespan = ins.makespan;
		numberAperiodicJob = ins.numberAperiodicJob;
		
		totalTardiness = ins.totalTardiness;
	}
	
	//Constructor
	public Instance(Job periodicJob, ArrayList<Job> listAperiodicJob, int cycle) {
		this.periodicJob = periodicJob;
		this.listAperiodicJob = listAperiodicJob;
		
		this.cycle = cycle;
		this.makespan = 0;
		this.totalTardiness = 0;
		this.numberAperiodicJob = listAperiodicJob.size();
		
		this.listRecord =  new ArrayList<Record>();
	}
	
	// Get list of aperiodic jobId
	public ArrayList<Integer> getListAperiodicJobId(){
		ArrayList<Integer> listAperiodicJobId = new ArrayList<Integer>();
		for(int i = 0; i < listAperiodicJob.size(); i++){
			listAperiodicJobId.add(listAperiodicJob.get(i).jobId);
		}
		
		return listAperiodicJobId;
	}
	
	// Get individual of population from scheduling result
	public Individual getIndividualFromSchedulingResult(){
		int[] listAperiodicJobId = new int[numberAperiodicJob];
		int aperiodicJobIndex = -1;
		
		for(int i = 0; i < listRecord.size(); i++){
			if(listRecord.get(i).jobType == "Aperiodic Job") {
				aperiodicJobIndex += 1;
				listAperiodicJobId[aperiodicJobIndex] = listRecord.get(i).jobId;
			}
		}
		
		Individual indiv = new Individual(listAperiodicJobId);
		indiv.makespan = makespan;
		indiv.totalTardiness = totalTardiness;
		
		return indiv;
	}
	
	//Print Instance
	public void printInstance(){
		System.out.println("Period Job info:");
		System.out.println("Id: " + periodicJob.jobId);
		System.out.println("Processing Time: " + periodicJob.processingTime);
    	System.out.println("Cycle:" + cycle);
    	System.out.println("Aperiod Jobs info:");
    	for(Job job : listAperiodicJob){
    		System.out.println("Id: " + job.jobId);
			System.out.println("Processing Time: " + job.processingTime);
			System.out.println("Deadline: " + job.deadline);
    	}
    	System.out.println();
	}
	
	public void printListAperiodicJob() {
		System.out.print("P: ");
		for(int i = 0; i < listAperiodicJob.size(); i++) {
			System.out.print(listAperiodicJob.get(i).processingTime + " ");
		}
		System.out.println();
	}
	
	public void printListAperiodicJobId() {
		System.out.print("P: ");
		for(int i = 0; i < listAperiodicJob.size(); i++) {
			System.out.print(listAperiodicJob.get(i).jobId + " ");
		}
		System.out.println();
	}
	
	public void printListRecord(){
		System.out.print("Record: ");
		for(int i = 0; i < listRecord.size(); i++){
			System.out.print(listRecord.get(i).jobId + " ");
		}
		System.out.println();
	}
}