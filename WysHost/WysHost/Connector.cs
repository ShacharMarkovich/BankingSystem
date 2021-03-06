using System;
using System.Text;
using Client;
using Intel.Dal;
using System.Collections.Generic;
using System.IO;

namespace WysHost
{
    public class Connector
    {
        #region Singelton properties
        private static Connector _instance = null;
        public static Connector getInstance
        {
            get
            {
                if (_instance == null)
                    _instance = new Connector();
                return _instance;
            }
        }
        #endregion

        #region TA variables
        private string _appletID = "4c507bdd-2853-417b-98ee-630275d44822";
        private string _appletPath = @"C:\DALProjects\BankingSystem\WysTA\bin\WysTA.dalp";


        private Jhi _jhi;
        private JhiSession _session;

        /// <summary>
        /// Get generated modulus from TA
        /// </summary>
        /// <returns>modulus</returns>
        public byte[] Modulus
        {
            get
            {
                if (_modulus == null)
                    _modulus = SendAndRecvDAL(null, cmdID.getModulus);
                return _modulus;
            }
            private set { _modulus = value; }
        }
        private byte[] _modulus = null;

        /// <summary>
        /// Get generated exponent from TA
        /// </summary>
        /// <returns>exponent</returns>
        public byte[] Exponent
        {
            get
            {
                if (_exponent == null)
                    _exponent = SendAndRecvDAL(null, cmdID.getExponent);
                return _exponent;
            }
            private set { _exponent = value; }
        }

        public string GetAccount(string accID) => _accountsOTP[accID];

        private byte[] _exponent = null;
        #endregion

        /// <summary>
        /// Dictionary of accounts ID and muches Base32 encoded encrypted OTP secret.
        /// _accountsOTP = {'ID1': Base32(E_aes(secret1)),...}
        /// </summary>
        private Dictionary<string, string> _accountsOTP;
        private static string _fileName = @"C:\bin\data";
        private Client.Client _client;
        private string SEP = "|";


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
            _jhi = Jhi.Instance;
            //Install the applet
            Console.WriteLine("Installing applet.");
            _jhi.Install(_appletID, _appletPath);
            byte[] initBuffer = new byte[] { };
            Console.WriteLine("Opening session.");
            _jhi.CreateSession(_appletID, JHI_SESSION_FLAGS.None, initBuffer, out _session);

            //setTime();
            parseSecrets();

            _client = Client.Client.getInstance;
        }

        /// <summary>
        /// parse OTP encrypted Secrets from file
        /// </summary>
        private void parseSecrets()
        {
            _accountsOTP = new Dictionary<string, string>();
            // Read the file and display it line by line.
            try
            {
                using (StreamReader file = new StreamReader(_fileName))
                {
                    string line;
                    while ((line = file.ReadLine()) != null)
                    {
                        string[] parts = line.Split(Utils.SEP);
                        _accountsOTP[parts[0]] = parts[1];
                        Console.WriteLine(line);
                    }
                }
            }
            catch // file does not exists
            {
                using (FileStream f = File.Create(_fileName)) { }
            }
        }

        /// <summary>
        /// Keys Exchange with the server and TA
        /// </summary>
        public void KeyExchange()
        {
            byte[] encSharedKey = _client.KeyExchange(Exponent, Modulus);
            SendAndRecvDAL(encSharedKey, cmdID.sendEncryptedSession);
        }

        /// <summary>
        /// add new account OTP data
        /// </summary>
        /// <param name="accID">account ID</param>
        /// <param name="secret">OTP Base32 + encrypted secret</param>
        public void addAccountOTP(string accID, string secret)
        {
            _accountsOTP[accID] = secret;
            using (StreamWriter file = new StreamWriter(_fileName, true))
                file.WriteLine($"{accID}|{secret}");
        }

        /// <summary>
        /// set Dal time to current time
        /// </summary>
        private void setTime()
        {
            int unix = (int)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1))).TotalSeconds;
            SendAndRecvDAL(Encoding.ASCII.GetBytes(unix.ToString()), cmdID.setTime);
        }

        /// <summary>
        /// send & receive the data to/from server, after encrypt/decrypt by the symmetric key in TA
        /// </summary>
        /// <param name="opCode">command opcode</param>
        /// <param name="data">data to send</param>
        /// <returns>server respone</returns>
        public string SendAndRecvServer(serverOpcode opCode, string data)
        {
            string padString = Utils.pad(opCode.ToString() + SEP + data); // pading the data
            byte[] encMsg = SendAndRecvDAL(Encoding.ASCII.GetBytes(padString), cmdID.encrypt); // encrypt by TA

            // send and receive to/from server:
            _client.send(encMsg);
            byte[] response = _client.recv();

            // decrypt by TA and unpad response:
            string padres = Encoding.UTF8.GetString(SendAndRecvDAL(response, cmdID.decrypt));
            return Utils.unpad(padres);
        }

        /// <summary>
        /// send & receive the data to/from TA
        /// </summary>
        /// <param name="sendBuff">buffer to send</param>
        /// <param name="cmdId">command number</param>
        /// <returns>TA respone</returns>
        public byte[] SendAndRecvDAL(byte[] sendBuff, cmdID cmdId)
        {
            byte[] recvBuff = new byte[4096];
            int responseCode;
            _jhi.SendAndRecv2(_session, (int)cmdId, sendBuff, ref recvBuff, out responseCode);
            return recvBuff;
        }

        /// <summary>
        /// d'tor. End communication - close socket
        /// </summary>
        ~Connector()
        {
            try
            {
                //Close session
                Console.WriteLine("Closing session.");
                _jhi.CloseSession(_session);

                //Uninstall the applet
                Console.WriteLine("Uninstalling applet.");
                _jhi.Uninstall(_appletID);
            }
            catch (Exception) { }
        }
    }
}
