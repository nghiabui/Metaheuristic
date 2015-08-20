package SchedulingAlgorithms;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Collections;
import java.util.Iterator;
import java.util.Random;

import Elements.Individual;
import Elements.Instance;
import Elements.Population;
import Elements.Record;

/**
 * @author VuHT
 * Description: LPT genetic scheduling algorithm
 */
public class LPTGenetic {
	// Genetic algorithm
	public static Individual geneticScheduling(ArrayList<Integer> listOriginalAperiodicJob, ArrayList<Individual> listLPTHeuristicIndividual, HashMap<String, Double> listParam, Instance ins, long usedTime){		
		long start = System.currentTimeMillis();
		
		// Get parameters of genetic algorithm
		int populationSize = (int)listParam.get("populationSize").doubleValue();
		double mutateRate = listParam.get("mutateRate");
		double crossoverRate = listParam.get("crossoverRate");
		int runtime = (int)listParam.get("runtime").doubleValue();
		
		// Create population
		Population schedulePopulation = generatePopulation(listOriginalAperiodicJob, listLPTHeuristicIndividual, populationSize, ins);
		
		long end = System.currentTimeMillis();
		long generateTime = end - start;
		
		// Run genetic algorithm
		start = System.currentTimeMillis();
		end = start + runtime*1000 - usedTime - generateTime;   // runtime seconds * 1000 ms/sec
		while (System.currentTimeMillis() < end){
			// Crossover
			Individual indiv1 = tournamentSelection(schedulePopulation, crossoverRate);
			Individual indiv2 = tournamentSelection(schedulePopulation, crossoverRate);
			ArrayList<Individual> listCrossoverChildIndividual = crossover(indiv1, indiv2, schedulePopulation);
			
			schedulePopulation.replaceListWorstMakespanIndividual(listCrossoverChildIndividual);
		   
			// Mutate
			mutate(mutateRate, schedulePopulation);
		}
		
		Individual bestIndividual = schedulePopulation.getBestMakespanIndividual();
		
		return bestIndividual;
	}
	
	// Generate population
	public static Population generatePopulation(ArrayList<Integer> listOriginalAperiodicJob, ArrayList<Individual> listLPTHeuristicIndividual, int populationSize, Instance ins){
		Population pop = new Population(ins);
		int numberLPTHeuristicIndividual = listLPTHeuristicIndividual.size();
		
		// Add list of aperiodic jobs of heuristic schedules to population
		for(int i = 0; i < numberLPTHeuristicIndividual; i++){
			Individual heuristicIndividual = listLPTHeuristicIndividual.get(i);
			pop.addIndividual(new Individual(heuristicIndividual));
		}
		
		// Add remain random list of aperiodic jobs
		//for(int i = 0; i < (populationSize - numberLPTHeuristicIndividual) / 3; i++){
//		int count = 0;
		while(pop.listIndividual.size() < populationSize){
			Collections.shuffle(listOriginalAperiodicJob);
			
			Individual initialIndiv = new Individual(listOriginalAperiodicJob);
			pop.changeInstanceFromIndividual(initialIndiv);
			
			Individual[] listHEUIndividual = new Individual[3];
			listHEUIndividual[0] = pop.getLPTHEU1Individual();
			listHEUIndividual[1] = pop.getLPTHEU2Individual();
			listHEUIndividual[2] = pop.getLPTHEU3Individual();
			
			pop.addListIndividual(listHEUIndividual);
//			count++;
//			System.out.println(count);
		}
		
		while(pop.listIndividual.size() > populationSize){
			pop.listIndividual.remove(pop.listIndividual.size()-1);
		}
		
		return pop;
	}
	
	// Get tournament individual in population
	public static Individual tournamentSelection(Population pop, double crossoverRate){
		int tournamentSize = (int)(crossoverRate*pop.listIndividual.size());
		Population tournamentPop = new Population();
		for (int i = 0; i < tournamentSize; i++) {
			int randomId = (int)(Math.random() * pop.listIndividual.size());
			Individual indiv = new Individual(pop.listIndividual.get(randomId));
			tournamentPop.addIndividual(indiv);
		}
		Individual fittestIndiv = tournamentPop.getBestMakespanIndividual();
		return fittestIndiv;
	}
	
