using System;
using System.Text;
using Intel.Dal;
using System.Security.Cryptography;


/*
 * 
 * הסבר - יצירת חתימה זה כאשר שולחים מידע לפונקציה. הוא מקצר אותו על ידי פונקציית גיבוב להודעה קצרה, ולאחר מכן מצפין עם מפתח פרטי שרק לו יש.
 * כדי לפתוח לוקחים את החתימה ושוברים עם המפתח הכללי. לאחר מכן לוקחים את ההודעה המקורית ועושים לה גיבוב ומשווים ביניהם. וזה בודק אם זה נכון או לא.
 * 
 */


namespace ex04_RonKeinanHost
{

    enum commands
    {
        CMD_SET = 1,
        CMD_LOGIN,
        CMD_RESET,
        CMD_GET_MEMORY,
        CMD_LOGOUT,
        CMD_GENERATE_KEY,
        CMD_GET_PUBLIC_KEY,
        CMD_SEND_AND_VERIFY,
        CMD_EXIT
    }


    class Program
    {
        /*cmd ID:
         * 1 = set password
         * 2 = login
         * 3 = reset password
         * 4 = get account info
         * 5 = logout
         * 6 = generate key pair
         * 7 = get public key
         * 8 = send message, get it coded and verify
         * 9 = exit
         */

        public const int SUCCESS = 1;
        public const int FAILURE = 0;

        private static Jhi jhi;
        private static JhiSession session;

        private static byte[] sendBuff = new byte[256]; // A message to send to the TA
        private static byte[] recvBuff = new byte[256]; // A buffer to hold the output data from the TA
        private static byte[] initBuffer = new byte[] { }; // Data to send to the applet onInit function
        private static int responseCode; // The return value that the TA provides using the IntelApplet.setResponseCode method
        private static int cmdId = 1; // The ID of the command to be performed by the TA

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

        //set the password for the first time only.
        private static bool SetPassword()
        {
            Console.WriteLine("Enter password:");
            string pass = Console.ReadLine();
            sendBuff = UTF32Encoding.UTF8.GetBytes(pass);

            cmdId = (int)commands.CMD_SET;

            recvBuff = new byte[256];
            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);

            if (responseCode == SUCCESS) //applet return 1 means success
                return true;
            return false;

        }

        //allows access to the system(session based)
        private static bool Login()
        {
            Console.WriteLine("Enter password:");
            string pass = Console.ReadLine();
            sendBuff = UTF32Encoding.UTF8.GetBytes(pass);

            cmdId = (int)commands.CMD_LOGIN;

            recvBuff = new byte[256];
            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);

