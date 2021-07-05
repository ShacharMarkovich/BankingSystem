using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Intel.Dal;

namespace BankingSystemHost
{
    enum Option
    {
        LOGIN,
        REGISTER,
        EXIT
    }
    class Program
    {



        static int MainMenu()
        {
            int choice = -1;
            Console.WriteLine("Hello!\nWellcome to our Secure Bank Transaction System. Please choose:");
            Console.WriteLine("0. Login to your account");
            Console.WriteLine("1. Create new account");
            Console.WriteLine("2. Echo Test");

            while (choice != (int)Option.LOGIN && choice != (int)Option.REGISTER)
            {
                Console.Write("Your choice: ");
                int.TryParse(Console.ReadLine(), out choice);
            }

            return choice;
        }
        static void Main(string[] args)
        {
            client c = new client();
            c.send("TEST");
            Console.WriteLine(c.recv(1024));
            c.Shutdown();
            return;

            Jhi jhi;
            JhiSession session;
            string appletID;
            InitSession(out jhi, out session, out appletID);

            //Send-Recieved
            ProcessCommunication(jhi, session);

            CloseSession(jhi, session, appletID);
        }


        private static void ProcessCommunication(Jhi jhi, JhiSession session)
        {
            while (true)
            {
                switch ((Option)MainMenu())
                {
                    case Option.LOGIN:
                        break;
                    case Option.REGISTER:
                        break;
                    case Option.EXIT:
                        return;
                    default:
                        Console.WriteLine("Error");
                        break;
                }
            }

            int choice = MainMenu();
            byte[] sendBuff = UTF32Encoding.UTF8.GetBytes("Hello from Shachar");
            byte[] recvBuff = new byte[2000];
            int responseCode;
            int cmdId = 1;
            Console.WriteLine("Performing send and receive.");
            jhi.SendAndRecv2(session, cmdId, sendBuff, ref recvBuff, out responseCode);
            Console.Out.WriteLine("Response buffer is " + UTF32Encoding.UTF8.GetString(recvBuff));
        }

        private static void CloseSession(Jhi jhi, JhiSession session, string appletID)
        {
            //Close session
            Console.WriteLine("Closing session.");
            jhi.CloseSession(session);

            //Uninstall the applet
            Console.WriteLine("Uninstalling applet.");
            jhi.Uninstall(appletID);

            Console.WriteLine("Press any key to finish");
            Console.Read();
        }

        private static void InitSession(out Jhi jhi, out JhiSession session, out string appletID)
        {

#if AMULET
            //When compiled for amulet the Jhi.DisableDllValidation flag is set to true in order to load the JHI.dll without .dll validation.
            //This is done because the JHI.dll is not in the regular JHI installation folder, and therefore will not be found by the JhiSharp.dll.
            //After disabling the .dll validation the dll will be loaded using the Windows search path http://msdn.microsoft.com/en-us/library/7d83bc18(v=vs.100).aspx and not by the JhiSharp.dll.
            //In this case the JHI.dll will be loaded from bin\Amulet folder, which is the directory where the executable module for the current process is located.
            //The JHI.dll was placed in the bin\Amulet folder during project creation.
            Jhi.DisableDllValidation = true;
#endif

            jhi = Jhi.Instance;

            //Those values were defined during project creation.
            appletID = "8a61d132-0f79-4532-8dce-ab405e04c9a4";
            string appletPath = "C:\\DALProjects\\Project\\BankingSystem\\bin\\BankingSystem.dalp";

            //Install the applet
            Console.WriteLine("Installing applet.");
            jhi.Install(appletID, appletPath);

            //Start session
            byte[] initBuffer = new byte[] { };
            Console.WriteLine("Opening session.");
            jhi.CreateSession(appletID, JHI_SESSION_FLAGS.None, initBuffer, out session);
        }
    }
}