	// Crossover
	public static ArrayList<Individual> crossover(Individual parent1, Individual parent2, Population pop) {
		Individual []offspring = new Individual[2];
		offspring[0] = new Individual(parent1);
		offspring[1] = new Individual(parent2);
		
		int individualLength = offspring[0].listAperiodicJob.length;
		
		int parent1Vector[]    = new int[individualLength];
		for(int i = 0; i < individualLength; i++){
			parent1Vector[i] = parent1.listAperiodicJob[i];
		}
		int parent2Vector[]    = new int[individualLength];
		
		for(int i = 0; i < individualLength; i++){
			parent2Vector[i] = parent2.listAperiodicJob[i];
		}
		
		int offspring1Vector[] = new int[individualLength];
		for(int i = 0; i < individualLength; i++){
			offspring1Vector[i] = offspring[0].listAperiodicJob[i];
		}
		int offspring2Vector[] = new int[individualLength];
		for(int i = 0; i < individualLength; i++){
			offspring2Vector[i] = offspring[1].listAperiodicJob[i];
		}
	
		int cuttingPoint1 ;
		int cuttingPoint2 ;
	
		// STEP 1: Get two cutting points
		Random random = new Random();
		cuttingPoint1 = random.nextInt(individualLength) ;
		cuttingPoint2 = random.nextInt(individualLength) ;
		while (cuttingPoint2 == cuttingPoint1){
			cuttingPoint2 = random.nextInt(individualLength) ;
		}
		if (cuttingPoint1 > cuttingPoint2) {
			int swap ;
			swap = cuttingPoint1 ;
			cuttingPoint1 = cuttingPoint2 ;
			cuttingPoint2 = swap          ;
		}
	
		// STEP 2: Get the subchains to interchange
		int replacement1[] = new int[individualLength] ;
		int replacement2[] = new int[individualLength] ;
		for (int i = 0; i < individualLength; i++){
			replacement1[i] = replacement2[i] = -1;
		}
		
		// STEP 3: Interchange   	
		for (int i = cuttingPoint1; i <= cuttingPoint2; i++){
			offspring1Vector[i] = parent2Vector[i] ;
			offspring2Vector[i] = parent1Vector[i] ;
		
			replacement1[parent2Vector[i]-1] = parent1Vector[i];
			replacement2[parent1Vector[i]-1] = parent2Vector[i];
		}
	
		// STEP 4: Repair offsprings
		for (int i = 0; i < individualLength; i++) {
			if ((i >= cuttingPoint1) && (i <= cuttingPoint2)){
		         continue ;
			}

			int n1 = parent1Vector[i] ;
			int m1 = replacement1 [n1-1] ;
		
			int n2 = parent2Vector[i] ;
			int m2 = replacement2 [n2-1] ;
			
			while (m1 != -1){
				n1 = m1 ;
				m1 = replacement1[m1-1] ;
			}
			while (m2 != -1){
				n2 = m2 ;
				m2 = replacement2[m2-1] ;
			}
			offspring1Vector[i] = n1 ;
			offspring2Vector[i] = n2 ;
		}
		
		// STEP 5: Calculate makespan
		for(int j = 0; j < offspring1Vector.length; j++) {
			offspring[0].listAperiodicJob[j] = offspring1Vector[j];
		}
		for(int j = 0; j < offspring2Vector.length; j++) {
			offspring[1].listAperiodicJob[j] = offspring2Vector[j];
		}
		
		ArrayList<Individual> listOffspringIndividual = new ArrayList<Individual>();
		pop.changeInstanceFromIndividual(offspring[0]);
		listOffspringIndividual.add(pop.getLPTHEU1Individual());
		listOffspringIndividual.add(pop.getLPTHEU2Individual());
		listOffspringIndividual.add(pop.getLPTHEU3Individual());
		
		pop.changeInstanceFromIndividual(offspring[1]);
		listOffspringIndividual.add(pop.getLPTHEU1Individual());
		listOffspringIndividual.add(pop.getLPTHEU2Individual());
		listOffspringIndividual.add(pop.getLPTHEU3Individual());
		
		return listOffspringIndividual; 
	}
	
	// Mutate
	public static void mutate(double mutateRate, Population pop){
		double randomNumber = Math.random();
		if (randomNumber < mutateRate){
			Random random = new Random();
			// Get random individual in pop
			int randomIndex = random.nextInt(pop.listIndividual.size());
			Individual considerIndiv = pop.listIndividual.get(randomIndex);
		
			int individualLength = considerIndiv.listAperiodicJob.length;
			int numberMutateGen = random.nextInt(individualLength)+1;
		
			int[] listPos1 = new int[numberMutateGen];
			int[] listPos2 = new int[numberMutateGen];
			
			for(int i = 0; i < numberMutateGen; i++) {
				listPos1[i] = random.nextInt(individualLength);
				listPos2[i] = random.nextInt(individualLength);
			}
			
//			while (pos1 == pos2){ 
//				if (pos1 == (individualLength - 1)){
//					pos2 = random.nextInt(individualLength - 1);
//				}
//				else{
//					pos2 = random.nextInt(individualLength - pos1) + pos1;
//				}
//			}
			
			for(int i = 0; i < numberMutateGen; i++) {
				if(listPos1[i] != listPos2[i]) {
					// swap
					int temp = considerIndiv.listAperiodicJob[listPos1[i]];
					considerIndiv.listAperiodicJob[listPos1[i]] = considerIndiv.listAperiodicJob[listPos2[i]];
					considerIndiv.listAperiodicJob[listPos2[i]] = temp;
				}
			}
			
			// Calculate new makespan
			Individual[] listMutateIndividual = new Individual[3];
			pop.changeInstanceFromIndividual(considerIndiv);
			listMutateIndividual[0] = pop.getLPTHEU1Individual();
			listMutateIndividual[1] = pop.getLPTHEU2Individual();
			listMutateIndividual[2] = pop.getLPTHEU3Individual();
			Individual bestIndividual = listMutateIndividual[0];
			for(int i = 1; i < listMutateIndividual.length; i++){
				if(bestIndividual.makespan > listMutateIndividual[i].makespan){
					bestIndividual = listMutateIndividual[i];
				}
			}
			
			pop.replaceIndividual(considerIndiv, bestIndividual);
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