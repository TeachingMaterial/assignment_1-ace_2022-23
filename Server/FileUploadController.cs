using Microsoft.AspNetCore.Mvc;

[Route("api/[controller]")]
[ApiController]
public class FileUploadController : ControllerBase
{

    private readonly IWebHostEnvironment env;

    public FileUploadController
(IWebHostEnvironment env)
    {
        this.env = env;
    }

    [HttpPost]
    public void Post(UploadedFile uploadedFile)
    {
        var path = $"{env.WebRootPath}\\{uploadedFile.FileName}";
        var fs = System.IO.File.Create(path);
        fs.Write(uploadedFile.FileContent, 0,
uploadedFile.FileContent.Length);
        fs.Close();
    }
}