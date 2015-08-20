package Elements;

import java.util.ArrayList;

/**
 * @author VuHT
 * Description: This is a population of individuals
 */
public class Population {
	public ArrayList<Individual> listIndividual;
	public Instance instance;
	
	// Variables of each permutation run
	public static int TIMER;
	public static ArrayList<Record> listRecord;
	public static int Tmax;
	
	// The global total tardiness of the schedule
	public static int glTotalTardiness;
	
	// Constructor
	public Population(){
		listIndividual = new ArrayList<Individual>();
		instance = new Instance();
	}
	
	// Constructor
	public Population(int populationSize){
		listIndividual = new ArrayList<Individual>();
		for(int i = 0; i < populationSize; i++){
			listIndividual.add(new Individual());
		}
		instance = new Instance();
	}
	
	// Constructor
	public Population(Instance ins){
		listIndividual = new ArrayList<Individual>();
		instance = new Instance(ins);
	}
	
	// Constructor
	public Population(Population pop){
		listIndividual = new ArrayList<Individual>();
		for(int i = 0; i < pop.listIndividual.size(); i++){
			listIndividual.add(new Individual(pop.listIndividual.get(i)));
		}
		
		instance = new Instance(pop.instance);
	}
	
	// Add an individual to population
	public void addIndividual(Individual indiv){
		listIndividual.add(new Individual(indiv));
	}
	
	// Add list of individuals to population
	public void addListIndividual(Individual[] listIndiv){
		for(int i = 0; i < listIndiv.length; i++) {
			listIndividual.add(new Individual(listIndiv[i]));
		}
	}
	
	// Get individual by index
	public Individual getIndividual(int index){
		return listIndividual.get(index);
	}
	
	// Save an individual at index position
	public void saveIndividual(int index, Individual indiv){
		listIndividual.set(index, new Individual(indiv));
	}
	
	// Get better individual which has more smaller makespan
	public Individual getBetterMakespanIndividual(Individual firstIndividual, Individual secondIndividual){
		return firstIndividual.makespan <= secondIndividual.makespan ? firstIndividual : secondIndividual;
	}
	
	// Get better individual which has more smaller total tardiness
	public Individual getBetterTotalTardinessIndividual(Individual firstIndividual, Individual secondIndividual){
		return firstIndividual.totalTardiness <= secondIndividual.totalTardiness ? firstIndividual : secondIndividual;
	}
	
	// Get the worst makespan individual of population
	public Individual getWorstMakespanIndividual(){
		Individual worstIndiv = listIndividual.get(0);
		for(int i = 1; i < listIndividual.size(); i++) {
			if(worstIndiv.makespan < getIndividual(i).makespan) {
				worstIndiv = getIndividual(i);
			}
		}
		
		return worstIndiv;
	}
	
	// Get the worst total tardiness individual of population
	public Individual getWorstTotalTardinessIndividual(){
		Individual worstIndiv = listIndividual.get(0);
		for(int i = 1; i < listIndividual.size(); i++) {
			if(worstIndiv.totalTardiness < getIndividual(i).totalTardiness) {
				worstIndiv = getIndividual(i);
			}
		}
		
		return worstIndiv;
	}
	
	// Get the best makespan individual of population
	public Individual getBestMakespanIndividual(){
		Individual bestIndiv = listIndividual.get(0);
		for(int i = 1; i < listIndividual.size(); i++) {
			if(bestIndiv.makespan > getIndividual(i).makespan) {
				bestIndiv = getIndividual(i);
			}
		}
		
		return bestIndiv;
	}
	
	// Get the best total tardiness individual of population
	public Individual getBestTotalTardinessIndividual(){
		Individual bestIndiv = listIndividual.get(0);
		for(int i = 1; i < listIndividual.size(); i++) {
			if(bestIndiv.totalTardiness > getIndividual(i).totalTardiness) {
				bestIndiv = getIndividual(i);
			}
		}
		
		return bestIndiv;
	}
	
	// Replace an old individual by a new individual
	public void replaceIndividual(Individual oldIndiv, Individual newIndiv){
		int oldIndividualIndex = listIndividual.indexOf(oldIndiv);
		listIndividual.set(oldIndividualIndex, new Individual(newIndiv));
	}
	
