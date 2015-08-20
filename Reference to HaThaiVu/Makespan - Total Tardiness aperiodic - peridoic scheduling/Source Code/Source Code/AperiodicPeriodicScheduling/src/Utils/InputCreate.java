package Utils;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.Random;

/**
 * @author VuHT
 * Description: Create input files
 */
public class InputCreate {
	public int[] cycles;
	public int[] numberJobs;
	public int[] processingTimes;
	public int NUMBERRUN = 1;
	
	// Constructor
	public InputCreate(int[] cycles, int[] numberJobs, int[] processingTimes, int NUMBERRUN){
		this.cycles = new int[cycles.length];
		for(int i = 0; i < cycles.length; i++){
			this.cycles[i] = cycles[i];
		}
		
		this.numberJobs = new int[numberJobs.length];
		for(int i = 0; i < numberJobs.length; i++){
			this.numberJobs[i] = numberJobs[i];
		}
		
		this.processingTimes = new int[processingTimes.length];
		for(int i = 0; i < processingTimes.length; i++){
			this.processingTimes[i] = processingTimes[i];
		}
		
		this.NUMBERRUN = NUMBERRUN;
	}
	
	//Read input file and create the instance
	public void createInputFile() {
		try {
			for(int i = 0; i < cycles.length; i++) {
				for(int j = 0; j < processingTimes.length; j++) {
					for(int k = 0; k < numberJobs.length; k++) {
						for(int m = 0; m < NUMBERRUN; m++) {
							String fileName = ".\\src\\InputFiles\\input_"+String.valueOf(cycles[i])+"_"+String.valueOf(processingTimes[j])+"_"+String.valueOf(numberJobs[k])+"_"+String.valueOf(m+1)+".txt";
							
							FileOutputStream fostream = new FileOutputStream(fileName);
							OutputStreamWriter oswriter = new OutputStreamWriter(fostream);
							BufferedWriter bwriter = new BufferedWriter(oswriter);
							
							bwriter.write("// periodic");bwriter.newLine();
							bwriter.write("// number of jobs");bwriter.newLine();
							bwriter.write("1");bwriter.newLine();
							bwriter.write("// pi");bwriter.newLine();
							bwriter.write(String.valueOf(processingTimes[j]));bwriter.newLine();
							bwriter.write("// Cycle");bwriter.newLine();
							bwriter.write(String.valueOf(cycles[i]));bwriter.newLine();
							bwriter.write("// aperiodic");bwriter.newLine();
							bwriter.write("// number of jobs");bwriter.newLine();
							bwriter.write(String.valueOf(numberJobs[k]));bwriter.newLine();
							bwriter.write("// pi");bwriter.newLine();
							Random randomGenerator;
							for(int p = 0; p < numberJobs[k]; p++){
								randomGenerator = new Random();
								//int index = randomGenerator.nextInt(processingTimes.length);
								//bwriter.write(String.valueOf(processingTimes[index]));bwriter.write(" ");
								
								int maximumProcessingTime = 2*(cycles[i]-processingTimes[j]);
								int aperiodJobProcessingTime = 1 + randomGenerator.nextInt(maximumProcessingTime-1);
								bwriter.write(String.valueOf(aperiodJobProcessingTime));bwriter.write(" ");
							}
							bwriter.newLine();
							bwriter.write("// di");bwriter.newLine();
							for(int p = 0; p < numberJobs[k]; p++){
								randomGenerator = new Random();
								
								int maximumDeadline = numberJobs[k]*(cycles[i]-processingTimes[j])/10;
								int aperiodJobDeadline = 1 + randomGenerator.nextInt(maximumDeadline);
								bwriter.write(String.valueOf(aperiodJobDeadline));bwriter.write(" ");
							}
							bwriter.newLine();
							
							bwriter.close();
							oswriter.close();
							fostream.close();
						}
					}
				}
			}
		}
		catch(IOException e) {
            e.printStackTrace();
        }
	}
}