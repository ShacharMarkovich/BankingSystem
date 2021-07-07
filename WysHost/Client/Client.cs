using System.Net;
using System.Net.Sockets;
using System;
using System.Text;

namespace Client
{
    public class Client
    {
        Socket _sender;
        IPEndPoint _remoteEP;
        private string username = "";
        private string password = "";
        public Client(string ip = "127.0.0.1", int port = 65432)
        {
            // Connect to a Remote server  
            // Get Host IP Address that is used to establish a connection  
            // In this case, we get one IP address of localhost that is IP : 127.0.0.1  
            // If a host has multiple addresses, you will get a list of addresses  


            _remoteEP = new IPEndPoint(IPAddress.Parse(ip), port);
            // Create a TCP/IP  socket.    
            _sender = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                _sender.Connect(_remoteEP);
            }
            catch
            {
                Console.WriteLine("[!] Server is not Working!");
            }

            Console.WriteLine("[!] Socket connected to {0}", _sender.RemoteEndPoint.ToString());
        }

        public bool Check()
        {
            send("hello");
            return recv(1024).Length > 0;
        }

        public void login(string user, string pass)
        {
            username = user;
            password = pass;

            send("user");

        }

        public void send(string msg)
        {
            // Encode the data string into a byte array.    
            byte[] byteMsg = Encoding.ASCII.GetBytes(msg);
            try
            {
                // Send the data through the socket.    
                int bytesSent = _sender.Send(byteMsg);
            }
            catch
            {
                Console.WriteLine("[!] Server is not Working!");
            }
        }

        public string recv(int count)
        {
            byte[] buffer = new byte[count];
            string msg = "";
            try
            {
                int bufferSize = _sender.Receive(buffer);
                msg = Encoding.ASCII.GetString(buffer, 0, bufferSize); // TODO: maybe leave it as byte[]
            }
            catch
            {
                Console.WriteLine("[!] Server is not Working!");
            }

            return msg;
        }


        public void Shutdown()
        {
            // Release the socket.    
            _sender.Shutdown(SocketShutdown.Both);
            _sender.Close();
        }
    }
}