	// Consider to replace list of the worst makespan individual of population
	public void replaceListWorstMakespanIndividual(ArrayList<Individual> listIndividual){
		while(!listIndividual.isEmpty()){
			Individual worstIndiv = getWorstMakespanIndividual();
			Individual indiv = listIndividual.remove(0);
			if(indiv.makespan < worstIndiv.makespan){
				replaceIndividual(worstIndiv, indiv);
			}
		}
	}
	
	// Consider to replace list of the worst total tardiness individual of population
	public void replaceListWorstTotalTardinessIndividual(ArrayList<Individual> listIndividual){
		while(!listIndividual.isEmpty()){
			Individual worstIndiv = getWorstMakespanIndividual();
			Individual indiv = listIndividual.remove(0);
			if(indiv.totalTardiness < worstIndiv.totalTardiness){
				replaceIndividual(worstIndiv, indiv);
			}
		}
	}
	
	// Change instance based on an individual
	public void changeInstanceFromIndividual(Individual indiv){
		Individual individual = new Individual(indiv);
		int[] listAperiodicJob = individual.listAperiodicJob;
		ArrayList<Job> tmpListAperiodicJob = new ArrayList<Job>();
		for(int i = 0; i < listAperiodicJob.length; i++){
			for(int j = 0; j < instance.listAperiodicJob.size(); j++){
				if(instance.listAperiodicJob.get(j).jobId == listAperiodicJob[i]){
					tmpListAperiodicJob.add(instance.listAperiodicJob.remove(j));
					break;
				}
			}
		}
		while(!tmpListAperiodicJob.isEmpty()){
			instance.listAperiodicJob.add(tmpListAperiodicJob.remove(0));
		}
	}
	
	// Get the LPT_HEU1 individual
	public Individual getLPTHEU1Individual(){
		Instance ins = new Instance(instance);
		Individual resultIndiv = runLPTHEU1Scheduling(ins);
		
		return resultIndiv;
	}
	
	// Run the LPT_HEU1 heuristic scheduling
	public Individual runLPTHEU1Scheduling(Instance ins) {
		TIMER = 0;
		listRecord = new ArrayList<Record>();
		
		// Add periodic beginning job
		addJob(ins.periodicJob, "Periodic Job");
		
		// Initialize Tmax
		Tmax = 2*(ins.cycle - ins.periodicJob.processingTime);
		
		// LPT scheduling
		while(!ins.listAperiodicJob.isEmpty()) {
			Job firstAperiodicJob = ins.listAperiodicJob.get(0);
			if(firstAperiodicJob.processingTime <= Tmax) {
				addJob(firstAperiodicJob, "Aperiodic Job");
				ins.listAperiodicJob.remove(0);
				
				if(TIMER % ins.cycle == 0) {
					addMoreSmallerJob(ins);
					beginNewCycle(ins);
				}
				else {
					// Check whether periodic job and aperiodic job are not in the same one cycle
					if(TIMER % ins.cycle < ins.periodicJob.processingTime + firstAperiodicJob.processingTime) {
						addMoreSmallerJob(ins);
						addJob(ins.periodicJob, "Periodic Job");
					}
					
					if(TIMER % ins.cycle == 0) {
						addMoreSmallerJob(ins);
						beginNewCycle(ins);
					}
					else {
						// Update Tmax
						Tmax = 2*ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime;
					}
				}
			}
			else { // LPT_HEU1: The largest processing time job that is smaller than Tmax
				if(ins.listAperiodicJob.size() == 1) {
					TIMER += ins.cycle - TIMER % ins.cycle;
					beginNewCycle(ins);
				}
				else {
					// When updating Tmax, we will begin normal LPT algorithm if we get bigger Tmax
					int checkingTmax = Tmax;
					
					// Index of removed job in list of aperiodic jobs
					int index;
					
					while(!ins.listAperiodicJob.isEmpty()) {
						// Compare the result to get the most adequate job index
						index = getLargestProcessingTimeJobIndex(ins);
						
						// No job found
						if(index == 0) { // Begin new cycle to run normal LPT algorithm when no appreciate job found
							TIMER += ins.cycle - TIMER % ins.cycle;
							beginNewCycle(ins);
							break;
						}
						
						Job job = ins.listAperiodicJob.get(index);
						addJob(job, "Aperiodic Job");
						ins.listAperiodicJob.remove(index);
						
						if(TIMER % ins.cycle == 0) {
							beginNewCycle(ins);
							break;
						}
						else {
							// Check whether periodic job and aperiodic job are not in the same one cycle
							if(TIMER % ins.cycle < job.processingTime + ins.periodicJob.processingTime) {
								addJob(ins.periodicJob, "Periodic Job");
								
								if(TIMER % ins.cycle == 0) {
									beginNewCycle(ins);
									break;
								}
								else {
									// Update Tmax
									Tmax = 2*ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime;
									if(checkingTmax <= Tmax) {
										break;
									}
								}
							}
							else {
								// Update Tmax
								Tmax -= job.processingTime;
							}
						}
					}
				}
			}
		}
		// Check to remove the last periodic jobs of listRecord
		while(listRecord.get(listRecord.size()-1).jobType == "Periodic Job") {
			listRecord.remove(listRecord.size()-1);
		}
		
		// Get the scheduling result
		ins.listRecord = listRecord;
		
		// Get the makespan of the schedule
		ins.makespan = listRecord.get(listRecord.size()-1).time + listRecord.get(listRecord.size()-1).processingTime;
		
		Individual resultIndiv = ins.getIndividualFromSchedulingResult();
		
		return resultIndiv;
	}
	
