using System;
using System.Text;
using Intel.Dal;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Security.Cryptography;


/*
 * when run more then once - seed changes but in applet not
 */


namespace ex05_B_RonKeinan_203735857Host
{
    class host_05b
    {

        public const int SUCCESS = 1;
        public const int FAILURE = 0;

        public const int CMD_SEND_SEED = 7;
        public const int CMD_SEND_ID = 8;
        public const int CMD_GET_OTP = 9;



        //server exe file - to run as different proccess
         public const string SERVER_PATH = @"C:\Users\ronke\Documents\Visual Studio 2015\Projects\ex05_B_RonKeinan_203735857Host\server_o5b\bin\Release\server_o5b.exe";


        private static Jhi jhi;
        private static JhiSession session;

        private static byte[] sendBuff = new byte[256]; // A message to send to the TA
        private static byte[] recvBuff = new byte[256]; // A buffer to hold the output data from the TA
        private static byte[] initBuffer = new byte[] { }; // Data to send to the applet onInit function
        private static int responseCode; // The return value that the TA provides using the IntelApplet.setResponseCode method
        private static int cmdId = 1; // The ID of the command to be performed by the TA

        // The port number for the remote device.  
        private const int port = 1234;


        //method to start a socket connection and send to it the message
        public static string StartClientSync(string message) // send and recv
        {
            // Data buffer for incoming data.  
            byte[] bytes = new byte[1024];

            // Connect to a remote device.  
            try
            {
                // Establish the remote endpoint for the socket.  
                // This example uses port 11000 on the local computer.  
                IPHostEntry ipHostInfo = Dns.GetHostByAddress("127.0.0.1");
                IPAddress ipAddress = ipHostInfo.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, port);

                // Create a TCP/IP  socket.  
                Socket sender = new Socket(ipAddress.AddressFamily,
                    SocketType.Stream, ProtocolType.Tcp);

                // Connect the socket to the remote endpoint. Catch any errors.  
                try
                {
                    sender.Connect(remoteEP);

                    Console.WriteLine("Socket connected to {0}", sender.RemoteEndPoint.ToString());

                    // Encode the data string into a byte array.  
                    byte[] msg = Encoding.ASCII.GetBytes(message);

                    // Send the data through the socket.  
                    int bytesSent = sender.Send(msg);

                    // Receive the response from the remote device.  
                    int bytesRec = sender.Receive(bytes);
                    Console.WriteLine("Echoed test = {0}",
                        Encoding.ASCII.GetString(bytes, 0, bytesRec));

                    // Release the socket.  
                    sender.Shutdown(SocketShutdown.Both);
                    sender.Close();
                    return Encoding.ASCII.GetString(bytes, 0, bytesRec);

                }
                catch (ArgumentNullException ane)
                {
                    Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
                    return "null";
                }
                catch (SocketException se)
                {
                    Console.WriteLine("SocketException : {0}", se.ToString());
                    return "null";
                }
                catch (Exception e)
                {
                    Console.WriteLine("Unexpected exception : {0}", e.ToString());
                    return "null";
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
                return "null";
            }
        }

        //method to start a socket connection and send to it the message
        public static string StartClientSync(byte[] msg)
        {
            // Data buffer for incoming data.  
            byte[] bytes = new byte[1024];

            // Connect to a remote device.  
            try
            {
                // Establish the remote endpoint for the socket.  
                // This example uses port 11000 on the local computer.  
                IPHostEntry ipHostInfo = Dns.GetHostByAddress("127.0.0.1");
                IPAddress ipAddress = ipHostInfo.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, port);

                // Create a TCP/IP  socket.  
                Socket sender = new Socket(ipAddress.AddressFamily,
                    SocketType.Stream, ProtocolType.Tcp);

                // Connect the socket to the remote endpoint. Catch any errors.  
                try
                {
                    sender.Connect(remoteEP);

                    Console.WriteLine("Socket connected to {0}",
                        sender.RemoteEndPoint.ToString());

                    // Send the data through the socket.  
                    int bytesSent = sender.Send(msg);

                    // Receive the response from the remote device.  
                    int bytesRec = sender.Receive(bytes);
                    Console.WriteLine("Echoed test = {0}",
                        Encoding.ASCII.GetString(bytes, 0, bytesRec));

                    // Release the socket.  
                    sender.Shutdown(SocketShutdown.Both);
                    sender.Close();
                    return Encoding.ASCII.GetString(bytes, 0, bytesRec);

                }
                catch (ArgumentNullException ane)
                {
                    Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
                    return "null";
                }
                catch (SocketException se)
                {
                    Console.WriteLine("SocketException : {0}", se.ToString());
                    return "null";
                }
                catch (Exception e)
                {
                    Console.WriteLine("Unexpected exception : {0}", e.ToString());
                    return "null";
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
                return "null";
            }
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



        //gets a random seed from the server
        public static byte[] getSeed(int userID)
        {
            string seed;
            seed = StartClientSync("getSEED " + userID.ToString()); //send request and USER id
            Console.WriteLine("Seed received from server - " + seed);

            byte[] ret = UTF32Encoding.UTF8.GetBytes(seed);
            return ret;
        }

        //send the seed to save in applet
        private static bool sendSeed(Jhi jhi)
        {
            byte[] recvBuff = new byte[256];
            int cmdId = CMD_SEND_SEED;

            Console.WriteLine("sending seed to applet");

            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);

            if (responseCode == SUCCESS)
                return true;

            return false;
        }

