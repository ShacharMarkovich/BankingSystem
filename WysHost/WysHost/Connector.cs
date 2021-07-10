using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Client;
using Intel.Dal;
using System.Threading;
using System.Windows.Forms;

namespace WysHost
{
    public enum cmdID
    {
        getModulus = 5,
        getExponent = 6,
        sendEncryptedSession = 7,
    }
    public class Connector
    {
        private Client.Client _client;
        private static Connector theInstance = null;
        private Jhi jhi;
        private JhiSession session;
        private string appletID = "4c507bdd-2853-417b-98ee-630275d44822";


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


        public static Connector getInstance()
        {
            if (theInstance == null)
                theInstance = new Connector();
            return theInstance;
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
            _client = Client.Client.getInstance();
        }

        public void KeyExchange()
        {
            byte[] encSharedKey = _client.KeyExchange(Exponent, Modulus);
            MessageBox.Show(UTF32Encoding.UTF8.GetString(SendDAL(encSharedKey, cmdID.sendEncryptedSession)));
        }

        public byte[] getModulusFromDAL() => SendDAL(null, cmdID.getModulus);

        public byte[] getExponentFromDAL() => SendDAL(null, cmdID.getExponent);

        public byte[] SendDAL(byte[] data, cmdID cmdId)
        {
            byte[] sendBuff = data;
            byte[] recvBuff = new byte[2000];
            int responseCode;
            Console.WriteLine("Performing send and receive.");
            jhi.SendAndRecv2(session, (int)cmdId, sendBuff, ref recvBuff, out responseCode);
            Console.Out.WriteLine("Response buffer is " + UTF32Encoding.UTF8.GetString(recvBuff));
            Thread.Sleep(1000);
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
