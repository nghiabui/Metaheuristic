package Elements;

import java.util.ArrayList;

/**
 * @author VuHT
 * Description: This is the result of one algorithm
 */
public class Result {
	public ArrayList<Run> listRun;
	
	// Constructor
	public Result(ArrayList<Run> listRun) {
		this.listRun = new ArrayList<Run>();
		for(int i = 0; i < listRun.size(); i++){
			this.listRun.add(new Run(listRun.get(i)));
		}
	}
	
	// Constructor
	public Result(Result res){
		this.listRun = new ArrayList<Run>();
		for(int i = 0; i < res.listRun.size(); i++){
			this.listRun.add(new Run(res.listRun.get(i)));
		}
	}
	
	public void printlnListRun(){
		for(int i = 0; i < listRun.size(); i++) {
			System.out.println("Algorithm: " + listRun.get(i).algorithmName);
			System.out.println("Makespan: " + listRun.get(i).makespan);
			System.out.println("Total Tardiness: " + listRun.get(i).totalTardiness);
		}
	}
}