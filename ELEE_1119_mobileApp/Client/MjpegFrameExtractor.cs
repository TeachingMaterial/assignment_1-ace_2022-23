using static System.Net.Mime.MediaTypeNames;
using System;
using System.IO;
using System.Net.Http;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;

namespace ELEE_1119_mobileApp.Client
{
    public class MJPEGFrameExtractor
    {
        private readonly HttpClient _httpClient;
        private readonly string _mjpegUrl;
        private readonly string _outputDirectory;

        public MJPEGFrameExtractor(HttpClient httpClient, string mjpegUrl, string outputDirectory)
        {
            _httpClient = httpClient;
            _mjpegUrl = mjpegUrl;
            _outputDirectory = outputDirectory;
        }

        public async Task StartExtractingFrames()
        {
            // Download the MJPEG stream.
            using (var stream = await _httpClient.GetStreamAsync(_mjpegUrl))
            {
                // Parse the MJPEG stream to extract each frame.
                var mjpegParser = new MJPEGStreamParser(stream);
                await foreach (var frame in mjpegParser.GetFrames())
                {
                    // Save each frame as a JPEG file to the output directory.
                    var fileName = $"{DateTime.Now:yyyyMMdd_HHmmss_fff}.jpg";
                    var filePath = Path.Combine(_outputDirectory, fileName);
                    using (var fileStream = new FileStream(filePath, FileMode.Create))
                    {
                        frame.Save(fileStream, ImageFormat.Jpeg);
                    }
                }
            }
        }
    }

    public class MJPEGStreamParser
    {
        private readonly Stream _stream;

        public MJPEGStreamParser(Stream stream)
        {
            _stream = stream;
        }

        public async IAsyncEnumerable<System.Drawing.Image> GetFrames()
        {
            while (true)
            {
                // Find the start of the next JPEG frame.
                if (!await FindStartOfNextFrame())
                {
                    break;
                }

                // Read the JPEG frame data.
                using (var memoryStream = new MemoryStream())
                {
                    while (true)
                    {
                        var b = _stream.ReadByte();
                        if (b == -1)
                        {
                            break;
                        }
                        memoryStream.WriteByte((byte)b);
                        if (memoryStream.Length > 2 && memoryStream.GetBuffer()[memoryStream.Length - 2] == 0xFF && memoryStream.GetBuffer()[memoryStream.Length - 1] == 0xD9)
                        {
                            // End of JPEG frame.
                            break;
                        }
                    }

                    // Create an Image object from the JPEG frame data.
                    memoryStream.Seek(0, SeekOrigin.Begin);
                    var image = System.Drawing.Image.FromStream(memoryStream);

                    yield return image;
                }
            }
        }

        private async Task<bool> FindStartOfNextFrame()
        {
            while (true)
            {
                var b = _stream.ReadByte();
                if (b == -1)
                {
                    return false;
                }
                if (b == 0xFF)
                {
                    b = _stream.ReadByte();
                    if (b == -1)
                    {
                        return false;
                    }
                    if (b == 0xD8)
                    {
                        // Start of JPEG frame.
                        return true;
                    }
                }
            }
        }
    }
}

