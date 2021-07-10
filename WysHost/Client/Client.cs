using System.Net;
using System.Net.Sockets;
using System;
using System.Text;

namespace Client
{
    public class Client
    {
        private static Client theInstance = null;
        public static Client getInstance()
        {
            if (theInstance == null)
                theInstance = new Client();
            return theInstance;
        }
        Socket _sender;
        IPEndPoint _remoteEP;

        /// <summary>
        /// Open socket with server, starting the comminication.
        /// </summary>
        /// <param name="ip">server IP</param>
        /// <param name="port">listening server's port</param>
        private Client(string ip = "127.0.0.1", int port = 12345)
        {
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

        public void send(byte[] msg)
        {
            try
            {
                // Send the data through the socket.    
                int bytesSent = _sender.Send(msg);
            }
            catch
            {
                Console.WriteLine("[!] Server is not Working!");
            }
        }

        public byte[] KeyExchange(byte[] exponent, byte[] modulus)
        {
            send(exponent);
            send(modulus);
            return getEncryptedSession();
        }
        public byte[] getEncryptedSession()
        {
            return recv(1024);
        }


        public byte[] recv(int count)
        {
            byte[] buffer = new byte[count];
            try
            {
                int bufferSize = _sender.Receive(buffer);
            }
            catch
            {
                Console.WriteLine("[!] Server is not Working!");
            }

            return buffer;
        }


        public void Shutdown()
        {
            // Release the socket.    
            _sender.Shutdown(SocketShutdown.Both);
            _sender.Close();
        }
    }
}
