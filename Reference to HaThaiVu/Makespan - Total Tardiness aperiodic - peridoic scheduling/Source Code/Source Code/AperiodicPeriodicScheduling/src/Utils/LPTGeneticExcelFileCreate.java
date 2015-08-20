package Utils;

import java.io.File;
import java.util.ArrayList;

import Elements.Result;
import Elements.Run;

import java.io.IOException;
import java.util.Locale;

import jxl.CellView;
import jxl.Workbook;
import jxl.WorkbookSettings;
import jxl.format.UnderlineStyle;
import jxl.write.Formula;
import jxl.write.Label;
import jxl.write.Number;
import jxl.write.WritableCellFormat;
import jxl.write.WritableFont;
import jxl.write.WritableSheet;
import jxl.write.WritableWorkbook;
import jxl.write.WriteException;
import jxl.write.biff.RowsExceededException;

/**
 * @author VuHT
 * Description: Create experimental result file for LPT genetic algorithms
 */
public class LPTGeneticExcelFileCreate {
	private static WritableCellFormat timesBold;
	private static WritableCellFormat times;
	
	// Constructor
	public LPTGeneticExcelFileCreate(){
	}
	
	// Write the excel file based on the result
	public static File write(ArrayList<Result> listResult, InputCreate inputCreate, GeneticParamCreate geneticParam, File file) throws IOException, WriteException {
	    WorkbookSettings wbSettings = new WorkbookSettings();

	    wbSettings.setLocale(new Locale("en", "EN"));

	    WritableWorkbook workbook = Workbook.createWorkbook(file, wbSettings);
	    workbook.createSheet("LPT Genetic", 0);
	    WritableSheet excelSheet = workbook.getSheet(0);
	    createLabel(excelSheet);
	    createContent(excelSheet, listResult, inputCreate, geneticParam);

	    workbook.write();
	    workbook.close();
	    
	    return file;
	}
	
	// Create the label
	private static void createLabel(WritableSheet sheet) throws WriteException {
	    // Lets create a times font
	    WritableFont times9pt = new WritableFont(WritableFont.TIMES, 9);
	    // Define the cell format
	    times = new WritableCellFormat(times9pt);
	    // Lets automatically wrap the cells
	    times.setWrap(true);

	    // Create create a bold font with underlines
	    WritableFont times9ptBold = new WritableFont(WritableFont.TIMES, 9, WritableFont.BOLD, false,
	        UnderlineStyle.NO_UNDERLINE);
	    timesBold = new WritableCellFormat(times9ptBold);
	    // Lets automatically wrap the cells
	    timesBold.setWrap(true);

	    CellView cv = new CellView();
	    cv.setFormat(times);
	    cv.setFormat(timesBold);
	    cv.setAutosize(true);

	    // Write headers
	    addCaption(sheet, 0, 0, "Cycle");
	    addCaption(sheet, 1, 0, "Periodic job processing time");
	    addCaption(sheet, 2, 0, "Number of aperiodic jobs");
	    addCaption(sheet, 3, 0, "Runtime(s)");
	    addCaption(sheet, 4, 0, "Population size");
	    addCaption(sheet, 5, 0, "Mutate rate");
	    addCaption(sheet, 6, 0, "Crossover rate");
	    addCaption(sheet, 7, 0, "LPT Heuristic makespan");
	    addCaption(sheet, 8, 0, "LPT Genetic makespan");
	    addCaption(sheet, 9, 0, "LPT Genetic better makespan percentage (%)");
	}
	
	public static void printListResult(ArrayList<Result> listResult){
    	for(int i = 0; i < listResult.size(); i++) {
    		ArrayList<Run> listRun = listResult.get(i).listRun;
    		for(int j = 0; j < listRun.size(); j++){
    			System.out.println(listRun.get(j).algorithmName + " "  + listRun.get(j).makespan);
    		}
    	}
    }
	
