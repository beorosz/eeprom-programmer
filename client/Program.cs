using System.CommandLine;
using System.IO.Ports;

var fileOption = new Option<FileInfo?>(name: "--file", description: "The path to the file that is to be uploaded to the port.");
var serialPortOption = new Option<string?>(name: "--port", description: "The port the file content will be sent to.");

var rootCommand = new RootCommand(description: "EEPROM programmer client, working through a serial port.");
var uploadCommand = new Command("upload", "Uploads a file.");
var verifyCommand = new Command("verify", "Verifies a file.");
var bulkUploadCommand = new Command("bulkupload", "Uploads a file in bulk.");
var bulkVerifyCommand = new Command("bulkverify", "Verifies a file in bulk.");
rootCommand.Add(uploadCommand);
rootCommand.Add(verifyCommand);
rootCommand.Add(bulkUploadCommand);
rootCommand.Add(bulkVerifyCommand);

rootCommand.AddGlobalOption(fileOption);
rootCommand.AddGlobalOption(serialPortOption);

uploadCommand.SetHandler(UploadFile.UploadFileToSerial, fileOption, serialPortOption);
verifyCommand.SetHandler(VerifyFile.VerifyFileFromSerial, fileOption, serialPortOption);
bulkUploadCommand.SetHandler(BulkUpload.BulkUploadFileToSerial, fileOption, serialPortOption);
bulkVerifyCommand.SetHandler(BulkVerify.BulkVerifyFileFromSerial, fileOption, serialPortOption);

return await rootCommand.InvokeAsync(args);