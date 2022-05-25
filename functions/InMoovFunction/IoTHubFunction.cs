using IoTHubTrigger = Microsoft.Azure.WebJobs.EventHubTriggerAttribute;

using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Host;
using Microsoft.Azure.EventHubs;
using System.Text;
using System.Net.Http;
using Microsoft.Extensions.Logging;
using Azure.Identity;
using Azure.DigitalTwins.Core;
using System;
using System.Threading.Tasks;

namespace InMoovFunction
{
    public static class IoTHubFunction
    {
        private static readonly string adtInstanceUrl = Environment.GetEnvironmentVariable("ADT_SERVICE_URL");
        private static readonly HttpClient httpClient = new HttpClient();

        [FunctionName("IoTHubFunction")]
        public static async Task Run([IoTHubTrigger("messages/events", Connection = "IoTHubConnectionString")]EventData message, ILogger log)
        {
            var msg = Encoding.UTF8.GetString(message.Body.Array);
            log.LogInformation($"C# IoT Hub trigger function processed a message: {msg}");

            if (adtInstanceUrl == null)
            {
                log.LogError("Application setting \"ADT_SERVICE_URL\" not set");
                return;
            }

            try
            {
                //Authenticate with Digital Twins
                var cred = new DefaultAzureCredential();
                DigitalTwinsClient client = new DigitalTwinsClient(new Uri(adtInstanceUrl), cred);
                log.LogInformation($"ADT service client connection created.");

                string deviceId = message.SystemProperties["iothub-connection-device-id"].ToString();
                string msgId = Guid.NewGuid().ToString().ToLower(); // message.SystemProperties["message-id"].ToString();
                log.LogInformation($"Device:{deviceId}");

                await client.PublishTelemetryAsync(deviceId, msgId, msg);

            }
            catch (Exception e)
            {
                log.LogError(e.Message);
            }
        }
    }
}