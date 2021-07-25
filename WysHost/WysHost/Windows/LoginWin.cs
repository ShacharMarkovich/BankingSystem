using System;
using System.Windows.Forms;
using Newtonsoft.Json;
using Client;
using Newtonsoft.Json.Converters;
using System.Drawing;
using System.Text;

namespace WysHost
{
    public partial class LoginWin : Form
    {
        public LoginWin()
        {
            InitializeComponent();
            showPassBtn.BackgroundImage = Image.FromFile("../../Images/show.jpg");
            showPassBtn.BackgroundImageLayout = ImageLayout.Stretch;
            passwordTextBox.UseSystemPasswordChar = true;
        }

        private void retBtn_Click(object sender, EventArgs e) => Close();

        /// <summary>
        /// try to login with the params the user entered
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void loginBtn_Click(object sender, EventArgs e)
        {
            if (usernameTextBox.Text != "" && passwordTextBox.Text != "")
            {
                // Serialize data into json string and send to server and TA
                var loginData = new { username = usernameTextBox.Text, hash_password = Utils.sha256_hash(passwordTextBox.Text) };
                string jsonString = JsonConvert.SerializeObject(loginData);
                string res = MainWin.connector.SendAndRecvServer(serverOpcode.login, jsonString); // account data
                if (res[0] == Utils.FAIL)
                    MessageBox.Show(res.Substring(2));
                else
                {
                    // Deserialize accuont info
                    var dateTimeConverter = new IsoDateTimeConverter { DateTimeFormat = "yyyy-MM-dd" };
                    Account acc = JsonConvert.DeserializeObject<Account>(res.Substring(13), dateTimeConverter);
                    string secret;
                    try
                    {
                        secret = MainWin.connector.GetAccount(acc.accNum.ToString());
                    }
                    catch
                    {
                        MessageBox.Show("DAL error! acount not found");
                        throw new Exception("DAL error! acount not found");
                    }
                    MainWin.connector.SendAndRecvDAL(Encoding.ASCII.GetBytes(secret), cmdID.setBase32OTP);
                    new AccountPage(acc).ShowDialog(); // open fit screen
                    MainWin.connector.SendAndRecvServer(serverOpcode.logout, "{}");
                }
            }
            else
                MessageBox.Show("Please fill both username & password!"); // show error msg

            usernameTextBox.Text = "";
            passwordTextBox.Text = "";
        }

        /// <summary>
        /// event hadle the clicking on show/hide password
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void showPassBtn_Click(object sender, EventArgs e)
        {
            if (passwordTextBox.UseSystemPasswordChar)
                showPassBtn.BackgroundImage = Image.FromFile("../../Images/hide.jpg");
            else
                showPassBtn.BackgroundImage = Image.FromFile("../../Images/show.jpg");

            passwordTextBox.UseSystemPasswordChar = !passwordTextBox.UseSystemPasswordChar;
        }
    }
}
