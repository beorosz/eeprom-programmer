using System.CommandLine;
using System.IO.Ports;

var fileOption = new Option<FileInfo?>(name: "--file", description: "The path to the file that is to be uploaded to the port.");
var serialPortOption = new Option<string?>(name: "--port", description: "The port the file content will be sent to.");

var rootCommand = new RootCommand(description: "EEPROM programmer client, working through a serial port.");
var uploadCommand = new Command("upload", "Uploads a file.");
var verifyCommand = new Command("verify", "Verifies a file.");
rootCommand.Add(uploadCommand);
rootCommand.Add(verifyCommand);

rootCommand.AddGlobalOption(fileOption);
rootCommand.AddGlobalOption(serialPortOption);

uploadCommand.SetHandler(UploadFileToSerial, fileOption, serialPortOption);
verifyCommand.SetHandler(VerifyFileFromSerial, fileOption, serialPortOption);

return await rootCommand.InvokeAsync(args);

void VerifyFileFromSerial(FileInfo? contentFile, string? serialPortName)
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

void UploadFileToSerial(FileInfo? contentFile, string? serialPortName)
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

void ValidateOptions(FileInfo? contentFile, string? serialPortName)
{
    if(contentFile == null)
    {
        throw new Exception($"Content file was not defined.");
    }
    if(string.IsNullOrEmpty(serialPortName))
    {
        throw new Exception($"Serial port name was not defined.");
    }

    if (!contentFile.Exists)
    {
        throw new Exception($"File {contentFile.Name} does not exist.");
    }
    var maxFileLength = Math.Pow(2, 15);
    if (contentFile.Length > maxFileLength)
    {
        throw new Exception($"File {contentFile.Name} is larger than {maxFileLength} bytes.");
    }
    var serialPort = new SerialPort(serialPortName)
    {
        ReadTimeout = 500,
        WriteTimeout = 500
    };
    serialPort.Open();
    var serialPortAvailable = serialPort.IsOpen;
    serialPort.Close();
    if(!serialPortAvailable)
    {
        throw new Exception($"Serial port {serialPortName} cannot be opened.");
    }
}

void UploadToSerial(FileInfo file, string serialPortName)
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
        BaudRate = 57600,
        ReadTimeout = 500,        
        WriteTimeout = 500
    };
    serialPort.Open();

    Console.WriteLine($"--file = {file}");
    Console.WriteLine($"--port = {serialPortName}");    
    Console.WriteLine($"File size: {file.Length} bytes.");

    UInt16 filePosition = Convert.ToUInt16(stream.Position);
    fileDataByte = stream.ReadByte();
    while(fileDataByte > -1)
    {
        writeRequestBuffer[0] = (byte)'W';
        writeRequestBuffer[1] = (byte)(filePosition >> 8);
        writeRequestBuffer[2] = (byte)filePosition;
        writeRequestBuffer[3] = (byte)fileDataByte;
        serialPort.Write(writeRequestBuffer, 0, REQUEST_BUFFER_SIZE);

        while(serialPort.BytesToRead < RESPONSE_BUFFER_SIZE);
        
        var numberOfResponseBytes = serialPort.Read(responseBuffer, 0, RESPONSE_BUFFER_SIZE);
        if(filePosition % 64 == 0) System.Console.Write($"\r{filePosition}/{file.Length} bytes written.");
        
        filePosition = Convert.ToUInt16(stream.Position);
        fileDataByte = stream.ReadByte();
    }
    
    serialPort.Close();
    stream.Close();
    
    Console.WriteLine($"\rUpload finished at {DateTime.Now}, elapsed time: {DateTime.Now - startTime}");
}

void VerifyFromSerial(FileInfo file, string serialPortName)
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
        BaudRate = 57600,
        ReadTimeout = 500,        
        WriteTimeout = 500
    };
    serialPort.Open();

    Console.WriteLine($"--file = {file}");
    Console.WriteLine($"--port = {serialPortName}");
    Console.WriteLine($"File size: {file.Length} bytes.");
    
    UInt16 filePosition = Convert.ToUInt16(stream.Position);
    fileDataByte = stream.ReadByte();
    while(fileDataByte > -1)
    {
        readRequestBuffer[0] = (byte)'R';
        readRequestBuffer[1] = (byte)(filePosition >> 8);
        readRequestBuffer[2] = (byte)filePosition;
        serialPort.Write(readRequestBuffer, 0, REQUEST_BUFFER_SIZE);

        while(serialPort.BytesToRead < RESPONSE_BUFFER_SIZE);
        
        var numberOfResponseBytes = serialPort.Read(responseBuffer, 0, RESPONSE_BUFFER_SIZE);
        if(Convert.ToInt32(responseBuffer[1]) != fileDataByte)
        {
            Console.WriteLine($"[FAIL] at position 0x{readRequestBuffer[1]:X2}{readRequestBuffer[2]:X2} - expected: 0x{fileDataByte:X2}, actual: 0x{responseBuffer[1]:X2}");
            verifyResult = false;
        
        }
        if(filePosition % 64 == 0) Console.Write($"\r{filePosition}/{file.Length} bytes verified.");

        filePosition = Convert.ToUInt16(stream.Position);
        fileDataByte = verifyResult ? stream.ReadByte() : -1;        
    }    
    serialPort.Close();
    stream.Close();

    var resultString = verifyResult == true ? "SUCCEEDED" : "FAILED";
    Console.WriteLine($"\rVerify {resultString} at {DateTime.Now}, elapsed time: {DateTime.Now - startTime}");
}
