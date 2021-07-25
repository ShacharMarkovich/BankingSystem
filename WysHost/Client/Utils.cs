using System;
using System.Security.Cryptography;
using System.Text;

namespace Client
{
    public enum cmdID // TA op codes
    {
        setTime = 4,
        getModulus,
        getExponent,
        sendEncryptedSession,
        encrypt,
        decrypt,
        base32OTP
    }

    public enum serverOpcode
    {
        register,
        login,
        logout,
        setData,
        updateData,
    }
    /// <summary>
    /// Utils class
    /// </summary>
    public static class Utils
    {
        public const char SUCCESSED = '1'; // succeed code
        public const char FAIL = '0'; // faliture code
        public const char PAD = '*'; // pading char
        public const char SEP = '|';
        public const int BLOCK_SIZE = 16; // AES block size


        /// <summary>
        /// hashing given string according to SHA256 algoritm
        /// </summary>
        /// <param name="value">string to be hashed</param>
        /// <returns>hashed string</returns>
        public static string sha256_hash(string value)
        {
            StringBuilder Sb = new StringBuilder();
            using (SHA256 hash = SHA256.Create())
            {
                Encoding enc = Encoding.UTF8;
                Byte[] result = hash.ComputeHash(enc.GetBytes(value));

                foreach (byte b in result) Sb.Append(b.ToString("x2"));
            }
            return Sb.ToString();
        }

        /// <summary>
        /// add pading in right edge to the given data to fit `BLOCK_SIZE`
        /// </summary>
        /// <param name="data">data to pad</param>
        /// <returns>padded data</returns>
        public static string pad(string data)
        {
            int padLen = BLOCK_SIZE - data.Length % BLOCK_SIZE;
            return data.PadRight(data.Length + padLen, PAD);
        }

        /// <summary>
        /// remove pad from data
        /// </summary>
        /// <param name="data">paded data</param>
        /// <returns>unpad data</returns>
        public static string unpad(string data)
        {
            for (int i = data.LastIndexOf(PAD); i > 0; i--)
                if (data[i] != PAD)
                    return data.Substring(0, i + 1);
            return data;
        }
    }
}
