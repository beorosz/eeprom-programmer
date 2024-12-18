using System.IO.Ports;

public class BulkUpload : OptionsValidator
{
    public static void BulkUploadFileToSerial(FileInfo? contentFile, string? serialPortName)
    {
        try
        {
            ValidateOptions(contentFile, serialPortName);
            BulkUploadToSerial(contentFile!, serialPortName!);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }

    private static void BulkUploadToSerial(FileInfo file, string serialPortName)
    {
        int fileLength = Convert.ToInt32(file.Length);
        int REQUEST_BUFFER_SIZE = 3 + fileLength;     // 1st byte is the command 'B', 2nd and 3rd byte is the file length, rest is the file content 
        int RESPONSE_BUFFER_SIZE = 3;                 // 1st byte is the response type (WRITE_INPROGRESS or WRITE_FINISHED), 2nd and 3rd bytes is the number of bytes written
        byte[] writeRequestBuffer = new byte[REQUEST_BUFFER_SIZE];
        byte[] responseBuffer = new byte[RESPONSE_BUFFER_SIZE];
        responseBuffer[0] = 0x00;

        var startTime = DateTime.Now;
        Console.WriteLine($"Upload started at {startTime}");

        Console.WriteLine($"--file = {file}");
        Console.WriteLine($"--port = {serialPortName}");
        Console.WriteLine($"File size: {fileLength} bytes.");
        Console.Write($"Uploading file to microcontroller...");

        writeRequestBuffer[0] = (byte)'B';
        // data is sent in little endian format
        writeRequestBuffer[1] = (byte)fileLength;
        writeRequestBuffer[2] = (byte)(fileLength >> 8);

        var serialPort = new SerialPort(serialPortName)
        {
            BaudRate = 115200,
            ReadTimeout = Timeout.Infinite,
            WriteTimeout = Timeout.Infinite
        };
        serialPort.Open();

        var stream = file.OpenRead();
        stream.ReadExactly(writeRequestBuffer, 3, fileLength);
        stream.Close();
        serialPort.Write(writeRequestBuffer, 0, REQUEST_BUFFER_SIZE);
        Console.WriteLine($"done.");
        
        while (responseBuffer[0] != 0x2)     // WRITE_FINISHED
        {
            while (serialPort.BytesToRead < RESPONSE_BUFFER_SIZE) ;

            serialPort.Read(responseBuffer, 0, RESPONSE_BUFFER_SIZE);
            uint addressPositionWritten = BitConverter.ToUInt16(responseBuffer, 1) + 1u;
            Console.Write($"\r{addressPositionWritten}/{fileLength} bytes written.");
        }

        if (responseBuffer[0] == 0x2)     // WRITE_FINISHED
        {            
            Console.WriteLine($"\r{fileLength}/{fileLength} bytes written.");
            Console.WriteLine($"\rUpload finished at {DateTime.Now}, elapsed time: {DateTime.Now - startTime}");
        }
        else
        {
            Console.WriteLine($"\rUpload was failed at {DateTime.Now}, elapsed time: {DateTime.Now - startTime}");
        }

        serialPort.Close();
    }

}