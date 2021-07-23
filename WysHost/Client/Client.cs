using System.Net;
using System.Net.Sockets;
using System;
using System.Text;

namespace Client
{
    public class Client
    {
        #region Singelton properties
        private static Client theInstance = null;
        public static Client getInstance
        {
            get
            {
                if (theInstance == null)
                    theInstance = new Client();
                return theInstance;
            }
        }
        #endregion

        #region Socket Communication Properties
        Socket _sender;
        IPEndPoint _remoteEP;
        private const int MAX = 4096;

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

        /// <summary>
        /// send the given msg to the server
        /// </summary>
        /// <param name="msg">msg to send</param>
        public void send(byte[] msg)
        {
            try
            {
                // Send the data through the socket.    
                _sender.Send(msg);
            }
            catch
            {
                Console.WriteLine("[!] Server is not Working!");
            }
        }

        /// <summary>
        /// receive `count` bytes from server
        /// </summary>
        /// <param name="count">data bytes' amonut to receive</param>
        /// <returns>the received buffer</returns>
        public byte[] recv(int count = MAX)
        {
            byte[] buffer = null;
            try
            {
                byte[] tmp = new byte[count];
                int bufferSize = _sender.Receive(tmp);
                buffer = new byte[bufferSize];
                Array.Copy(tmp, 0, buffer, 0, bufferSize);
            }
            catch
            {
                Console.WriteLine("[!] Server is not Working!");
            }

            return buffer;
        }

        /// <summary>
        /// Close the connection with the server
        /// </summary>
        ~Client()
        {
            // Release the socket.    
            _sender.Shutdown(SocketShutdown.Both);
            _sender.Close();
        }
        #endregion

        /// <summary>
        /// Keys Exchange with the server
        /// </summary>
        /// <param name="exponent">asymmetric crypto algoritm's exponent</param>
        /// <param name="modulus">asymmetric crypto algoritm's modulus</param>
        /// <returns>shared key encrypted by the fit public key which made from given exponent and modulus</returns>
        public byte[] KeyExchange(byte[] exponent, byte[] modulus)
        {
            send(exponent);
            send(modulus);
            return recv(1024);
        }
    }
}
