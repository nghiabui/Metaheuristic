package Main;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;

import jxl.write.WriteException;

import Elements.Individual;
import Elements.Instance;
import Elements.Job;
import Elements.Population;
import Elements.Record;
import Elements.Result;
import Elements.Run;

import SchedulingAlgorithms.LPTNormal;
import SchedulingAlgorithms.LPTFirstJobHeuristic;
import SchedulingAlgorithms.LPTLastJobHeuristic;
import SchedulingAlgorithms.LPTAdequateJobHeuristic;
import SchedulingAlgorithms.LPTGenetic;

import SchedulingAlgorithms.EDFNormal;
import SchedulingAlgorithms.EDFFirstJobHeuristic;
import SchedulingAlgorithms.EDFGenetic;

import Utils.EDFGeneticExcelFileCreate;
import Utils.EDFNormalExcelFileCreate;
import Utils.LPTGeneticExcelFileCreate;
import Utils.LPTNormalExcelFileCreate;
import Utils.LPTStatisticExcelFileCreate;
import Utils.InputCreate;
import Utils.InputParse;
import Utils.GeneticParamCreate;

public class Main {
	public static int[] cycles;
	public static int[] numberJobs;
	public static int[] processingTimes;
	public static int NUMBERRUN;
	public static int[] runtimes;
	public static int[] populationSizes;
	public static double[] mutateRates;
	public static double[] crossoverRates;
	
    public static void main(String[] args) throws FileNotFoundException, WriteException {
    	InputCreate inputCreate;
    	GeneticParamCreate geneticParam;
    	ArrayList<Result> listResult;
    	
    	// Run an instance
    	String fileName = "input.txt";						
		File inputFile = new File(fileName);
		Instance instance = InputParse.pasreInputFile(inputFile);
		Instance ins;
    	
		ins = new Instance(instance);
		LPTLastJobHeuristic.schedule(ins);
		LPTLastJobHeuristic.printListRecord(ins.listRecord);
		System.out.println(ins.makespan);
		System.out.println(ins.totalTardiness);
		ins = new Instance(instance);
		EDFNormal.schedule(ins);
		EDFNormal.printListRecord(ins.listRecord);
		System.out.println(ins.makespan);
		System.out.println(ins.totalTardiness);
		
//		ins = new Instance(instance);
//		long start = System.currentTimeMillis();
//		ArrayList<Integer> listOriginalAperiodicJob = ins.getListAperiodicJobId();
//		ArrayList<Individual> listLPTHeuristicIndividual = getListLPTHeuristicIndividual(ins, inputFile);
//		System.out.println(listLPTHeuristicIndividual.get(0).makespan);
//		HashMap<String, Double> listParam = getListGeneticParam(1000, 0.05, 0.2, 600);
//		long end = System.currentTimeMillis();
//		long usedTime = end - start;
//		Individual indiv = LPTGenetic.geneticScheduling(listOriginalAperiodicJob, listLPTHeuristicIndividual, listParam, ins, usedTime);
//		Population pop = new Population(ins);
//		pop.changeInstanceFromIndividual(indiv);
//		ArrayList<Record> listRecord = pop.runNormalScheduling(pop.instance);
//		LPTGenetic.printListRecord(listRecord);
//		System.out.println(pop.instance.makespan);
		
		ins = new Instance(instance);
		long start = System.currentTimeMillis();
		ArrayList<Integer> listOriginalAperiodicJob = ins.getListAperiodicJobId();
		Individual EDFHeuristicIndividual = getEDFHeuristicIndividual(ins, inputFile);
		HashMap<String, Double> listParam = getListGeneticParam(5000, 0.01, 0.2, 30);
		long end = System.currentTimeMillis();
		long usedTime = end - start;
		Individual indiv = EDFGenetic.geneticScheduling(listOriginalAperiodicJob, EDFHeuristicIndividual, listParam, ins, usedTime);
		Population pop = new Population(ins);
		pop.changeInstanceFromIndividual(indiv);
		ArrayList<Record> listRecord = pop.runNormalScheduling(pop.instance);
		LPTGenetic.printListRecord(listRecord);
		System.out.println(pop.instance.totalTardiness);
    	
		
//		// Run and create excel file
//    	// LPT normal experimental results
    	LPTNormalCreateInputParams();
    	inputCreate = new InputCreate(cycles, numberJobs, processingTimes, NUMBERRUN);
    	inputCreate.createInputFile();
    	listResult = new ArrayList<Result>();
    	// Save the result of all schedules
    	listResult = runLPTNormalSchedulingAndSave();
    	// Create the excel result file
    	LPTNormalExcelFileCreate.createExcelFile(listResult, inputCreate, "LPTNormal.xls");
    	
    	// EDF normal experimental results    	
    	EDFNormalCreateInputParams();
    	inputCreate = new InputCreate(cycles, numberJobs, processingTimes, NUMBERRUN);
    	inputCreate.createInputFile();
    	listResult = new ArrayList<Result>();
    	// Save the result of all schedules
    	listResult = runEDFNormalSchedulingAndSave();
    	// Create the excel result file
    	EDFNormalExcelFileCreate.createExcelFile(listResult, inputCreate, "EDFNormal.xls");
    	
//    	// LPT statistic experimental results
    	LPTStatisticCreateInputParams();
    	inputCreate = new InputCreate(cycles, numberJobs, processingTimes, NUMBERRUN);
    	inputCreate.createInputFile();
    	listResult = new ArrayList<Result>();
    	// Save the result of all schedules
    	listResult = runLPTStatisticSchedulingAndSave();
    	// Create the excel result file
    	LPTStatisticExcelFileCreate.createExcelFile(listResult, inputCreate, "LPTStatistic.xls");    		

    	// LPT genetic experimental results
    	LPTGeneticCreateInputParams();
    	inputCreate = new InputCreate(cycles, numberJobs, processingTimes, NUMBERRUN);
    	inputCreate.createInputFile();
    	geneticParam = new GeneticParamCreate(populationSizes, mutateRates, crossoverRates, runtimes);
    	listResult = new ArrayList<Result>();
    	// Save the result of all schedules
    	listResult = runLPTGeneticSchedulingAndSave();
    	// Create the excel result file.
    	LPTGeneticExcelFileCreate.createExcelFile(listResult, inputCreate, geneticParam, "LPTGenetic.xls");  	
    	
    	// EDF genetic experimental results
    	EDFGeneticCreateInputParams();
    	inputCreate = new InputCreate(cycles, numberJobs, processingTimes, NUMBERRUN);
    	inputCreate.createInputFile();
    	geneticParam = new GeneticParamCreate(populationSizes, mutateRates, crossoverRates, runtimes);
    	listResult = new ArrayList<Result>();
    	// Save the result of all schedules
    	listResult = runEDFGeneticSchedulingAndSave();
    	// Create the excel result file
    	EDFGeneticExcelFileCreate.createExcelFile(listResult, inputCreate, geneticParam, "EDFGenetic.xls");  
    }
    