	// Get the largest processing time job that is smaller than Tmax
	public int getLargestProcessingTimeJobIndex(Instance ins){
		int index = 0;
		for(int i = 1; i < ins.listAperiodicJob.size(); i++){
			if(ins.listAperiodicJob.get(i).processingTime <= Tmax) {
				index = i;
				break;
			}
		}
		
		if(index != 0){
			if(ins.listAperiodicJob.size() > index + 1) {
				for(int i = index + 1; i < ins.listAperiodicJob.size(); i++){
					if(ins.listAperiodicJob.get(i).processingTime <= Tmax){
						if(ins.listAperiodicJob.get(i).processingTime > ins.listAperiodicJob.get(index).processingTime){
							index = i;
						}
					}
				}
			}
		}
		
		return index;
	}
	
	// Get the LPT_HEU2 individual
	public Individual getLPTHEU2Individual(){
		Instance ins = new Instance(instance);
		Individual resultIndividual = runLPTHEU2Scheduling(ins);
		
		return resultIndividual;
	}
	
	// Run the LPT_HEU2 heuristic scheduling
	public Individual runLPTHEU2Scheduling(Instance ins) {
		TIMER = 0;
		listRecord = new ArrayList<Record>();
		
		// Add periodic beginning job
		addJob(ins.periodicJob, "Periodic Job");
		
		// Initialize Tmax
		Tmax = 2*(ins.cycle - ins.periodicJob.processingTime);
		
		// LPT scheduling
		while(!ins.listAperiodicJob.isEmpty()) {
			Job firstAperiodicJob = ins.listAperiodicJob.get(0);
			if(firstAperiodicJob.processingTime <= Tmax) {
				addJob(firstAperiodicJob, "Aperiodic Job");
				ins.listAperiodicJob.remove(0);
				
				if(TIMER % ins.cycle == 0) {
					addMoreSmallerJob(ins);
					beginNewCycle(ins);
				}
				else {
					// Check whether periodic job and aperiodic job are not in the same one cycle
					if(TIMER % ins.cycle < ins.periodicJob.processingTime + firstAperiodicJob.processingTime) {
						addMoreSmallerJob(ins);
						addJob(ins.periodicJob, "Periodic Job");
					}
					
					if(TIMER % ins.cycle == 0) {
						addMoreSmallerJob(ins);
						beginNewCycle(ins);
					}
					else {
						// Update Tmax
						Tmax = 2*ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime;
					}
				}
			}
			else { // LPT_HEU2: The smallest job that is smaller than Tmax
				if(ins.listAperiodicJob.size() == 1) {
					TIMER += ins.cycle - TIMER % ins.cycle;
					beginNewCycle(ins);
				}
				else {
					// When updating Tmax, we will begin normal LPT algorithm if we get bigger Tmax
					int checkingTmax = Tmax;
					
					// Index of removed job in list of aperiodic jobs
					int index;
					
					while(!ins.listAperiodicJob.isEmpty()) {
						// Compare the result to get the most adequate job index
						index = getSmallestProcessingTimeJobIndex(ins);
						
						// No job found
						if(index == 0) { // Begin new cycle to run normal LPT algorithm when no appreciate job found
							TIMER += ins.cycle - TIMER % ins.cycle;
							beginNewCycle(ins);
							break;
						}
						
						Job job = ins.listAperiodicJob.get(index);
						addJob(job, "Aperiodic Job");
						ins.listAperiodicJob.remove(index);
						
						if(TIMER % ins.cycle == 0) {
							beginNewCycle(ins);
							break;
						}
						else {
							// Check whether periodic job and aperiodic job are not in the same one cycle
							if(TIMER % ins.cycle < job.processingTime + ins.periodicJob.processingTime) {
								addJob(ins.periodicJob, "Periodic Job");
								
								if(TIMER % ins.cycle == 0) {
									beginNewCycle(ins);
									break;
								}
								else {
									// Update Tmax
									Tmax = 2*ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime;
									if(checkingTmax <= Tmax) {
										break;
									}
								}
							}
							else {
								// Update Tmax
								Tmax -= job.processingTime;
							}
						}
					}
				}
			}
		}
		// Check to remove the last periodic jobs of listRecord
		while(listRecord.get(listRecord.size()-1).jobType == "Periodic Job") {
			listRecord.remove(listRecord.size()-1);
		}
		
		// Get the scheduling result
		ins.listRecord = listRecord;
		
		// Get the makespan of the schedule
		ins.makespan = listRecord.get(listRecord.size()-1).time + listRecord.get(listRecord.size()-1).processingTime;
		
		Individual resultIndiv = ins.getIndividualFromSchedulingResult();
		
		return resultIndiv;
	}
	
