using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace Server
{
    public class Server
    {
        static Socket _listener;
        static Socket _handler;
        public static int Main(String[] args)
        {
            StartServer();
            int count = 0;
            while (true)
            {
                Console.WriteLine("Waiting for a connection...");
                _handler = _listener.Accept();
                while (count < 5) // TODO: real communication
                {
                    string data = recv(4096);
                    send("I got it");
                }
                _handler.Shutdown(SocketShutdown.Both);
                _handler.Close();
            }

            return 0;
        }

        public static void send(string msg) => _handler.Send(Encoding.ASCII.GetBytes(msg));


        public static string recv(int count)
        {
            byte[] buffer = new byte[count];
            int bytesRecv = _handler.Receive(buffer);
            return Encoding.ASCII.GetString(buffer, 0, bytesRecv); ;
        }

        public static void StartServer()
        {
            // Get Host IP Address that is used to establish a connection  
            // In this case, we get one IP address of localhost that is IP : 127.0.0.1  
            // If a host has multiple addresses, you will get a list of addresses  
            IPHostEntry host = Dns.GetHostEntry("localhost");
            IPAddress ipAddress = host.AddressList[0];
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 11000);

            // Create a Socket that will use Tcp protocol      
            _listener = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                // A Socket must be associated with an endpoint using the Bind method  
                _listener.Bind(localEndPoint);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
            // Specify how many requests a Socket can listen before it gives Server busy response.  
            // We will listen 10 requests at a time  
            _listener.Listen(10);
        }
    }
}