    // Run 4 LPT schedule algorithms and save the results
    public static ArrayList<Result> runLPTStatisticSchedulingAndSave() {
    	// Save LPT scheduling result
    	ArrayList<Result> listResult = new ArrayList<Result>();
    	
    	for(int i = 0; i < cycles.length; i++) {
			for(int j = 0; j < processingTimes.length; j++) {
				for(int k = 0; k < numberJobs.length; k++) {
					for(int m = 0; m < NUMBERRUN; m++) {
						String fileName = ".\\src\\InputFiles\\input_"+String.valueOf(cycles[i])+"_"+String.valueOf(processingTimes[j])+"_"+String.valueOf(numberJobs[k])+"_"+String.valueOf(m+1)+".txt";						
						File inputFile = new File(fileName);
						Instance ins = InputParse.pasreInputFile(inputFile);
						ArrayList<Run> listRun = new ArrayList<Run>();
						Instance instance;
						
						instance = new Instance(ins);
				    	LPTNormal.schedule(instance);
				    	listRun.add(new Run(instance.makespan, "LPTNormal"));
				    	
				    	instance = new Instance(ins);
				    	LPTFirstJobHeuristic.schedule(instance);
				    	listRun.add(new Run(instance.makespan, "LPTFirstJobHeuristic"));
				    	
				    	instance = new Instance(ins);
				    	LPTLastJobHeuristic.schedule(instance);
				    	listRun.add(new Run(instance.makespan, "LPTLastJobHeuristic"));
				    	
				    	instance = new Instance(ins);
	    				LPTAdequateJobHeuristic.schedule(instance);
	    				listRun.add(new Run(instance.makespan, "LPTAdequateJobHeuristic"));
	    				
	    				// Get results of 4 algorithms
	    				listResult.add(new Result(listRun));
					}
				}
			}
		}
    	
    	return listResult;
    }
    
