using System.IO.Ports;

public class BulkVerify : OptionsValidator
{
    public static void BulkVerifyFileFromSerial(FileInfo? contentFile, string? serialPortName)
    {
        try
        {
            ValidateOptions(contentFile, serialPortName);
            BulkVerifyFromSerial(contentFile!, serialPortName!);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
            Console.WriteLine($"Stack trace: {ex.StackTrace}");
        }
    }

    private static void BulkVerifyFromSerial(FileInfo file, string serialPortName)
    {
        bool verifyResult = true;
        int REQUEST_BUFFER_SIZE = 4;
        byte FILE_READ_CHUNK_SIZE = 64;
        int RESPONSE_BUFFER_SIZE = FILE_READ_CHUNK_SIZE + 1;
        byte[] readRequestBuffer = new byte[REQUEST_BUFFER_SIZE];
        byte[] responseBuffer = new byte[RESPONSE_BUFFER_SIZE];
        byte[] fileBuffer = new byte[FILE_READ_CHUNK_SIZE];

        var startTime = DateTime.Now;
        Console.WriteLine($"Bulk verify started at {startTime}");

        var stream = file.OpenRead();
        var serialPort = new SerialPort(serialPortName)
        {
            BaudRate = 115200,
            ReadTimeout = Timeout.Infinite,
            WriteTimeout = Timeout.Infinite
        };
        serialPort.Open();

        Console.WriteLine($"--file = {file}");
        Console.WriteLine($"--port = {serialPortName}");
        Console.WriteLine($"File size: {file.Length} bytes.");

        UInt16 filePosition = Convert.ToUInt16(stream.Position);
        int readFileBytes = stream.Read(fileBuffer, filePosition, FILE_READ_CHUNK_SIZE);
        while (readFileBytes > 0 && verifyResult)
        {
            readRequestBuffer[0] = (byte)'P';
            readRequestBuffer[1] = (byte)filePosition;
            readRequestBuffer[2] = (byte)(filePosition >> 8);
            readRequestBuffer[3] = FILE_READ_CHUNK_SIZE;
            serialPort.Write(readRequestBuffer, 0, REQUEST_BUFFER_SIZE);

            while (serialPort.BytesToRead < RESPONSE_BUFFER_SIZE) ;
            var numberOfResponseBytes = serialPort.Read(responseBuffer, 0, RESPONSE_BUFFER_SIZE);

            if (responseBuffer[0] != 0x10 || !fileBuffer.SequenceEqual(responseBuffer.Skip(1)))
            {
                Console.WriteLine($"[FAIL] at position {filePosition}");
                verifyResult = false;

                continue;
            }

            filePosition = Convert.ToUInt16(stream.Position);
            Console.Write($"\r{filePosition}/{file.Length} bytes verified.");
            readFileBytes = stream.Read(fileBuffer, 0, FILE_READ_CHUNK_SIZE);

        }
        serialPort.Close();
        stream.Close();

        var resultString = verifyResult == true ? "SUCCEEDED" : "FAILED";
        Console.WriteLine($"\rVerify {resultString} at {DateTime.Now}, elapsed time: {DateTime.Now - startTime}");
    }

}