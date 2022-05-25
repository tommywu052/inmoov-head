using Microsoft.AspNetCore.SignalR;
using System.Threading.Tasks;

namespace SingnalRService.Hubs
{
    public class ADTHub : Hub
    {
        public Task ADTTelemetry(string message) =>
            Clients.All.SendAsync("adtTelemetry", message);
    }
}