    // Run LPT normal schedule algorithm and save the results
    public static ArrayList<Result> runLPTNormalSchedulingAndSave() {
    	// Save LPT scheduling result
    	ArrayList<Result> listResult = new ArrayList<Result>();
    	
    	for(int i = 0; i < cycles.length; i++) {
			for(int j = 0; j < processingTimes.length; j++) {
				for(int k = 0; k < numberJobs.length; k++) {
					for(int m = 0; m < NUMBERRUN; m++) {
						System.out.println("NUMBERRUN"+NUMBERRUN);
						String fileName = ".\\src\\InputFiles\\input_"+String.valueOf(cycles[i])+"_"+String.valueOf(processingTimes[j])+"_"+String.valueOf(numberJobs[k])+"_"+String.valueOf(m+1)+".txt";						
						File inputFile = new File(fileName);
						Instance ins = InputParse.pasreInputFile(inputFile);
						ArrayList<Run> listRun = new ArrayList<Run>();
						Instance instance;
						
						instance = new Instance(ins);
				    	LPTNormal.schedule(instance);
				    	listRun.add(new Run(instance.makespan, "LPTNormal"));
				    	
				    	instance = new Instance(ins);
				    	LPTFirstJobHeuristic.schedule(instance);
				    	listRun.add(new Run(instance.makespan, "LPTFirstJobHeuristic"));
				    	
				    	instance = new Instance(ins);
				    	LPTLastJobHeuristic.schedule(instance);
				    	listRun.add(new Run(instance.makespan, "LPTLastJobHeuristic"));
				    	
				    	instance = new Instance(ins);
	    				LPTAdequateJobHeuristic.schedule(instance);
	    				listRun.add(new Run(instance.makespan, "LPTAdequateJobHeuristic"));
	    				
	    				// Get results of 4 algorithms
	    				listResult.add(new Result(listRun));
					}
				}
			}
		}
    	
    	return listResult;
    }
    
    // Run EDF normal schedule algorithm and save the results
    public static ArrayList<Result> runEDFNormalSchedulingAndSave() {
    	// Save LPT scheduling result
    	ArrayList<Result> listResult = new ArrayList<Result>();
    	
    	for(int i = 0; i < cycles.length; i++) {
			for(int j = 0; j < processingTimes.length; j++) {
				for(int k = 0; k < numberJobs.length; k++) {
					for(int m = 0; m < NUMBERRUN; m++) {
						String fileName = ".\\src\\InputFiles\\input_"+String.valueOf(cycles[i])+"_"+String.valueOf(processingTimes[j])+"_"+String.valueOf(numberJobs[k])+"_"+String.valueOf(m+1)+".txt";						
						File inputFile = new File(fileName);
						Instance ins = InputParse.pasreInputFile(inputFile);
						ArrayList<Run> listRun = new ArrayList<Run>();
						Instance instance;
						
						instance = new Instance(ins);
				    	EDFNormal.schedule(instance);
				    	listRun.add(new Run("EDFNormal", instance.totalTardiness));
				    	
				    	instance = new Instance(ins);
				    	EDFFirstJobHeuristic.schedule(instance);
				    	listRun.add(new Run("EDFFirstJobHeuristic", instance.totalTardiness));
	    				
	    				// Get results of 2 algorithms
	    				listResult.add(new Result(listRun));
					}
				}
			}
		}
    	
    	return listResult;
    }
    
