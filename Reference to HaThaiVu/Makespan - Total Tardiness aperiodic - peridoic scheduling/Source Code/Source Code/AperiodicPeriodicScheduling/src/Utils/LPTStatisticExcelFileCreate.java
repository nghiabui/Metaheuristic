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
 * Description: Create experimental file for 4 LPT algorithms
 */
public class LPTStatisticExcelFileCreate {
	private static WritableCellFormat timesBold;
	private static WritableCellFormat times;
	
	public static int NUMBERRUN = 10;
	public static int NUMBERALGORITHM = 4;
	
	// Constructor
	public LPTStatisticExcelFileCreate(){
	}
	
	// Write the excel file based on the result
	public static File write(ArrayList<Result> listResult, InputCreate inputCreate, File file) throws IOException, WriteException {
	    WorkbookSettings wbSettings = new WorkbookSettings();

	    wbSettings.setLocale(new Locale("en", "EN"));

	    WritableWorkbook workbook = Workbook.createWorkbook(file, wbSettings);
	    workbook.createSheet("LPT Statistics", 0);
	    WritableSheet excelSheet = workbook.getSheet(0);
	    createLabel(excelSheet);
	    createContent(excelSheet, listResult, inputCreate);

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
	    addCaption(sheet, 3, 0, "Percent of LPT (%)");
	    addCaption(sheet, 4, 0, "Deviation of LPT");
	    addCaption(sheet, 5, 0, "Percent of HEU1 (%)");
	    addCaption(sheet, 6, 0, "Deviation of HEU1");
	    addCaption(sheet, 7, 0, "Percent of HEU2 (%)");
	    addCaption(sheet, 8, 0, "Deviation of HEU2");
	    addCaption(sheet, 9, 0, "Percent of HEU3 (%)");
	    addCaption(sheet, 10, 0, "Deviation of HEU3");
	}
	
	// Create the content
	private static void createContent(WritableSheet sheet, ArrayList<Result> listResult, InputCreate inputCreate) throws WriteException, RowsExceededException {
		for(int k = 0; k < inputCreate.cycles.length; k++) {
			for(int m = 0; m < inputCreate.processingTimes.length; m++) {
				for(int n = 0; n < inputCreate.numberJobs.length; n++) {
					// Initialize the result
			    	double[] listPerformancePercent = new double[NUMBERALGORITHM];
			    	for(int i = 0; i < NUMBERALGORITHM; i++) {
			    		listPerformancePercent[i] = 0;
			    	}
			    	double[] listDeviation = new double[NUMBERALGORITHM];
			    	for(int i = 0; i < NUMBERALGORITHM; i++) {
			    		listDeviation[i] = 0;
			    	}
			    	
			    	for(int i = 0; i < NUMBERRUN; i++) {
			    		ArrayList<Run> listRun = listResult.remove(0).listRun;
			    		
			    		// Get the minimum makespan
			    		Run minRun = listRun.get(0);
			    		for(int j = 1; j < listRun.size(); j++) {
			    			Run tmpRun = listRun.get(j);
			    			if(tmpRun.makespan < minRun.makespan) {
			    				minRun = tmpRun;
			    			}
			    		}
			    		
			    		// Compute performance percent ratio
			    		for(int j = 0; j < listRun.size(); j++) {
			    			Run tmpRun = listRun.get(j);
			    			int percent = 100 / NUMBERRUN; // Each run we get 10% of 100%
			    			
			    			if(tmpRun.makespan == minRun.makespan) {
			    				listPerformancePercent[j] += percent;
			    			}
			    			
			    			// Compute deviation
			    			double deviation = (double)(tmpRun.makespan - minRun.makespan)/minRun.makespan;
			    			listDeviation[j] += deviation;
			    		}
			    	}
			    	
			    	// Write to cell in excel file
			    	int lineNumber = k*(inputCreate.processingTimes.length*inputCreate.numberJobs.length) + m*(inputCreate.numberJobs.length) + n + 1;
			    	addNumber(sheet, 0, lineNumber, inputCreate.cycles[k]);
			    	addNumber(sheet, 1, lineNumber, inputCreate.processingTimes[m]);
			    	addNumber(sheet, 2, lineNumber, inputCreate.numberJobs[n]);
			    	addNumber(sheet, 3, lineNumber, listPerformancePercent[0]);
			    	addNumber(sheet, 4, lineNumber, listDeviation[0]);
			    	addNumber(sheet, 5, lineNumber, listPerformancePercent[1]);
			    	addNumber(sheet, 6, lineNumber, listDeviation[1]);
			    	addNumber(sheet, 7, lineNumber, listPerformancePercent[2]);
			    	addNumber(sheet, 8, lineNumber, listDeviation[2]);
			    	addNumber(sheet, 9, lineNumber, listPerformancePercent[3]);
			    	addNumber(sheet, 10, lineNumber, listDeviation[3]);
				}
			}
		}
		
		// Write the final average result
		// Create label
		int numberTestResult = inputCreate.cycles.length*inputCreate.processingTimes.length*inputCreate.numberJobs.length;
		addCaption(sheet, 0, numberTestResult+1, "Average result");
		
		// Create average results
		StringBuffer buf = new StringBuffer();
	    buf.append("AVERAGE(D2:D"+String.valueOf(numberTestResult+1)+")");
	    Formula f = new Formula(3, numberTestResult+1, buf.toString());
	    sheet.addCell(f);
	    buf = new StringBuffer();
	    buf.append("AVERAGE(E2:E"+String.valueOf(numberTestResult+1)+")");
	    f = new Formula(4, numberTestResult+1, buf.toString());
	    sheet.addCell(f);
	    buf = new StringBuffer();
	    buf.append("AVERAGE(F2:F"+String.valueOf(numberTestResult+1)+")");
	    f = new Formula(5, numberTestResult+1, buf.toString());
	    sheet.addCell(f);
	    buf = new StringBuffer();
	    buf.append("AVERAGE(G2:G"+String.valueOf(numberTestResult+1)+")");
	    f = new Formula(6, numberTestResult+1, buf.toString());
	    sheet.addCell(f);
	    buf = new StringBuffer();
	    buf.append("AVERAGE(H2:H"+String.valueOf(numberTestResult+1)+")");
	    f = new Formula(7, numberTestResult+1, buf.toString());
	    sheet.addCell(f);
	    buf = new StringBuffer();
	    buf.append("AVERAGE(I2:I"+String.valueOf(numberTestResult+1)+")");
	    f = new Formula(8, numberTestResult+1, buf.toString());
	    sheet.addCell(f);
	    buf = new StringBuffer();
	    buf.append("AVERAGE(J2:J"+String.valueOf(numberTestResult+1)+")");
	    f = new Formula(9, numberTestResult+1, buf.toString());
	    sheet.addCell(f);
	    buf = new StringBuffer();
	    buf.append("AVERAGE(K2:K"+String.valueOf(numberTestResult+1)+")");
	    f = new Formula(10, numberTestResult+1, buf.toString());
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
	public static void createExcelFile(ArrayList<Result> listResult, InputCreate inputCreate, String fileName) throws WriteException {
		File file = new File(fileName);
		try {
			file = write(listResult, inputCreate, file);
		}
		catch(IOException e) {
            e.printStackTrace();
        }
    }
}