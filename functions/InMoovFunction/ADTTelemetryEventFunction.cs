using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using Azure;
using Azure.DigitalTwins.Core;
using Azure.Identity;
using Microsoft.AspNetCore.SignalR.Client;
using Microsoft.Azure.EventHubs;
using Microsoft.Azure.WebJobs;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace InMoovFunction
{
    public static class ADTTelemetryEventFunction
    {
        private static HttpClient httpClient = new HttpClient();
        private static string Etag = string.Empty;
        private static readonly string adtInstanceUrl = Environment.GetEnvironmentVariable("ADT_SERVICE_URL");

        [FunctionName("ADTTelemetryEventFunction")]
        public static async Task Run([EventHubTrigger("telemetry-event", Connection = "TelemetnryEHConnString")] EventData[] events, ILogger log)
        {
            var exceptions = new List<Exception>();

            foreach (EventData eventData in events)
            {
                try
                {
                    string messageBody = Encoding.UTF8.GetString(eventData.Body.Array, eventData.Body.Offset, eventData.Body.Count);

                    // Replace these two lines with your processing logic.
                    log.LogInformation($"Telemetry Event Hub: {messageBody}");

                    // SignalR connection
                    var url = Environment.GetEnvironmentVariable("signalRHubUrl");
                    var connection = new HubConnectionBuilder()
                                            .WithUrl(url)
                                            .Build();

                    await connection.StartAsync();
                    await connection.InvokeAsync("ADTTelemetry", messageBody);

                    log.LogInformation("Send msg to SignalR");

                    //Authenticate with Digital Twins
                    var cred = new DefaultAzureCredential();
                    DigitalTwinsClient client = new DigitalTwinsClient(new Uri(adtInstanceUrl), cred);

                    JObject msg = (JObject)JsonConvert.DeserializeObject(messageBody);

                    // Update ADT properties. 
                    var eyesTwinUpdate = new JsonPatchDocument();
                    eyesTwinUpdate.AppendAdd("/eyesLRServo/angle", msg["eyesLRAngle"].Value<int>());
                    eyesTwinUpdate.AppendAdd("/eyesUpDownServo/angle", msg["eyesUpDownAngle"].Value<int>());
                    await client.UpdateDigitalTwinAsync("eyes", eyesTwinUpdate);

                    var jawTwinUpdate = new JsonPatchDocument();
                    jawTwinUpdate.AppendAdd("/neckServo/angle", msg["jawAngle"].Value<int>());
                    await client.UpdateDigitalTwinAsync("jaw", jawTwinUpdate);

                    var neckTwinUpdate = new JsonPatchDocument();
                    neckTwinUpdate.AppendAdd("/neckServo/angle", msg["neckAngle"].Value<int>());
                    await client.UpdateDigitalTwinAsync("neck", neckTwinUpdate);

                    await Task.Yield();
                }
                catch (Exception e)
                {
                    // We need to keep processing the rest of the batch - capture this exception and continue.
                    // Also, consider capturing details of the message that failed processing so it can be processed again later.
                    exceptions.Add(e);
                }
            }

            // Once processing of the batch is complete, if any messages in the batch failed processing throw an exception so that there is a record of the failure.

            if (exceptions.Count > 1)
                throw new AggregateException(exceptions);

            if (exceptions.Count == 1)
                throw exceptions.Single();
        }
    }
}