        //send user ID to applet
        private static bool sendID(Jhi jhi)
        {
            byte[] recvBuff = new byte[256];
            int cmdId = CMD_SEND_ID;

            Console.WriteLine("sending ID to applet");

            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);

            if (responseCode == SUCCESS)
                return true;

            return false;
        }

        //get OTP from applet - based on HMAC of seed+counter, hashed by SHA1
        private static byte[] getOTP(Jhi jhi)
        {
            byte[] recvBuff = new byte[512];
            int cmdId = CMD_GET_OTP;

            Console.WriteLine("getting OTP from applet");

            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);

            if (responseCode == SUCCESS)
                return recvBuff;

            return null;
        }


        //chain 2 byte arrays to 1 united array
        private static byte[] chainByteArray(byte[] x, byte[] y)
        {
            byte[] united = new byte[x.Length + y.Length];
            Array.Copy(x, 0, united, 0, x.Length);
            Array.Copy(y, 0, united, x.Length, y.Length);

            return united;

        }

        //send the OTP to server and it will check validity(calc the same hash)
        public static bool sendOTP(byte[] otp, byte[] seed)
        {
            string ans;
            byte[] send = chainByteArray(UTF32Encoding.UTF8.GetBytes("checkOTP "), otp);
               ans = StartClientSync(send);

            if (ans == "valid")
                return true;
            return false;
        }




        static void Main(string[] args)
        {
            //open server in new proccess
            System.Diagnostics.Process.Start(SERVER_PATH);
           

            int userID = 203735857;
            byte[] seed = getSeed(userID); //from server
            string seedNum = Encoding.ASCII.GetString(seed);



#if AMULET
            // When compiled for Amulet the Jhi.DisableDllValidation flag is set to true 
            // in order to load the JHI.dll without DLL verification.
            // This is done because the JHI.dll is not in the regular JHI installation folder, 
            // and therefore will not be found by the JhiSharp.dll.
            // After disabling the .dll validation, the JHI.dll will be loaded using the Windows search path
            // and not by the JhiSharp.dll (see http://msdn.microsoft.com/en-us/library/7d83bc18(v=vs.100).aspx for 
            // details on the search path that is used by Windows to locate a DLL) 
            // In this case the JHI.dll will be loaded from the $(OutDir) folder (bin\Amulet by default),
            // which is the directory where the executable module for the current process is located.
            // The JHI.dll was placed in the bin\Amulet folder during project build.
            Jhi.DisableDllValidation = true;
#endif

            Jhi jhi = Jhi.Instance;

            // This is the UUID of this Trusted Application (TA).
            //The UUID is the same value as the applet.id field in the Intel(R) DAL Trusted Application manifest.
            string appletID = "1847cab0-cd2b-4ac2-83cc-9ef32d6e1961";
            // This is the path to the Intel Intel(R) DAL Trusted Application .dalp file that was created by the Intel(R) DAL Eclipse plug-in.
            string appletPath = "C:/Users/ronke/eclipse-workspace\\ex05_B_RonKeinan_203735857\\bin\\ex05_B_RonKeinan_203735857.dalp";

            // Install the Trusted Application
            Console.WriteLine("Installing the applet.");
            jhi.Install(appletID, appletPath);

            // Start a session with the Trusted Application
            Console.WriteLine("Opening a session.");
            jhi.CreateSession(appletID, JHI_SESSION_FLAGS.None, initBuffer, out session);


            sendBuff = seed;
            if (sendSeed(jhi) == true)
            {
                Console.WriteLine("Seed sent successfully to applet");
            }
            else
            {
                Console.WriteLine("Seed sending failure");
            }

            sendBuff = UTF32Encoding.UTF8.GetBytes(userID.ToString());
            if (sendID(jhi) == true)
            {
                Console.WriteLine("ID sent successfully to applet");
            }
            else
            {
                Console.WriteLine("ID sending failure");
            }


            byte[] otp = getOTP(jhi);
            if (otp != null)
            {
                Console.WriteLine("Otp sent successfully from applet: " + Encoding.ASCII.GetString(otp));
                Console.WriteLine("sending otp to server...: ");
                if (sendOTP(otp, seed) == true)
                {
                    Console.WriteLine("otp is Valid!!! ");
                }
                else
                {
                    Console.WriteLine("otp is not Valid!!! ");
                }
            }
            else
            {
                Console.WriteLine("Otp sending from applet failed");
            }



            // Close the session
            Console.WriteLine("Closing the session.");
            jhi.CloseSession(session);

            //Uninstall the Trusted Application
            Console.WriteLine("Uninstalling the applet.");
            jhi.Uninstall(appletID);

            Console.WriteLine("Press Enter to finish.");
            Console.Read();
        }
    }
}