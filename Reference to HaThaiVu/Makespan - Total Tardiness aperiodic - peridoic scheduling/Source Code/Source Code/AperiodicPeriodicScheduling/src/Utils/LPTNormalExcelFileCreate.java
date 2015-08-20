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
 * Description: Create experimental file for LPT normal algorithm
 */
public class LPTNormalExcelFileCreate {
	private static WritableCellFormat timesBold;
	private static WritableCellFormat times;
	
	public static int NUMBERALGORITHM = 4;
	
	// Constructor
	public LPTNormalExcelFileCreate(){
	}
	
	// Write the excel file based on the result
	public static File write(ArrayList<Result> listResult, InputCreate inputCreate, File file) throws IOException, WriteException {
	    WorkbookSettings wbSettings = new WorkbookSettings();

	    wbSettings.setLocale(new Locale("en", "EN"));

	    WritableWorkbook workbook = Workbook.createWorkbook(file, wbSettings);
	    workbook.createSheet("LPT Normal", 0);
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
	    addCaption(sheet, 3, 0, "LPT makespan");
	    addCaption(sheet, 4, 0, "LPT_HEU1 makespan");
	    addCaption(sheet, 5, 0, "LPT_HEU1 better makespan percentage (%)");
	    addCaption(sheet, 6, 0, "LPT_HEU2 makespan");
	    addCaption(sheet, 7, 0, "LPT_HEU2 better makespan percentage (%)");
	    addCaption(sheet, 8, 0, "LPT_HEU3 makespan");
	    addCaption(sheet, 9, 0, "LPT_HEU3 better makespan percentage (%)");
	}
	
	// Create the content
	private static void createContent(WritableSheet sheet, ArrayList<Result> listResult, InputCreate inputCreate) throws WriteException, RowsExceededException {
		for(int k = 0; k < inputCreate.cycles.length; k++) {
			for(int m = 0; m < inputCreate.processingTimes.length; m++) {
				for(int n = 0; n < inputCreate.numberJobs.length; n++) {
					// Initialize the result
					ArrayList<Run> listRun = listResult.remove(0).listRun;
					
			    	int[] listMakespan = new int[NUMBERALGORITHM];
			    	for(int i = 0; i < NUMBERALGORITHM; i++) {
			    		listMakespan[i] = listRun.remove(0).makespan;
			    	}
			    	double[] listBetterPercentage = new double[NUMBERALGORITHM-1];
			    	for(int i = 1; i < NUMBERALGORITHM; i++) {
			    		listBetterPercentage[i-1] = (double)(listMakespan[0] - listMakespan[i]) / listMakespan[0] * 100;
			    	}
			    	
			    	// Write to cell in excel file
			    	int lineNumber = k*(inputCreate.processingTimes.length*inputCreate.numberJobs.length) + m*(inputCreate.numberJobs.length) + n + 1;
			    	addNumber(sheet, 0, lineNumber, inputCreate.cycles[k]);
			    	addNumber(sheet, 1, lineNumber, inputCreate.processingTimes[m]);
			    	addNumber(sheet, 2, lineNumber, inputCreate.numberJobs[n]);
			    	addNumber(sheet, 3, lineNumber, listMakespan[0]);
			    	addNumber(sheet, 4, lineNumber, listMakespan[1]);
			    	addNumber(sheet, 5, lineNumber, listBetterPercentage[0]);
			    	addNumber(sheet, 6, lineNumber, listMakespan[2]);
			    	addNumber(sheet, 7, lineNumber, listBetterPercentage[1]);
			    	addNumber(sheet, 8, lineNumber, listMakespan[3]);
			    	addNumber(sheet, 9, lineNumber, listBetterPercentage[2]);
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