	// Get the smallest processing time job that is smaller than Tmax
	public int getSmallestProcessingTimeJobIndex(Instance ins){
		int index = 0;
		for(int i = 1; i < ins.listAperiodicJob.size(); i++){
			if(ins.listAperiodicJob.get(i).processingTime <= Tmax) {
				index = i;
				break;
			}
		}
		
		if(index != 0){
			if(ins.listAperiodicJob.size() > index + 1) {
				for(int i = index + 1; i < ins.listAperiodicJob.size(); i++){
					if(ins.listAperiodicJob.get(i).processingTime <= Tmax){
						if(ins.listAperiodicJob.get(i).processingTime < ins.listAperiodicJob.get(index).processingTime){
							index = i;
						}
					}
				}
			}
		}
		
		return index;
	}
	
	// Get the LPT_HEU3 individual
	public Individual getLPTHEU3Individual(){
		Instance ins = new Instance(instance);
		Individual resultIndiv = runLPTHEU3Scheduling(ins);
		
		return resultIndiv;
	}
	
	// Run the LPT_HEU3 heuristic scheduling
	public Individual runLPTHEU3Scheduling(Instance ins) {
		TIMER = 0;
		listRecord = new ArrayList<Record>();
		
		// Add periodic beginning job
		addJob(ins.periodicJob, "Periodic Job");
		
		// Initialize Tmax
		Tmax = 2*(ins.cycle - ins.periodicJob.processingTime);
		
		// LPT scheduling
		while(!ins.listAperiodicJob.isEmpty()) {
			Job firstAperiodicJob = ins.listAperiodicJob.get(0);
			if(firstAperiodicJob.processingTime <= Tmax) {
				addJob(firstAperiodicJob, "Aperiodic Job");
				ins.listAperiodicJob.remove(0);
				
				if(TIMER % ins.cycle == 0) {
					addMoreSmallerJob(ins);
					beginNewCycle(ins);
				}
				else {
					// Check whether periodic job and aperiodic job are not in the same one cycle
					if(TIMER % ins.cycle < ins.periodicJob.processingTime + firstAperiodicJob.processingTime) {
						addMoreSmallerJob(ins);
						addJob(ins.periodicJob, "Periodic Job");
					}
					
					if(TIMER % ins.cycle == 0) {
						addMoreSmallerJob(ins);
						beginNewCycle(ins);
					}
					else {
						// Update Tmax
						Tmax = 2*ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime;
					}
				}
			}
			else { // LPT_HEU3: The most adequate job that is smaller than Tmax
				if(ins.listAperiodicJob.size() == 1) {
					TIMER += ins.cycle - TIMER % ins.cycle;
					beginNewCycle(ins);
				}
				else {
					// When updating Tmax, we will begin normal LPT algorithm if we get bigger Tmax
					int checkingTmax = Tmax;
					
					// Index of removed job in list of aperiodic jobs
					int index;
					
					while(!ins.listAperiodicJob.isEmpty()) {
						// Compare the result to get the most adequate job index
						// Get remain time in one cycle and in two cycles
						int TmaxOneCycle = ins.cycle - TIMER % ins.cycle;
						int TmaxTwoCycle = 2*ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime;
						
						Job oneCycleSearchJob = getLargestProcessingTimeJobByCycle(ins.listAperiodicJob, TmaxOneCycle);
						Job twoCycleSearchJob = getLargestProcessingTimeJobByCycle(ins.listAperiodicJob, TmaxTwoCycle);
						
						index = getMostAdequateProcessingTimeJobIndex(ins, oneCycleSearchJob, twoCycleSearchJob, TmaxOneCycle, TmaxTwoCycle);
						
						// No job found
						if(index == 0) { // Begin new cycle to run normal LPT algorithm when no appreciate job found
							TIMER += ins.cycle - TIMER % ins.cycle;
							beginNewCycle(ins);
							break;
						}
						
						Job job = ins.listAperiodicJob.get(index);
						addJob(job, "Aperiodic Job");
						ins.listAperiodicJob.remove(index);
						
						if(TIMER % ins.cycle == 0) {
							beginNewCycle(ins);
							break;
						}
						else {
							// Check whether periodic job and aperiodic job are not in the same one cycle
							if(TIMER % ins.cycle < job.processingTime + ins.periodicJob.processingTime) {
								addJob(ins.periodicJob, "Periodic Job");
								
								if(TIMER % ins.cycle == 0) {
									beginNewCycle(ins);
									break;
								}
								else {
									// Update Tmax
									Tmax = 2*ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime;
									if(checkingTmax <= Tmax) {
										break;
									}
								}
							}
							else {
								// Update Tmax
								Tmax -= job.processingTime;
							}
						}
					}
				}
			}
		}
		// Check to remove the last periodic jobs of listRecord
		while(listRecord.get(listRecord.size()-1).jobType == "Periodic Job") {
			listRecord.remove(listRecord.size()-1);
		}
		
		// Get the scheduling result
		ins.listRecord = listRecord;
		
		// Get the makespan of the schedule
		ins.makespan = listRecord.get(listRecord.size()-1).time + listRecord.get(listRecord.size()-1).processingTime;
		
		Individual resultIndiv = ins.getIndividualFromSchedulingResult();
		
		return resultIndiv;
	}
	
