using System;
using System.Text;
using Client;
using Intel.Dal;
using System.Windows.Forms;

namespace WysHost
{
    public class Connector
    {
        private Client.Client _client;
        private Jhi jhi;
        private JhiSession session;
        private string appletID = "4c507bdd-2853-417b-98ee-630275d44822";
        private string SEP = "|";

        private byte[] _modulus = null;
        public byte[] Modulus
        {
            get
            {
                if (_modulus == null)
                    _modulus = getModulusFromDAL();
                return _modulus;
            }
            private set { _modulus = value; }
        }

        private byte[] _exponent = null;
        public byte[] Exponent
        {
            get
            {
                if (_exponent == null)
                    _exponent = getExponentFromDAL();
                return _exponent;
            }
            private set { _exponent = value; }
        }

        private static Connector theInstance = null;
        public static Connector getInstance
        {
            get
            {
                if (theInstance == null)
                    theInstance = new Connector();
                return theInstance;
            }
        }

        /// <summary>
        /// send & receive the data to/from server, after encrypt/decrypt by the symmetric key in TA
        /// </summary>
        /// <param name="opCode">command opcode</param>
        /// <param name="data">data to send</param>
        /// <returns>server respone</returns>
        public string SendAndRecvServer(serverOpcode opCode, string data)
        {
            byte[] encMsg = SendAndRecvDAL(Encoding.ASCII.GetBytes(opCode.ToString() + SEP + data), cmdID.encrypt);
            MessageBox.Show("SendAndRecvDAL encrypt work!");
            _client.send(encMsg);
            byte[] encRes = _client.recv();
            return Encoding.UTF8.GetString(SendAndRecvDAL(encRes, cmdID.decrypt));
        }

        /// <summary>
        /// Create Session with Dal applet
        /// </summary>
        private Connector()
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
            string appletPath = @"C:\DALProjects\BankingSystem\WysTA\bin\WysTA.dalp";
            //Install the applet
            Console.WriteLine("Installing applet.");
            jhi.Install(appletID, appletPath);
            byte[] initBuffer = new byte[] { };
            Console.WriteLine("Opening session.");
            jhi.CreateSession(appletID, JHI_SESSION_FLAGS.None, initBuffer, out session);

            _client = Client.Client.getInstance;
        }

        /// <summary>
        /// Keys Exchange with the server and TA
        /// </summary>
        public void KeyExchange()
        {
            byte[] encSharedKey = _client.KeyExchange(Exponent, Modulus);
            MessageBox.Show(Encoding.UTF8.GetString((SendAndRecvDAL(encSharedKey, cmdID.sendEncryptedSession))));

        }

        /// <summary>
        /// Get generated modulus from TA
        /// </summary>
        /// <returns>modulus</returns>
        private byte[] getModulusFromDAL() => SendAndRecvDAL(null, cmdID.getModulus);

        /// <summary>
        /// Get generated exponent from TA
        /// </summary>
        /// <returns>exponent</returns>
        private byte[] getExponentFromDAL() => SendAndRecvDAL(null, cmdID.getExponent);

        /// <summary>
        /// send & receive the data to/from TA
        /// </summary>
        /// <param name="sendBuff">buffer to send</param>
        /// <param name="cmdId">command number</param>
        /// <returns>TA respone</returns>
        private byte[] SendAndRecvDAL(byte[] sendBuff, cmdID cmdId)
        {
            byte[] recvBuff = new byte[4096];
            int responseCode;
            Console.WriteLine("[!] Performing send and receive.");
            jhi.SendAndRecv2(session, (int)cmdId, sendBuff, ref recvBuff, out responseCode);
            return recvBuff;
        }

        ~Connector()
        {
            //Close session
            Console.WriteLine("Closing session.");
            jhi.CloseSession(session);

            //Uninstall the applet
            Console.WriteLine("Uninstalling applet.");
            jhi.Uninstall(appletID);
        }
    }
}