    // Run LPT genetic schedule algorithm and save the results
    public static ArrayList<Result> runLPTGeneticSchedulingAndSave() {
    	// Save LPT Genetic scheduling result
    	ArrayList<Result> listResult = new ArrayList<Result>();
    	
    	for(int i = 0; i < cycles.length; i++) {
			for(int j = 0; j < processingTimes.length; j++) {
				for(int k = 0; k < numberJobs.length; k++) {
					for(int m = 0; m < NUMBERRUN; m++) {
						String fileName = ".\\src\\InputFiles\\input_"+String.valueOf(cycles[i])+"_"+String.valueOf(processingTimes[j])+"_"+String.valueOf(numberJobs[k])+"_"+String.valueOf(m+1)+".txt";						
						File inputFile = new File(fileName);
						Instance ins = InputParse.pasreInputFile(inputFile);
						ArrayList<Run> listRun = new ArrayList<Run>();
						
						for(int t = 0; t < populationSizes.length; t++) {
							for(int q = 0; q < mutateRates.length; q++) {
								for(int r = 0; r < runtimes.length; r++) {
									for(int u = 0; u < crossoverRates.length; u++){
										Instance instance = new Instance(ins);
										long start = System.currentTimeMillis();
										ArrayList<Integer> listOriginalAperiodicJob = instance.getListAperiodicJobId();
										ArrayList<Individual> listLPTHeuristicIndividual = getListLPTHeuristicIndividual(instance, inputFile);
										Individual bestLPTHeuristicIndividual = getBestLPTMakespanIndividual(listLPTHeuristicIndividual);
	//									System.out.println(cycles[i] + " " + processingTimes[j] + " " +
	//											numberJobs[k] + " " + populationSizes[t] + " " + mutateRates[q] + " LPT: " + bestLPTHeuristicIndividual.makespan);
										listRun.add(new Run(bestLPTHeuristicIndividual.makespan, "LPTHeuristic"));
										
										HashMap<String, Double> listParam = getListGeneticParam(populationSizes[t], mutateRates[q], crossoverRates[u], runtimes[r]);
										long end = System.currentTimeMillis();
										long usedTime = end - start;
										Individual indiv = LPTGenetic.geneticScheduling(listOriginalAperiodicJob, listLPTHeuristicIndividual, listParam, ins, usedTime);
										
	//									System.out.println(cycles[i] + " " + processingTimes[j] + " " +
	//											numberJobs[k] + " " + populationSizes[t] + " " + mutateRates[q] + " Genetic: " + indiv.makespan);
										listRun.add(new Run(indiv.makespan, "LPTGenetic"));
									}
								}
							}
						}
						// Get results of 2 algorithms
	    				listResult.add(new Result(listRun));
					}
				}
			}
    	}
    	
    	return listResult;
    }
    
    // Run EDF genetic schedule algorithm and save the results
    public static ArrayList<Result> runEDFGeneticSchedulingAndSave() {
    	// Save EDF Genetic scheduling result
    	ArrayList<Result> listResult = new ArrayList<Result>();
    	
    	for(int i = 0; i < cycles.length; i++) {
			for(int j = 0; j < processingTimes.length; j++) {
				for(int k = 0; k < numberJobs.length; k++) {
					for(int m = 0; m < NUMBERRUN; m++) {
						String fileName = ".\\src\\InputFiles\\input_"+String.valueOf(cycles[i])+"_"+String.valueOf(processingTimes[j])+"_"+String.valueOf(numberJobs[k])+"_"+String.valueOf(m+1)+".txt";						
						File inputFile = new File(fileName);
						Instance ins = InputParse.pasreInputFile(inputFile);
						ArrayList<Run> listRun = new ArrayList<Run>();
						
						for(int t = 0; t < populationSizes.length; t++) {
							for(int q = 0; q < mutateRates.length; q++) {
								for(int r = 0; r < runtimes.length; r++) {
									for(int u = 0; u < crossoverRates.length; u++){
										Instance instance = new Instance(ins);
										long start = System.currentTimeMillis();
										ArrayList<Integer> listOriginalAperiodicJob = instance.getListAperiodicJobId();
										Individual EDFHeuristicIndividual = getEDFHeuristicIndividual(instance, inputFile);
										listRun.add(new Run("EDFHeuristic", EDFHeuristicIndividual.totalTardiness));
										
										HashMap<String, Double> listParam = getListGeneticParam(populationSizes[t], mutateRates[q], crossoverRates[u], runtimes[r]);
										long end = System.currentTimeMillis();
										long usedTime = end - start;
										Individual indiv = EDFGenetic.geneticScheduling(listOriginalAperiodicJob, EDFHeuristicIndividual, listParam, ins, usedTime);
										
	//									System.out.println(cycles[i] + " " + processingTimes[j] + " " +
	//											numberJobs[k] + " " + populationSizes[t] + " " + mutateRates[q] + " Genetic: " + indiv.makespan);
										listRun.add(new Run("EDFGenetic", indiv.totalTardiness));
									}
								}
							}
						}
						// Get results of 2 algorithms
	    				listResult.add(new Result(listRun));
					}
				}
			}
    	}
    	
    	return listResult;
    }
    