	// Get the largest processing time job that is smaller than Tmax based on number of cycles
	public Job getLargestProcessingTimeJobByCycle(ArrayList<Job> listAperiodicJob, int tmax){
		Job resultJob = new Job();
		
		int index = 0;
		for(int i = 1; i < listAperiodicJob.size(); i++){
			if(listAperiodicJob.get(i).processingTime <= tmax) {
				index = i;
				break;
			}
		}
		
		if(index != 0){
			if(listAperiodicJob.size() > index + 1) {
				for(int i = index + 1; i < listAperiodicJob.size(); i++){
					if(listAperiodicJob.get(i).processingTime <= tmax){
						if(listAperiodicJob.get(i).processingTime > listAperiodicJob.get(index).processingTime){
							index = i;
						}
					}
				}
			}
		}
		resultJob = listAperiodicJob.get(index);
		
		return resultJob;
	}
	
	// Get the most adequate processing time job index
	public static int getMostAdequateProcessingTimeJobIndex(Instance ins, Job oneCycleSearchJob, Job twoCycleSearchJob, int TmaxOneCycle, int TmaxTwoCycle) {
		int index = 0;
		
		if(oneCycleSearchJob.processingTime == 0) {
			if(twoCycleSearchJob.processingTime == 0) { // No job found
				index = 0;
			}
			else { // No job found in one cycle, job found in two cycles
				index = ins.listAperiodicJob.indexOf(twoCycleSearchJob);
			}
		}
		else {
			if(twoCycleSearchJob.processingTime == 0) { // No job found in two cycles, job found in one cycle
				index = ins.listAperiodicJob.indexOf(oneCycleSearchJob);
			}
			else { // Jobs found in one cycle and in two cycles
				if(TmaxOneCycle - oneCycleSearchJob.processingTime < TmaxTwoCycle - twoCycleSearchJob.processingTime) {
					// Add job in one cycle, more adequate
					index = ins.listAperiodicJob.indexOf(oneCycleSearchJob);
				}
				else {
					// Add job in two cycles, more adequate
					index = ins.listAperiodicJob.indexOf(twoCycleSearchJob);
				}
			}
		}
		
		return index;
	}
	