	// Create the content
	private static void createContent(WritableSheet sheet, ArrayList<Result> listResult, InputCreate inputCreate, GeneticParamCreate geneticParam) throws WriteException, RowsExceededException {
		for(int k = 0; k < inputCreate.cycles.length; k++) {
			for(int m = 0; m < inputCreate.processingTimes.length; m++) {
				for(int n = 0; n < inputCreate.numberJobs.length; n++) {
					// Initialize the result
					ArrayList<Run> listRun = listResult.remove(0).listRun;
					for(int t = 0; t < geneticParam.populationSizes.length; t++) {
						for(int q = 0; q < geneticParam.mutateRates.length; q++) {
							for(int r = 0; r < geneticParam.runtimes.length; r++) {
								for(int u = 0; u < geneticParam.crossoverRates.length; u++) {
							    	int bestLPTMakespan = listRun.remove(0).makespan;
							    	int LPTGeneticMakespan = listRun.remove(0).makespan;
							    	
							    	double betterPercentage = (double)(bestLPTMakespan - LPTGeneticMakespan) / bestLPTMakespan * 100;
							    	
							    	// Write to cell in excel file
							    	int lineNumber = k*(inputCreate.processingTimes.length*inputCreate.numberJobs.length*geneticParam.populationSizes.length*geneticParam.mutateRates.length*geneticParam.runtimes.length*geneticParam.crossoverRates.length) + 
							    					 m*(inputCreate.numberJobs.length*geneticParam.populationSizes.length*geneticParam.mutateRates.length*geneticParam.runtimes.length*geneticParam.crossoverRates.length) + 
							    					 n*(geneticParam.populationSizes.length*geneticParam.mutateRates.length*geneticParam.runtimes.length*geneticParam.crossoverRates.length) +
							    					 t*(geneticParam.mutateRates.length*geneticParam.runtimes.length*geneticParam.crossoverRates.length) + 
							    					 q*(geneticParam.runtimes.length*geneticParam.crossoverRates.length) + 
							    					 r*(geneticParam.crossoverRates.length) +
							    					 u + 1;
							    	addNumber(sheet, 0, lineNumber, inputCreate.cycles[k]);
							    	addNumber(sheet, 1, lineNumber, inputCreate.processingTimes[m]);
							    	addNumber(sheet, 2, lineNumber, inputCreate.numberJobs[n]);
							    	addNumber(sheet, 3, lineNumber, geneticParam.runtimes[r]);
							    	addNumber(sheet, 4, lineNumber, geneticParam.populationSizes[t]);
							    	addNumber(sheet, 5, lineNumber, geneticParam.mutateRates[q]);
							    	addNumber(sheet, 6, lineNumber, geneticParam.crossoverRates[u]);
							    	addNumber(sheet, 7, lineNumber, bestLPTMakespan);
							    	addNumber(sheet, 8, lineNumber, LPTGeneticMakespan);
							    	addNumber(sheet, 9, lineNumber, betterPercentage);
								}
							}
						}
					}
				}
			}
		}
		
		// Write the final average result
		// Create label
		int numberTestResult = inputCreate.cycles.length*inputCreate.processingTimes.length*inputCreate.numberJobs.length*geneticParam.populationSizes.length*geneticParam.mutateRates.length*geneticParam.runtimes.length*geneticParam.crossoverRates.length;
		addCaption(sheet, 0, numberTestResult+1, "Average result");
		
		// Create average results
		StringBuffer buf = new StringBuffer();
	    buf.append("AVERAGE(H2:H"+String.valueOf(numberTestResult+1)+")");
	    Formula f = new Formula(7, numberTestResult+1, buf.toString());
	    sheet.addCell(f);
	    buf = new StringBuffer();
	    buf.append("AVERAGE(I2:I"+String.valueOf(numberTestResult+1)+")");
	    f = new Formula(8, numberTestResult+1, buf.toString());
	    sheet.addCell(f);
	    buf = new StringBuffer();
	    buf.append("AVERAGE(J2:J"+String.valueOf(numberTestResult+1)+")");
	    f = new Formula(9, numberTestResult+1, buf.toString());
	    sheet.addCell(f);
	}

	// Add label value
	private static void addCaption(WritableSheet sheet, int column, int row, String s) throws RowsExceededException, WriteException {
	    Label label;
	    label = new Label(column, row, s, timesBold);
	    sheet.addCell(label);
	}
	
	// Add number value
	private static void addNumber(WritableSheet sheet, int column, int row, double value) throws WriteException, RowsExceededException {
	    Number number;
	    number = new Number(column, row, value, times);
	    sheet.addCell(number);
	}

//	private static void addLabel(WritableSheet sheet, int column, int row, String s) throws WriteException, RowsExceededException {
//	    Label label;
//	    label = new Label(column, row, s, times);
//	    sheet.addCell(label);
//	}
	
	// Create excel result file
	public static void createExcelFile(ArrayList<Result> listResult, InputCreate inputCreate, GeneticParamCreate geneticParam, String fileName) throws WriteException {
		File file = new File(fileName);
		try {
			file = write(listResult, inputCreate, geneticParam, file);
		}
		catch(IOException e) {
            e.printStackTrace();
        }
    }
}