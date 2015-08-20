package Utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

import Elements.Instance;
import Elements.Job;

/**
 * @author VuHT
 * Description: Read input file and save info to an instance
 */
public class InputParse {
	//Read input file and create the instance
	public static Instance pasreInputFile(File inputFile) {
		Instance ins = new Instance();
		
		try {
			FileInputStream inputStream = new FileInputStream(inputFile);
            BufferedReader br = new BufferedReader(new InputStreamReader(inputStream));
            String str;
            
            while((str = br.readLine()) != null) {
            	str = str.trim();
            	
            	if(str.compareTo("// periodic") == 0) { //Read period tasks info
            		ins.periodicJob = readPeriodicJobInfo(br);
            	}
            	else if(str.compareTo("// aperiodic") == 0) { //Read aperiod works info
            		ins.listAperiodicJob = readListJobInfo(br);
            		ins.numberAperiodicJob = ins.listAperiodicJob.size();
            	}
            	else if(str.compareTo("// Cycle") == 0) { //Read cycle info
            		ins.cycle = readCycleInfo(br);
            	}
            	else {
            		//do nothing, just check end of an instance
            	}
            }
            br.close();
		}
		catch(IOException e) {
            e.printStackTrace();
        }
		
		return ins;
	}
	
	//Read one line with no unnecessary blank
	public static String readOneLine(BufferedReader br) {
		String str = "";
		try {
			str = br.readLine();
			str = str.trim();
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		
		return str;
	}
	
	//Read list jobs info
	public static Job readPeriodicJobInfo(BufferedReader br) {
		Job periodicJob = new Job();
		String str;
		str = readOneLine(br);
		str = readOneLine(br);
		if((str = readOneLine(br)).compareTo("// pi") == 0) {
			str = readOneLine(br);
			
			periodicJob.processingTime = Integer.parseInt(str);
			periodicJob.jobId = 1;
		}
		
		return periodicJob;
	}
	
	//Read list jobs info
	public static ArrayList<Job> readListJobInfo(BufferedReader br) {
		ArrayList<Job> listJob = new ArrayList<Job>();
		int jobId = 0;
		String str;
		str = readOneLine(br);
		str = readOneLine(br);
		if((str = readOneLine(br)).compareTo("// pi") == 0) {
			str = readOneLine(br);
			
			String[] listProcessingTimeString = str.split(" ");
			for(String s : listProcessingTimeString) {
				jobId += 1;
				Job result = new Job(Integer.parseInt(s), 0);
				result.jobId = jobId;
				listJob.add(result);
			}
		}
		
		if((str = readOneLine(br)).compareTo("// di") == 0) {
			str = readOneLine(br);
			
			String[] listDeadlineString = str.split(" ");
			for(int i = 0; i < listDeadlineString.length; i++) {
				listJob.get(i).deadline = Integer.parseInt(listDeadlineString[i]);
			}
		}
		
		return listJob;
	}
	
	//Read cycle info
	public static int readCycleInfo(BufferedReader br) {
		int cycle = 0;
		String str = readOneLine(br);
		cycle = Integer.parseInt(str);
		
		return cycle;
	}
}