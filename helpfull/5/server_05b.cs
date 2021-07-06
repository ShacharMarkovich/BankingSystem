using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.IO;
using System.Security.Cryptography;


namespace server_o5b
{

    public class SynchronousSocketListener
    {
        static Random rnd = new Random();


        // The port number for the remote device.  
        private const int port = 1234;

        // Incoming data from the client.  
        public static string data = null;

        private static int counter = 1;

        static IPHostEntry ipHostInfo;
        static IPAddress ipAddress;
        static IPEndPoint localEndPoint;
        static Socket listener;

        //socket recivieng method
        public static void StartListening()
        {
            // Data buffer for incoming data.  
            byte[] bytes = new Byte[1024];

            // Establish the local endpoint for the socket.  
            // Dns.GetHostName returns the name of the
            // host running the application.  
            ipHostInfo = Dns.GetHostByAddress("127.0.0.1");
            ipAddress = ipHostInfo.AddressList[0];
            localEndPoint = new IPEndPoint(ipAddress, port);

            listener = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            // Bind the socket to the local endpoint and
            // listen for incoming connections.  
            try
            {
                listener.Bind(localEndPoint);
                listener.Listen(100);


                // Start listening for connections.  
                while (true)
                {

                    Console.WriteLine("Waiting for a connection...");
                    // Program is suspended while waiting for an incoming connection.  
                    Socket handler = listener.Accept();
                    data = null;

                    // An incoming connection needs to be processed.  

                    int bytesRec = handler.Receive(bytes);
                    data = Encoding.ASCII.GetString(bytes, 0, bytesRec);


                    // Show the data on the console.  
                    Console.WriteLine("Text received : {0}", data);

                    // Echo the data back to the client.  
                    string message="";

                    //divide received string into 2 parts
                    var firstSpaceIndex = data.IndexOf(" ");
                    var firstString = data.Substring(0, firstSpaceIndex); 
                    var secondString = data.Substring(firstSpaceIndex + 1);

                    if (firstString == "getSEED")
                    {

                        int id = int.Parse(secondString);
                        int seed = rnd.Next(1000); //create random seed
                        message = seed.ToString();

                        //write seed and ID to file
                        using (StreamWriter writetext = new StreamWriter("memory.txt"))
                        {
                            writetext.WriteLine(id.ToString());
                            writetext.WriteLine(message);
                        }
                    }
                    //get OTP, hash seed and counter and compare hashes
                    else if (firstString == "checkOTP")
                    {
                        byte[] otp = new byte[256];
                        Array.Copy(bytes, 9, otp, 0, 64);

                        string seedSTR;
                        //read seed from file
                        using (StreamReader Readtext = new StreamReader("memory.txt"))
                        {
                            Readtext.ReadLine();
                            seedSTR = Readtext.ReadLine();
                        }

                        byte[] seed = UTF32Encoding.UTF8.GetBytes(seedSTR);

                        //check 4 cases of counter - and stop in the first true (in order to fix synchronization problem
                        for (int i=0; i<4; i++)
                        {
                            //chain seed to cpunter and hash it
                            byte[] united = chainSeedCounter(seed, counter+i);
                            byte[] otpHash = new SHA1CryptoServiceProvider().ComputeHash(united);

                            if (compareHash(otp, otpHash) == true)
                            {
                                message = "valid";
                                counter += i + 1;
                                break;
                            }
                            else
                            {
                                message = "ERROR!";
                            }
                        }
                    }

                    else
                    {
                        message = "InValid Command";
                    }

                    Console.WriteLine("Text sent : {0}", message);
                    byte[] msg = Encoding.ASCII.GetBytes(message);

                    handler.Send(msg);
                    handler.Shutdown(SocketShutdown.Both);
                    handler.Close();
                }
            }


            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }

            Console.WriteLine("\nPress ENTER to continue...");
            Console.Read();

        }

        //get 2 hashed byte arrays and compare them. second is occurate, first has many zeros in its end.
        private static bool compareHash(byte[] first, byte[] second)
        {
            bool bEqual = false;

            int i = 0;
            while ((i < second.Length) && (first[i] == second[i]))
            {
                i += 1;
            }
            if (i == second.Length)
            {
                bEqual = true;
            }

            return bEqual;
        }

        //chain 2 byte arrays to 1 array
        private static byte[] chainSeedCounter(byte[] seed, int counterTemp)
        {
            //turn counter to byte array
            byte[] count = intToByteArray(counterTemp);

            //make seed shorter array(delete zeros in end)
            byte[] seed2 = new byte[256];
            Array.Copy(seed, 0, seed2, 0, seed.Length);

            //chain
            byte[] united = new byte[seed.Length + count.Length];
            Array.Copy(seed, 0, united, 0, seed.Length);
            Array.Copy(count, 0, united, seed.Length, count.Length);

            return united;

        }

        //get integer and turn it to byte array by shifting right
        private static byte[] intToByteArray(int v)
        {
            return new byte[] { (byte)v, (byte)(v >> 8), (byte)(v >> 16), (byte)(v >> 24) };
        }

        //get byte array and turn it to int by shifting left
        private static int byteArrayToInt(byte[] b)
        {
            return (((b[3] & 0xFF) << 24) | ((b[2] & 0xFF) << 16) | ((b[1] & 0xFF) << 8) | ((b[0] & 0xFF) << 0));
        }

        public static int Main(String[] args)
        {
            StartListening();
            return 0;
        }
    }
}