	// Add job to the schedule record
	public void addJob(Job job, String jobType) {
		listRecord.add(new Record(TIMER, job, jobType));
		TIMER += job.processingTime;
	}
	
	// Add more jobs after we schedule a job in one cycle
	public void addMoreSmallerJob(Instance ins) {
		while(!ins.listAperiodicJob.isEmpty()) {
			Job job = ins.listAperiodicJob.get(0);
			if (job.processingTime <= ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime) {
				addJob(job, "Aperiodic Job");
				updateTardiness(ins.listAperiodicJob.remove(0));
			}
			else {
				break;
			}
		}
	}
	
	// Begin new cycle
	public void beginNewCycle(Instance ins) {
		addJob(ins.periodicJob, "Periodic Job");
		Tmax = 2*(ins.cycle - ins.periodicJob.processingTime);
	}
	
	// Get list of records from individual
	public ArrayList<Record> getListRecordFromIndividual(){
		Instance ins = new Instance(instance);
		ArrayList<Record> listRecord = runNormalScheduling(ins);
		
		return listRecord;
	}
	
	// Run the normal scheduling based on an order of aperiodic jobs
	public ArrayList<Record> runNormalScheduling(Instance ins) {
		TIMER = 0;
		listRecord = new ArrayList<Record>();
		glTotalTardiness = 0;
		
		// Add periodic beginning job
		addJob(ins.periodicJob, "Periodic Job");
		
		// Initialize Tmax
		Tmax = 2*(ins.cycle - ins.periodicJob.processingTime);
		
		while(!ins.listAperiodicJob.isEmpty()) {
			Job firstAperiodicJob = ins.listAperiodicJob.get(0);
			if(firstAperiodicJob.processingTime <= Tmax) {
				addJob(firstAperiodicJob, "Aperiodic Job");
				updateTardiness(ins.listAperiodicJob.remove(0));
				
				if(TIMER % ins.cycle == 0) {
					addMoreSmallerJob(ins);
					beginNewCycle(ins);
				}
				else {
					// Check whether periodic job and aperiodic job are not in the same one cycle
					if(TIMER % ins.cycle < ins.periodicJob.processingTime + firstAperiodicJob.processingTime) {
						addMoreSmallerJob(ins);
						addJob(ins.periodicJob, "Periodic Job");
					}
					
					if(TIMER % ins.cycle == 0) {
						addMoreSmallerJob(ins);
						beginNewCycle(ins);
					}
					else {
						// Update Tmax
						Tmax = 2*ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime;
					}
				}
			}
			else {
				TIMER += ins.cycle - TIMER % ins.cycle;
				beginNewCycle(ins);
			}
		}
			
		// Check to remove the last periodic jobs of listRecord
		while(listRecord.get(listRecord.size()-1).jobType == "Periodic Job") {
			listRecord.remove(listRecord.size()-1);
		}
		
		// Get the scheduling result
		ins.listRecord = listRecord;
		// Get the makespan of the schedule
		ins.makespan = listRecord.get(listRecord.size()-1).time + listRecord.get(listRecord.size()-1).processingTime;
		// Get total tardiness of the schedule
		ins.totalTardiness = glTotalTardiness;
		
		ArrayList<Record> listResult = new ArrayList<Record>();
		for(int i = 0; i < ins.listRecord.size(); i++){
			listResult.add(new Record(ins.listRecord.get(i)));
		}
		
		return listResult;
	}
	
	// Get the EDF_HEU individual
	public Individual getEDFHEUIndividual(){
		Instance ins = new Instance(instance);
		Individual resultIndiv = runEDFHEUScheduling(ins);
		
		return resultIndiv;
	}
	