    // Get list of LPT heuristic individuals from LPT heuristic schedules to use in genetic algorithm
    public static ArrayList<Individual> getListLPTHeuristicIndividual(Instance ins, File inputFile){
    	ArrayList<Individual> listHeuristicIndividual = new ArrayList<Individual>();  
    	Instance instance;
    	
    	instance = new Instance(ins);
    	LPTFirstJobHeuristic.schedule(instance);
    	listHeuristicIndividual.add(instance.getIndividualFromSchedulingResult());
    	
    	instance = new Instance(ins);
    	LPTLastJobHeuristic.schedule(instance);
    	listHeuristicIndividual.add(instance.getIndividualFromSchedulingResult());
    	
    	instance = new Instance(ins);
		LPTAdequateJobHeuristic.schedule(instance);
		listHeuristicIndividual.add(instance.getIndividualFromSchedulingResult());
		
		return listHeuristicIndividual;
    }
    
    // Get EDF heuristic individual from EDF heuristic schedule to use in genetic algorithm
    public static Individual getEDFHeuristicIndividual(Instance ins, File inputFile){
    	Individual heuristicIndividual = new Individual();  
    	Instance instance;
    	
    	instance = new Instance(ins);
    	EDFFirstJobHeuristic.schedule(instance);
    	heuristicIndividual = instance.getIndividualFromSchedulingResult();
    	
		return heuristicIndividual;
    }
    
    // Get list of parameters of genetic algorithm
    public static HashMap<String, Double> getListGeneticParam(int populationSize, double mutateRate, double crossoverRate, int runtime){
    	HashMap<String, Double> listParam = new HashMap<String, Double>();
    	listParam.put("populationSize", (double)populationSize);
    	listParam.put("mutateRate", mutateRate);
    	listParam.put("runtime", (double)runtime);
    	listParam.put("crossoverRate", crossoverRate);
    	
    	return listParam;
    }
    
    // Change list of aperiodic jobs by an individual
    public static void changeListAperiodicJobByIndividual(ArrayList<Job> listJob, int[] listIndividualJob){
    	ArrayList<Job> listTmpJob = new ArrayList<Job>();
    	for(int i = 0; i < listIndividualJob.length; i++) {
    		for(int j = 0; j < listJob.size(); j++){
    			if(listIndividualJob[i] == listJob.get(j).jobId){
    				listTmpJob.add(listJob.remove(j));
    				break;
    			}
    		}
    	}
    	
    	while(!listTmpJob.isEmpty()){
    		listJob.add(listTmpJob.remove(0));
    	}
    }
    
    // Create input parameters for LPT normal algorithm
    public static void LPTNormalCreateInputParams(){
    	cycles = new int[1];
		cycles[0] = 100;
//		cycles[1] = 90;
//		cycles[2] = 100;
		
		numberJobs = new int[4];
		numberJobs[0] = 10;
		numberJobs[1] = 100;
		numberJobs[2] = 1000;
		numberJobs[3] = 10000;
		
		processingTimes = new int[5];
		processingTimes[0] = 10;
		processingTimes[1] = 30;
		processingTimes[2] = 50;
		processingTimes[3] = 70;
		processingTimes[4] = 90;
		
		NUMBERRUN = 1;
    }
    
    // Create input parameters for 4 LPT heuristic algorithms
    public static void LPTStatisticCreateInputParams(){
    	cycles = new int[1];
		cycles[0] = 100;
//		cycles[1] = 90;
//		cycles[2] = 100;
		
		numberJobs = new int[4];
		numberJobs[0] = 9000;
		numberJobs[1] = 10000;
		numberJobs[2] = 11000;
		numberJobs[3] = 12000;
		
		processingTimes = new int[5];
		processingTimes[0] = 10;
		processingTimes[1] = 30;
		processingTimes[2] = 50;
		processingTimes[3] = 70;
		processingTimes[4] = 90;
		
		NUMBERRUN = 10;
    }
    
