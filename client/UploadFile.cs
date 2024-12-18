using System.IO.Ports;

public class UploadFile : OptionsValidator
{
    public static void UploadFileToSerial(FileInfo? contentFile, string? serialPortName)
    {
        try
        {
            ValidateOptions(contentFile, serialPortName);
            UploadToSerial(contentFile!, serialPortName!);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }

    private static void UploadToSerial(FileInfo file, string serialPortName)
    {
        int fileDataByte;
        int REQUEST_BUFFER_SIZE = 4, RESPONSE_BUFFER_SIZE = 1;
        byte[] writeRequestBuffer = new byte[REQUEST_BUFFER_SIZE];
        byte[] responseBuffer = new byte[RESPONSE_BUFFER_SIZE];

        var startTime = DateTime.Now;
        Console.WriteLine($"Upload started at {startTime}");

        var stream = file.OpenRead();
        var serialPort = new SerialPort(serialPortName)
        {
            BaudRate = 115200,
            ReadTimeout = 500,
            WriteTimeout = 500
        };
        serialPort.Open();

        Console.WriteLine($"--file = {file}");
        Console.WriteLine($"--port = {serialPortName}");
        Console.WriteLine($"File size: {file.Length} bytes.");

        UInt16 filePosition = Convert.ToUInt16(stream.Position);
        fileDataByte = stream.ReadByte();
        while (fileDataByte > -1)
        {
            writeRequestBuffer[0] = (byte)'W';
            writeRequestBuffer[1] = (byte)(filePosition >> 8);
            writeRequestBuffer[2] = (byte)filePosition;
            writeRequestBuffer[3] = (byte)fileDataByte;
            serialPort.Write(writeRequestBuffer, 0, REQUEST_BUFFER_SIZE);

            while (serialPort.BytesToRead < RESPONSE_BUFFER_SIZE) ;

            serialPort.Read(responseBuffer, 0, RESPONSE_BUFFER_SIZE);
            if (filePosition % 64 == 0) System.Console.Write($"\r{filePosition}/{file.Length} bytes written.");

            filePosition = Convert.ToUInt16(stream.Position);
            fileDataByte = stream.ReadByte();
        }

        serialPort.Close();
        stream.Close();

        Console.WriteLine($"\rUpload finished at {DateTime.Now}, elapsed time: {DateTime.Now - startTime}");
    }

}