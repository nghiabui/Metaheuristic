package SchedulingAlgorithms;

import java.util.ArrayList;
import java.util.Iterator;

import Elements.Instance;
import Elements.Job;
import Elements.Record;

/**
 * @author VuHT
 * Description: Normal LPT scheduling algorithm
 */
public class LPTNormal {
	// Start time
	public static int TIMER;
	// Scheduling result
	public static ArrayList<Record> listRecord;
	// Maximum available interval
	public static int Tmax;
	
	// Constructor
	public LPTNormal() {
		
	}
	
	// Normal LPT Scheduling
	public static void schedule(Instance ins) {
		TIMER = 0;
		listRecord = new ArrayList<Record>();
		
		// Arrange jobs in LPT order
		reorderProcessingTime(ins);
		
		// Add periodic beginning job
		addJob(ins.periodicJob, "Periodic Job");
		
		// Initialize Tmax
		Tmax = 2*(ins.cycle - ins.periodicJob.processingTime);
		// Normal LPT scheduling
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
				else if(tmp1.processingTime == tmp2.processingTime) {
					if(tmp1.deadline < tmp2.deadline){
						ins.listAperiodicJob.set(j-1,tmp1);
						ins.listAperiodicJob.set(j,tmp2);
					}
				}
				else {
					
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
}