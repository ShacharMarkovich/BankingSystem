using System;
using System.Windows.Forms;
using Newtonsoft.Json;
using Client;
using Newtonsoft.Json.Converters;
using System.Drawing;

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
                string respone = MainWin.connector.SendAndRecvServer(serverOpcode.login, jsonString);

                if (respone[0] == Utils.SUCCESSED)
                {
                    // Deserialize accuont info
                    var dateTimeConverter = new IsoDateTimeConverter { DateTimeFormat = "yyyy-MM-dd" };
                    Account acc = JsonConvert.DeserializeObject<Account>(respone.Substring(2), dateTimeConverter);
                    new AccountPage(acc).ShowDialog(); // open fit screen
                }
                else
                    MessageBox.Show(respone.Substring(2)); // show error msg
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