            if (responseCode == SUCCESS) //applet return 1 means success
                return true;
            return false;
        }

        //replace the password with a new password(only if session is active)
        private static bool ResetPassword()
        {
            Console.WriteLine("Enter new password:");
            string pass = Console.ReadLine();
            sendBuff = UTF32Encoding.UTF8.GetBytes(pass);

            cmdId = (int)commands.CMD_RESET;

            recvBuff = new byte[256];
            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);

            if (responseCode == SUCCESS) //applet return 1 means success
                return true;
            return false;
        }

        private static byte[] GetAcountInfo()
        {
            recvBuff = new byte[256];
            cmdId = (int)commands.CMD_GET_MEMORY;

            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);

            return recvBuff;
        }

        private static bool Logout()
        {
            cmdId = (int)commands.CMD_LOGOUT;

            recvBuff = new byte[256];
            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);

            if (responseCode == SUCCESS) //applet return 1 means success
                return true;
            return false;
        }

        private static bool GenerateKeys()
        {
            cmdId = (int)commands.CMD_GENERATE_KEY;

            recvBuff = new byte[256];
            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);

            if (responseCode == SUCCESS) //applet return 1 means success
                return true;
            return false;
        }

        private static byte[] GetKey()
        {
            recvBuff = new byte[768];
            cmdId = (int)commands.CMD_GET_PUBLIC_KEY;

            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);

            return recvBuff;
        }

        private static bool sendAndVerify(byte[] publicKey)
        {
            Console.WriteLine("Enter word to send:");
            string word = Console.ReadLine();
            sendBuff = UTF32Encoding.UTF8.GetBytes(word);

            cmdId = (int)commands.CMD_SEND_AND_VERIFY;

            recvBuff = new byte[1024];
            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);

            if (responseCode == SUCCESS) //applet return 1 means success
            {
                byte[] signedData = sendBuff;
                byte[] signature = new byte[256];
                Array.Copy(recvBuff, 0, signature, 0, 256);

                byte[] keys = GetKey();

                RSAParameters rsaParams = new RSAParameters();
                byte[] mod = new byte[256];
                byte[] e = new byte[4];
                Array.Copy(keys, 0, mod, 0, 256);
                Array.Copy(keys, 256, e, 0, 4);
                rsaParams.Exponent = e; //4 bytes
                rsaParams.Modulus = mod; //256 bytes


                RSACryptoServiceProvider rsaCSP = new RSACryptoServiceProvider();
                rsaCSP.ImportParameters(rsaParams);
                bool check = rsaCSP.VerifyData(signedData, CryptoConfig.MapNameToOID("SHA256"), signature);
                return check;
            }
            return false;
        }


        static void Main(string[] args)
        {
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

            jhi = Jhi.Instance;

            // This is the UUID of this Trusted Application (TA).
            //The UUID is the same value as the applet.id field in the Intel(R) DAL Trusted Application manifest.
            string appletID = "b0df799d-7304-44af-8cdb-e4892c0fc583";
            // This is the path to the Intel Intel(R) DAL Trusted Application .dalp file that was created by the Intel(R) DAL Eclipse plug-in.
            string appletPath = "C:/safeEnviormentWS\\ex04_RonKeinan\\bin\\ex04_RonKeinan.dalp";

            // Install the Trusted Application
            Console.WriteLine("Installing the applet.");
            jhi.Install(appletID, appletPath);

            // Start a session with the Trusted Application
            Console.WriteLine("Opening a session.");
            jhi.CreateSession(appletID, JHI_SESSION_FLAGS.None, initBuffer, out session);


            Console.WriteLine("Welcome, your options are: ");
            Console.WriteLine("1 = set password ");
            Console.WriteLine("2 = login ");
            Console.WriteLine("3 = reset password ");
            Console.WriteLine("4 = get account info ");
            Console.WriteLine("5 = logout");
            Console.WriteLine("6 = generate key pair");
            Console.WriteLine("7 = get public key");
            Console.WriteLine("8 = send message, get it coded and verify");
            Console.WriteLine("9 = exit");



            int choice = 0;



            while (choice != 9)
            {

                Console.WriteLine("Please enter your choice: ");
                choice = Int32.Parse(Console.ReadLine());

                switch (choice)
                {
                    case (int)commands.CMD_SET:
                        if (SetPassword() == true)
                        {
                            Console.WriteLine("Password is set!");
                        }
                        else
                        {
                            Console.WriteLine("ERROR seting a new password");
                        }
                        break;

                    case (int)commands.CMD_LOGIN:
                        if (Login() == true)
                        {
                            Console.WriteLine("You are logged in!");
                        }
                        else
                        {
                            Console.WriteLine("ERROR while login");
                        }
                        break;

                    case (int)commands.CMD_RESET:
                        if (ResetPassword() == true)
                        {
                            Console.WriteLine("Password is reset!");
                        }
                        else
                        {
                            Console.WriteLine("ERROR reseting a new password");
                        }
                        break;

                    case (int)commands.CMD_GET_MEMORY:
                        Console.WriteLine("Your account info: " + UTF32Encoding.UTF8.GetString(GetAcountInfo()));
                        break;

                    case (int)commands.CMD_LOGOUT:
                        if (Logout() == true)
                        {
                            Console.WriteLine("You are logged out!!");
                        }
                        else
                        {
                            Console.WriteLine("ERROR while log out");
                        }
                        break;


                    case (int)commands.CMD_GENERATE_KEY:
                        if (GenerateKeys() == true)
                        {
                            Console.WriteLine("Applet created keys!");
                        }
                        break;


                    case (int)commands.CMD_GET_PUBLIC_KEY:
                        Console.WriteLine("Your public key: " + UTF32Encoding.UTF8.GetString(GetKey()));
                        break;


                    case (int)commands.CMD_SEND_AND_VERIFY:
                        if (sendAndVerify(GetKey()) == true)
                        {
                            Console.WriteLine("Your message recived from applet with a signature and was verified!");
                        }
                        else
                        {
                            Console.WriteLine("Verifacation failed!");
                        }
                        break;

                    case (int)commands.CMD_EXIT:
                        Console.WriteLine("Bye bye!");
                        break;

                    default:
                        Console.WriteLine("Input not valid, please try again.");
                        break;
                }

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





