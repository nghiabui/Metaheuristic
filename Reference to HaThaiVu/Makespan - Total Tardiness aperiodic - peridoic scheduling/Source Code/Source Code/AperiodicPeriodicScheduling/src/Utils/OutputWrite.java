package Utils;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

import Elements.Instance;

/**
 * @author VuHT
 * Description: Write output file
 */
public class OutputWrite {
	//Read input file and create the instance
	public static void writeOutput(File inputFile, Instance ins, File outputFile, String algorithmName){
		try {
			FileOutputStream fostream = new FileOutputStream(outputFile);
			OutputStreamWriter oswriter = new OutputStreamWriter(fostream);
			BufferedWriter bwriter = new BufferedWriter(oswriter);
			
			bwriter.write(inputFile.toString());bwriter.write("\t");
			bwriter.write(String.valueOf(ins.cycle));bwriter.write("\t");
			bwriter.write(String.valueOf(ins.periodicJob.processingTime));bwriter.write("\t");
			bwriter.write(String.valueOf(ins.listAperiodicJob.size()));bwriter.write("\t");
			bwriter.write(String.valueOf(ins.makespan));bwriter.write("\t");
			bwriter.write(String.valueOf(ins.totalTardiness));bwriter.write("\t");
			bwriter.write(algorithmName);
			bwriter.newLine();
			
			bwriter.close();
			oswriter.close();
			fostream.close();
						
		}
		catch(IOException e) {
            e.printStackTrace();
        }
	}
}