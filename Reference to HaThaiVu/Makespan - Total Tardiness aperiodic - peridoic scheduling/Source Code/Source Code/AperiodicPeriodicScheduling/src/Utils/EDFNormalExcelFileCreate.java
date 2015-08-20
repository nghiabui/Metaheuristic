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
 * Description: Create experimental file for EDF normal algorithm
 */
public class EDFNormalExcelFileCreate {
	private static WritableCellFormat timesBold;
	private static WritableCellFormat times;
	
	public static int NUMBERALGORITHM = 2;
	
	// Constructor
	public EDFNormalExcelFileCreate(){
	}
	
	// Write the excel file based on the result
	public static File write(ArrayList<Result> listResult, InputCreate inputCreate, File file) throws IOException, WriteException {
	    WorkbookSettings wbSettings = new WorkbookSettings();

	    wbSettings.setLocale(new Locale("en", "EN"));

	    WritableWorkbook workbook = Workbook.createWorkbook(file, wbSettings);
	    workbook.createSheet("EDF Normal", 0);
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
	    addCaption(sheet, 3, 0, "EDF total tardiness");
	    addCaption(sheet, 4, 0, "EDF_HEU total tardiness");
	    addCaption(sheet, 5, 0, "EDF_HEU better total tardiness percentage (%)");
	}
	
	// Create the content
	private static void createContent(WritableSheet sheet, ArrayList<Result> listResult, InputCreate inputCreate) throws WriteException, RowsExceededException {
		for(int k = 0; k < inputCreate.cycles.length; k++) {
			for(int m = 0; m < inputCreate.processingTimes.length; m++) {
				for(int n = 0; n < inputCreate.numberJobs.length; n++) {
					// Initialize the result
					ArrayList<Run> listRun = listResult.remove(0).listRun;
					
			    	int[] listTotalTardiness = new int[NUMBERALGORITHM];
			    	for(int i = 0; i < NUMBERALGORITHM; i++) {
			    		listTotalTardiness[i] = listRun.remove(0).totalTardiness;
			    	}
			    	double[] listBetterPercentage = new double[NUMBERALGORITHM-1];
			    	for(int i = 1; i < NUMBERALGORITHM; i++) {
			    		listBetterPercentage[i-1] = (double)(listTotalTardiness[0] - listTotalTardiness[i]) / listTotalTardiness[0] * 100;
			    	}
			    	
			    	// Write to cell in excel file
			    	int lineNumber = k*(inputCreate.processingTimes.length*inputCreate.numberJobs.length) + m*(inputCreate.numberJobs.length) + n + 1;
			    	addNumber(sheet, 0, lineNumber, inputCreate.cycles[k]);
			    	addNumber(sheet, 1, lineNumber, inputCreate.processingTimes[m]);
			    	addNumber(sheet, 2, lineNumber, inputCreate.numberJobs[n]);
			    	addNumber(sheet, 3, lineNumber, listTotalTardiness[0]);
			    	addNumber(sheet, 4, lineNumber, listTotalTardiness[1]);
			    	addNumber(sheet, 5, lineNumber, listBetterPercentage[0]);
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