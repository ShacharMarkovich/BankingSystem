using System;
using System.Security.Cryptography;
using System.Text;

namespace Client
{
    public enum cmdID
    {
        getModulus = 5,
        getExponent,
        sendEncryptedSession,
        encrypt,
        decrypt
    }

    public enum serverOpcode
    {
        register,
        login,
        logout,
        setData,
        updateData,
    }
    public static class Utils
    {
        public const char SUCCESSED = '1';
        public const char FAIL = '0';
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

                foreach (byte b in result)
                    Sb.Append(b.ToString("x2"));
            }
            return Sb.ToString();
        }
    }
}
