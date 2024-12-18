using System.IO.Ports;

public class VerifyFile : OptionsValidator
{
    public static void VerifyFileFromSerial(FileInfo? contentFile, string? serialPortName)
    {
        try
        {
            ValidateOptions(contentFile, serialPortName);
            VerifyFromSerial(contentFile!, serialPortName!);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }

    private static void VerifyFromSerial(FileInfo file, string serialPortName)
    {
        bool verifyResult = true;
        int fileDataByte;
        int REQUEST_BUFFER_SIZE = 3, RESPONSE_BUFFER_SIZE = 2;
        byte[] readRequestBuffer = new byte[REQUEST_BUFFER_SIZE];
        byte[] responseBuffer = new byte[RESPONSE_BUFFER_SIZE];

        var startTime = DateTime.Now;
        Console.WriteLine($"Verify started at {startTime}");

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
            readRequestBuffer[0] = (byte)'R';
            readRequestBuffer[1] = (byte)(filePosition >> 8);
            readRequestBuffer[2] = (byte)filePosition;
            serialPort.Write(readRequestBuffer, 0, REQUEST_BUFFER_SIZE);

            while (serialPort.BytesToRead < RESPONSE_BUFFER_SIZE) ;

            var numberOfResponseBytes = serialPort.Read(responseBuffer, 0, RESPONSE_BUFFER_SIZE);
            if (Convert.ToInt32(responseBuffer[1]) != fileDataByte)
            {
                Console.WriteLine($"[FAIL] at position 0x{readRequestBuffer[1]:X2}{readRequestBuffer[2]:X2} - expected: 0x{fileDataByte:X2}, actual: 0x{responseBuffer[1]:X2}");
                verifyResult = false;

            }
            if (filePosition % 64 == 0) Console.Write($"\r{filePosition}/{file.Length} bytes verified.");

            filePosition = Convert.ToUInt16(stream.Position);
            fileDataByte = verifyResult ? stream.ReadByte() : -1;
        }
        serialPort.Close();
        stream.Close();

        var resultString = verifyResult == true ? "SUCCEEDED" : "FAILED";
        Console.WriteLine($"\rVerify {resultString} at {DateTime.Now}, elapsed time: {DateTime.Now - startTime}");
    }

}