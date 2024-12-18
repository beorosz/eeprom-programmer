using System.IO.Ports;

public class OptionsValidator
{
    protected static void ValidateOptions(FileInfo? contentFile, string? serialPortName)
    {
        if (contentFile == null)
        {
            throw new Exception($"Content file was not defined.");
        }
        if (string.IsNullOrEmpty(serialPortName))
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
        if (!serialPortAvailable)
        {
            throw new Exception($"Serial port {serialPortName} cannot be opened.");
        }
    }
}