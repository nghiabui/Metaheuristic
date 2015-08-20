package Elements;

/**
 * @author VuHT
 * Description: Record scheduling process (at time t, job j will be executed)
 */
public class Record {
	public String jobType;
	public int time;
	public int jobId;
	public int processingTime;
	
	// Constructor
	public Record(int time, Job resultJob, String jobType) {
		this.time = time;
		this.jobId = resultJob.jobId;
		this.processingTime = resultJob.processingTime;
		this.jobType = jobType;
	}
	
	// Constructor
	public Record(Record rec){
		this.time = rec.time;
		this.jobId = rec.jobId;
		this.processingTime = rec.processingTime;
		this.jobType = rec.jobType;
	}
}