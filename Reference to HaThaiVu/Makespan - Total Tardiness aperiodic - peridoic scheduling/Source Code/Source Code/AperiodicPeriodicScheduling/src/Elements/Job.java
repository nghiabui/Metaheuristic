package Elements;

/**
 * @author VuHT
 * Description: Job.
 */
public class Job {
	public int jobId;
	
	public int processingTime;
	public int deadline;
	public int tardiness;
	
	//Constructor
	public Job(){
		jobId = 0;
		
		processingTime = 0;
		deadline = 0;
		tardiness = 0;
	}
	
	//Constructor
	public Job(int processingTime, int deadline) {
		this.jobId = 0;
		
		this.processingTime = processingTime;
		this.deadline = deadline;
		this.tardiness = 0;
	}
	
	// Constructor
	public Job(Job job){
		jobId = job.jobId;
		
		processingTime = job.processingTime;
		deadline = job.deadline;
		tardiness = job.tardiness;
	}
	
	public void printJob(){
		System.out.print("JobId: " + jobId);
		System.out.print("\tJobProcessingTime: " + processingTime);
		System.out.print("\tJobDeadline: " + deadline);
		System.out.print("\tJobTardiness: " + tardiness);
		System.out.println();
	}
	
	//Constructor
	public Job(int processingTime, int deadline, int computationTime, int tardiness) {
		this.jobId = 0;
		
		this.processingTime = processingTime;
		this.deadline = deadline;
		this.tardiness = tardiness;
	}
	
	// Check whether job is empty
	public boolean isEmptyJob() {
		return processingTime == 0 ? true : false;
	}
}