	// Run the EDF_HEU heuristic scheduling
	public Individual runEDFHEUScheduling(Instance ins) {
		TIMER = 0;
		listRecord = new ArrayList<Record>();
		glTotalTardiness = 0;
		
		// Add periodic beginning job
		addJob(ins.periodicJob, "Periodic Job");
		
		// Initialize Tmax
		Tmax = 2*(ins.cycle - ins.periodicJob.processingTime);
		
		while(!ins.listAperiodicJob.isEmpty()) {
			Job firstAperiodicJob = ins.listAperiodicJob.get(0);
			if(firstAperiodicJob.processingTime <= Tmax) {
				addJob(firstAperiodicJob, "Aperiodic Job");
				updateTardiness(ins.listAperiodicJob.remove(0));
				
				if(TIMER % ins.cycle == 0) {
					addMoreSmallerJob(ins);
					beginNewCycle(ins);
				}
				else {
					// Check whether periodic job and aperiodic job are not in the same one cycle
					if(TIMER % ins.cycle < ins.periodicJob.processingTime + firstAperiodicJob.processingTime) {
						addMoreSmallerJob(ins);
						addJob(ins.periodicJob, "Periodic Job");
					}
					
					if(TIMER % ins.cycle == 0) {
						addMoreSmallerJob(ins);
						beginNewCycle(ins);
					}
					else {
						// Update Tmax
						Tmax = 2*ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime;
					}
				}
			}
			else {
				if(ins.listAperiodicJob.size() == 1) {
					TIMER += ins.cycle - TIMER % ins.cycle;
					beginNewCycle(ins);
				}
				else {
					// When updating Tmax, we will begin normal LPT algorithm if we get bigger Tmax
					int checkingTmax = Tmax;
					
					// Index of removed job in list of aperiodic jobs
					int index;
					
					while(!ins.listAperiodicJob.isEmpty()) {
						// Compare the result to get the most adequate job index
						index = getSmallestDeadlineJobIndex(ins);
						
						// No job found
						if(index == 0) { // Begin new cycle to run normal LPT algorithm when no appreciate job found
							TIMER += ins.cycle - TIMER % ins.cycle;
							beginNewCycle(ins);
							break;
						}
						
						Job job = ins.listAperiodicJob.get(index);
						addJob(job, "Aperiodic Job");
						updateTardiness(ins.listAperiodicJob.remove(index));
						
						if(TIMER % ins.cycle == 0) {
							beginNewCycle(ins);
							break;
						}
						else {
							// Check whether periodic job and aperiodic job are not in the same one cycle
							if(TIMER % ins.cycle < job.processingTime + ins.periodicJob.processingTime) {
								addJob(ins.periodicJob, "Periodic Job");
								
								if(TIMER % ins.cycle == 0) {
									beginNewCycle(ins);
									break;
								}
								else {
									// Update Tmax
									Tmax = 2*ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime;
									if(checkingTmax <= Tmax) {
										break;
									}
								}
							}
							else {
								// Update Tmax
								Tmax -= job.processingTime;
							}
						}
					}
				}
			}
		}
		
		// Check to remove the last periodic jobs of listRecord
		while(listRecord.get(listRecord.size()-1).jobType == "Periodic Job") {
			listRecord.remove(listRecord.size()-1);
		}
		
		// Get the scheduling result
		ins.listRecord = listRecord;
		
		// Get the total tardiness of the schedule
		ins.totalTardiness = glTotalTardiness;
		
		Individual resultIndiv = ins.getIndividualFromSchedulingResult();
		
		return resultIndiv;
	}
	
	// Update maximum tardiness of the schedule
	public static void updateTardiness(Job removedJob){
		int tardiness = TIMER - removedJob.deadline;
		tardiness = tardiness > 0 ? tardiness : 0;
		
		glTotalTardiness += tardiness;
	}
	
	// Get the smallest deadline job that is smaller than Tmax
	public int getSmallestDeadlineJobIndex(Instance ins){
		int index = 0;
		for(int i = 1; i < ins.listAperiodicJob.size(); i++){
			if(ins.listAperiodicJob.get(i).processingTime <= Tmax) {
				index = i;
				break;
			}
		}
		
		if(index != 0){
			if(ins.listAperiodicJob.size() > index + 1) {
				for(int i = index + 1; i < ins.listAperiodicJob.size(); i++){
					if(ins.listAperiodicJob.get(i).processingTime <= Tmax){
						if(ins.listAperiodicJob.get(i).deadline < ins.listAperiodicJob.get(index).deadline){
							index = i;
						}
					}
				}
			}
		}
		
		return index;
	}
	
	public void printPopulationAllMakespan(){
		for(int i = 0; i < listIndividual.size(); i++){
			System.out.print(listIndividual.get(i).makespan + " ");
		}
		System.out.println();
	}
	
	public void printPopulationAllTotalTardiness(){
		for(int i = 0; i < listIndividual.size(); i++){
			System.out.print(listIndividual.get(i).totalTardiness + " ");
		}
		System.out.println();
	}
	
	public void printPopulationListIndividual(){
		for(int i = 0; i < listIndividual.size(); i++){
			listIndividual.get(i).printListAperiodicJob();
		}
	}
}