    // Create input parameters for LPT genetic algorithm
    public static void LPTGeneticCreateInputParams(){
    	cycles = new int[1];
		cycles[0] = 100;
		
//		cycles[1] = 100;
		
		numberJobs = new int[1];
		numberJobs[0] = 1000;
//		numberJobs[1] = 6000;
//		numberJobs[2] = 7000;
//		numberJobs[3] = 8000;
		
		processingTimes = new int[1];
		processingTimes[0] = 50;
//		processingTimes[1] = 50;
		
		NUMBERRUN = 1;
		
		populationSizes = new int[1];
		populationSizes[0] = 5000;
//		populationSizes[1] = 2000;
//		populationSizes[2] = 3000;
//		populationSizes[3] = 4000;
//		populationSizes[4] = 5000;
//		populationSizes[5] = 6000;
//		populationSizes[6] = 7000;
//		populationSizes[7] = 8000;
//		populationSizes[8] = 9000;
//		populationSizes[9] = 10000;
		
		mutateRates = new double[10];
		mutateRates[0] = 0.105;
		mutateRates[1] = 0.11;
		mutateRates[2] = 0.115;
		mutateRates[3] = 0.12;
		mutateRates[4] = 0.125;
		mutateRates[5] = 0.13;
		mutateRates[6] = 0.135;
		mutateRates[7] = 0.14;
		mutateRates[8] = 0.145;
		mutateRates[9] = 0.15;
		
		crossoverRates = new double[1];
		crossoverRates[0] = 0.5;
		
		runtimes = new int[1];
		runtimes[0] = 90;
//		runtimes[1] = 60;
//		runtimes[2] = 90;
//		runtimes[3] = 120;
//		runtimes[4] = 150;
//		runtimes[5] = 180;
//		runtimes[6] = 210;
//		runtimes[7] = 240;
    }
    
    // Create input parameters for EDF normal algorithm
    public static void EDFNormalCreateInputParams(){
    	cycles = new int[1];
		cycles[0] = 100;
//		cycles[1] = 90;
//		cycles[2] = 100;
		
		numberJobs = new int[4];
		numberJobs[0] = 10;
		numberJobs[1] = 100;
		numberJobs[2] = 1000;
		numberJobs[3] = 10000;
		
		processingTimes = new int[5];
		processingTimes[0] = 10;
		processingTimes[1] = 30;
		processingTimes[2] = 50;
		processingTimes[3] = 70;
		processingTimes[4] = 90;
		
		NUMBERRUN = 1;
    }
    
    // Create input parameters for EDF genetic algorithm
    public static void EDFGeneticCreateInputParams(){
    	cycles = new int[1];
		cycles[0] = 100;
		
//		cycles[1] = 100;
		
		numberJobs = new int[1];
		numberJobs[0] = 1000;
//		numberJobs[1] = 1000;
		
		processingTimes = new int[1];
		processingTimes[0] = 70;
//		processingTimes[1] = 50;
		
		NUMBERRUN = 1;
		
		populationSizes = new int[2];
		populationSizes[0] = 5000;
		populationSizes[1] = 10000;
		
		mutateRates = new double[2];
		mutateRates[0] = 0.005;
		mutateRates[1] = 0.05;
		
		crossoverRates = new double[2];
		crossoverRates[0] = 0.2;
		crossoverRates[1] = 0.5;
		
		runtimes = new int[3];
		runtimes[0] = 30;
		runtimes[1] = 60;
		runtimes[2] = 90;
    }
    
    // Get the best makespan individual from list of individuals
    public static Individual getBestLPTMakespanIndividual(ArrayList<Individual> listLPTHeuristicIndividual) {
    	Individual resultIndiv = new Individual(listLPTHeuristicIndividual.get(0));
    	if(listLPTHeuristicIndividual.size() >= 2){
    		for(int i = 1; i < listLPTHeuristicIndividual.size(); i++){
    			if(listLPTHeuristicIndividual.get(i).makespan < resultIndiv.makespan){
    				resultIndiv = new Individual(listLPTHeuristicIndividual.get(i));
    			}
    		}
    	}
    	
    	return resultIndiv;
    }
    
    public static void printListResult(ArrayList<Result> listResult){
    	for(int i = 0; i < listResult.size(); i++) {
    		ArrayList<Run> listRun = listResult.get(i).listRun;
    		for(int j = 0; j < listRun.size(); j++){
    			System.out.println(listRun.get(j).algorithmName + " "  + listRun.get(j).makespan);
    		}
    	}
    }
}