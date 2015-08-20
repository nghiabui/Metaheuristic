package SchedulingAlgorithms;

import java.util.ArrayList;
import java.util.Iterator;

import Elements.Instance;
import Elements.Job;
import Elements.Record;

/**
 * @author VuHT
 * Description: LPT_HEU3 scheduling algorithm (get the adequate job)
 */
public class LPTAdequateJobHeuristic {
	// Start time
	public static int TIMER;
	// Scheduling result
	public static ArrayList<Record> listRecord;
	// Maximum available interval
	public static int Tmax;
	
	// Constructor
	public LPTAdequateJobHeuristic() {
		
	}
	
	// Adequate job LPT heuristic scheduling
	public static void schedule(Instance ins) {
		TIMER = 0;
		listRecord = new ArrayList<Record>();
		
		// Arrange jobs in LPT order
		reorderProcessingTime(ins);
		
		// Add periodic beginning job
		addJob(ins.periodicJob, "Periodic Job");
		
		// Initialize Tmax
		Tmax = 2*(ins.cycle - ins.periodicJob.processingTime);
		
		// Adequate job LPT heuristic scheduling
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
						// Get remain time in one cycle and in two cycles
						int TmaxOneCycle = ins.cycle - TIMER % ins.cycle;
						int TmaxTwoCycle = 2*ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime;
						
						// Binary search for the most adequate jobs in one cycle and in two cycles
						Job oneCycleBinarySearchJob = binarySearchAdequateJob(ins.listAperiodicJob, TmaxOneCycle);
						Job twoCycleBinarySearchJob = binarySearchAdequateJob(ins.listAperiodicJob, TmaxTwoCycle);
						
						// Compare the result to get the most adequate job index
						index = getMostAdequateJobIndex(ins, oneCycleBinarySearchJob, twoCycleBinarySearchJob, TmaxOneCycle, TmaxTwoCycle);
						
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
	}
	
	// Add job to the schedule record
	public static void addJob(Job job, String jobType) {
		listRecord.add(new Record(TIMER, job, jobType));
		TIMER += job.processingTime;
	}
	
	// Add more jobs after we schedule a job in one cycle
	public static void addMoreSmallerJob(Instance ins) {
		while(!ins.listAperiodicJob.isEmpty()) {
			Job job = ins.listAperiodicJob.get(0);
			if (job.processingTime <= ins.cycle - TIMER % ins.cycle - ins.periodicJob.processingTime) {
				addJob(job, "Aperiodic Job");
				ins.listAperiodicJob.remove(0);
			}
			else {
				break;
			}
		}
	}
	
	// Begin new cycle
	public static void beginNewCycle(Instance ins) {
		addJob(ins.periodicJob, "Periodic Job");
		Tmax = 2*(ins.cycle - ins.periodicJob.processingTime);
	}
	
	// Get the most adequate job index
	public static int getMostAdequateJobIndex(Instance ins, Job oneCycleBinarySearchJob, Job twoCycleBinarySearchJob, int TmaxOneCycle, int TmaxTwoCycle) {
		int index = 0;
		
		if(oneCycleBinarySearchJob.processingTime == 0) {
			if(twoCycleBinarySearchJob.processingTime == 0) { // No job found
				index = 0;
			}
			else { // No job found in one cycle, job found in two cycles
				index = ins.listAperiodicJob.indexOf(twoCycleBinarySearchJob);
			}
		}
		else {
			if(twoCycleBinarySearchJob.processingTime == 0) { // No job found in two cycles, job found in one cycle
				index = ins.listAperiodicJob.indexOf(oneCycleBinarySearchJob);
			}
			else { // Jobs found in one cycle and in two cycles
				if(TmaxOneCycle - oneCycleBinarySearchJob.processingTime < TmaxTwoCycle - twoCycleBinarySearchJob.processingTime) {
					// Add job in one cycle, more adequate
					index = ins.listAperiodicJob.indexOf(oneCycleBinarySearchJob);
				}
				else {
					// Add job in two cycles, more adequate
					index = ins.listAperiodicJob.indexOf(twoCycleBinarySearchJob);
				}
			}
		}
		
		return index;
	}
	
	//Order list of aperiodic jobs follows non-increase order
	public static void reorderProcessingTime(Instance ins) {
		int numberAperiodicJob = ins.listAperiodicJob.size();
		for(int i = 0; i < numberAperiodicJob-1; i++) {
			for(int j = numberAperiodicJob-1; j > i; j--) {
				Job tmp1 = ins.listAperiodicJob.get(j);
				Job tmp2 = ins.listAperiodicJob.get(j-1);
				if(tmp1.processingTime > tmp2.processingTime) {
					ins.listAperiodicJob.set(j-1,tmp1);
					ins.listAperiodicJob.set(j,tmp2);
				}
			}
		}
	}
	
	public static void printListRecord(ArrayList<Record> listRecord) {
		Iterator<Record> itr = listRecord.iterator();
		while(itr.hasNext()) {
			Record record = itr.next();
			
			System.out.println("Time: "+record.time+" Job: "+record.jobId+" Processing time: "+record.processingTime+" "+record.jobType);
		}
	}
	
	// Use binary search to get the adequate job
	public static Job binarySearchAdequateJob(ArrayList<Job> listAperiodicJob, int comparingTmax) {
		Job resultJob = new Job();
		int low = 1;
		int high = listAperiodicJob.size() - 1;
		int middle = (low+high)/2;;
		
		// Flag to check whether the is a job with the processing time equal the remain time
		boolean hasEqualJob = false;
		
		while(low <= high) {
			middle = (low+high)/2;
			if (comparingTmax < listAperiodicJob.get(middle).processingTime){
		        low = middle +1;
		    } else if (comparingTmax > listAperiodicJob.get(middle).processingTime){
		        high = middle -1;
		    } else { // The element is found
		    	resultJob = listAperiodicJob.get(middle);
		    	hasEqualJob = true;
		    	
				break;
		    }
		}
		
		// If no exact job found, we get the near exact job
		if(!hasEqualJob) {
			if(listAperiodicJob.get(middle).processingTime < comparingTmax) {
				resultJob = listAperiodicJob.get(middle);
			}
			else {
				if(listAperiodicJob.size() > middle + 1) {
					resultJob = listAperiodicJob.get(middle+1);
				}
			}
		}
		
		return resultJob;
	}
}